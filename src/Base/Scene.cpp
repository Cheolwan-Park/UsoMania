#include "Scene.h"
#include "ShaderProgram.h"
#include <algorithm>
#include <functional>

namespace GamesEngine
{
    // Scene class
    Scene::Scene()
    {
        ;
    }
    
    Scene::~Scene()
    {
        ;
    }
    
    int32 Scene::Init()
    {
        if(RET_SUCC != LoadShaders())
            return RET_FAILED;
        if(RET_SUCC != LoadTextures())
            return RET_FAILED;
        return RET_SUCC;
    }
    
    
    // ObjectScene class
    ObjectScene::ObjectScene()
    :Scene(), m_objstorages(4,2), m_spritestorages(4,2)
    {
        ;
    }
    
    ObjectScene::~ObjectScene()
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->~ObjectStorage();
            m_objstorages[i] = nullptr;
        }
        for(size_t i=0; i<m_spritestorages.GetCount(); ++i)
        {
            m_spritestorages[i]->~DrawAbleStorage();
            m_spritestorages[i] = nullptr;
        }
    }
    
    int32 ObjectScene::Init()
    {
        auto &app = Application::Get();
        auto &allocator = app.GetAllocator();
        auto &shaders = ShaderStorage::GetGlobal();
        void *mem = nullptr;
        
        if(RET_SUCC != Scene::Init())
            return RET_FAILED;
        
        mem = allocator.Alloc(sizeof(ObjectStorage));
        ObjectStorage *global = new (mem) ObjectStorage("global"_hash, 512);
        mem = allocator.Alloc(1024*sizeof(ObjectStorage::Type));
        global->AssignMemory(mem, 1024);
        AddObjectStorage(global);
        
        mem = allocator.Alloc(sizeof(DrawAbleStorage));
        DrawAbleStorage *unlit = new (mem) DrawAbleStorage("unlit"_hash, 1024);
        mem = allocator.Alloc(512*sizeof(DrawAbleStorage::Type));
        unlit->AssignMemory(mem, 512);
        unlit->SetShader(shaders["default"_hash]);
        AddDrawableStorage(unlit);
        
        mem = allocator.Alloc(sizeof(DrawAbleStorage));
        DrawAbleStorage *transparent = new (mem) DrawAbleStorage("transparent"_hash, 128);
        mem = allocator.Alloc(512*sizeof(DrawAbleStorage::Type));
        transparent->AssignMemory(mem, 512);
        transparent->SetShader(shaders["default"_hash]);
        AddDrawableStorage(transparent);
        
        return RET_SUCC;
    }
    
    void ObjectScene::Update()
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->UpdateObjects();
        }
    }
    
    void ObjectScene::Render()
    {
        for(size_t i=0; i<m_spritestorages.GetCount(); ++i)
        {
            m_spritestorages[i]->DrawSprites();
        }
    }
    
    void ObjectScene::CheckDeletedObjects()
    {
        for(size_t i=0; i<m_spritestorages.GetCount(); ++i)
        {
            m_spritestorages[i]->CheckDeletedSprite();
        }
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            m_objstorages[i]->CheckAndDeleteObjects();
        }
    }
    
    void ObjectScene::AddObjectStorage(ObjectStorage *storage)
    {
        if(nullptr == storage)
        {
            DEBUG_LOG(ObjectScene::AddObjectStorage(), LOG_ERROR, ("storage is null"));
            return;
        }
        m_objstorages.Append(storage);
        auto *front = m_objstorages.GetBuffer();
        auto *end = front + m_objstorages.GetCount();
        std::sort(front, end,
                  [](ObjectStorage *a, ObjectStorage *b) {
                      return (a->GetOrder() > b->GetOrder());
                  });
    }
    
    void ObjectScene::AddDrawableStorage(DrawAbleStorage *storage)
    {
        if(nullptr == storage)
        {
            DEBUG_LOG(ObjectScene::AddSpriteStorage(), LOG_ERROR, ("storage is null"));
            return;
        }
        m_spritestorages.Append(storage);
        auto *front = m_spritestorages.GetBuffer();
        auto *end = front + m_spritestorages.GetCount();
        std::sort(front, end,
                  [](DrawAbleStorage* a, DrawAbleStorage *b) {
                      return (a->GetOrder()) > (b->GetOrder());
                  });
    }
    
    ObjectStorage *ObjectScene::GetObjectStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_objstorages.GetCount(); ++i)
        {
            if(hash == m_objstorages[i]->GetID())
                return m_objstorages[i];
        }
        return nullptr;
    }
    
    DrawAbleStorage *ObjectScene::GetDrawableStorage(Uint32 hash)
    {
        for(size_t i=0; i<m_spritestorages.GetCount(); ++i)
        {
            if(hash == m_spritestorages[i]->GetID())
                return m_spritestorages[i];
        }
        return nullptr;
    }
    
    int32 ObjectScene::LoadShaders()
    {
        auto &allocator = Application::Get().GetAllocator();
        auto &global_shaders = ShaderStorage::GetGlobal();
        
        // set shader
        void *mem_vert = nullptr, *mem_frag = nullptr;
        
        FILE *vert = OpenFile("res/shader/default_vert.glsl", "r");
        FILE *frag = OpenFile("res/shader/default_frag.glsl", "r");
        size_t size_vert = GetFileSize(vert);
        size_t size_frag = GetFileSize(frag);
        FileIO vert_io, frag_io;
        mem_vert = malloc(size_vert+1);
        mem_frag = malloc(size_frag+1);
        memset(mem_vert, 0, size_vert+1);
        memset(mem_frag, 0, size_frag+1);
        
        if(RET_SUCC != vert_io.Open(vert, mem_vert, size_vert))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        if(RET_SUCC != frag_io.Open(frag, mem_frag, size_frag))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        void *mem = allocator.Alloc(sizeof(ShaderProgram));
        ShaderProgram *program = new (mem) ShaderProgram();
        
        if(RET_SUCC != program->Init(vert_io, frag_io))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        global_shaders.Register(program, "default"_hash);
        
        free(mem_vert);
        free(mem_frag);
        
        return RET_SUCC;
    }
    
    int32 ObjectScene::LoadTextures()
    {
        return RET_SUCC;
    }
}














