#include "ShaderProgram.h"

namespace GamesEngine
{
    // ShaderProgram class
    
    ShaderProgram::ShaderProgram()
    :m_program(0), m_vert(0), m_frag(0), m_uniform_locs()
    {
        ;
    }
    
    ShaderProgram::~ShaderProgram()
    {
        Release();
    }
    
    int32 ShaderProgram::Init(const FileIO &vert, const FileIO &frag)
    {
        if(0 != m_program)
        {
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("already be initted"));
            return RET_FAILED;
        }
        if(false == vert.IsAvailable())
        {
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("vertex shader source is not available"));
            return RET_INVALID_PARAMS;
        }
        if(false == frag.IsAvailable())
        {
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("fragment shader source is not available"));
            return RET_INVALID_PARAMS;
        }
        
        int32 status_vert=GL_TRUE, status_frag=GL_TRUE, status_link=GL_TRUE;
        int32 log_len = 0;
        
        // compile vertex shader
        m_vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vert, 1, vert.GetBufferPointer(), 0);
        glCompileShader(m_vert);
        
        glGetShaderiv(m_vert, GL_COMPILE_STATUS, &status_vert);
        if(GL_FALSE == status_vert)
        {
            glGetShaderiv(m_vert, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetShaderInfoLog(m_vert, log_len, &log_len, info);
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("failed compiling vertex shader, Info Log :"));
            printf("%s\n\n", info);
            free(info);
            return RET_FAILED;
        }
        
        // compile fragment shader
        m_frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_frag, 1, frag.GetBufferPointer(), 0);
        glCompileShader(m_frag);
        
        glGetShaderiv(m_frag, GL_COMPILE_STATUS, &status_frag);
        if(GL_FALSE == status_frag)
        {
            glGetShaderiv(m_frag, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetShaderInfoLog(m_frag, log_len, &log_len, info);
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("failed compiling fragment shader, Info Log :"));
            printf("%s\n\n", info);
            free(info);
            glDeleteShader(m_vert);
            m_vert = m_frag = 0;
            return RET_FAILED;
        }
        
        // link shader
        m_program = glCreateProgram();
        glAttachShader(m_program, m_vert);
        glAttachShader(m_program, m_frag);
        glLinkProgram(m_program);
        
        glGetProgramiv(m_program, GL_LINK_STATUS, &status_link);
        if(GL_FALSE == status_link)
        {
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_len);
            GLchar *info = (GLchar*)malloc(log_len);
            glGetProgramInfoLog(m_program, log_len, &log_len, info);
            DEBUG_LOG(ShaderProgram::Init(), LOG_ERROR,
                      ("failed link shaders, Info Log :"));
            printf("%s\n\n", info);
            free(info);
            glDeleteShader(m_vert);
            glDeleteShader(m_frag);
            m_program = m_vert = m_frag = 0;
            return RET_FAILED;
        }
        
        m_uniform_locs[0] = glGetUniformLocation(m_program, "vp");
        m_uniform_locs[1] = glGetUniformLocation(m_program, "tex");
        
        return RET_SUCC;
    }
    
    void ShaderProgram::Release()
    {
        if(0 != m_program)
        {
            glDeleteShader(m_vert);
            glDeleteShader(m_frag);
            glDeleteProgram(m_program);
            m_vert = m_frag = m_program = 0;
        }
    }
    
    GLuint ShaderProgram::GetProgram()const
    {
        return m_program;
    }
    
    GLuint ShaderProgram::GetViewProjectionLocation()const
    {
        return m_uniform_locs[0];
    }
    
    GLuint ShaderProgram::GetTextureLocation()const
    {
        return m_uniform_locs[1];
    }
    
    
    // ShaderStorage class
    ShaderStorage::ShaderStorage()
    :m_len(0), m_shaders(nullptr)
    {
        ;
    }
    
    ShaderStorage::~ShaderStorage()
    {
        Clear();
    }
    
    void ShaderStorage::AssignMemory(void *memory, Uint32 len)
    {
        m_len = len;
        m_shaders = (ShaderProgram**)memory;
    }
    
    int32 ShaderStorage::Register(ShaderProgram *shader, Uint32 hash)
    {
        if(nullptr == m_shaders)
        {
            DEBUG_LOG(ShaderStorage::Register(), LOG_ERROR, ("storage is null"));
            return RET_FAILED;
        }
        
        if(nullptr == shader)
        {
            DEBUG_LOG(ShaderStorage::Register(), LOG_ERROR, ("shader pointer is null"));
            return RET_INVALID_PARAMS;
        }
        if(0 == shader->GetProgram())
        {
            DEBUG_LOG(ShaderStorage::Register(), LOG_ERROR, ("program id is invalid"));
            return RET_INVALID_PARAMS;
        }
        
        Uint32 idx = hash%m_len;
        
        if(nullptr != m_shaders[idx])
        {
            DEBUG_LOG(ShaderStorage::Register(), LOG_ERROR, ("exist same hash"));
            return RET_FAILED;
        }
        
        m_shaders[idx] = shader;
        return RET_SUCC;
    }
    
    ShaderProgram *ShaderStorage::DeRegister(Uint32 hash)
    {
        Uint32 idx = hash%m_len;
        ShaderProgram *result = m_shaders[idx];
        m_shaders[idx] = nullptr;
        return result;
    }
    
    void ShaderStorage::Clear()
    {
        for(Uint32 i=0; i<m_len; ++i)
        {
            if(nullptr != m_shaders[i])
            {
                m_shaders[i]->~ShaderProgram();
                m_shaders[i] = nullptr;
            }
        }
    }
    
    const ShaderProgram *ShaderStorage::operator[](Uint32 hash)const
    {
        Uint32 idx = hash%m_len;
        return m_shaders[idx];
    }
    
    ShaderStorage &ShaderStorage::GetGlobal()
    {
        static ShaderStorage global;
        return global;
    }
}










