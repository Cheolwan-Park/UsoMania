#include "RhythmScore.h"
#include "Application.h"

namespace UsoMania
{
    // Score class
    
    const float32 RhythmScore::SCOREMAP[Note::SCORE_KINDS] =
    {0.0f, 0.0f, 50.0f, 100.0f, 300.0f};
    
    RhythmScore::RhythmScore()
    :m_score(0), m_combo(0), m_lastscore(Note::Score::NONE)
    {
        ;
    }
    
    RhythmScore::RhythmScore(const RhythmScore &other)
    :m_score(other.m_score), m_combo(other.m_combo), m_lastscore(other.m_lastscore)
    {
        ;
    }
    
    RhythmScore::~RhythmScore()
    {
        ;
    }
    
    RhythmScore &RhythmScore::operator=(const RhythmScore &other)
    {
        this->m_score = other.m_score;
        this->m_combo = other.m_combo;
        return (*this);
    }
    
    void RhythmScore::Update(const Note::Score score)
    {
        if(Note::Score::NONE != score)
        {
            if(score <= Note::Score::BAD)
                m_combo = 0;
            else
                ++m_combo;
            m_score += SCOREMAP[(Uint32)score] * (1.0f+m_combo/COMBO_FACTOR);
            m_lastscore = score;
        }
    }
    
    void RhythmScore::Reset()
    {
        m_score = 0;
        m_combo = 0;
    }
    
    Uint32 RhythmScore::GetScore()const
    {
        return m_score;
    }
    
    Uint32 RhythmScore::GetCombo()const
    {
        return m_combo;
    }
    
    const Note::Score RhythmScore::PopLastScore()
    {
        Note::Score result = m_lastscore;
        m_lastscore = Note::Score::NONE;
        return result;
    }
    
    RhythmScore &RhythmScore::GetGlobal()
    {
        static RhythmScore instance;
        return instance;
    }
    
    
    // ScoreBanner class
    ScoreBanner::ScoreBanner(Uint32 id, Bool isStatic)
    :Text(id, isStatic)
    {
        ;
    }
    
    ScoreBanner::ScoreBanner(Uint32 id, const GameObject *parent, Bool isStatic)
    :Text(id, parent, isStatic)
    {
        ;
    }
    
    ScoreBanner::~ScoreBanner()
    {
        ;
    }
    
    void ScoreBanner::Start()
    {
        auto &allocator = Application::Get().GetAllocator();
        auto &storage = TextureStorage::GetGlobal();
        void *mem = nullptr;
        
        mem = allocator.Alloc<CharSprite>(BANNER_LEN);
        AssignMemory(mem, BANNER_LEN);
        SetFont({8, 16, storage["atlas_text"_hash]});
    }
    
    void ScoreBanner::Update()
    {
        auto &score = RhythmScore::GetGlobal();
        char8 score_str[BANNER_LEN] = {0,};
        sprintf(score_str, "%0*u", BANNER_LEN, score.GetScore());
        SetString(score_str, BANNER_LEN);
    }
    
    
    // ComboBanner class
    
    ComboBanner::ComboBanner(Uint32 id, Bool isStatic)
    :Text(id, isStatic)
    {
        ;
    }
    
    ComboBanner::ComboBanner(Uint32 id, const GameObject *parent, Bool isStatic)
    :Text(id, parent, isStatic)
    {
        ;
    }
    
    ComboBanner::~ComboBanner()
    {
        ;
    }
    
    void ComboBanner::Start()
    {
        auto &allocator = Application::Get().GetAllocator();
        auto &storage = TextureStorage::GetGlobal();
        void *mem = nullptr;
        
        mem = allocator.Alloc<CharSprite>(BANNER_LEN);
        AssignMemory(mem, BANNER_LEN);
        SetFont({8, 16, storage["atlas_text"_hash]});
    }
    
    void ComboBanner::Update()
    {
        auto &score = RhythmScore::GetGlobal();
        char8 combo_str[BANNER_LEN] = {0,};
        sprintf(combo_str, "%u Combo", score.GetCombo());
        SetString(combo_str, BANNER_LEN);
    }
}






