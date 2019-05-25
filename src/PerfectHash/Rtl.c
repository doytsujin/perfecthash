/*++

Copyright (c) 2018-2019 Trent Nelson <trent@trent.me>

Module Name:

    Rtl.c

Abstract:

    This module implements functionality related to the Rtl component of
    the perfect hash table library.

--*/

#include "stdafx.h"

//
// Forward definitions.
//

extern LOAD_SYMBOLS_FROM_MULTIPLE_MODULES LoadSymbolsFromMultipleModules;
extern SET_C_SPECIFIC_HANDLER SetCSpecificHandler;

#define EXPAND_AS_RTL_FUNCTION_NAME(Upper, Name) \
    #Name,

const PCSTR RtlFunctionNames[] = {
    RTL_FUNCTION_TABLE_ENTRY(EXPAND_AS_RTL_FUNCTION_NAME)
};
#define Names RtlFunctionNames

const PCZPCWSTR RtlModuleNames[] = {
    _RTL_MODULE_NAMES_HEAD
};
#define ModuleNames RtlModuleNames


//
// As we don't link to the CRT, we don't get a __C_specific_handler entry,
// which the linker will complain about as soon as we use __try/__except.
// What we do is define a __C_specific_handler_impl pointer to the original
// function (that lives in ntoskrnl), then implement our own function by the
// same name that calls the underlying impl pointer.  In order to do this
// we have to disable some compiler/linker warnings regarding mismatched
// stuff.
//

P__C_SPECIFIC_HANDLER __C_specific_handler_impl = NULL;

#pragma warning(push)
#pragma warning(disable: 4028 4273 28251)

EXCEPTION_DISPOSITION
__cdecl
__C_specific_handler(
    PEXCEPTION_RECORD ExceptionRecord,
    ULONG_PTR Frame,
    PCONTEXT Context,
    struct _DISPATCHER_CONTEXT *Dispatch
    )
{
    return __C_specific_handler_impl(ExceptionRecord,
                                     Frame,
                                     Context,
                                     Dispatch);
}

#pragma warning(pop)

INIT_ONCE InitOnceCSpecificHandler = INIT_ONCE_STATIC_INIT;

BOOL
CALLBACK
SetCSpecificHandlerCallback(
    PINIT_ONCE InitOnce,
    PVOID Parameter,
    PVOID *Context
    )
{
    UNREFERENCED_PARAMETER(InitOnce);
    UNREFERENCED_PARAMETER(Context);

    __C_specific_handler_impl = (P__C_SPECIFIC_HANDLER)Parameter;

    return TRUE;
}

VOID
SetCSpecificHandler(
    _In_ P__C_SPECIFIC_HANDLER Handler
    )
{
    BOOL Status;

    Status = InitOnceExecuteOnce(&InitOnceCSpecificHandler,
                                 SetCSpecificHandlerCallback,
                                 (PVOID)Handler,
                                 NULL);

    //
    // This should never return FALSE.
    //

    ASSERT(Status);
}

//
// Initialize and rundown functions.
//

RTL_INITIALIZE RtlInitialize;

_Use_decl_annotations_
HRESULT
RtlInitialize(
    PRTL Rtl
    )
{
    BOOL Success;
    ULONG Index;
    HRESULT Result = S_OK;
    HMODULE *Module;
    PWSTR *Name;
    ULONG NumberOfModules;
    ULONG NumberOfSymbols;
    ULONG NumberOfResolvedSymbols;

    //
    // Define an appropriately sized bitmap we can passed to LoadSymbols().
    //

    ULONG BitmapBuffer[(ALIGN_UP(ARRAYSIZE(Names),
                        sizeof(ULONG) << 3) >> 5)+1] = { 0 };
    RTL_BITMAP FailedBitmap;

    FailedBitmap.SizeOfBitMap = ARRAYSIZE(Names) + 1;
    FailedBitmap.Buffer = (PULONG)&BitmapBuffer;

    if (!ARGUMENT_PRESENT(Rtl)) {
        return E_POINTER;
    }

    //
    // Attempt to initialize the error handling scaffolding such that SYS_ERROR
    // is available as early as possible.
    //

    Rtl->SysErrorOutputHandle = GetStdHandle(STD_ERROR_HANDLE);

    //
    // Create an error message buffer.
    //

    InitializeSRWLock(&Rtl->SysErrorMessageBufferLock);
    AcquireRtlSysErrorMessageBufferLock(Rtl);

    Rtl->SizeOfSysErrorMessageBufferInBytes = PAGE_SIZE;

    Rtl->SysErrorMessageBuffer = (PCHAR)(
        HeapAlloc(GetProcessHeap(),
                  HEAP_ZERO_MEMORY,
                  Rtl->SizeOfSysErrorMessageBufferInBytes)
    );

    if (!Rtl->SysErrorMessageBuffer) {
        ReleaseRtlSysErrorMessageBufferLock(Rtl);
        return E_OUTOFMEMORY;
    }

    ReleaseRtlSysErrorMessageBufferLock(Rtl);

    //
    // Load the modules.
    //

    Name = (PWSTR *)ModuleNames;
    Module = Rtl->Modules;
    NumberOfModules = GetNumberOfRtlModules(Rtl);

    for (Index = 0; Index < NumberOfModules; Index++, Module++, Name++) {
        *Module = LoadLibraryW(*Name);
        if (!*Module) {
            SYS_ERROR(LoadLibraryW);
            goto Error;
        }
        Rtl->NumberOfModules++;
    }
    ASSERT(Rtl->NumberOfModules == NumberOfModules);

    //
    // Calculate the number of symbols in the function pointer array.
    //

    NumberOfSymbols = sizeof(RTL_FUNCTIONS) / sizeof(ULONG_PTR);
    ASSERT(NumberOfSymbols == ARRAYSIZE(Names));

    //
    // Load the symbols.
    //

    Success = LoadSymbolsFromMultipleModules(Names,
                                             NumberOfSymbols,
                                             (PULONG_PTR)&Rtl->RtlFunctions,
                                             NumberOfSymbols,
                                             Rtl->Modules,
                                             (USHORT)Rtl->NumberOfModules,
                                             &FailedBitmap,
                                             &NumberOfResolvedSymbols);

    if (!Success) {
        Result = PH_E_RTL_LOAD_SYMBOLS_FROM_MULTIPLE_MODULES_FAILED;
        goto Error;
    }

    SetCSpecificHandler(Rtl->__C_specific_handler);

    ASSERT(NumberOfSymbols == NumberOfResolvedSymbols);

    Success = CryptAcquireContextW(&Rtl->CryptProv,
                                   NULL,
                                   NULL,
                                   PROV_RSA_FULL,
                                   CRYPT_VERIFYCONTEXT);

    if (!Success) {
        SYS_ERROR(CryptAcquireContextW);
        goto Error;
    }

    Result = RtlInitializeCpuFeatures(Rtl);
    if (FAILED(Result)) {
        PH_ERROR(RtlInitializeCpuFeatures, Result);
        goto Error;
    }

    //
    // XXX TODO: initialize the bit manipulation function pointers.
    //

    Result = RtlInitializeLargePages(Rtl);
    if (FAILED(Result)) {
        PH_ERROR(RtlInitializeLargePages, Result);
        goto Error;
    }

#if defined(_M_AMD64) || defined(_M_X64)
    if (Rtl->CpuFeatures.AVX2 != FALSE) {
        Rtl->Vtbl->CopyPages = RtlCopyPages_AVX2;
        Rtl->Vtbl->FillPages = RtlFillPages_AVX2;
    }
#endif

    //
    // We're done, indicate success and finish up.
    //

    Result = S_OK;
    goto End;

Error:

    if (Result == S_OK) {
        Result = E_UNEXPECTED;
    }

    //
    // Intentional follow-on to End.
    //

End:

    return Result;
}

RTL_RUNDOWN RtlRundown;

_Use_decl_annotations_
VOID
RtlRundown(
    PRTL Rtl
    )
{
    ULONG Index;
    HMODULE *Module;

    if (!ARGUMENT_PRESENT(Rtl)) {
        return;
    }

    if (Rtl->CryptProv) {
        if (!CryptReleaseContext(Rtl->CryptProv, 0)) {
            SYS_ERROR(CryptReleaseContext);
        }
        Rtl->CryptProv = 0;
    }

    //
    // Free any loaded modules.
    //

    Module = Rtl->Modules;

    for (Index = 0; Index < Rtl->NumberOfModules; Index++) {
        if (!FreeLibrary(*Module)) {
            SYS_ERROR(FreeLibrary);
        }
        *Module++ = 0;
    }

    //
    // Free the sys error buffer.
    //

    if (Rtl->SysErrorMessageBuffer) {
        if (!HeapFree(GetProcessHeap(), 0, Rtl->SysErrorMessageBuffer)) {

            //
            // We can't reliably report this with SYS_ERROR() as the buffer is
            // in an unknown state.
            //

            NOTHING;
        }

        Rtl->SysErrorMessageBuffer = NULL;
    }
}

_Use_decl_annotations_
HRESULT
GetContainingType(
    PRTL Rtl,
    ULONG_PTR Value,
    PTYPE TypePointer
    )
/*++

Routine Description:

    Obtain the appropriate type for a given power-of-2 based value.

Arguments:

    Rtl - Supplies a pointer to an RTL instance.

    Value - Supplies a power-of-2 value for which the type is to be obtained.

    TypePointer - Receives the corresponding type on success.

Return Value:

    S_OK - Type was obtained successfully.

    E_INVALIDARG - Value was not a power-of-2 (more than 1 bit was set).

    E_UNEXPECTED - Internal error.

--*/
{
    TYPE Type;
    ULONG_PTR Bits;
    ULONG_PTR Trailing;

    //
    // If no bits are set in the incoming value, default to ByteType.
    //

    if (Value == 0) {
        *TypePointer = ByteType;
        return S_OK;
    }

    //
    // Count the number of bits in the value.  If more than one bit is present,
    // error out; the value isn't a power-of-2.
    //

    Bits = PopulationCountPointer(Value);
    if (Bits > 1) {
        return E_INVALIDARG;
    }

    //
    // Count the number of trailing zeros.
    //

    Trailing = TrailingZerosPointer(Value);

    switch (Trailing) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            Type = ByteType;
            break;

        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            Type = ShortType;
            break;

        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            Type = LongType;
            break;

        default:
            Type = LongLongType;
            break;

    }

    //
    // Update the caller's pointer and return success.
    //

    *TypePointer = Type;
    return S_OK;
}


#if defined(_M_AMD64) || defined(_M_X64) || defined(_M_IX86)
_Use_decl_annotations_
HRESULT
RtlInitializeCpuFeatures(
    PRTL Rtl
    )
/*++

Routine Description:

    This routine calls the x86/x64 CPUID function and initializes CPU-specific
    features of the provided Rtl instance.

Arguments:

    Rtl - Supplies a pointer to an RTL instance.

Return Value:

    S_OK - CPU features were initialized successfully.

--*/
{
    HRESULT Result;
    CPU_INFO CpuInfo;
    RTL_CPU_FEATURES Features;
    const LONG BaseExtendedId = 0x80000000;
    LONG ExtendedId;
    LONG HighestId;
    LONG HighestExtendedId;

    ZeroStruct(CpuInfo);
    ZeroStruct(Features);

    __cpuid((PINT)&CpuInfo.AsIntArray, 0);

    HighestId = CpuInfo.Eax;

    Features.Vendor.IsIntel = (
        CpuInfo.Ebx == (LONG)'uneG' &&
        CpuInfo.Ecx == (LONG)'letn' &&
        CpuInfo.Edx == (LONG)'Ieni'
    );

    if (!Features.Vendor.IsIntel) {
        Features.Vendor.IsAMD = (
            CpuInfo.Ebx == (LONG)'htuA' &&
            CpuInfo.Ecx == (LONG)'DMAc' &&
            CpuInfo.Edx == (LONG)'itne'
        );

        if (!Features.Vendor.IsAMD) {
            Features.Vendor.Unknown = TRUE;
        }
    }

    if (HighestId >= 1) {
        ZeroStruct(CpuInfo);
        __cpuidex((PINT)&CpuInfo.AsIntArray, 1, 0);

        Features.F1Ecx.AsLong = CpuInfo.Ecx;
        Features.F1Edx.AsLong = CpuInfo.Edx;
    }

    if (HighestId >= 7) {
        ZeroStruct(CpuInfo);
        __cpuidex((PINT)&CpuInfo.AsIntArray, 7, 0);

        Features.F7Ebx.AsLong = CpuInfo.Ebx;
        Features.F7Ecx.AsLong = CpuInfo.Ecx;
    }

    ZeroStruct(CpuInfo);
    __cpuid((PINT)&CpuInfo.AsIntArray, BaseExtendedId);
    HighestExtendedId = CpuInfo.Eax;

    ExtendedId = BaseExtendedId + 1;
    if (HighestExtendedId >= ExtendedId) {
        ZeroStruct(CpuInfo);
        __cpuidex((PINT)&CpuInfo.AsIntArray, ExtendedId, 0);

        Features.F81Ecx.AsLong = CpuInfo.Ecx;
        Features.F81Edx.AsLong = CpuInfo.Edx;
    }

    Features.HighestFeatureId = HighestId;
    Features.HighestExtendedFeatureId = HighestExtendedId;

    if (Features.Vendor.IsIntel) {
        Features.Intel.HLE = Features.HLE;
        Features.Intel.RTM = Features.RTM;
        Features.Intel.LZCNT = Features.LZCNT;
        Features.Intel.SYSCALL = Features.SYSCALLSYSRET;
        Features.Intel.RDTSCP = Features.RDTSCP_IA32_TSC_AUX;
    } else if (Features.Vendor.IsAMD) {
        LONG F81Ecx;
        LONG F81Edx;

        F81Ecx = Features.F81Ecx.AsLong;
        F81Edx = Features.F81Edx.AsLong;

        Features.AMD.ABM = BooleanFlagOn(F81Ecx, 1 << 5);
        Features.AMD.SSE4A = BooleanFlagOn(F81Ecx, 1 << 6);
        Features.AMD.XOP = BooleanFlagOn(F81Ecx, 1 << 11);
        Features.AMD.TBM = BooleanFlagOn(F81Ecx, 1 << 21);
        Features.AMD.SVM = BooleanFlagOn(F81Ecx, 1 << 2);
        Features.AMD.IBS = BooleanFlagOn(F81Ecx, 1 << 10);
        Features.AMD.LWP = BooleanFlagOn(F81Ecx, 1 << 15);
        Features.AMD.MMXEXT = BooleanFlagOn(F81Edx, 1 << 22);
        Features.AMD.THREEDNOW = BooleanFlagOn(F81Edx, 1 << 31);
        Features.AMD.THREEDNOWEXT = BooleanFlagOn(F81Edx, 1 << 30);
    }

    CopyMemory(&Rtl->CpuFeatures,
               &Features,
               sizeof(Rtl->CpuFeatures));

    Result = S_OK;

    return Result;
}
#endif // defined(_M_AMD64) || defined(_M_X64) || defined(_M_IX86)

// vim:set ts=8 sw=4 sts=4 tw=80 expandtab                                     :
