#ifndef UsoManiaScene_hpp
#define UsoManiaScene_hpp

#include "Application.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Allocator.h"
#include "types.h"
#include "Array.h"

namespace GamesEngine
{
    class Scene
    {
    public:
        Scene();
        
        Scene(const Scene &other) = delete;
        
        virtual ~Scene();
        
        Scene &operator=(const Scene &other) = delete;
        
        virtual int32 Init();
        
        virtual void Update() = 0;
        
        virtual void Render() = 0;
        
        virtual void CheckDeletedObjects() = 0;
        
    protected:
        virtual int32 LoadShaders() = 0;
        virtual int32 LoadTextures() = 0;
    };
    
    class ObjectScene : public Scene
    {
    public:
        ObjectScene();
        
        ObjectScene(const ObjectScene &other) = delete;
        
        virtual ~ObjectScene();
        
        ObjectScene &operator=(const ObjectScene &other) = delete;
        
        virtual int32 Init();
        
        virtual void Update();
        
        virtual void Render();
        
        virtual void CheckDeletedObjects();
        
        void AddObjectStorage(ObjectStorage *storage);
        
        void AddDrawableStorage(DrawAbleStorage *storage);
        
        ObjectStorage *GetObjectStorage(Uint32 hash);
        
        DrawAbleStorage *GetDrawableStorage(Uint32 hash);
        
    protected:
        virtual int32 LoadShaders();
        
        virtual int32 LoadTextures();
        
    private:
        Array<ObjectStorage*> m_objstorages;
        Array<DrawAbleStorage*> m_spritestorages;
    };
}

#endif
