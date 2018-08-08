#ifndef UsoManiaIMGStorage_hpp
#define UsoManiaIMGStorage_hpp

#include "types.h"
#include "GLFW.h"

namespace GamesEngine
{
    GLuint LoadTexture(const char8 *filename,
                       GLint *w, GLint *h,
                       GLenum min_filter = GL_LINEAR,
                       GLenum mag_filter = GL_LINEAR);
    
    GLuint LoadTexture(const char8 *filename,
                       GLenum min_filter = GL_LINEAR,
                       GLenum mag_filter = GL_LINEAR);
    
    class TextureStorage
    {
    public:
        struct Val
        {
            GLuint id;
            GLint w;
            GLint h;
        };
        
        typedef Val* Type;
        
        TextureStorage();
        
        TextureStorage(const TextureStorage &other) = delete;
        
        ~TextureStorage();
        
        TextureStorage &operator=(const TextureStorage &other) = delete;
        
        void AssignMemory(void *memory, Uint32 len);
        
        int32 Register(Val *tex, Uint32 hash);
        
        GLuint DeRegister(Uint32 hash);
        
        void Clear();
        
        const Val *operator[](Uint32 hash)const;
        
        static TextureStorage &GetGlobal();
        
    private:
        Uint32 m_len;
        Val **m_textures;
    };
}

#endif
