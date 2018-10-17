
//
// Begin CompiledPerfectHashTableRoutinesPre.c.
//

#ifdef CPH_INLINE_ROUTINES

#define INDEX_ROUTINE EXPAND_INDEX_INLINE_ROUTINE(CPH_TABLENAME)
#define LOOKUP_ROUTINE EXPAND_LOOKUP_INLINE_ROUTINE(CPH_TABLENAME)
#define INSERT_ROUTINE EXPAND_INSERT_INLINE_ROUTINE(CPH_TABLENAME)
#define DELETE_ROUTINE EXPAND_DELETE_INLINE_ROUTINE(CPH_TABLENAME)

#define DECLARE_INDEX_ROUTINE() EXPAND_INDEX_INLINE_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_LOOKUP_ROUTINE() EXPAND_LOOKUP_INLINE_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_INSERT_ROUTINE() EXPAND_INSERT_INLINE_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_DELETE_ROUTINE() EXPAND_DELETE_INLINE_ROUTINE_HEADER(CPH_TABLENAME)

#else

#define INDEX_ROUTINE EXPAND_INDEX_ROUTINE(CPH_TABLENAME)
#define LOOKUP_ROUTINE EXPAND_LOOKUP_ROUTINE(CPH_TABLENAME)
#define INSERT_ROUTINE EXPAND_INSERT_ROUTINE(CPH_TABLENAME)
#define DELETE_ROUTINE EXPAND_DELETE_ROUTINE(CPH_TABLENAME)

#define DECLARE_INDEX_ROUTINE() EXPAND_INDEX_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_LOOKUP_ROUTINE() EXPAND_LOOKUP_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_INSERT_ROUTINE() EXPAND_INSERT_ROUTINE_HEADER(CPH_TABLENAME)
#define DECLARE_DELETE_ROUTINE() EXPAND_DELETE_ROUTINE_HEADER(CPH_TABLENAME)

#endif

//
// End CompiledPerfectHashTableRoutinesPre.c.
//