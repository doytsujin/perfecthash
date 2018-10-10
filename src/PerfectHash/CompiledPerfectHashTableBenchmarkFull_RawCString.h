//
// Auto-generated from ../CompiledPerfectHashTable/CompiledPerfectHashTableBenchmarkFull.c.
//

DECLSPEC_ALIGN(16)
const CHAR CompiledPerfectHashTableBenchmarkFullRawCStr[] =
    "extern\n"
    "BOOLEAN\n"
    "QueryPerformanceCounter(\n"
    "    _Out_ PLARGE_INTEGER Count\n"
    "    );\n"
    "\n"
    "extern volatile ULONG CtrlCPressed;\n"
    "\n"
    "DECLARE_BENCHMARK_FULL_COMPILED_PERFECT_HASH_TABLE_ROUTINE_HEADER()\n"
    "{\n"
    "    ULONG Key;\n"
    "    ULONG Count;\n"
    "    ULONG Index;\n"
    "    ULONG Value = 0;\n"
    "    ULONG Rotated;\n"
    "    ULONG Previous;\n"
    "    ULONG NumberOfKeys;\n"
    "    ULONG Best = (ULONG)-1;\n"
    "    ULONG Attempts = 100;\n"
    "    const ULONG Iterations = 1000;\n"
    "    const ULONG *Source;\n"
    "    LARGE_INTEGER Start;\n"
    "    LARGE_INTEGER End;\n"
    "    LARGE_INTEGER Delta;\n"
    "\n"
    "    NumberOfKeys = CphTableNumberOfKeys;\n"
    "\n"
    "#define FOR_EACH_KEY                      \\\n"
    "    for (Index = 0, Source = CphTableKeys; \\\n"
    "         Index < NumberOfKeys;            \\\n"
    "         Index++)\n"
    "\n"
    "    if (Seconds) {\n"
    "\n"
    "        while (!CtrlCPressed) {\n"
    "\n"
    "            QueryPerformanceCounter(&Start);\n"
    "\n"
    "            for (Count = Iterations; Count != 0; Count--) {\n"
    "\n"
    "                //\n"
    "                // Loop through the entire key set and insert a rotated version of the key.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Previous = CphTableInsert(Key, Rotated);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // Loop through the entire set again and ensure that lookup returns the\n"
    "                // rotated version.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Value = CphTableLookup(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // Loop through again and delete everything.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Previous = CphTableDelete(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // And a final loop through to confirm all lookups now return 0.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "\n"
    "                    Value = CphTableLookup(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "            }\n"
    "\n"
    "            QueryPerformanceCounter(&End);\n"
    "\n"
    "            Delta.QuadPart = End.QuadPart - Start.QuadPart;\n"
    "\n"
    "            if (Delta.LowPart < Best) {\n"
    "                Best = Delta.LowPart;\n"
    "            }\n"
    "        }\n"
    "\n"
    "    } else {\n"
    "\n"
    "        while (Attempts--) {\n"
    "\n"
    "            QueryPerformanceCounter(&Start);\n"
    "\n"
    "            for (Count = Iterations; Count != 0; Count--) {\n"
    "\n"
    "                //\n"
    "                // Loop through the entire key set and insert a rotated version of the key.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Previous = CphTableInsert(Key, Rotated);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // Loop through the entire set again and ensure that lookup returns the\n"
    "                // rotated version.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Value = CphTableLookup(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // Loop through again and delete everything.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "                    Rotated = _rotl(Key, 15);\n"
    "\n"
    "                    Previous = CphTableDelete(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "                //\n"
    "                // And a final loop through to confirm all lookups now return 0.\n"
    "                //\n"
    "\n"
    "                FOR_EACH_KEY {\n"
    "\n"
    "                    Key = *Source++;\n"
    "\n"
    "                    Value = CphTableLookup(Key);\n"
    "\n"
    "                }\n"
    "\n"
    "            }\n"
    "\n"
    "            QueryPerformanceCounter(&End);\n"
    "\n"
    "            Delta.QuadPart = End.QuadPart - Start.QuadPart;\n"
    "\n"
    "            if (Delta.LowPart < Best) {\n"
    "                Best = Delta.LowPart;\n"
    "            }\n"
    "\n"
    "        }\n"
    "\n"
    "    }\n"
    "\n"
    "    return Best;\n"
    "}\n"
    "\n"
    "// vim:set ts=8 sw=4 sts=4 tw=80 expandtab                                     :\n"
;

const STRING CompiledPerfectHashTableBenchmarkFullRawString = {
    sizeof(CompiledPerfectHashTableBenchmarkFullRawCStr) - sizeof(CHAR),
    sizeof(CompiledPerfectHashTableBenchmarkFullRawCStr),
#ifdef _WIN64
    0,
#endif
    (PCHAR)&CompiledPerfectHashTableBenchmarkFullRawCStr,
};
