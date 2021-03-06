//
// Auto-generated.
//

DECLSPEC_ALIGN(16)
const CHAR CompiledPerfectHashTableSupportCSourceRawCStr[] =
    "\n"
    "//\n"
    "// Begin CompiledPerfectHashTableSupport.c.\n"
    "//\n"
    "\n"
    "\n"
    "volatile ULONG CtrlCPressed = 0;\n"
    "\n"
    "void\n"
    "CphQueryPerformanceCounter(\n"
    "    _Out_ PLARGE_INTEGER Count\n"
    "    )\n"
    "{\n"
    "    Count->QuadPart = __rdtsc();\n"
    "}\n"
    "\n"
    "\n"
    "//\n"
    "// End CompiledPerfectHashTableSupport.c.\n"
    "//\n"
    "\n"
;

const STRING CompiledPerfectHashTableSupportCSourceRawCString = {
    sizeof(CompiledPerfectHashTableSupportCSourceRawCStr) - sizeof(CHAR),
    sizeof(CompiledPerfectHashTableSupportCSourceRawCStr),
#ifdef _WIN64
    0,
#endif
    (PCHAR)&CompiledPerfectHashTableSupportCSourceRawCStr,
};

#ifndef RawCString
#define RawCString (&CompiledPerfectHashTableSupportCSourceRawCString)
#endif
