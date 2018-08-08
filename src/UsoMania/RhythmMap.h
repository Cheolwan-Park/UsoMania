#ifndef UsoManiaRhythmMap_h
#define UsoManiaRhythmMap_h

#include "types.h"
#include "FileIO.h"

//using namespace GamesEngine;

namespace UsoMania
{
    using namespace GamesEngine;
    class RhythmMap
    {
    public:
        static constexpr Uint32 MAX_LINE_NUM = 11;
        struct Note
        {
            Uint32 timing;
            Uint32 endtime;     // for hold (if not 0, hitnote)
            Uint8 hitsound;
            Uint8 line;
        };
        struct MetaData
        {
            Uint32 audioleadin;
            char8 audiofile[128];
            char8 title[64];
            char8 artist[64];
            char8 creator[64];
            char8 version[32];
            char8 source[64];
            char8 folder[128];
            char8 bgname[32];
        };
    public:
        RhythmMap()
        :m_numlines(0), m_notes(nullptr), m_numnotes(0), m_meta() { ; }
        
        RhythmMap(const RhythmMap &other) = delete;
        
        virtual ~RhythmMap() { ; }
        
        RhythmMap &operator=(const RhythmMap &other) = delete;
        
        virtual Bool Open(const FileIO &file, const char8 *folder,
                          StackAllocator &allocator) = 0;
        
        Uint32 GetNumLines()const
        {
            return m_numlines;
        }
        
        const Note *GetNotes()const
        {
            return m_notes;
        }
        
        Uint32 GetNumNotes()const
        {
            return m_numnotes;
        }
        
        const MetaData &GetMetaData()const
        {
            return m_meta;
        }
        
    protected:
        Uint32 m_numlines;
        Note *m_notes;
        Uint32 m_numnotes;
        MetaData m_meta;
    };
}

#endif
