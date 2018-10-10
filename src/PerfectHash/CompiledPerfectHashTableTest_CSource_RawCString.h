//
// Auto-generated.
//

DECLSPEC_ALIGN(16)
const CHAR CompiledPerfectHashTableTestCSourceRawCStr[] =
    "//\n"
    "// Disable global optimizations, even in release builds.  Without this, the\n"
    "// compiler does clever things with regards to optimizing our __debugbreak()\n"
    "// logic below, such that it's impossible to tell which ASSERT() triggered it.\n"
    "//\n"
    "\n"
    "#pragma optimize(\"\", off)\n"
    "\n"
    "//\n"
    "// Use a custom ASSERT() macro that optionally issues a __debugbreak() based\n"
    "// on the caller's DebugBreakOnFailure boolean flag.\n"
    "//\n"
    "\n"
    "#undef ASSERT\n"
    "#define ASSERT(Condition)          \\\n"
    "    if (!(Condition)) {            \\\n"
    "        if (DebugBreakOnFailure) { \\\n"
    "            __debugbreak();        \\\n"
    "        };                         \\\n"
    "        NumberOfErrors++;          \\\n"
    "    }\n"
    "\n"
    "DECLARE_TEST_COMPILED_PERFECT_HASH_TABLE_ROUTINE_HEADER()\n"
    "/*++\n"
    "\n"
    "Routine Description:\n"
    "\n"
    "    Tests a compiled perfect hash table for correctness.\n"
    "\n"
    "Arguments:\n"
    "\n"
    "    DebugBreakOnFailure - Supplies a boolean flag that indicates whether or\n"
    "        not a __debugbreak() should be issued as soon as a test fails.\n"
    "\n"
    "Return Value:\n"
    "\n"
    "    The number of failed tests.  If 0, all tests passed.\n"
    "\n"
    "--*/\n"
    "{\n"
    "    ULONG Key;\n"
    "    ULONG Index;\n"
    "    ULONG Value;\n"
    "    ULONG Rotated;\n"
    "    ULONG Previous;\n"
    "    ULONG NumberOfKeys;\n"
    "    ULONG NumberOfErrors = 0;\n"
    "    const ULONG *Source;\n"
    "\n"
    "    NumberOfKeys = CphTableNumberOfKeys;\n"
    "\n"
    "    Key = *CphTableKeys;\n"
    "\n"
    "    //\n"
    "    // Rotate the key such that it differs from the original value, but in a\n"
    "    // way that can be easily reversed.\n"
    "    //\n"
    "\n"
    "    Rotated = _rotl(Key, 15);\n"
    "    ASSERT(Key == _rotr(Rotated, 15));\n"
    "\n"
    "    //\n"
    "    // Verify looking up a key that hasn't been inserted returns 0 as the value.\n"
    "    //\n"
    "\n"
    "    Value = CphTableLookup(Key);\n"
    "    ASSERT(Value == 0);\n"
    "\n"
    "    //\n"
    "    // Verify insertion.\n"
    "    //\n"
    "\n"
    "    Previous = CphTableInsert(Key, Rotated);\n"
    "    ASSERT(Previous == 0);\n"
    "\n"
    "    //\n"
    "    // Lookup the inserted key.\n"
    "    //\n"
    "\n"
    "    Value = CphTableLookup(Key);\n"
    "    ASSERT(Value == Rotated);\n"
    "\n"
    "    //\n"
    "    // Delete the inserted key.  Returned value should be Rotated.\n"
    "    //\n"
    "\n"
    "    Value = CphTableDelete(Key);\n"
    "    ASSERT(Value == Rotated);\n"
    "\n"
    "    //\n"
    "    // Verify a subsequent lookup returns 0.\n"
    "    //\n"
    "\n"
    "    Value = CphTableLookup(Key);\n"
    "    ASSERT(Value == 0);\n"
    "\n"
    "    //\n"
    "    // Loop through the entire key set and insert a rotated version of the key.\n"
    "    //\n"
    "\n"
    "    for (Index = 0, Source = CphTableKeys; Index < NumberOfKeys; Index++) {\n"
    "\n"
    "        Key = *Source++;\n"
    "        Rotated = _rotl(Key, 15);\n"
    "\n"
    "        Previous = CphTableInsert(Key, Rotated);\n"
    "        ASSERT(Previous == 0);\n"
    "\n"
    "    }\n"
    "\n"
    "    //\n"
    "    // Loop through the entire set again and ensure that lookup returns the\n"
    "    // rotated version.\n"
    "    //\n"
    "\n"
    "    for (Index = 0, Source = CphTableKeys; Index < NumberOfKeys; Index++) {\n"
    "\n"
    "        Key = *Source++;\n"
    "        Rotated = _rotl(Key, 15);\n"
    "\n"
    "        Value = CphTableLookup(Key);\n"
    "        ASSERT(Value == Rotated);\n"
    "\n"
    "    }\n"
    "\n"
    "    //\n"
    "    // Loop through again and delete everything.\n"
    "    //\n"
    "\n"
    "    for (Index = 0, Source = CphTableKeys; Index < NumberOfKeys; Index++) {\n"
    "\n"
    "        Key = *Source++;\n"
    "        Rotated = _rotl(Key, 15);\n"
    "\n"
    "        Previous = CphTableDelete(Key);\n"
    "        ASSERT(Previous == Rotated);\n"
    "\n"
    "    }\n"
    "\n"
    "    //\n"
    "    // And a final loop through to confirm all lookups now return 0.\n"
    "    //\n"
    "\n"
    "    for (Index = 0, Source = CphTableKeys; Index < NumberOfKeys; Index++) {\n"
    "\n"
    "        Key = *Source++;\n"
    "\n"
    "        Value = CphTableLookup(Key);\n"
    "        ASSERT(Value == 0);\n"
    "\n"
    "    }\n"
    "\n"
    "    //\n"
    "    // We're finished!  Return the number of errors.\n"
    "    //\n"
    "\n"
    "    return NumberOfErrors;\n"
    "}\n"
    "\n"
    "// vim:set ts=8 sw=4 sts=4 tw=80 expandtab                                     :\n"
;

const STRING CompiledPerfectHashTableTestCSourceRawCString = {
    sizeof(CompiledPerfectHashTableTestCSourceRawCStr) - sizeof(CHAR),
    sizeof(CompiledPerfectHashTableTestCSourceRawCStr),
#ifdef _WIN64
    0,
#endif
    (PCHAR)&CompiledPerfectHashTableTestCSourceRawCStr,
};

#ifndef RawCString
#define RawCString (&CompiledPerfectHashTableTestCSourceRawCString)
#endif
