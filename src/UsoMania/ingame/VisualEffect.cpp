#include "VisualEffect.h"
#include "Time.h"
#include "ShaderProgram.h"
#include "RhythmScore.h"

namespace UsoMania
{
    // HitEffect
    const Math::IRect HitEffect::anim_uvs[HitEffect::ANIM_COUNT] =
    {
        {300, 524, 100, 100},
        {300, 424, 100, 100},
        {300, 324, 100, 100},
        {300, 224, 100, 100}
    };
    
    HitEffect::HitEffect(Uint32 id, Bool isStatic)
    :Sprite(id, isStatic), m_iter(0), m_loop(0), m_speed(0), m_nextchange(0)
    {
        ;
    }
    
    HitEffect::HitEffect(Uint32 id, const GameObject *parent, Bool isStatic)
    :Sprite(id, parent, isStatic), m_iter(0), m_loop(0), m_speed(0), m_nextchange(0)
    {
        ;
    }
    
    HitEffect::HitEffect(const HitEffect &other)
    :Sprite(other), m_iter(other.m_iter), m_loop(other.m_loop),
    m_speed(other.m_speed), m_nextchange(other.m_nextchange)
    {
        ;
    }
    
    HitEffect::~HitEffect()
    {
        ;
    }
    
    HitEffect &HitEffect::operator=(const HitEffect &other)
    {
        Sprite::operator=(other);
        this->m_iter = other.m_iter;
        this->m_loop = other.m_loop;
        this->m_speed = other.m_speed;
        this->m_nextchange = other.m_nextchange;
        return (*this);
    }
    
    void HitEffect::Start()
    {
        auto &time = Time::Get();
        m_nextchange = time.GetIElapsedtime() + m_speed;
        Sprite::Start();
    }
    
    void HitEffect::Update()
    {
        auto &time = Time::Get();
        if(time.GetIElapsedtime() > m_nextchange)
        {
            ++m_iter;
            if(m_iter >= ANIM_COUNT)
            {
                if(0 == (m_loop-1))
                {
                    m_iter = 0;
                    SetAvailable(false);
                }
                else
                {
                    m_loop = m_loop == 0 ? 0 : (m_loop-1);
                    m_iter = 0;
                }
            }
            SetUV(anim_uvs[m_iter]);
            m_nextchange = time.GetIElapsedtime() + m_speed;
        }
        Sprite::Update();
    }
    
    void HitEffect::Init(Uint8 loop, Uint16 speed)
    {
        m_iter = 0;
        m_loop = loop;
        m_speed = speed;
        
        // set texture
        auto &textures = TextureStorage::GetGlobal();
        SetTexture(textures["atlas_ingame"_hash]);
        SetUV(anim_uvs[0]);
        SetScale(anim_uvs[0].w, anim_uvs[0].h);
    }
    
    void HitEffect::Reset(Uint8 loop, Uint16 speed)
    {
        auto &time = Time::Get();
        m_iter = 0;
        m_loop = loop;
        m_speed = speed;
        m_nextchange = time.GetIElapsedtime() + m_speed;
    }
    
    // HitEffects
    
    HitEffects *HitEffects::global = nullptr;
    
    HitEffects::HitEffects(Uint32 id, Bool isStatic)
    :GameObject(id, isStatic), m_hiteffects(nullptr), m_linesprites(nullptr)
    {
        ;
    }
    
    HitEffects::HitEffects(Uint32 id, const GameObject *parent, Bool isStatic)
    :GameObject(id, parent, isStatic), m_hiteffects(nullptr), m_linesprites(nullptr)
    {
        ;
    }
    
    HitEffects::~HitEffects()
    {
        for(Uint32 i=0; i<RhythmMap::MAX_LINE_NUM; ++i)
        {
            m_hiteffects[i].~HitEffect();
        }
    }
    
    void HitEffects::Start()
    {
        auto &allocator = Application::Get().GetAllocator();
        auto *scene = (ObjectScene*)Application::Get().GetScene();
        auto &shaders = ShaderStorage::GetGlobal();
        
        void *mem = allocator.Alloc<DrawAbleStorage>();
        auto *effects_storage = new (mem) DrawAbleStorage("hiteffects"_hash, 1);
        mem = allocator.Alloc(sizeof(DrawAbleStorage::Type)*m_linenum);
        effects_storage->AssignMemory(mem, m_linenum);
        effects_storage->SetShader(shaders["default"_hash]);
        scene->AddDrawableStorage(effects_storage);
        
        m_hiteffects = allocator.Alloc<HitEffect>(m_linenum);
        HitEffect *effect = nullptr;
        
        GLint w=0, h=0;
        Application::Get().GetWindow().GetSize(&w, &h);
        
        for(Uint32 i=0; i<m_linenum; ++i)
        {
            effect = new (m_hiteffects+i) HitEffect("hiteffect"_hash+i, &m_linesprites[i]);
            effect->SetLocalPosition(0.0f, -1.0f*h*0.4f);
            effect->Init(0, 100);
            effect->SetAvailable(false);
            effects_storage->Register(effect);
        }
        GameObject::Start();
    }
    
    void HitEffects::Update()
    {
        for(Uint32 i=0; i<m_linenum; ++i)
        {
            m_hiteffects[i].Update();
        }
        GameObject::Update();
    }
    
    void HitEffects::Init(const Sprite *lines, Uint32 linenum)
    {
        m_linesprites = lines;
        m_linenum = linenum;
    }
    
    void HitEffects::PlayHitEffect(Uint8 line, float32 y)
    {
        m_hiteffects[line].SetAvailable(true);
        m_hiteffects[line].SetLocalPosition(0.0f, y);
        m_hiteffects[line].Reset(1, EFFECT_SPEED);
    }
    
    void HitEffects::StartLongNoteEffect(Uint8 line)
    {
        int32 w=0, h=0;
        Application::Get().GetWindow().GetSize(&w, &h);
        m_hiteffects[line].SetAvailable(true);
        m_hiteffects[line].SetLocalPosition(0.0f, -0.4f*h);
        m_hiteffects[line].Reset(0, EFFECT_SPEED);
    }
    
    void HitEffects::EndLongNoteEffect(Uint8 line)
    {
        m_hiteffects[line].SetAvailable(false);
    }
    
    HitEffects *HitEffects::GetGlobal()
    {
        return global;
    }
    
    void HitEffects::SetGlobal(HitEffects *hiteffects)
    {
        global = hiteffects;
    }
    
    
    // ScoreEffect class
    
    const Math::IRect ScoreEffect::score_text_uvs[Note::SCORE_KINDS] =
    {
        {400, 224, 300, 100},
        {400, 324, 300, 100},
        {400, 424, 300, 100},
        {400, 524, 300, 100}
    };
    
    ScoreEffect::ScoreEffect(Uint32 id, Bool isStatic)
    :Sprite(id, isStatic)
    {
        ;
    }
    
    ScoreEffect::ScoreEffect(Uint32 id, const GameObject *parent, Bool isStatic)
    :Sprite(id, parent, isStatic)
    {
        ;
    }
    
    ScoreEffect::~ScoreEffect()
    {
        ;
    }
    
    void ScoreEffect::Start()
    {
        auto &textures = TextureStorage::GetGlobal();
        SetTexture(textures["atlas_ingame"_hash]);
    }
    
    void ScoreEffect::Update()
    {
        auto &global_score = RhythmScore::GetGlobal();
        SetScore(global_score.PopLastScore());
        
        // Update scale
        auto &scale = GetScale();
        if(scale.x > 0.0f) {
            auto &t = Time::Get();
            float32 shrink = SHRINK_SPEED * t.GetDeltatime();
            shrink = (scale.x - shrink)/scale.x;
            Scale(shrink);
        }
        else
            SetScale(0.0f, 0.0f);
    }
    
    void ScoreEffect::SetScore(const Note::Score score)
    {
        if(Note::Score::NONE != score)
        {
            SetScale(1.0f, (float32)score_text_uvs[0].h/score_text_uvs[0].w);
            Scale(SIZE);
            SetUV(score_text_uvs[(Uint32)score-1]);
        }
    }
}






