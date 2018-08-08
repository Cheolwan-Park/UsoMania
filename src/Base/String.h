#ifndef GamesEngineString_h
#define GamesEngineString_h

#include "types.h"
#include "Allocator.h"
#include <memory.h>
#include <stdarg.h>

namespace GamesEngine
{
    template <size_t MaxSize>
    class String
    {
    public:
        String()
        :m_str(), m_size(0)
        {
            memset(m_str, 0, MaxSize);
        }
        
        String(const char *str)
        :m_str(), m_size(strlen(str))
        {
            memset(m_str, 0, MaxSize);
            for(size_t i=0; i<m_size; ++i)
            {
                m_str[i] = str[i];
            }
        }
        
        String(const char *format, ...)
        :m_str(), m_size(0)
        {
            memset(m_str, 0, MaxSize);
            va_list ap;
            va_start(ap, format);
            m_size = vsnprintf(m_str, MaxSize, format, ap);
            va_end(ap);
        }
        
        String(const char *format, va_list ap)
        :m_str(), m_size(0)
        {
            memset(m_str, 0, MaxSize);
            m_size = vsnprintf(m_str, MaxSize, format, ap);
        }
        
        String(const String &other)
        :m_str(), m_size(other.m_size)
        {
            memset(m_str, 0, MaxSize);
            for(size_t i=0; i<m_size; ++i)
            {
                this->m_str[i] = other.m_str[i];
            }
        }
        
        // member functions
        void clear()
        {
            memset(m_str, 0, MaxSize);
            m_size = 0;
        }
        
        void SPrint(const char *format, ...)
        {
            memset(m_str, 0, MaxSize);
            va_list ap;
            va_start(ap, format);
            m_size = vsnprintf(m_str, MaxSize, format, ap);
            va_end(ap);
        }
        
        void SPrint(const char *format, va_list ap)
        {
            memset(m_str, 0, MaxSize);
            m_size = vsnprintf(m_str, MaxSize, format, ap);
        }
        
        String &Append(const String &other)
        {
            size_t sizesum = this->m_size + other.m_size;
            if(sizesum > MaxSize)
            {
                DEBUG_LOG(String::Append(), LOG_ERROR, ("can't append due to capacity"));
                return (*this);
            }
            size_t other_i = 0;
            for(size_t i=m_size; i<sizesum; ++i)
            {
                this->m_str[i] = other.m_str[other_i];
                ++other_i;
            }
            this->m_size = sizesum;
            return (*this);
        }
        
        String &Append(const char *str)
        {
            size_t sizesum = m_size + strlen(str);
            if(sizesum > MaxSize)
            {
                DEBUG_LOG(String::Append(), LOG_ERROR, ("can't append due to capacity"));
                return (*this);
            }
            size_t other_i = 0;
            for(size_t i=m_size; i<sizesum; ++i)
            {
                this->m_str[i] = str[other_i];
                ++other_i;
            }
            m_size = sizesum;
            return (*this);
        }
        
        char8 At(unsigned int idx)const
        {
            if(idx >= m_size)
            {
                DEBUG_LOG(String::At(), LOG_ERROR, ("index is out of range"));
                return 0;
            }
            return m_str[idx];
        }
        
        // get functions
        const char8 *C_Str()const
        {
            return m_str;
        }
        
        size_t GetMaxSize()const
        {
            return MaxSize;
        }
        size_t GetSize()const
        {
            return m_size;
        }
        
        String &operator=(const String &other)
        {
            this->m_size = other.m_size;
            for(size_t i=0; i<this->m_size; ++i)
            {
                this->m_str[i] = other.m_str[i];
            }
            return (*this);
        }
        
        String &operator+=(const String &other)
        {
            size_t sizesum = this->m_size + other.m_size;
            if(sizesum > MaxSize)
            {
                DEBUG_LOG(String::Operator+(), LOG_ERROR, ("can't append due to capacity"));
                return (*this);
            }
            size_t other_i = 0;
            for(size_t i=m_size; i<sizesum; ++i)
            {
                this->m_str[i] = other.m_str[other_i];
                ++other_i;
            }
            this->m_size = sizesum;
            return (*this);
        }
        
        String &operator+=(const char *str)
        {
            size_t sizesum = m_size + strlen(str);
            if(sizesum > MaxSize)
            {
                DEBUG_LOG(String::Operator+(), LOG_ERROR, ("can't append due to capacity"));
                return (*this);
            }
            size_t other_i = 0;
            for(size_t i=m_size; i<sizesum; ++i)
            {
                this->m_str[i] = str[other_i];
                ++other_i;
            }
            m_size = sizesum;
            return (*this);
        }
        
        char8 &operator[](Uint32 idx)
        {
            if(idx >= m_size)
            {
                DEBUG_LOG(String::At(), LOG_ERROR, ("index is out of range"));
            }
            return m_str[idx];

        }
        
        const char8 &operator[](Uint32 idx)const
        {
            if(idx >= m_size)
            {
                DEBUG_LOG(String::At(), LOG_ERROR, ("index is out of range"));
            }
            return m_str[idx];
        }
        
        String operator+(const String &other)
        {
            String result(*this);
            result += other;
            return result;
        }
        
        bool operator==(const String &other)
        {
            if(this->m_size != other.m_size)
            {
                return false;
            }
            else
            {
                for(size_t i=0; i<this->m_size; ++i)
                {
                    if(this->m_str[i] != other.m_str[i])
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        
        bool operator!=(const String &other)
        {
            if(this->m_size != other.m_size)
            {
                return true;
            }
            else
            {
                for(size_t i=0; i<this->m_size; ++i)
                {
                    if(this->m_str[i] == other.m_str[i])
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        
    protected:
        char8  m_str[MaxSize];
        size_t m_size;
    };
    
    class StringID
    {
    private:
        StringID() = delete;
        
    public:
        explicit StringID(const char *str, size_t length)
        :m_id(0)
        {
            m_id = 0;
            for(size_t i=0; i<length; ++i)
            {
                m_id = 65599 * m_id + str[i];
            }
            m_id = m_id ^ (m_id >> 16);
        }
        
        template <class StringT>
        explicit StringID(const StringT &str)
        {
            m_id = 0;
            for(size_t i=0; i<str.GetSize(); ++i)
            {
                m_id = 65599 * m_id + str[i];
            }
            m_id = m_id ^ (m_id >> 16);
        }
        
        explicit StringID(const StringID &other)
        :m_id(other.m_id)
        {
            ;
        }
        
        ~StringID()
        {
            ;
        }
        
        operator Uint32()const
        {
            return m_id;
        }
        
        bool operator!=(const StringID &other)const
        {
            if(this->m_id == other.m_id)
                return false;
            else
                return true;
        }
        
        bool operator==(const StringID &other)const
        {
            if(this->m_id == other.m_id)
                return true;
            else
                return false;
        }
        
        StringID &operator=(const StringID &other)
        {
            this->m_id = other.m_id;
            return (*this);
        }
        
    private:
        Uint32 m_id;
    };
    
    typedef String<16>  String16;
    typedef String<32>  String32;
    typedef String<48>  String48;
    typedef String<64>  String64;
    typedef String<128> String128;
    
    // compile time hash function for Create LogModuleID
    // https://gist.github.com/Lee-R/3839813
    namespace CompileTimeHash
    {
        constexpr Uint32 val_const = 0x811c9dc5;
        constexpr Uint32 prime_const = 0x1000193;
        inline constexpr Uint32 fnv1a_32(const char *const str, size_t count = val_const)
        {
            return ((count ? fnv1a_32(str, count - 1) : val_const) ^ str[count]) * prime_const;
        }
    }
    
    inline constexpr Uint32 operator""_hash(const char *const str, size_t count)
    {
        return CompileTimeHash::fnv1a_32(str, count);
    }
}



#endif
