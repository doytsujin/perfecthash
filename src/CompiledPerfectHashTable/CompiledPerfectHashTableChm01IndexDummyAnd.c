
DECLARE_INDEX_ROUTINE()
{
    CPHINDEX Index;
    CPHDKEY Vertex1;
    CPHDKEY Vertex2;
    CPHDKEY MaskedLow;
    CPHDKEY MaskedHigh;
    CPHDKEY DownsizedKey;

    DownsizedKey = DOWNSIZE_KEY(Key);
    Vertex1 = _mm_crc32_u32(SEED1, DownsizedKey);
    Vertex2 = _mm_crc32_u32(SEED2, _rotl(DownsizedKey, (BYTE)(SEED3 & 0x1f)));

    MaskedLow = Vertex1 & HASH_MASK;
    MaskedHigh = Vertex2 & HASH_MASK;

    Vertex1 = TABLE_DATA[MaskedLow];
    Vertex2 = TABLE_DATA[MaskedHigh];

    Index = (CPHINDEX)((Vertex1 + Vertex2) & INDEX_MASK);

    return Index;
}
