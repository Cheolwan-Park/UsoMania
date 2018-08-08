#ifndef UsoManiaScore_hpp
#define UsoManiaScore_hpp

#include "Note.h"
#include "Text.h"

namespace UsoMania
{
    class RhythmScore
    {
    private:
        static constexpr float32 COMBO_FACTOR = 100.0f;
        static const float32 SCOREMAP[Note::SCORE_KINDS];
        
    public:
        RhythmScore();
        
        RhythmScore(const RhythmScore &other);
        
        ~RhythmScore();
        
        RhythmScore &operator=(const RhythmScore &other);
        
        void Update(const Note::Score score);
        
        void Reset();
        
        Uint32 GetScore()const;
        
        Uint32 GetCombo()const;
        
        const Note::Score PopLastScore();
        
        static RhythmScore &GetGlobal();
        
    private:
        Uint32 m_score;
        Uint32 m_combo;
        Note::Score m_lastscore;
    };
    
    class ScoreBanner : public Text
    {
    private:
        static constexpr Uint32 BANNER_LEN = 10;
        
    public:
        ScoreBanner() = delete;
        
        ScoreBanner(Uint32 id, Bool isStatic = false);
        
        ScoreBanner(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        ScoreBanner(const ScoreBanner &other) = delete;
        
        virtual ~ScoreBanner();
        
        ScoreBanner &operator=(const ScoreBanner &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
    };
    
    class ComboBanner : public Text
    {
    private:
        static constexpr Uint32 BANNER_LEN = 10;
        
    public:
        ComboBanner() = delete;
        
        ComboBanner(Uint32 id, Bool isStatic = false);
        
        ComboBanner(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        ComboBanner(const ScoreBanner &other) = delete;
        
        virtual ~ComboBanner();
        
        ComboBanner &operator=(const ComboBanner &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
    };
}

#endif
