#ifndef GamesEngineFileIO_h
#define GamesEngineFileIO_h

#include "types.h"
#include "String.h"

namespace GamesEngine
{
    class FileIO
    {
    public:
        explicit FileIO();
        
        explicit FileIO(FILE *file, void *memory, size_t size);
        
        ~FileIO();
        
        void Open(void *memory, size_t size);
        
        int32 Open(FILE *file, void *memory, size_t size);
        
        int32 Write(FILE *file)const;
        
        int32 IsAvailable()const;
        
        byte *GetBuffer();
        
        const byte *GetBuffer()const;
        
        const byte *const *GetBufferPointer()const;
        
        size_t GetSize()const;
        
    private:
        byte *m_buffer;
        size_t m_size;
    };
    
    
    // File class
    
    FILE *OpenFile(const char *filename, const char *mode);
    void CloseFile(FILE *file);
    size_t GetFileSize(FILE *file);     // this function call rewind()
}


#endif
