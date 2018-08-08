#ifndef UsoManiaVisualEffect_hpp
#define UsoManiaVisualEffect_hpp

#include "Sprite.h"
#include "RhythmScene.h"
#include "Note.h"

namespace UsoMania
{
    class HitEffect : public Sprite
    {
    private:
        static constexpr Uint8 ANIM_COUNT = 4;
        static const Math::IRect anim_uvs[ANIM_COUNT];
        
    public:
        HitEffect() = delete;
        
        HitEffect(Uint32 id, Bool isStatic = false);
        
        HitEffect(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        HitEffect(const HitEffect &other);
        
        virtual ~HitEffect();
        
        HitEffect &operator=(const HitEffect &other);
        
        virtual void Start();
        
        virtual void Update();
        
        // if loop is 0 then loop permanently
        // else loop is n (n>0) loop n times
        // speed is changing sprite speed (in milisecond)
        void Init(Uint8 loop, Uint16 speed);
        
        void Reset(Uint8 loop, Uint16 speed);
        
    private:
        Uint8 m_iter;
        Uint8 m_loop;
        Uint16 m_speed;
        Uint32 m_nextchange;
    };
    
    class HitEffects : public GameObject
    {
    private:
        static HitEffects *global;
        static constexpr Uint16 EFFECT_SPEED = 50;
        
    public:
        HitEffects() = delete;
        
        HitEffects(Uint32 id, Bool isStatic = false);
        
        HitEffects(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        HitEffects(const HitEffects &other) = delete;
        
        virtual ~HitEffects();
        
        HitEffects &operator=(const HitEffects &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
        
        void Init(const Sprite *lines, Uint32 linenum);
        
        void PlayHitEffect(Uint8 line, float32 y);
        
        void StartLongNoteEffect(Uint8 line);
        
        void EndLongNoteEffect(Uint8 line);
        
        static HitEffects *GetGlobal();
        
        static void SetGlobal(HitEffects *hiteffects);
        
    private:
        HitEffect *m_hiteffects;
        const Sprite *m_linesprites;
        Uint32 m_linenum;
    };
    
    class ScoreEffect : public Sprite
    {
    private:
        static constexpr float32 SIZE = 370.0f;
        static constexpr float32 SHRINK_SPEED = 640.0f;
        static const Math::IRect score_text_uvs[Note::SCORE_KINDS];
        
    public:
        ScoreEffect() = delete;
        
        ScoreEffect(Uint32 id, Bool isStatic = false);
        
        ScoreEffect(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        ScoreEffect(const ScoreEffect &other) = delete;
        
        virtual ~ScoreEffect();
        
        ScoreEffect &operator=(const ScoreEffect &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
        
        void SetScore(const Note::Score score);
    };
}

#endif
