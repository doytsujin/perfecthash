/*++

Copyright (c) 2018 Trent Nelson <trent@trent.me>

Module Name:

    PerfectHashKeys.h

Abstract:

    This is the private header file for the PERFECT_HASH_KEYS
    component of the perfect hash table library.  It defines the structure,
    and function pointer typedefs for the initialize and rundown functions.

--*/

#pragma once

#include "stdafx.h"

//
// Define the PERFECT_HASH_KEYS_FLAGS structure.
//

typedef union _PERFECT_HASH_KEYS_STATE {
    struct _Struct_size_bytes_(sizeof(ULONG)) {

        //
        // When set, indicates a keys file has been loaded successfully.
        //

        ULONG Loaded:1;

        //
        // Unused bits.
        //

        ULONG Unused:31;
    };

    LONG AsLong;
    ULONG AsULong;
} PERFECT_HASH_KEYS_STATE;
C_ASSERT(sizeof(PERFECT_HASH_KEYS_STATE) == sizeof(ULONG));
typedef PERFECT_HASH_KEYS_STATE *PPERFECT_HASH_KEYS_STATE;

#define IsLoadedKeys(Keys) ((Keys)->State.Loaded == TRUE)
#define SkipKeysVerification(Keys) \
    ((Keys)->LoadFlags.SkipKeysVerification == TRUE)

#define DisableImplicitKeyDownsizing(Keys) \
    ((Keys)->LoadFlags.DisableImplicitKeyDownsizing == TRUE)

#define KeysWereDownsized(Keys) \
    ((Keys)->Flags.DownsizingOccurred == TRUE)

//
// Define the PERFECT_HASH_KEYS_STATS structure.
//

typedef struct _PERFECT_HASH_KEYS_STATS {

    PERFECT_HASH_KEYS_BITMAP KeysBitmap;

    ULONGLONG MinValue;
    ULONGLONG MaxValue;

    //
    // Histograms for capturing the count of each bit in the key set, and count
    // of popcounts (number of set bits) for each key in the set.
    //

    ULONG BitCount[64];
    ULONG PopCount[64];

} PERFECT_HASH_KEYS_STATS;
typedef PERFECT_HASH_KEYS_STATS *PPERFECT_HASH_KEYS_STATS;

//
// Define the PERFECT_HASH_KEYS structure.
//

typedef struct _Struct_size_bytes_(SizeOfStruct) _PERFECT_HASH_KEYS {

    COMMON_COMPONENT_HEADER(PERFECT_HASH_KEYS);

    //
    // Size of each key element, in bytes.  If downsizing has occurred, this
    // will reflect the downsized key size.
    //

    ULONG KeySizeInBytes;

    //
    // Original size of each key element, in bytes.  If downsizing has occurred,
    // this will reflect the original key size specified during Load(); if not,
    // it will be identical to SizeOfKeyInBytes.
    //

    ULONG OriginalKeySizeInBytes;

    //
    // Load flags provided to the Load() routine.
    //

    PERFECT_HASH_KEYS_LOAD_FLAGS LoadFlags;

    //
    // Pad out to an 8-byte boundary.
    //

    ULONG Padding;

    //
    // The bitmap used to perform key downsizing, if applicable.  This is used
    // to extract up to 32 bits from a 64 bit value via _pext_u64().
    //
    // Invariant:
    //
    //      PopulationCount64(DownsizeBitmap) <= 32
    //

    ULONGLONG DownsizeBitmap;

    //
    // Number of keys in the mapping.
    //

    ULARGE_INTEGER NumberOfElements;

    //
    // Reference to a file instance backing the keys.
    //

    PPERFECT_HASH_FILE File;

    //
    // Pointer to the base address of the key array.  If no downsizing has
    // occurred, this will be equivalent to File->BaseAddress.  Otherwise,
    // it will be heap-allocated chunk of memory.
    //

    PVOID KeyArrayBaseAddress;

    //
    // Capture simple statistics about the keys that were loaded.
    //

    PERFECT_HASH_KEYS_STATS Stats;

    //
    // Backing interface.
    //

    PERFECT_HASH_KEYS_VTBL Interface;

} PERFECT_HASH_KEYS;
typedef PERFECT_HASH_KEYS *PPERFECT_HASH_KEYS;

#define TryAcquirePerfectHashKeysLockExclusive(Keys) \
    TryAcquireSRWLockExclusive(&Keys->Lock)

#define AcquirePerfectHashKeysLockExclusive(Keys) \
    AcquireSRWLockExclusive(&Keys->Lock)

#define ReleasePerfectHashKeysLockExclusive(Keys) \
    ReleaseSRWLockExclusive(&Keys->Lock)

#define TryAcquirePerfectHashKeysLockShared(Keys) \
    TryAcquireSRWLockShared(&Keys->Lock)

#define AcquirePerfectHashKeysLockShared(Keys) \
    AcquireSRWLockShared(&Keys->Lock)

#define ReleasePerfectHashKeysLockShared(Keys) \
    ReleaseSRWLockShared(&Keys->Lock)

typedef
HRESULT
(NTAPI PERFECT_HASH_KEYS_INITIALIZE)(
    _In_ PPERFECT_HASH_KEYS Keys
    );
typedef PERFECT_HASH_KEYS_INITIALIZE
      *PPERFECT_HASH_KEYS_INITIALIZE;

typedef
VOID
(NTAPI PERFECT_HASH_KEYS_RUNDOWN)(
    _In_ _Post_ptr_invalid_ PPERFECT_HASH_KEYS Keys
    );
typedef PERFECT_HASH_KEYS_RUNDOWN
      *PPERFECT_HASH_KEYS_RUNDOWN;

typedef
HRESULT
(NTAPI PERFECT_HASH_KEYS_LOAD_STATS)(
    _In_ PPERFECT_HASH_KEYS Keys
    );
typedef PERFECT_HASH_KEYS_LOAD_STATS
      *PPERFECT_HASH_KEYS_LOAD_STATS;

typedef
_Must_inspect_result_
_Success_(return >= 0)
HRESULT
(NTAPI PERFECT_HASH_KEYS_LOAD_TABLE_SIZE)(
    _In_ PPERFECT_HASH_KEYS Keys,
    _In_ PERFECT_HASH_ALGORITHM_ID AlgorithmId,
    _In_ PERFECT_HASH_HASH_FUNCTION_ID HashFunctionId,
    _In_ PERFECT_HASH_MASK_FUNCTION_ID MaskFunctionId,
    _Inout_ PPERFECT_HASH_FILE *File,
    _Out_ PULARGE_INTEGER RequestedNumberOfTableElements
    );
typedef PERFECT_HASH_KEYS_LOAD_TABLE_SIZE *PPERFECT_HASH_KEYS_LOAD_TABLE_SIZE;

#ifndef __INTELLISENSE__
extern PERFECT_HASH_KEYS_INITIALIZE PerfectHashKeysInitialize;
extern PERFECT_HASH_KEYS_RUNDOWN PerfectHashKeysRundown;
extern PERFECT_HASH_KEYS_LOAD_STATS PerfectHashKeysLoadStats32;
extern PERFECT_HASH_KEYS_LOAD_STATS PerfectHashKeysLoadStats64;
extern PERFECT_HASH_KEYS_LOAD_TABLE_SIZE PerfectHashKeysLoadTableSize;
extern PERFECT_HASH_KEYS_LOAD PerfectHashKeysLoad;
extern PERFECT_HASH_KEYS_GET_FLAGS PerfectHashKeysGetFlags;
extern PERFECT_HASH_KEYS_GET_ADDRESS PerfectHashKeysGetAddress;
extern PERFECT_HASH_KEYS_GET_BITMAP PerfectHashKeysGetBitmap;
extern PERFECT_HASH_KEYS_GET_FILE PerfectHashKeysGetFile;
#endif

// vim:set ts=8 sw=4 sts=4 tw=80 expandtab                                     :
