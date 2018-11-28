/*++

Copyright (c) 2018 Trent Nelson <trent@trent.me>

Module Name:

    PerfectHashConstants.c

Abstract:

    This module declares constants used by the perfect hash library.

--*/

#include "stdafx.h"

#define RCS RTL_CONSTANT_STRING
#define NULL_STRING RCS("")
#define NULL_UNICODE_STRING RCS(L"")

#define VERIFY_ALGORITHM_ARRAY_SIZE(Name) \
    C_ASSERT(ARRAYSIZE(Name) == PerfectHashInvalidAlgorithmId + 1)

#define VERIFY_HASH_ARRAY_SIZE(Name) \
    C_ASSERT(ARRAYSIZE(Name) == PerfectHashInvalidHashFunctionId + 1)

#define VERIFY_MASK_ARRAY_SIZE(Name) \
    C_ASSERT(ARRAYSIZE(Name) == PerfectHashInvalidMaskFunctionId + 1)

//
// Declare the array of creation routines.
//

const PCREATE_PERFECT_HASH_TABLE_IMPL CreationRoutines[] = {
    NULL,
    CreatePerfectHashTableImplChm01,
    NULL
};
VERIFY_ALGORITHM_ARRAY_SIZE(CreationRoutines);

//
// Define the array of loader routines.
//

const PLOAD_PERFECT_HASH_TABLE_IMPL LoaderRoutines[] = {
    NULL,
    LoadPerfectHashTableImplChm01,
    NULL
};
VERIFY_ALGORITHM_ARRAY_SIZE(LoaderRoutines);

//
// Define the array of hash routines.
//

#define EXPAND_AS_HASH_ROUTINE(Name, NumberOfSeeds) \
    PerfectHashTableHash##Name,

const PPERFECT_HASH_TABLE_HASH HashRoutines[] = {
    NULL,
    PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(EXPAND_AS_HASH_ROUTINE)
    NULL
};
VERIFY_HASH_ARRAY_SIZE(HashRoutines);

//
// Define the array of number of seeds required per hash routine.
//

#define EXPAND_AS_HASH_NUMBER_OF_SEEDS(Name, NumberOfSeeds) NumberOfSeeds,
const SHORT HashRoutineNumberOfSeeds[] = {
    -1,
    PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(EXPAND_AS_HASH_NUMBER_OF_SEEDS)
    -1,
};
VERIFY_HASH_ARRAY_SIZE(HashRoutineNumberOfSeeds);

//
// Define the array of seeded hash routines.
//

#define EXPAND_AS_SEEDED_HASH_ROUTINE(Name, NumberOfSeeds) \
    PerfectHashTableSeededHash##Name,

const PPERFECT_HASH_TABLE_SEEDED_HASH SeededHashRoutines[] = {
    NULL,
    PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(EXPAND_AS_SEEDED_HASH_ROUTINE)
    NULL
};
VERIFY_HASH_ARRAY_SIZE(SeededHashRoutines);

//
// Define the array of hash mask routines.
//

const PPERFECT_HASH_TABLE_MASK_HASH MaskHashRoutines[] = {
    NULL,
    PerfectHashTableMaskHashModulus,
    PerfectHashTableMaskHashAnd,
    NULL
};
VERIFY_MASK_ARRAY_SIZE(MaskHashRoutines);

//
// Define the array of index mask routines.
//

const PPERFECT_HASH_TABLE_MASK_INDEX MaskIndexRoutines[] = {
    NULL,
    PerfectHashTableMaskIndexModulus,
    PerfectHashTableMaskIndexAnd,
    NULL
};
VERIFY_MASK_ARRAY_SIZE(MaskIndexRoutines);

//
// Define the array of index routines.
//

const PPERFECT_HASH_TABLE_INDEX IndexRoutines[] = {
    NULL,
    PerfectHashTableIndexImplChm01,
    NULL
};
VERIFY_ALGORITHM_ARRAY_SIZE(IndexRoutines);

//
// Define the array of fast-index routines.
//

const PERFECT_HASH_TABLE_FAST_INDEX_TUPLE FastIndexRoutines[] = {

    {
        PerfectHashChm01AlgorithmId,
        PerfectHashHashCrc32RotateFunctionId,
        PerfectHashAndMaskFunctionId,
        PerfectHashTableFastIndexImplChm01Crc32RotateHashAndMask,
    },

    {
        PerfectHashChm01AlgorithmId,
        PerfectHashHashJenkinsFunctionId,
        PerfectHashAndMaskFunctionId,
        PerfectHashTableFastIndexImplChm01JenkinsHashAndMask,
    },

};

const BYTE NumberOfFastIndexRoutines = ARRAYSIZE(FastIndexRoutines);

//
// Define the array of raw C string Index() implementations.
//

#include "CompiledPerfectHashTableIndexRoutines.h"
#undef RawCString

#define EXPAND_AS_CHM01_AND_INDEX_IMPL_TUPLE(Name, NumberOfSeeds)        \
    {                                                                    \
        PerfectHashChm01AlgorithmId,                                     \
        PerfectHashHash##Name##FunctionId,                               \
        PerfectHashAndMaskFunctionId,                                    \
        &CompiledPerfectHashTableChm01Index##Name##AndCSourceRawCString, \
    },


const PERFECT_HASH_TABLE_INDEX_IMPL_STRING_TUPLE IndexImplStringTuples[] = {

    PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(
        EXPAND_AS_CHM01_AND_INDEX_IMPL_TUPLE
    )

};

const BYTE NumberOfIndexImplStrings = ARRAYSIZE(IndexImplStringTuples);

//
// Define UNICODE_STRING structures for each algorithm name.
//

const UNICODE_STRING PerfectHashChm01AlgorithmName =
    RCS(L"Chm01");

//
// Define the array of algorithm names.  This is intended to be indexed by the
// PERFECT_HASH_ALGORITHM_ID enum.
//

const PCUNICODE_STRING AlgorithmNames[] = {
    NULL,
    &PerfectHashChm01AlgorithmName,
    NULL,
};
VERIFY_ALGORITHM_ARRAY_SIZE(AlgorithmNames);

//
// Define UNICODE_STRING structures for each hash function name.
//

#define EXPAND_AS_HASH_FUNCTION_NAME(Name, NumberOfSeeds) \
    const UNICODE_STRING PerfectHashHash##Name##FunctionName = RCS(L#Name);

PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(EXPAND_AS_HASH_FUNCTION_NAME)

//
// Define the array of hash function names.  This is intended to be indexed by
// the PERFECT_HASH_TABLE_HASH_FUNCTION_ID enum.
//

#define EXPAND_AS_HASH_FUNCTION_NAME_PTR(Name, NumberOfSeeds) \
    &PerfectHashHash##Name##FunctionName,

const PCUNICODE_STRING HashFunctionNames[] = {
    NULL,
    PERFECT_HASH_HASH_FUNCTION_TABLE_ENTRY(EXPAND_AS_HASH_FUNCTION_NAME_PTR)
    NULL,
};
VERIFY_HASH_ARRAY_SIZE(HashFunctionNames);

//
// Define UNICODE_STRING structures for each mask function name.
//

const UNICODE_STRING PerfectHashModulusMaskFunctionName =
    RCS(L"Modulus");

const UNICODE_STRING PerfectHashAndMaskFunctionName =
    RCS(L"And");

//
// Define the array of mask function names.  This is intended to be indexed by
// the PERFECT_HASH_MASK_FUNCTION_ID enum.
//

const PCUNICODE_STRING MaskFunctionNames[] = {
    NULL,
    &PerfectHashModulusMaskFunctionName,
    &PerfectHashAndMaskFunctionName,
    NULL,
};
VERIFY_MASK_ARRAY_SIZE(MaskFunctionNames);

//
// Array of UNICODE_STRING event prefix names used by the runtime context.
//

const UNICODE_STRING ContextShutdownEventPrefix =
    RCS(L"PerfectHashContext_ShutdownEvent_");

const UNICODE_STRING ContextSucceededEventPrefix =
    RCS(L"PerfectHashContext_SucceededEvent_");

const UNICODE_STRING ContextFailedEventPrefix =
    RCS(L"PerfectHashContext_FailedEvent_");

const UNICODE_STRING ContextCompletedEventPrefix =
    RCS(L"PerfectHashContext_CompletedEvent_");

const UNICODE_STRING ContextTryLargerTableSizeEventPrefix =
    RCS(L"PerfectHashContext_TryLargerTableSizeEvent_");

const UNICODE_STRING ContextVerifiedTableEventPrefix =
    RCS(L"PerfectHashContext_VerifiedTableEvent_");

#define EXPAND_AS_EVENT_NAME(                                           \
    Verb, VUpper, Name, Upper,                                          \
    EofType, EofValue,                                                  \
    Suffix, Extension, Stream, Base                                     \
)                                                                       \
    const UNICODE_STRING Context##Verb##d##Name##EventPrefix =          \
        RCS(L"PerfectHashContext_" L#Verb L"d" L#Name L"EventPrefix_");

PREPARE_FILE_WORK_TABLE_ENTRY(EXPAND_AS_EVENT_NAME);
SAVE_FILE_WORK_TABLE_ENTRY(EXPAND_AS_EVENT_NAME);

#define EXPAND_AS_EVENT_NAME_ADDRESS(     \
    Verb, VUpper, Name, Upper,            \
    EofType, EofValue,                    \
    Suffix, Extension, Stream, Base       \
)                                         \
    &Context##Verb##d##Name##EventPrefix,

const PCUNICODE_STRING ContextObjectPrefixes[] = {
    &ContextShutdownEventPrefix,
    &ContextSucceededEventPrefix,
    &ContextFailedEventPrefix,
    &ContextCompletedEventPrefix,
    &ContextTryLargerTableSizeEventPrefix,
    &ContextVerifiedTableEventPrefix,

    PREPARE_FILE_WORK_TABLE_ENTRY(EXPAND_AS_EVENT_NAME_ADDRESS)
    SAVE_FILE_WORK_TABLE_ENTRY(EXPAND_AS_EVENT_NAME_ADDRESS)
};

//
// We only have events at the moment so number of event prefixes will equal
// number of object prefixes.
//

const BYTE NumberOfContextEventPrefixes = ARRAYSIZE(ContextObjectPrefixes);
const BYTE NumberOfContextObjectPrefixes = ARRAYSIZE(ContextObjectPrefixes);

//
// Miscellaneous string constants.
//

const UNICODE_STRING No = RCS(L"No.\n");
const UNICODE_STRING Yes = RCS(L"Yes.\n");
const UNICODE_STRING CsvSuffix = RCS(L".csv");
const UNICODE_STRING CsvExtension = RCS(L"csv");
const UNICODE_STRING KeysExtension = RCS(L"keys");
const UNICODE_STRING DotKeysSuffix = RCS(L".keys");
const UNICODE_STRING DotTableSuffix = RCS(L".pht1");
const UNICODE_STRING DotCHeaderSuffix = RCS(L".h");
const UNICODE_STRING DotCSourceSuffix = RCS(L".c");
const UNICODE_STRING KeysWildcardSuffix = RCS(L"*.keys");
const UNICODE_STRING KeysTableSizeSuffix = RCS(L".TableSize");
const UNICODE_STRING PerfectHashBulkCreateCsvBaseName =
    RCS(L"PerfectHashBulkCreate");
const UNICODE_STRING PerfectHashTableCreateCsvBaseName =
    RCS(L"PerfectHashTableCreate");

const STRING DotExeSuffixA = RCS(".exe");
const STRING DotDllSuffixA = RCS(".dll");
const STRING DotLibSuffixA = RCS(".lib");
const STRING DynamicLibraryConfigurationTypeA = RCS("DynamicLibrary");
const STRING ApplicationConfigurationTypeA = RCS("Application");

#define EXPAND_AS_DECL_BEST_COVERAGE_TYPE_STRING(Name, Comparison, Comparator) \
    RCS(#Comparison#Name),

const STRING BestCoverageTypeNames[] = {
    RCS("N/A"),
    BEST_COVERAGE_TYPE_TABLE_ENTRY(EXPAND_AS_DECL_BEST_COVERAGE_TYPE_STRING)
    RCS("N/A"),
};

//
// Stream names.
//

const UNICODE_STRING TableInfoStreamName = RCS(L"Info");

//
// Extensions.
//

const UNICODE_STRING TextFileExtension = RCS(L"txt");
const UNICODE_STRING CSourceFileExtension = RCS(L"c");
const UNICODE_STRING CHeaderFileExtension = RCS(L"h");
const UNICODE_STRING TableFileExtension = RCS(L"pht1");
const UNICODE_STRING VCPropsFileExtension = RCS(L"props");
const UNICODE_STRING VCProjectFileExtension = RCS(L"vcxproj");
const UNICODE_STRING VSSolutionFileExtension = RCS(L"sln");

#define VERIFY_FILE_WORK_ARRAY_SIZE(Name) \
    C_ASSERT(ARRAYSIZE(Name) == NUMBER_OF_FILES + 2)

#define VERIFY_CONTEXT_FILE_WORK_ARRAY_SIZE(Name) \
    C_ASSERT(ARRAYSIZE(Name) == NUMBER_OF_CONTEXT_FILES + 2)

#define EXPAND_AS_FILE_WORK_ITEM_EXTENSION( \
    Verb, VUpper, Name, Upper,              \
    EofType, EofValue,                      \
    Suffix, Extension, Stream, Base         \
)                                           \
    Extension,

const PCUNICODE_STRING FileWorkItemExtensions[] = {
    NULL,
    FILE_WORK_TABLE_ENTRY(EXPAND_AS_FILE_WORK_ITEM_EXTENSION)
    NULL,
};
VERIFY_FILE_WORK_ARRAY_SIZE(FileWorkItemExtensions);

//
// Suffixes.
//

const UNICODE_STRING CHeaderStdAfxFileSuffix = RCS(L"StdAfx");
const UNICODE_STRING CSourceStdAfxFileSuffix = RCS(L"StdAfx");
const UNICODE_STRING CSourceKeysFileSuffix = RCS(L"Keys");
const UNICODE_STRING CHeaderSupportFileSuffix = RCS(L"Support");
const UNICODE_STRING CSourceSupportFileSuffix = RCS(L"Support");
const UNICODE_STRING CSourceTableDataFileSuffix = RCS(L"TableData");
const UNICODE_STRING CSourceTestFileSuffix = RCS(L"Test");
const UNICODE_STRING CSourceTestExeFileSuffix = RCS(L"TestExe");
const UNICODE_STRING CSourceBenchmarkFullFileSuffix = RCS(L"BenchmarkFull");
const UNICODE_STRING CSourceBenchmarkFullExeFileSuffix = RCS(L"BenchmarkFullExe");
const UNICODE_STRING CSourceBenchmarkIndexFileSuffix = RCS(L"BenchmarkIndex");
const UNICODE_STRING CSourceBenchmarkIndexExeFileSuffix = RCS(L"BenchmarkIndexExe");
const UNICODE_STRING VCProjectDllFileSuffix = RCS(L"Dll");
const UNICODE_STRING VCProjectTestExeFileSuffix = RCS(L"TestExe");
const UNICODE_STRING VCProjectBenchmarkFullExeFileSuffix = RCS(L"BenchmarkFullExe");
const UNICODE_STRING VCProjectBenchmarkIndexExeFileSuffix = RCS(L"BenchmarkIndexExe");
const UNICODE_STRING CHeaderCompiledPerfectHashFileSuffix = RCS(L"CompiledPerfectHash");
const UNICODE_STRING VCPropsCompiledPerfectHashFileSuffix = RCS(L"CompiledPerfectHash");
const UNICODE_STRING TableStatsTextFileSuffix = RCS(L"Stats");

const PCUNICODE_STRING FileWorkItemSuffixPointers[] = {
    NULL,

    NULL,                                   // TableFile
    NULL,                                   // TableInfoStream
    NULL,                                   // CHeaderFile
    NULL,                                   // CSourceFile
    &CHeaderStdAfxFileSuffix,               // CHeaderStdAfxFile
    &CSourceStdAfxFileSuffix,               // CSourceStdAfxFile
    &CSourceKeysFileSuffix,                 // CSourceKeysFile
    &CSourceTableDataFileSuffix,            // CSourceTableDataFile
    &CHeaderSupportFileSuffix,              // CHeaderSupportFile
    &CSourceSupportFileSuffix,              // CSourceSupportFile
    &CSourceTestFileSuffix,                 // CSourceTestFile
    &CSourceTestExeFileSuffix,              // CSourceTestExeFile
    &CSourceBenchmarkFullFileSuffix,        // CSourceBenchmarkFullFile
    &CSourceBenchmarkFullExeFileSuffix,     // CSourceBenchmarkFullExeFile
    &CSourceBenchmarkIndexFileSuffix,       // CSourceBenchmarkIndexFile
    &CSourceBenchmarkIndexExeFileSuffix,    // CSourceBenchmarkIndexExeFile
    &VCProjectDllFileSuffix,                // VCProjectDllFile
    &VCProjectTestExeFileSuffix,            // VCProjectTestExeFile
    &VCProjectBenchmarkFullExeFileSuffix,   // VCProjectBenchmarkFullExeFile
    &VCProjectBenchmarkIndexExeFileSuffix,  // VCProjectBenchmarkIndexExeFile
    NULL,                                   // VSSolutionFile
    NULL,                                   // CHeaderCompiledPerfectHashFile
    NULL,                                   // CHeaderCompiledPerfectHashMacroGlueFile
    NULL,                                   // VCPropsCompiledPerfectHashFile
    &TableStatsTextFileSuffix,              // TableStatsTextFile

    NULL,
};
VERIFY_FILE_WORK_ARRAY_SIZE(FileWorkItemSuffixPointers);

#define EXPAND_AS_FILE_WORK_ITEM_SUFFIX( \
    Verb, VUpper, Name, Upper,           \
    EofType, EofValue,                   \
    Suffix, Extension, Stream, Base      \
)                                        \
    RCS(Suffix),


const UNICODE_STRING FileWorkItemSuffixes[] = {
    NULL_UNICODE_STRING,
    FILE_WORK_TABLE_ENTRY(EXPAND_AS_FILE_WORK_ITEM_SUFFIX)
    NULL_UNICODE_STRING,
};
//VERIFY_FILE_WORK_ARRAY_SIZE(FileWorkItemSuffixes);

//
// Stream names.
//

const PCUNICODE_STRING FileWorkItemStreamNamePointers[] = {
    NULL,

    NULL,                           // TableFile
    &TableInfoStreamName,           // TableInfoStream
    NULL,                           // CHeaderFile
    NULL,                           // CSourceFile
    NULL,                           // CHeaderStdAfxFile
    NULL,                           // CSourceStdAfxFile
    NULL,                           // CSourceKeysFile
    NULL,                           // CSourceTableDataFile
    NULL,                           // CHeaderSupportFile
    NULL,                           // CSourceSupportFile
    NULL,                           // CSourceTestFile
    NULL,                           // CSourceTestExeFile
    NULL,                           // CSourceBenchmarkFullFile
    NULL,                           // CSourceBenchmarkFullExeFile
    NULL,                           // CSourceBenchmarkIndexFile
    NULL,                           // CSourceBenchmarkIndexExeFile
    NULL,                           // VCProjectDllFile
    NULL,                           // VCProjectTestExeFile
    NULL,                           // VCProjectBenchmarkFullExeFile
    NULL,                           // VCProjectBenchmarkIndexExeFile
    NULL,                           // VSSolutionFile
    NULL,                           // CHeaderCompiledPerfectHashFile
    NULL,                           // CHeaderCompiledPerfectHashMacroGlueFile
    NULL,                           // VCPropsCompiledPerfectHashFile
    NULL,                           // TableStatsTextFile

    NULL,
};


#define EXPAND_AS_FILE_WORK_ITEM_STREAM_NAME( \
    Verb, VUpper, Name, Upper,                \
    EofType, EofValue,                        \
    Suffix, Extension, Stream, Base           \
)                                             \
    RCS(Stream),

const UNICODE_STRING FileWorkItemStreamNames[] = {
    NULL_UNICODE_STRING,
    FILE_WORK_TABLE_ENTRY(EXPAND_AS_FILE_WORK_ITEM_STREAM_NAME)
    NULL_UNICODE_STRING,
};
VERIFY_FILE_WORK_ARRAY_SIZE(FileWorkItemStreamNames);

//
// Base names.
//
// N.B. These are only used for context files (i.e. CONTEXT_FILE_ID).
//

#define EXPAND_AS_FILE_WORK_BASE_NAME( \
    Verb, VUpper, Name, Upper,         \
    EofType, EofValue,                 \
    Suffix, Extension, Stream, Base    \
)                                      \
    RCS(Base),

const UNICODE_STRING FileWorkItemBaseNames[] = {
    NULL_UNICODE_STRING,
    FILE_WORK_TABLE_ENTRY(EXPAND_AS_FILE_WORK_BASE_NAME)
    NULL_UNICODE_STRING,
};
VERIFY_FILE_WORK_ARRAY_SIZE(FileWorkItemBaseNames);

//
// End-of-file initializers.
//

#define EXPAND_AS_FILE_WORK_ITEM_EOF_INIT( \
    Verb, VUpper, Name, Upper,             \
    EofType, EofValue,                     \
    Suffix, Extension, Stream, Base        \
)                                          \
    { EofType, EofValue },


const EOF_INIT EofInits[] = {
    { EofInitTypeNull, },
    FILE_WORK_TABLE_ENTRY(EXPAND_AS_FILE_WORK_ITEM_EOF_INIT)
    { EofInitTypeInvalid, },
};
VERIFY_FILE_WORK_ARRAY_SIZE(EofInits);

//
// Placeholders for values we patch in the FastIndexEx() instruction streams.
//

const ULONG Seed1Placeholder = 0x11111111;
const ULONG Seed2Placeholder = 0x22222222;
const ULONG Seed3Placeholder = 0x33333333;
const ULONG Seed4Placeholder = 0x44444444;
const ULONG HashMaskPlaceholder = 0xaaaaaaaa;
const ULONG IndexMaskPlaceholder = 0xbbbbbbbb;

//
// COM glue.
//

//
// Bump this every time an interface is added.  This allows us to succeed every
// array declaration (for arrays intended to be indexed via the interface ID
// enum) with a VERIFY_ARRAY_SIZE() static assertion that ensures we've got
// entries for each ID.  The +2 on the EXPECTED_ARRAY_SIZE macro accounts for
// the leading NullInterfaceId and trailing InvalidInterfaceId slots.
//

#define NUMBER_OF_INTERFACES 12
#define EXPECTED_ARRAY_SIZE NUMBER_OF_INTERFACES+2
#define VERIFY_ARRAY_SIZE(Name) C_ASSERT(ARRAYSIZE(Name) == EXPECTED_ARRAY_SIZE)

C_ASSERT(EXPECTED_ARRAY_SIZE == PerfectHashInvalidInterfaceId+1);
C_ASSERT(NUMBER_OF_INTERFACES == PerfectHashLastInterfaceId);
C_ASSERT(NUMBER_OF_INTERFACES == PerfectHashInvalidInterfaceId-1);

const USHORT ComponentSizes[] = {
    0,

    sizeof(IUNKNOWN),
    sizeof(ICLASSFACTORY),
    sizeof(PERFECT_HASH_KEYS),
    sizeof(PERFECT_HASH_CONTEXT),
    sizeof(PERFECT_HASH_TABLE),
    sizeof(RTL),
    sizeof(ALLOCATOR),
    sizeof(PERFECT_HASH_FILE),
    sizeof(PERFECT_HASH_PATH),
    sizeof(PERFECT_HASH_DIRECTORY),
    sizeof(GUARDED_LIST),
    sizeof(GRAPH),

    0,
};
VERIFY_ARRAY_SIZE(ComponentSizes);

const USHORT ComponentInterfaceSizes[] = {
    0,

    sizeof(IUNKNOWN_VTBL),
    sizeof(ICLASSFACTORY_VTBL),
    sizeof(PERFECT_HASH_KEYS_VTBL),
    sizeof(PERFECT_HASH_CONTEXT_VTBL),
    sizeof(PERFECT_HASH_TABLE_VTBL),
    sizeof(RTL_VTBL),
    sizeof(ALLOCATOR_VTBL),
    sizeof(PERFECT_HASH_FILE_VTBL),
    sizeof(PERFECT_HASH_PATH_VTBL),
    sizeof(PERFECT_HASH_DIRECTORY_VTBL),
    sizeof(GUARDED_LIST_VTBL),
    sizeof(GRAPH_VTBL),

    0,
};
VERIFY_ARRAY_SIZE(ComponentInterfaceSizes);

//
// N.B. We use -1 for invalid offsets instead of 0, as 0 could be a legitimate
// field offset if the member is the first element in the structure.
//

const SHORT ComponentInterfaceOffsets[] = {
    -1,

    (SHORT)FIELD_OFFSET(IUNKNOWN, Interface),
    (SHORT)FIELD_OFFSET(ICLASSFACTORY, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_KEYS, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_CONTEXT, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TABLE, Interface),
    (SHORT)FIELD_OFFSET(RTL, Interface),
    (SHORT)FIELD_OFFSET(ALLOCATOR, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_FILE, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_PATH, Interface),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_DIRECTORY, Interface),
    (SHORT)FIELD_OFFSET(GUARDED_LIST, Interface),
    (SHORT)FIELD_OFFSET(GRAPH, Interface),

    -1,
};
VERIFY_ARRAY_SIZE(ComponentInterfaceOffsets);

const SHORT ComponentInterfaceTlsContextOffsets[] = {
    -1,

    -1, // IUnknown
    -1, // IClassFactory
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Keys),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Context),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Table),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Rtl),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Allocator),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, File),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Path),
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Directory),
    -1, // GuardedList
    (SHORT)FIELD_OFFSET(PERFECT_HASH_TLS_CONTEXT, Graph),

    -1,
};
VERIFY_ARRAY_SIZE(ComponentInterfaceTlsContextOffsets);

const SHORT GlobalComponentsInterfaceOffsets[] = {
    -1,

    -1, // IUnknown
    -1, // IClassFactory
    -1, // Keys
    -1, // Context
    -1, // Table
    (SHORT)FIELD_OFFSET(GLOBAL_COMPONENTS, Rtl),
    (SHORT)FIELD_OFFSET(GLOBAL_COMPONENTS, Allocator),
    -1, // File
    -1, // Path
    -1, // Directory
    -1, // GuardedList
    -1, // Graph

    -1,
};
VERIFY_ARRAY_SIZE(GlobalComponentsInterfaceOffsets);

extern COMPONENT_QUERY_INTERFACE ComponentQueryInterface;
extern COMPONENT_ADD_REF ComponentAddRef;
extern COMPONENT_RELEASE ComponentRelease;
extern COMPONENT_CREATE_INSTANCE ComponentCreateInstance;
extern COMPONENT_LOCK_SERVER ComponentLockServer;

//
// Define a helper macro for catching compile-time errors where additional vtbl
// members have been added to a struct, but the initializer in this module has
// not yet been updated to include the new function pointer.  The 'Count' param
// excludes the 5 members that are present on every interface (the IUnknown +
// IClassFactory).
//

#define VERIFY_VTBL_SIZE(Name, Count) \
    C_ASSERT((sizeof(Name##_VTBL) / sizeof(ULONG_PTR)) == (5 + Count))

//
// IUnknown
//

const IUNKNOWN_VTBL IUnknownInterface = {
    (PIUNKNOWN_QUERY_INTERFACE)&ComponentQueryInterface,
    (PIUNKNOWN_ADD_REF)&ComponentAddRef,
    (PIUNKNOWN_RELEASE)&ComponentRelease,
    (PIUNKNOWN_CREATE_INSTANCE)&ComponentCreateInstance,
    (PIUNKNOWN_LOCK_SERVER)&ComponentLockServer,
};
VERIFY_VTBL_SIZE(IUNKNOWN, 0);

//
// IClassFactory
//

const ICLASSFACTORY_VTBL IClassFactoryInterface = {
    (PICLASSFACTORY_QUERY_INTERFACE)&ComponentQueryInterface,
    (PICLASSFACTORY_ADD_REF)&ComponentAddRef,
    (PICLASSFACTORY_RELEASE)&ComponentRelease,
    (PICLASSFACTORY_CREATE_INSTANCE)&ComponentCreateInstance,
    (PICLASSFACTORY_LOCK_SERVER)&ComponentLockServer,
};
VERIFY_VTBL_SIZE(ICLASSFACTORY, 0);

//
// PerfectHashKeys
//

const PERFECT_HASH_KEYS_VTBL PerfectHashKeysInterface = {
    (PPERFECT_HASH_KEYS_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_KEYS_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_KEYS_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_KEYS_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_KEYS_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashKeysLoad,
    &PerfectHashKeysGetFlags,
    &PerfectHashKeysGetAddress,
    &PerfectHashKeysGetBitmap,
    &PerfectHashKeysGetFile,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_KEYS, 5);

//
// PerfectHashContext
//

const PERFECT_HASH_CONTEXT_VTBL PerfectHashContextInterface = {
    (PPERFECT_HASH_CONTEXT_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_CONTEXT_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_CONTEXT_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_CONTEXT_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_CONTEXT_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashContextSetMaximumConcurrency,
    &PerfectHashContextGetMaximumConcurrency,
    &PerfectHashContextSetBaseOutputDirectory,
    &PerfectHashContextGetBaseOutputDirectory,
    &PerfectHashContextSelfTest,
    &PerfectHashContextSelfTestArgvW,
    &PerfectHashContextExtractSelfTestArgsFromArgvW,
    &PerfectHashContextBulkCreate,
    &PerfectHashContextBulkCreateArgvW,
    &PerfectHashContextExtractBulkCreateArgsFromArgvW,
    &PerfectHashContextTableCreate,
    &PerfectHashContextTableCreateArgvW,
    &PerfectHashContextExtractTableCreateArgsFromArgvW,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_CONTEXT, 13);

//
// PerfectHashTable
//

const PERFECT_HASH_TABLE_VTBL PerfectHashTableInterface = {
    (PPERFECT_HASH_TABLE_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_TABLE_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_TABLE_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_TABLE_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_TABLE_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashTableCreate,
    &PerfectHashTableLoad,
    &PerfectHashTableGetFlags,
    &PerfectHashTableCompile,
    &PerfectHashTableTest,
    &PerfectHashTableInsert,
    &PerfectHashTableLookup,
    &PerfectHashTableDelete,
    NULL,   // Index
    NULL,   // Hash
    NULL,   // MaskHash
    NULL,   // MaskIndex
    NULL,   // SeededHash
    NULL,   // FastIndex
    NULL,   // SlowIndex
    &PerfectHashTableGetAlgorithmName,
    &PerfectHashTableGetHashFunctionName,
    &PerfectHashTableGetMaskFunctionName,
    &PerfectHashTableGetFile,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_TABLE, 19);

//
// Rtl
//

const RTL_VTBL RtlInterface = {
    (PRTL_QUERY_INTERFACE)&ComponentQueryInterface,
    (PRTL_ADD_REF)&ComponentAddRef,
    (PRTL_RELEASE)&ComponentRelease,
    (PRTL_CREATE_INSTANCE)&ComponentCreateInstance,
    (PRTL_LOCK_SERVER)&ComponentLockServer,
    &RtlGenerateRandomBytes,
    &RtlPrintSysError,
    &RtlCreateBuffer,
    &RtlCreateMultipleBuffers,
    &RtlDestroyBuffer,
    &RtlCopyPages,
    &RtlFillPages,
    &RtlCreateRandomObjectNames,
    &RtlCreateSingleRandomObjectName,
    &RtlTryLargePageVirtualAlloc,
    &RtlTryLargePageVirtualAllocEx,
    &RtlTryLargePageCreateFileMappingW,
};
VERIFY_VTBL_SIZE(RTL, 12);

//
// Allocator
//

const ALLOCATOR_VTBL AllocatorInterface = {
    (PALLOCATOR_QUERY_INTERFACE)&ComponentQueryInterface,
    (PALLOCATOR_ADD_REF)&ComponentAddRef,
    (PALLOCATOR_RELEASE)&ComponentRelease,
    (PALLOCATOR_CREATE_INSTANCE)&ComponentCreateInstance,
    (PALLOCATOR_LOCK_SERVER)&ComponentLockServer,
    &AllocatorMalloc,
    &AllocatorCalloc,
    &AllocatorReAlloc,
    &AllocatorReCalloc,
    &AllocatorFree,
    &AllocatorFreePointer,
    &AllocatorFreeStringBuffer,
    &AllocatorFreeUnicodeStringBuffer,
    &AllocatorAlignedMalloc,
    &AllocatorAlignedCalloc,
    &AllocatorAlignedReAlloc,
    &AllocatorAlignedReCalloc,
    &AllocatorAlignedFree,
    &AllocatorAlignedFreePointer,
    &AllocatorAlignedOffsetMalloc,
    &AllocatorAlignedOffsetCalloc,
    &AllocatorAlignedOffsetReAlloc,
    &AllocatorAlignedOffsetReCalloc,
};
VERIFY_VTBL_SIZE(ALLOCATOR, 18);

//
// PerfectHashFile
//

const PERFECT_HASH_FILE_VTBL PerfectHashFileInterface = {
    (PPERFECT_HASH_FILE_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_FILE_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_FILE_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_FILE_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_FILE_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashFileLoad,
    &PerfectHashFileCreate,
    &PerfectHashFileGetFlags,
    &PerfectHashFileGetPath,
    &PerfectHashFileGetResources,

    //
    // Begin private methods.
    //

    &PerfectHashFileClose,
    &PerfectHashFileExtend,
    &PerfectHashFileTruncate,
    &PerfectHashFileMap,
    &PerfectHashFileUnmap,
    &PerfectHashFileScheduleRename,
    &PerfectHashFileDoRename,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_FILE, 12);

//
// PerfectHashPath
//

const PERFECT_HASH_PATH_VTBL PerfectHashPathInterface = {
    (PPERFECT_HASH_PATH_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_PATH_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_PATH_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_PATH_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_PATH_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashPathCopy,
    &PerfectHashPathCreate,
    &PerfectHashPathReset,
    &PerfectHashPathGetParts,

    //
    // Begin private methods.
    //

    &PerfectHashPathExtractParts,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_PATH, 5);

//
// PerfectHashDirectory
//

const PERFECT_HASH_DIRECTORY_VTBL PerfectHashDirectoryInterface = {
    (PPERFECT_HASH_DIRECTORY_QUERY_INTERFACE)&ComponentQueryInterface,
    (PPERFECT_HASH_DIRECTORY_ADD_REF)&ComponentAddRef,
    (PPERFECT_HASH_DIRECTORY_RELEASE)&ComponentRelease,
    (PPERFECT_HASH_DIRECTORY_CREATE_INSTANCE)&ComponentCreateInstance,
    (PPERFECT_HASH_DIRECTORY_LOCK_SERVER)&ComponentLockServer,
    &PerfectHashDirectoryOpen,
    &PerfectHashDirectoryCreate,
    &PerfectHashDirectoryGetFlags,
    &PerfectHashDirectoryGetPath,

    //
    // Begin private methods.
    //

    &PerfectHashDirectoryClose,
    &PerfectHashDirectoryScheduleRename,
    &PerfectHashDirectoryDoRename,
    &PerfectHashDirectoryAddFile,
    &PerfectHashDirectoryRemoveFile,
};
VERIFY_VTBL_SIZE(PERFECT_HASH_DIRECTORY, 4 + 5);

//
// GuardedList
//

const GUARDED_LIST_VTBL GuardedListInterface = {
    (PGUARDED_LIST_QUERY_INTERFACE)&ComponentQueryInterface,
    (PGUARDED_LIST_ADD_REF)&ComponentAddRef,
    (PGUARDED_LIST_RELEASE)&ComponentRelease,
    (PGUARDED_LIST_CREATE_INSTANCE)&ComponentCreateInstance,
    (PGUARDED_LIST_LOCK_SERVER)&ComponentLockServer,
    &GuardedListIsEmpty,
    &GuardedListQueryDepth,
    &GuardedListInsertHead,
    &GuardedListInsertTail,
    &GuardedListAppendTail,
    &GuardedListRemoveHead,
    &GuardedListRemoveTail,
    &GuardedListRemoveEntry,
    &GuardedListRemoveHeadEx,
    &GuardedListReset,
};
VERIFY_VTBL_SIZE(GUARDED_LIST, 10);

//
// TSX versions of the GuardedList interface.  See dllmain.c for more info.
//

const GUARDED_LIST_VTBL GuardedListTsxInterface = {
    (PGUARDED_LIST_QUERY_INTERFACE)&ComponentQueryInterface,
    (PGUARDED_LIST_ADD_REF)&ComponentAddRef,
    (PGUARDED_LIST_RELEASE)&ComponentRelease,
    (PGUARDED_LIST_CREATE_INSTANCE)&ComponentCreateInstance,
    (PGUARDED_LIST_LOCK_SERVER)&ComponentLockServer,
    &GuardedListIsEmptyTsx,
    &GuardedListQueryDepthTsx,
    &GuardedListInsertHeadTsx,
    &GuardedListInsertTailTsx,
    &GuardedListAppendTailTsx,
    &GuardedListRemoveHeadTsx,
    &GuardedListRemoveTailTsx,
    &GuardedListRemoveEntryTsx,
    &GuardedListRemoveHeadExTsx,

    //
    // N.B. We don't have a TSX version for Reset() as it's not really
    //      necessary (because it shouldn't be called in contention).
    //

    &GuardedListReset,
};
VERIFY_VTBL_SIZE(GUARDED_LIST, 10);

//
// Graph
//

GRAPH_VTBL GraphInterface = {
    (PGRAPH_QUERY_INTERFACE)&ComponentQueryInterface,
    (PGRAPH_ADD_REF)&ComponentAddRef,
    (PGRAPH_RELEASE)&ComponentRelease,
    (PGRAPH_CREATE_INSTANCE)&ComponentCreateInstance,
    (PGRAPH_LOCK_SERVER)&ComponentLockServer,
    &GraphSetInfo,
    &GraphEnterSolvingLoop,
    &GraphLoadInfo,
    &GraphReset,
    &GraphLoadNewSeeds,
    &GraphSolve,
    &GraphVerify,
    &GraphCalculateAssignedMemoryCoverage,
    &GraphCalculateAssignedMemoryCoverageForKeysSubset,
    &GraphRegisterSolved,
};
VERIFY_VTBL_SIZE(GRAPH, 10);

//
// Interface array.
//

const VOID *ComponentInterfaces[] = {
    NULL,

    &IUnknownInterface,
    &IClassFactoryInterface,
    &PerfectHashKeysInterface,
    &PerfectHashContextInterface,
    &PerfectHashTableInterface,
    &RtlInterface,
    &AllocatorInterface,
    &PerfectHashFileInterface,
    &PerfectHashPathInterface,
    &PerfectHashDirectoryInterface,
    &GuardedListInterface,
    &GraphInterface,

    NULL,
};
VERIFY_ARRAY_SIZE(ComponentInterfaces);

const PCOMPONENT_INITIALIZE ComponentInitializeRoutines[] = {
    NULL,

    NULL, // IUnknown
    NULL, // IClassFactory

    (PCOMPONENT_INITIALIZE)&PerfectHashKeysInitialize,
    (PCOMPONENT_INITIALIZE)&PerfectHashContextInitialize,
    (PCOMPONENT_INITIALIZE)&PerfectHashTableInitialize,
    (PCOMPONENT_INITIALIZE)&RtlInitialize,
    (PCOMPONENT_INITIALIZE)&AllocatorInitialize,
    (PCOMPONENT_INITIALIZE)&PerfectHashFileInitialize,
    (PCOMPONENT_INITIALIZE)&PerfectHashPathInitialize,
    (PCOMPONENT_INITIALIZE)&PerfectHashDirectoryInitialize,
    (PCOMPONENT_INITIALIZE)&GuardedListInitialize,
    (PCOMPONENT_INITIALIZE)&GraphInitialize,

    NULL,
};
VERIFY_ARRAY_SIZE(ComponentInitializeRoutines);

const PCOMPONENT_RUNDOWN ComponentRundownRoutines[] = {
    NULL,

    NULL, // IUnknown
    NULL, // IClassFactory

    (PCOMPONENT_RUNDOWN)&PerfectHashKeysRundown,
    (PCOMPONENT_RUNDOWN)&PerfectHashContextRundown,
    (PCOMPONENT_RUNDOWN)&PerfectHashTableRundown,
    (PCOMPONENT_RUNDOWN)&RtlRundown,
    (PCOMPONENT_RUNDOWN)&AllocatorRundown,
    (PCOMPONENT_RUNDOWN)&PerfectHashFileRundown,
    (PCOMPONENT_RUNDOWN)&PerfectHashPathRundown,
    (PCOMPONENT_RUNDOWN)&PerfectHashDirectoryRundown,
    (PCOMPONENT_RUNDOWN)&GuardedListRundown,
    (PCOMPONENT_RUNDOWN)&GraphRundown,

    NULL,
};
VERIFY_ARRAY_SIZE(ComponentRundownRoutines);

//
// Include source files for any strings that are referenced in more than one
// compilation unit.
//

#include "CompiledPerfectHashTableRoutinesPre_CSource_RawCString.h"
#include "CompiledPerfectHashTableRoutines_CSource_RawCString.h"
#include "CompiledPerfectHashTableRoutinesPost_CSource_RawCString.h"

// vim:set ts=8 sw=4 sts=4 tw=80 expandtab nowrap                              :
