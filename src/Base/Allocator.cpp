#include "Allocator.h"
#include <cstdlib>
#include <cstring>

namespace GamesEngine
{
    // StackAllocator class
    
    StackAllocator::StackAllocator()
    : m_top(0), m_size(0), m_buffer(nullptr)
    {
        ;
    }
    
    StackAllocator::StackAllocator(const size_t size)
    :m_top(0), m_size(size), m_buffer(nullptr)
    {
        m_buffer = malloc(size);
        if(nullptr == m_buffer)
        {
            // handle error
            DEBUG_LOG(StackAllocator::Constructor(), LOG_ERROR,
                      ("failed to allocate buffer"));
            return;
        }
    }
    
    StackAllocator::~StackAllocator()
    {
        if(nullptr != m_buffer)
        {
            free(m_buffer);
        }
    }
    
    void *StackAllocator::Alloc(const size_t size)
    {
        // check if allocator can allocate memory offered
        if(m_top + size > m_size)
        {
            // handel error
            DEBUG_LOG(StackAllocator::Alloc(), LOG_ERROR, ("failed to allocate chunk"));
            return nullptr;
        }
        
        // allocate memory for chunk
        void *allocated_memory = (void*)((char*)m_buffer + m_top);
        memset(allocated_memory, 0, size);
        m_top += size;
        
        return allocated_memory;
    }
    
    bool StackAllocator::FreeWithMarker(Marker marker)
    {
        // check memory marker marks is allocated
        if(m_top < marker)
        {
            // handel error
            
            DEBUG_LOG(StackAllocator::FreeWithMarker(), LOG_ERROR,
                      ("failed to free"));
            return false;
        }
        
        // free memory
        m_top = marker;
        
        return true;
    }
    
    bool StackAllocator::ReAllocBuffer(const size_t size)
    {
        // reallocate buffer
        m_buffer = realloc(m_buffer, size);
        if(nullptr == m_buffer)
        {
            // handle error
            DEBUG_LOG(StackAllocator::ReAllocBuffer(), LOG_ERROR,
                      ("failed to reallocate chunk"));
            return false;
        }
        m_size = size;
        
        return true;
    }
    
    void StackAllocator::Clear()
    {
        m_top = 0;
    }
}



