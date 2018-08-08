#ifndef UsoManiaSprite_hpp
#define UsoManiaSprite_hpp

#include "TextureStorage.h"
#include "GameObject.h"
#include "FileIO.h"
#include "Math.h"
#include <functional>

namespace GamesEngine
{
    using Texture = TextureStorage::Val;
    
    class DrawAbleStorage;
    class ShaderProgram;
    
    class DrawAble : public GameObject
    {
    public:
        DrawAble() = delete;
        
        DrawAble(Uint32 id, Bool isStatic = false);
        
        DrawAble(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        DrawAble(const DrawAble &other);
        
        virtual ~DrawAble();
        
        DrawAble &operator=(const DrawAble &other);
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Draw() = 0;
        
        virtual void UpdateVBO() = 0;
        
        // get
        const glm::vec2 &GetScale()const;
        
        float32 GetRotation()const;
        
        // set
        void SetScale(const glm::vec2 &val);
        
        void SetScale(float32 x, float32 y);
        
        void Scale(const glm::vec2 &val);
        
        void Scale(float32 x, float32 y);
        
        void Scale(float32 x);
        
        void SetRotation(float32 val);
        
        void Rotate(float32 delta);
        
        void SetDrawer(DrawAbleStorage *drawer);
        
    private:
        glm::vec2 m_scale;
        float32 m_rotation;
    };
    
    class Sprite : public DrawAble
    {
    private:
        static const glm::vec3 verts[4];
        static const glm::vec2 uvs[4];
    public:
        Sprite() = delete;
        
        Sprite(Uint32 id, Bool isStatic = false);
        
        Sprite(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        Sprite(const Sprite &other);
        
        virtual ~Sprite();
        
        Sprite &operator=(const Sprite &other);
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Draw();
        
        virtual void UpdateVBO();
        
        // get
        float32 GetAlpha()const;
        
        const Math::Rect &GetUV()const;
        
        const Texture *GetTexture()const;
        
        // set
        void SetAlpha(float32 val);
        
        void SetUV(const Math::Rect &rect);
        
        void SetUV(const Math::IRect &rect);
        
        void SetTexture(const Texture *val);
        
    private:
        struct VAO
        {
            GLuint id;
            GLuint vbo[2];
        };
        void InitVAO();
        
        void ReleaseVAO();
        
    private:
        VAO m_vao;
        float32 m_alpha;
        Math::Rect m_uv;
        const Texture *m_tex;
    };
    
    class DrawAbleStorage
    {
    public:
        typedef DrawAble* Type;
        
        DrawAbleStorage() = delete;
        
        DrawAbleStorage(Uint32 id, Uint32 order);
        
        DrawAbleStorage(const DrawAbleStorage &other) = delete;
        
        ~DrawAbleStorage();
        
        DrawAbleStorage &operator=(const DrawAbleStorage &other) = delete;
        
        void AssignMemory(void *memory, Uint32 len);
        
        void SetShader(const ShaderProgram *shader);
        
        void SetRenderSettingFun(const std::function<void(void)> &fun);
        
        int32 Register(Type sprite);
        
        const Type operator[](Uint32 id)const;
        
        Type operator[](Uint32 id);
        
        void DrawSprites();
        
        void CheckDeletedSprite();
        
        // get
        Uint32 GetID()const;
        
        Uint32 GetOrder()const;
        
    private:
        Uint32 m_id;
        Uint32 m_order;
        const ShaderProgram *m_shader;
        Uint32 m_sprites_len;
        Type *m_objects;
        std::function<void(void)> m_rendersetting;
    };
}

#endif
