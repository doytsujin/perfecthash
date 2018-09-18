/*++

Copyright (c) 2018 Trent Nelson <trent@trent.me>

Module Name:

    PerfectHashTable.c

Abstract:

    This module implements the initialization and rundown routines for the
    PERFECT_HASH_TABLE instance, and the get flags routine.

--*/

#include "stdafx.h"

PERFECT_HASH_TABLE_INITIALIZE PerfectHashTableInitialize;

_Use_decl_annotations_
HRESULT
PerfectHashTableInitialize(
    PPERFECT_HASH_TABLE Table
    )
/*++

Routine Description:

    Initializes a perfect hash table.  This is a relatively simple method that
    just primes the COM scaffolding; the bulk of the work is done when either
    creating a new table (PerfectHashTableContext->Vtbl->CreateTable) or when
    loading an existing table (PerfectHashTable->Vtbl->Load).

Arguments:

    Table - Supplies a pointer to a PERFECT_HASH_TABLE structure for which
        initialization is to be performed.

Return Value:

    S_OK on success.  E_POINTER if Table is NULL.

--*/
{
    HRESULT Result = S_OK;

    if (!ARGUMENT_PRESENT(Table)) {
        return E_POINTER;
    }

    Table->SizeOfStruct = sizeof(*Table);

    //
    // Create Rtl and Allocator components.
    //

    Result = Table->Vtbl->CreateInstance(Table,
                                         NULL,
                                         &IID_PERFECT_HASH_RTL,
                                         &Table->Rtl);

    if (FAILED(Result)) {
        goto Error;
    }

    Result = Table->Vtbl->CreateInstance(Table,
                                         NULL,
                                         &IID_PERFECT_HASH_ALLOCATOR,
                                         &Table->Allocator);

    if (FAILED(Result)) {
        goto Error;
    }

    //
    // We're done!  Indicate success and finish up.
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

PERFECT_HASH_TABLE_RUNDOWN PerfectHashTableRundown;

_Use_decl_annotations_
VOID
PerfectHashTableRundown(
    PPERFECT_HASH_TABLE Table
    )
/*++

Routine Description:

    Release all resources associated with a perfect hash table.

Arguments:

    Table - Supplies a pointer to a PERFECT_HASH_TABLE structure for which
        rundown is to be performed.

Return Value:

    None.

--*/
{
    PRTL Rtl;
    PALLOCATOR Allocator;

    //
    // Validate arguments.
    //

    if (!ARGUMENT_PRESENT(Table)) {
        return;
    }

    Rtl = Table->Rtl;
    Allocator = Table->Allocator;

    if (!Rtl) {
        return;
    }

    if (!Allocator) {
        return;
    }

    //
    // Sanity check structure size.
    //

    ASSERT(Table->SizeOfStruct == sizeof(*Table));

    //
    // Close resources associated with the header file.
    //

    if (Table->HeaderBaseAddress) {
        if (!UnmapViewOfFile(Table->HeaderBaseAddress)) {
            SYS_ERROR(UnmapViewOfFile);
        }
        Table->HeaderBaseAddress = NULL;
    }

    if (Table->HeaderMappingHandle) {
        if (!CloseHandle(Table->HeaderMappingHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->HeaderMappingHandle = NULL;
    }

    if (Table->HeaderFileHandle) {
        if (!CloseHandle(Table->HeaderFileHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->HeaderFileHandle = NULL;
    }

    //
    // Close resources associated with the :Info stream.
    //

    if (Table->InfoStreamBaseAddress) {
        if (!UnmapViewOfFile(Table->InfoStreamBaseAddress)) {
            SYS_ERROR(UnmapViewOfFile);
        }
        Table->InfoStreamBaseAddress = NULL;
    }

    if (Table->InfoStreamMappingHandle) {
        if (!CloseHandle(Table->InfoStreamMappingHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->InfoStreamMappingHandle = NULL;
    }

    if (Table->InfoStreamFileHandle) {
        if (!CloseHandle(Table->InfoStreamFileHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->InfoStreamFileHandle = NULL;
    }

    //
    // Clean up any resources that are still active.
    //

    if (!Table->MappedAddress) {

        ASSERT(!Table->Flags.TableDataUsesLargePages);

    } else if (Table->BaseAddress) {

        //
        // If MappedAddress is non-NULL, BaseAddress is actually our
        // large page address which needs to be freed with VirtualFree().
        //

        ASSERT(Table->Flags.TableDataUsesLargePages);
        if (!VirtualFree(Table->BaseAddress, 0, MEM_RELEASE)) {
            SYS_ERROR(VirtualFree);
        }

        //
        // Switch the base address back so it's unmapped correctly below.
        //

        Table->BaseAddress = Table->MappedAddress;
        Table->MappedAddress = NULL;
    }

    if (Table->BaseAddress) {
        if (!UnmapViewOfFile(Table->BaseAddress)) {
            SYS_ERROR(UnmapViewOfFile);
        }
        Table->BaseAddress = NULL;
    }

    if (Table->MappingHandle) {
        if (!CloseHandle(Table->MappingHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->MappingHandle = NULL;
    }

    if (Table->FileHandle) {
        if (!CloseHandle(Table->FileHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->FileHandle = NULL;
    }

    //
    // Free the memory used for the values array, if applicable.
    //

    if (Table->ValuesBaseAddress) {
        if (!VirtualFree(Table->ValuesBaseAddress, 0, MEM_RELEASE)) {
            SYS_ERROR(VirtualFree);
        }
        Table->ValuesBaseAddress = NULL;
    }

    //
    // Clear all UNICODE_STRING and STRING structs.
    //

    ZeroStruct(Table->Path);
    ZeroStruct(Table->InfoStreamPath);
    ZeroStruct(Table->HeaderPath);
    ZeroStruct(Table->TableNameW);
    ZeroStruct(Table->TableNameA);

    //
    // Free the base path buffer address if applicable.
    //

    if (Table->BasePathBufferAddress) {
        Allocator->Vtbl->FreePointer(Allocator, &Table->BasePathBufferAddress);
    }

    //
    // Free handles related to table compilation, if applicable.
    //

    if (Table->HeaderFileHandle) {
        if (!CloseHandle(Table->HeaderFileHandle)) {
            SYS_ERROR(CloseHandle);
        }
        Table->HeaderFileHandle = NULL;
    }

    //
    // Release COM references, if applicable.
    //

    if (Table->Context) {
        Table->Context->Vtbl->Release(Table->Context);
        Table->Context = NULL;
    }

    if (Table->Keys) {
        Table->Keys->Vtbl->Release(Table->Keys);
        Table->Keys = NULL;
    }

    if (Table->Allocator) {
        Table->Allocator->Vtbl->Release(Table->Allocator);
        Table->Allocator = NULL;
    }

    if (Table->Rtl) {
        Table->Rtl->Vtbl->Release(Table->Rtl);
        Table->Rtl = NULL;
    }
}


PERFECT_HASH_TABLE_GET_FLAGS PerfectHashTableGetFlags;

_Use_decl_annotations_
HRESULT
PerfectHashTableGetFlags(
    PPERFECT_HASH_TABLE Table,
    ULONG SizeOfFlags,
    PPERFECT_HASH_TABLE_FLAGS Flags
    )
/*++

Routine Description:

    Returns the flags associated with a loaded table instance.

Arguments:

    Table - Supplies a pointer to a PERFECT_HASH_TABLE structure for which the
        flags are to be obtained.

    SizeOfFlags - Supplies the size of the structure pointed to by the Flags
        parameter, in bytes.

    Flags - Supplies the address of a variable that receives the flags.

Return Value:

    S_OK - Success.

    E_POINTER - Table or Flags is NULL.

    E_INVALIDARG - SizeOfFlags does not match the size of the flags structure.

    PH_E_TABLE_NOT_LOADED - No file has been loaded yet.

    PH_E_TABLE_LOCKED - The table is locked.

--*/
{
    if (!ARGUMENT_PRESENT(Table)) {
        return E_POINTER;
    }

    if (!ARGUMENT_PRESENT(Flags)) {
        return E_POINTER;
    }

    if (SizeOfFlags != sizeof(*Flags)) {
        return E_INVALIDARG;
    }

    if (!TryAcquirePerfectHashTableLockExclusive(Table)) {
        return PH_E_TABLE_LOCKED;
    }

    if (!Table->Flags.Loaded) {
        ReleasePerfectHashTableLockExclusive(Table);
        return PH_E_TABLE_NOT_LOADED;
    }

    Flags->AsULong = Table->Flags.AsULong;

    ReleasePerfectHashTableLockExclusive(Table);

    return S_OK;
}

// vim:set ts=8 sw=4 sts=4 tw=80 expandtab                                     :
