#ifndef UsoManiaOsuFile_hpp
#define UsoManiaOsuFile_hpp

#include "RhythmMap.h"

namespace UsoMania
{
    class OsuFile : public RhythmMap
    {
    private:
        static const char* const section_labels[8];
        static const size_t section_label_lens[8];
        
    public:
        OsuFile();
        
        OsuFile(const OsuFile &other) = delete;
        
        virtual ~OsuFile();
        
        OsuFile &operator=(const OsuFile &other) = delete;
        
        virtual Bool Open(const FileIO &file, const char8 *folder,
                          StackAllocator &allocator);
    };
    
    Uint8 XToLine(Uint32 x, Uint32 numline);
    Uint8 hitsoundConvert(Uint32 x);
}

#endif
