#ifndef GamesEngineAllocator_hpp
#define GamesEngineAllocator_hpp

#include "types.h"

namespace GamesEngine
{
    class StackAllocator
    {
    public:
        typedef Uint64 Marker;
    public:
        explicit StackAllocator();
        
        explicit StackAllocator(const size_t size);
        
        ~StackAllocator();
        
        // if failed to allocate, return nullptr
        void *Alloc(const size_t size);
        template <typename T>
        inline T *Alloc() { return (T*)Alloc(sizeof(T)); }
        template <typename T>
        inline T *Alloc(const size_t n) { return (T*)Alloc(sizeof(T)*n); }
        
        // if failed to free, return false
        bool FreeWithMarker(Marker marker);
        
        // if failed to allocate, return false
        // if there is any chunk allocated, return false
        bool ReAllocBuffer(const size_t size);
        
        // clear chunks
        void Clear();
        
        // get functions
        Marker GetTopMarker()const
        {
            return m_top;
        }
        
        size_t GetMaxSize()const
        {
            return m_size;
        }
        
    private:
        Marker m_top;            // to mark top of stack
        size_t m_size;           // size of buffer
        void *m_buffer;          // memory buffer
        
        StackAllocator(const StackAllocator &other) = delete;
        StackAllocator &operator=(const StackAllocator &other) = delete;
        
    public:
        // singleton
        static StackAllocator &GetGlobal()
        {
            static StackAllocator global;
            return global;
        }
    };
    
    template <class T>
    class PoolAllocator
    {
    public:
        union Node
        {
            T data;
            Node *next;
        };
    public:
        explicit PoolAllocator()
        :m_max_elements(0), m_mem_size(0),
        m_original_head(nullptr),m_head(nullptr), m_tail(nullptr)
        {
            ;
        }
        
        // allocate memory with specific StackAllocator object
        explicit PoolAllocator(StackAllocator &allocator, size_t max_elements)
        :m_max_elements(max_elements), m_mem_size(max_elements*sizeof(Node)),
        m_head(nullptr), m_tail(nullptr)
        {
            // allocate memory
            m_head = allocator.Alloc(m_mem_size);
            if(nullptr == m_head)
            {
                // handle error
                printf("failed to alloc memory");
                return;
            }
            
            // make list
            Node *iter = m_head;
            for(size_t i=0; i<max_elements; ++i)
            {
                iter->next = (iter + 1);
                iter = iter->next;
            }
            m_tail = (m_head + max_elements);
            m_tail->next = nullptr;
        }
        
        ~PoolAllocator()
        {
            ;
        }
        
        T *Alloc()
        {
            // check if there is enough memory
            if(m_head == nullptr)
            {
                // handle error
                printf("failed to allocate\n");
            }
            
            // allocate pointer
            T *result = &(m_head->data);
            m_head = m_head->next;
            return result;
        }
        
        void Free(T *ptr)
        {
            Node *node = (Node*)ptr;
            // check if ptr was allocated by this allocator
            if((node < m_original_head)
               || (node > (m_original_head + m_max_elements)))
            {
                // handle error
                printf("failed to free\n");
                return;
            }
            
            // free pointer
            m_tail->next = node;
            node->next = nullptr;
        }
        
        // get functions
        size_t GetNumMaxElements()const
        {
            return m_max_elements;
        }
        
        size_t GetMemorySize()const
        {
            return m_mem_size;
        }
        
    protected:
        size_t m_max_elements;       // max number of elements
        size_t m_mem_size;           // size of pool(byte)
        Node *m_original_head;       // original head pos
        Node *m_head;                // head of list(pool)
        Node *m_tail;                // tail of list(pool)
    };
}

#endif /* Allocator_hpp */
