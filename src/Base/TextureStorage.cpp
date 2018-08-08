#include "TextureStorage.h"
#include <FreeImage.h>
#include <cstring>

namespace GamesEngine
{
    GLuint LoadTexture(const char8 *filename,
                       GLint *w, GLint *h,
                       GLenum min_filter, GLenum mag_filter)
    {
        // check format
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
        if(-1 == format)
        {
            DEBUG_LOG(LoadTexture(), LOG_ERROR,
                      ("invalid file format, filename: %s", filename));
            return RET_FAILED;
        }
        if(FIF_UNKNOWN == format)
        {
            DEBUG_LOG(LoadTexture(), LOG_WARNING,
                      ("Unknown file format, try to get from file extension, "
                       "filename: %s", filename));
            format = FreeImage_GetFIFFromFilename(filename);
            if(false == FreeImage_FIFSupportsReading(format))
            {
                DEBUG_LOG(LoadTexture(), LOG_ERROR,
                          ("detected format can't be read by FreeImage lib, "
                           "filename: %s", filename));
                return RET_FAILED;
            }
        }
        
        // load image
        FIBITMAP *bitmap = FreeImage_Load(format, filename);
        
        // convert to 32bit image
        int32 bits_per_pixel = FreeImage_GetBPP(bitmap);
        FIBITMAP *bitmap32 = nullptr;
        if(32 == bits_per_pixel)
        {
            bitmap32 = bitmap;
        }
        else
        {
            bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
        }
        
        // get width, height
        int32 width  = FreeImage_GetWidth(bitmap32);
        int32 height = FreeImage_GetHeight(bitmap32);
        if(nullptr != w)
            *w = width;
        if(nullptr != h)
            *h = height;
        
        // make OpenGL Texture
        GLubyte *tex_data = FreeImage_GetBits(bitmap32);
        GLuint tmp_texid = 0;
        glGenTextures(1, &tmp_texid);
        glBindTexture(GL_TEXTURE_2D, tmp_texid);
        
        // construct texture
        glTexImage2D(GL_TEXTURE_2D,
                     0,                     // mipmap level
                     GL_RGBA,               // internal format
                     width,
                     height,
                     0,                     // border (always 0)
                     GL_BGRA,               // data format
                     GL_UNSIGNED_BYTE,      // type of data
                     tex_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        
        // check if need to create mipmaps
        if(GL_NEAREST_MIPMAP_LINEAR == min_filter ||
           GL_NEAREST_MIPMAP_NEAREST == min_filter ||
           GL_NEAREST_MIPMAP_LINEAR == mag_filter ||
           GL_NEAREST_MIPMAP_NEAREST == mag_filter )
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        
        // check for
        GLenum err = glGetError();
        if(GL_NO_ERROR != err)
        {
            DEBUG_LOG(LoadTexture(), LOG_ERROR, ("error with loading texture"));
            switch (err)
            {
                case GL_INVALID_ENUM:
                    DEBUG_LOG(LoadTexture(), LOG_INFO, ("Invalid Enum"));
                    break;
                    
                case GL_INVALID_VALUE:
                    DEBUG_LOG(LoadTexture(), LOG_INFO, ("Invalid Value"));
                    break;
                    
                case GL_INVALID_OPERATION:
                    DEBUG_LOG(LoadTexture(), LOG_INFO, ("Invalid "));
                    break;
                default:
                    DEBUG_LOG(LoadTexture(), LOG_INFO, ("Unrecognised GLenum"));
                    break;
            }
            
            DEBUG_LOG(LoadTexture(), LOG_INFO, ("See https://www.opengl.org/sdk/docs/"
                                                "man/html/glTexImage2D.xhtml for "
                                                "further details."));
        }
        
        FreeImage_Unload(bitmap32);
        if(32 != bits_per_pixel)
        {
            FreeImage_Unload(bitmap);
        }
        
        return tmp_texid;
    }
    
    GLuint LoadTexture(const char8 *filename,
                       GLenum min_filter,
                       GLenum mag_filter)
    {
        return LoadTexture(filename, nullptr, nullptr, min_filter, mag_filter);
    }
    
    
    // IMGStorage class
    
    TextureStorage::TextureStorage()
    :m_len(0), m_textures(nullptr)
    {
        ;
    }
    
    TextureStorage::~TextureStorage()
    {
        Clear();
    }
    
    void TextureStorage::AssignMemory(void *memory, Uint32 len)
    {
        m_len = len;
        m_textures = (TextureStorage::Val**)memory;
        memset(m_textures, 0, sizeof(Val*)*m_len);
    }
    
    int32 TextureStorage::Register(TextureStorage::Val *tex, Uint32 hash)
    {
        if(nullptr == m_textures)
        {
            DEBUG_LOG(IMGStorage::Register(), LOG_ERROR, ("storage is null"));
            return RET_FAILED;
        }
        
        if(nullptr == tex)
        {
            DEBUG_LOG(IMGStorage::Register(), LOG_ERROR, ("textuer pointer is null"));
            return RET_INVALID_PARAMS;
        }
        if(0 == tex->id)
        {
            DEBUG_LOG(IMGStorage::Register(), LOG_ERROR, ("textuer id is invalid"));
            return RET_INVALID_PARAMS;
        }
        
        Uint32 idx = hash%m_len;
        
        if(nullptr != m_textures[idx])
        {
            DEBUG_LOG(IMGStorage::Register(), LOG_ERROR, ("exist same hash"));
            return RET_FAILED;
        }
        
        m_textures[idx] = tex;
        return RET_SUCC;
    }
    
    GLuint TextureStorage::DeRegister(Uint32 hash)
    {
        Uint32 idx = hash%m_len;
        if(nullptr != m_textures[idx])
        {
            GLuint result = m_textures[idx]->id;
            m_textures[idx] = nullptr;
            return result;
        }
        else
            return 0;
    }
    
    void TextureStorage::Clear()
    {
        if(nullptr != m_textures)
        {
            for(Uint32 i=0; i<m_len; ++i)
            {
                if(nullptr != m_textures[i])
                {
                    if(0 != m_textures[i]->id)
                        glDeleteTextures(1, &(m_textures[i]->id));
                    m_textures[i] = nullptr;
                }
            }
        }
    }
    
    const TextureStorage::Val *TextureStorage::operator[](Uint32 hash)const
    {
        Uint32 idx = hash%m_len;
        return m_textures[idx];
    }
    
    TextureStorage &TextureStorage::GetGlobal()
    {
        static TextureStorage global;
        return global;
    }
}














