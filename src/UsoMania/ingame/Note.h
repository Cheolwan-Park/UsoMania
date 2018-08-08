#ifndef UsoManiaNote_hpp
#define UsoManiaNote_hpp

#include "Sprite.h"
#include "RhythmMap.h"

namespace UsoMania
{
    class Note;
    class LongNote;
    class NoteGenerator;
    class SoundPlayer;
    class NoteOrder;
    
    class NoteGenerator : public GameObject
    {
    private:
        template <class NoteType>
        struct Notes
        {
            typedef NoteType Type;
            Type *notes;
            Uint32 num;
            Uint32 iter;
        };
        
    public:
        NoteGenerator() = delete;
        
        NoteGenerator(Uint32 id, Bool isStatic = false);
        
        NoteGenerator(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        NoteGenerator(const NoteGenerator &other) = delete;
        
        virtual ~NoteGenerator();
        
        NoteGenerator &operator=(const NoteGenerator &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
        
        void Pause();
        
        void Resume();
        
        void Init(const RhythmMap *map, Uint32 speed, Uint32 wait = 0);
        
        // get
        const RhythmMap *GetRhythmMap()const;
        
        Uint32 GetTime()const;
        
        Uint32 GetSpeed()const;
        
        Uint32 GetWaitTime()const;
        
        // set
        void SetLineSprites(const Sprite *lines);
        
    private:
        const RhythmMap *m_map;
        const Sprite *m_linesprites;
        Notes<Note> m_defaultnotes;
        Notes<LongNote> m_longnotes;
        Uint32 m_time;
        Uint32 m_speed;
        Uint32 m_wait;
    };
    
    class Note : public Sprite
    {
    public:
        static const GLint line_keys[RhythmMap::MAX_LINE_NUM];
        static constexpr Uint32 SCORE_KINDS = 5;
        enum class Score : Uint32
        {
            NONE = 0,
            MISS = 1,
            BAD = 2,
            GOOD = 3,
            PERFECT = 4
        };
        
    private:
        static const Math::IRect note_uvs[4];
        static const GLint judge[3];
        
        enum class State : Uint8
        {
            NON_CLEARED = 0,
            SEMI_CLEARED = 1,
            CLEARED = 2
        };
        
    protected:
        static constexpr Uint32 JUDGE_RANGE = 200;
        static constexpr Uint32 REMOVE_NOTE_AFTER = 250;
        
        
    public:
        friend NoteGenerator;
        
        Note() = delete;
        
        Note(Uint32 id, Bool isStatic = false);
        
        Note(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        Note(const Note &other);
        
        virtual ~Note();
        
        Note &operator=(const Note &other);
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Init(const NoteGenerator *generator, const Note *prev,
                          GLint m_key, Uint8 line, const RhythmMap::Note &note);
        
        void Pause();
        
        void Resume();
        
        Bool isPrevCleared()const;
        
        // get
        const NoteGenerator *GetGenerator()const;
        
        Uint32 GetGenTime()const;
        
        Uint32 GetTiming()const;
        
        GLint GetKey()const;
        
        Uint8 GetLine()const;
        
        Uint8 GetHitSound()const;
        
        Bool isPaused()const;
        
    private:
        void MoveNote();
        
    protected:
        virtual void CheckHit();
        
        virtual void CheckOut();
        
        void SetSemiCleared();
        
        Score Judge(int32 delta);
        
    private:
        const NoteGenerator *m_generator;
        const Note *m_prev;
        Uint32 m_gentime;
        Uint32 m_timing;
        GLint m_key;
        Uint8 m_line;
        Uint8 m_hitsound;
        State m_cleared;
        Bool m_paused;
    };
    
    class LongNote : public Note
    {
    private:
        static const Math::IRect end_uv[4];
        static const Math::IRect mid_uv[4];
        
        enum class State : Uint32
        {
            NOT_HITTED,
            HITTING,
            HIT_FAILED,
            BEG_HIT_FAILED
        };
        
    public:
        LongNote() = delete;
        
        LongNote(Uint32 id, Bool isStatic = false);
        
        LongNote(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        LongNote(const LongNote &other) = delete;
        
        virtual ~LongNote();
        
        LongNote &operator=(const LongNote &other) = delete;
        
        virtual void Start();

        virtual void Update();
        
        virtual void Draw();

        virtual void Init(const NoteGenerator *generator, const Note *prev,
                          GLint m_key, Uint8 line, const RhythmMap::Note &note);
        
    protected:
        virtual void CheckHit();

        virtual void CheckOut();
        
    private:
        void checkBegHit();
        
        void UpdateHittingPos();
        
    private:
        Sprite m_longnote_end;
        Sprite m_longnote_mid;
        Uint32 m_endtime;
        State m_state;
    };
}

#endif
