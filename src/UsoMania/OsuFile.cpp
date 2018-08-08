#include "OsuFile.h"

namespace UsoMania
{
    const byte *nextline(const byte *str)
    {
        while(0 != str)
        {
            if('\n' == *str)
            {
                if(0 == *(str+1))
                    return nullptr;
                else
                    return (str+1);
            }
            ++str;
        }
        return nullptr;
    }
    
    // OsuFile class
    
    const char* const OsuFile::section_labels[8] =
    {
        "[General]",
        "[Editor]",
        "[Metadata]",
        "[Difficulty]",
        "[Events]",
        "[Timing Points]",
        "[Colours]",
        "[HitObjects]"
    };
    
    const size_t OsuFile::section_label_lens[8] =
    { 9, 8, 10, 12, 8, 15, 9, 12 };
    
    OsuFile::OsuFile()
    :RhythmMap()
    {
        ;
    }
    
    OsuFile::~OsuFile()
    {
        ;
    }
    
    Bool OsuFile::Open(const FileIO &file, const char8 *folder,
                       StackAllocator &allocator)
    {
        if(false == file.IsAvailable())
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR, ("FileIO is not available"));
            return RET_INVALID_PARAMS;
        }
        
        memcpy(m_meta.folder, folder, strlen(folder));
        
        const byte *buffer = file.GetBuffer();
        
        const byte *iter = buffer;
        const byte *next = nullptr;
        
        // find [General]
        next = strstr(iter, section_labels[0]);
        if(nullptr == next)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR,
                      ("Osu file does not have %s Label", section_labels[0]));
            return RET_FAILED;
        }
        iter = next;
        
        // check audiofilename
        next = strstr(iter, "AudioFilename");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "AudioFilename: %127[^\n\t\r]c", m_meta.audiofile);
        }
        // check audioleadin
        next = strstr(iter, "AudioLeadIn");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "AudioLeadIn: %u", &m_meta.audioleadin);
        }
        // check mode
        next = strstr(iter, "Mode");
        if(nullptr != next)
        {
            iter = next;
            Uint32 mode = 0;
            sscanf(iter, "Mode: %u", &mode);
            if(3 != mode)
            {
                DEBUG_LOG(OsuFile::Open, LOG_ERROR, ("This Osu file is not for osu!mania"));
                return RET_FAILED;
            }
        }
        
        // find [Metadata]
        next = strstr(iter, section_labels[2]);
        if(nullptr == next)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR,
                      ("Osu file does not have %s Label", section_labels[2]));
            return RET_FAILED;
        }
        iter = next;
        
        // read [Metadata]
        // read Title
        next = strstr(iter, "Title");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "Title:%63[^\n\t]c", m_meta.title);
        }
        // read Artist
        next = strstr(iter, "Artist");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "Artist:%63[^\n\t]c", m_meta.artist);
        }
        // read creator
        next = strstr(iter, "Creator");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "Creator:%63[^\n\t]c", m_meta.creator);
        }
        // read version
        next = strstr(iter, "Version");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "Version:%31[^\n\t]c", m_meta.version);
        }
        // read source
        next = strstr(iter, "Source");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "Source:%63[^\n\t]c", m_meta.source);
        }
        
        // find [Difficulty]
        next = strstr(iter, section_labels[3]);
        if(nullptr == next)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR,
                      ("Osu file does not have %s Label", section_labels[3]));
            return RET_FAILED;
        }
        iter = next;
        
        // read linenum
        next = strstr(iter, "CircleSize:");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "CircleSize:%u", &m_numlines);
        }
        
        // find [Events]
        next = strstr(iter, section_labels[4]);
        if(nullptr == next)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR,
                      ("Osu file does not have %s Label", section_labels[4]));
            return RET_FAILED;
        }
        iter = next;
        
        // read background
        next = strstr(iter, "0,0,\"");
        if(nullptr != next)
        {
            iter = next;
            sscanf(iter, "0,0,\"%31[^\n\t\"]c", m_meta.bgname);
        }
        
        
        // find [Hit Objects]
        next = strstr(iter, section_labels[7]);
        if(nullptr == next)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR,
                      ("Osu file does not have %s Label", section_labels[7]));
            return RET_FAILED;
        }
        iter = next;
        
        // read [Hit Objects]
        // calc note count
        next = nextline(iter);
        const byte *mark = next;
        Uint32 count = 0;
        while(nullptr != next)
        {
            iter = next;
            next = nextline(iter);
            ++count;
        }
        next = iter = mark;
        
        // reserve memory for notes
        m_notes = (Note*)allocator.Alloc(sizeof(Note)*count);
        if(nullptr == m_notes)
        {
            DEBUG_LOG(OsuFile::Open, LOG_ERROR, ("Can't Allocate memory for Notes"));
            return RET_FAILED;
        }
        m_numnotes = count;
        
        // read notes
        count = 0;
        Uint32 x=0, y=0, time=0, type=0, hitsound=0, endtime=0;
        constexpr Uint32 hold_mask = 0x80;
        while(nullptr != next)
        {
            iter = next;
            sscanf(iter, "%u,%u,%u,%u,%u", &x, &y, &time, &type, &hitsound);
            m_notes[count].timing = time;
            m_notes[count].endtime = 0;
            if(type&hold_mask)
            {
                sscanf(iter, "%u,%u,%u,%u,%u,%u", &x, &y, &time, &type, &hitsound, &endtime);
                m_notes[count].endtime = endtime;
            }
            m_notes[count].hitsound = hitsoundConvert(hitsound);
            m_notes[count].line = XToLine(x, m_numlines);
            next = nextline(iter);
            ++count;
        }
        
        return RET_SUCC;
    }
    
    Uint8 XToLine(Uint32 x, Uint32 numline)
    {
        Uint32 each = 512/numline;
        Uint32 now = 0;
        Uint8 line = 0;
        while(now < 512)
        {
            if(x < now+each)
                return line;
            now += each;
            ++line;
        }
        
        return 255;
    }
    
    Uint8 hitsoundConvert(Uint32 x)
    {
        if(x&1)
            return 0;
        else if(x&2)
            return 1;
        else if(x&4)
            return 2;
        else if(x&8)
            return 3;
        else
            return 0;
    }
    
}














