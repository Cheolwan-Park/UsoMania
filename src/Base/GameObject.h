//
//  GameObject.h
//  GamesEngine
//
//  Created by Park Cheol Wan on 2017. 8. 13..
//  Copyright © 2017년 Games. All rights reserved.
//

#ifndef GamesEngineGameObject_h
#define GamesEngineGameObject_h

#include "types.h"
#include "Math.h"
#include "Array.h"
#include "String.h"
#include "Schedule.h"
#include "BitFlag.h"
#include <glm/glm.hpp>

namespace GamesEngine
{
    class Schedule;
    
    class GameObject
    {
    public:
        GameObject() = delete;
        
        GameObject(Uint32 id, Bool isStatic = false);
        
        GameObject(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        GameObject(const GameObject &other);
        
        virtual ~GameObject();
        
        GameObject &operator=(const GameObject &other);
        
        virtual void Start();
        
        virtual void Update();
        
        GameObject *AddChild(GameObject *child)const;
        
        // get
        Uint32 GetID()const;
        
        const glm::vec3 &GetLocalPosition()const;
        
        const glm::vec3 GetWorldPosition()const;
        
        const GameObject *GetParent()const;
        
        Bool isDeleted()const;
        
        Bool isAvailable()const;
        
        Bool isStarted()const;
        
        Bool isStatic()const;
        
        // set
        void SetID(Uint32 id);
        
        void SetLocalPosition(const glm::vec3 &position);
        
        void SetLocalPosition(float32 x, float32 y);
        
        void SetLocalPosition(float32 x, float32 y, float32 z);
        
        void SetWorldPosition(const glm::vec3 &position);
        
        void SetWorldPosition(float32 x, float32 y);
        
        void SetWorldPosition(float32 x, float32 y, float32 z);
        
        void Move(const glm::vec3 &delta);
        
        void Move(float32 x, float32 y);
        
        void SetParent(const GameObject *parent);
        
        void Delete();
        
        void SetAvailable(Bool value);
        
    private:
        Uint32 m_id;
        glm::vec3 m_position;
        const GameObject *m_parent;     // worldpos = m_position + m_parent.GetWorldPos()
    protected:
        /*
         * 0 : isDeleted
         * 1 : isAvailable (default : true)
         * 2 : isStarted
         * 3 : isStatic
         * 4 : isIdSet
         */
        BitFlag m_flags;
    };
    
    class ObjectStorage
    {
    public:
        typedef GameObject* Type;
        
        ObjectStorage() = delete;
        
        ObjectStorage(Uint32 id, Uint32 order);
        
        ObjectStorage(const ObjectStorage &other) = delete;
        
        ~ObjectStorage();
        
        ObjectStorage &operator=(const ObjectStorage &other) = delete;
        
        void AssignMemory(void *memory, Uint32 len);
        
        int32 Register(Type object);
        
        const Type operator[](Uint32 id)const;
        
        Type operator[](Uint32 id);
        
        void UpdateObjects();
        
        void CheckAndDeleteObjects();
        
        // get
        Uint32 GetID()const;
        
        Uint32 GetOrder()const;
        
    private:
        Uint32 m_id;
        Uint32 m_order;
        Type *m_objects;
        Uint32 m_objects_len;
    };
}















#endif
