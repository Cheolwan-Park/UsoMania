#include "Note.h"
#include "RhythmScene.h"
#include "GLFWInput.h"
#include "ShaderProgram.h"
#include "VisualEffect.h"
#include "Time.h"
#include "RhythmScore.h"

namespace UsoMania
{
    // NoteGenerator class
    
    NoteGenerator::NoteGenerator(Uint32 id, Bool isStatic)
    :GameObject(id, isStatic), m_map(nullptr), m_linesprites(nullptr),
    m_defaultnotes({nullptr, 0, 0}), m_longnotes({nullptr, 0, 0}),
    m_time(0), m_speed(0), m_wait(0)
    {
        ;
    }
    
    NoteGenerator::NoteGenerator(Uint32 id, const GameObject *parent, Bool isStatic)
    :GameObject(id, parent, isStatic), m_map(nullptr), m_linesprites(nullptr),
    m_defaultnotes({nullptr, 0, 0}), m_longnotes({nullptr, 0, 0}),
    m_time(0), m_speed(0), m_wait(0)
    {
        ;
    }
    
    NoteGenerator::~NoteGenerator()
    {
        if(nullptr != m_defaultnotes.notes)
        {
            for(Uint32 i=0; i<m_defaultnotes.num; ++i)
                m_defaultnotes.notes[i].Delete();
        }
        
        if(nullptr != m_longnotes.notes)
        {
            for(Uint32 i=0; i<m_longnotes.num; ++i)
                m_defaultnotes.notes[i].Delete();
        }
    }
    
    void NoteGenerator::Start()
    {
        if(nullptr == m_map)
        {
            DEBUG_LOG(NoteGenerator::Start(), LOG_ERROR,
                      ("map is null"));
            return;
        }
        
        auto &app = Application::Get();
        auto &allocator = app.GetAllocator();
        auto &shaders = ShaderStorage::GetGlobal();
        auto *scene = (ObjectScene*)app.GetScene();
        void *mem = nullptr;
        
        // add note storage
        Uint32 notecount = m_map->GetNumNotes();
        mem = allocator.Alloc<DrawAbleStorage>();
        auto *notes_storage = new (mem) DrawAbleStorage("notes"_hash, 16);
        mem = allocator.Alloc(notecount*sizeof(DrawAbleStorage::Type));
        notes_storage->AssignMemory(mem, notecount);
        notes_storage->SetShader(shaders["default"_hash]);
        scene->AddDrawableStorage(notes_storage);
        
        mem = allocator.Alloc<ObjectStorage>();
        auto *note_objs = new (mem) ObjectStorage("notes"_hash, 1);
        mem = allocator.Alloc(notecount*sizeof(ObjectStorage::Type));
        note_objs->AssignMemory(mem, notecount);
        scene->AddObjectStorage(note_objs);
        
        // set keymap
        Uint32 lines = m_map->GetNumLines();
        GLint keymap[11] = {0,};
        Uint32 half = lines/2;
        Uint32 s = 4 -(half-1);
        if(lines == half*2)
        {
            for(Uint32 i=0; i<lines; ++i)
                keymap[i] = Note::line_keys[s+i];
        }
        else
        {
            for(Uint32 i=0; i<half; ++i)
                keymap[i] = Note::line_keys[s+i];
            keymap[half] = Note::line_keys[0];
            for(Uint32 i=half+1; i<lines; ++i)
                keymap[i] = Note::line_keys[s+i-1];
        }
        
        // count longnote
        Uint32 longnote_count = 0;
        const RhythmMap::Note *notes_data = m_map->GetNotes();
        for(Uint32 i=0; i<notecount; ++i)
        {
            if(0 != notes_data[i].endtime)
                ++longnote_count;
        }
        m_defaultnotes.num = notecount - longnote_count;
        m_longnotes.num = longnote_count;
        
        // add notes
        Note* prevnote[RhythmMap::MAX_LINE_NUM] = {nullptr, };
        m_defaultnotes.notes = allocator.Alloc<Note>(m_defaultnotes.num);
        m_longnotes.notes = allocator.Alloc<LongNote>(m_longnotes.num);
        Note *note = nullptr;
        Uint32 line = 0;
        Uint32 defnote_i = 0, longnote_i = 0;
        
        for(Uint32 i=0; i<notecount; ++i)
        {
            line = notes_data[i].line;
            if(0 != notes_data[i].endtime)
                note = new (m_longnotes.notes + longnote_i++) LongNote(i, &m_linesprites[line]);
            else
                note = new (m_defaultnotes.notes + defnote_i++) Note(i, &m_linesprites[line]);
            note->Init(this, prevnote[line], keymap[line], line, notes_data[i]);
            note->SetAvailable(false);
            prevnote[line] = note;
            note_objs->Register(note);
            notes_storage->Register(note);
        }
    }
    
    void NoteGenerator::Update()
    {
        // gen new note
        m_time += Time::Get().GetIDeltatime();
        
        // gen default notes
        Uint32 i = 0;
        for(i = m_defaultnotes.iter;
            (i<m_defaultnotes.num) && (m_defaultnotes.notes[i].m_gentime < m_time);
            ++i)
            m_defaultnotes.notes[i].SetAvailable(true);
        m_defaultnotes.iter = i;
        
        // gen long notes
        for(i = m_longnotes.iter;
            (i<m_longnotes.num) && (m_longnotes.notes[i].m_gentime < m_time);
            ++i)
            m_longnotes.notes[i].SetAvailable(true);
        m_longnotes.iter = i;
    }
    
    void NoteGenerator::Pause()
    {
        if(nullptr != m_defaultnotes.notes)
        {
            for(Uint32 i=0; i<m_defaultnotes.num; ++i)
                m_defaultnotes.notes[i].Pause();
        }
        if(nullptr != m_longnotes.notes)
        {
            for(Uint32 i=0; i<m_longnotes.num; ++i)
                m_longnotes.notes[i].Pause();
        }
        SetAvailable(false);
    }
    
    void NoteGenerator::Resume()
    {
        if(nullptr != m_defaultnotes.notes)
        {
            for(Uint32 i=0; i<m_defaultnotes.num; ++i)
                m_defaultnotes.notes[i].Resume();
        }
        if(nullptr != m_longnotes.notes)
        {
            for(Uint32 i=0; i<m_longnotes.num; ++i)
                m_longnotes.notes[i].Resume();
        }
        SetAvailable(true);
    }
    
    void NoteGenerator::Init(const RhythmMap *map, Uint32 speed, Uint32 wait)
    {
        if(nullptr == map)
        {
            DEBUG_LOG(NoteGenerator::Init(), LOG_ERROR, ("map is null"));
            return;
        }
        m_map = map;
        m_speed = speed;
        m_wait = wait < speed ? speed : wait;
    }
    
    const RhythmMap *NoteGenerator::GetRhythmMap()const
    {
        return m_map;
    }
    
    Uint32 NoteGenerator::GetTime()const
    {
        return m_time;
    }
    
    Uint32 NoteGenerator::GetSpeed()const
    {
        return m_speed;
    }
    
    Uint32 NoteGenerator::GetWaitTime()const
    {
        return m_wait;
    }
    
    void NoteGenerator::SetLineSprites(const Sprite *lines)
    {
        m_linesprites = lines;
    }
    
    
    // Note class
    
    const Math::IRect Note::note_uvs[4] =
    {
        {200,524,100,100},
        {200,424,100,100},
        {200,324,100,100},
        {200,224,100,100}
    };
    
    const GLint Note::line_keys[11] =
    {
        GLFW_KEY_SPACE,     // middle line (odd linecount)
        
        GLFW_KEY_A,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_F,
        
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_SEMICOLON
    };
    
    const GLint Note::judge[3] =
    {
        45,     // great
        75,     // good
        125     // bad
    };
    
    Note::Note(Uint32 id, Bool isStatic)
    :Sprite(id, isStatic), m_generator(nullptr), m_prev(nullptr),
    m_gentime(0), m_timing(0), m_key(0), m_line(0), m_hitsound(0),
    m_cleared(State::NON_CLEARED), m_paused(false)
    {
        ;
    }
    
    Note::Note(Uint32 id, const GameObject *parent, Bool isStatic)
    :Sprite(id, parent, isStatic), m_generator(nullptr), m_prev(nullptr),
    m_gentime(0), m_timing(0), m_key(0), m_line(0), m_hitsound(0),
    m_cleared(State::NON_CLEARED), m_paused(false)
    {
        ;
    }
    
    Note::Note(const Note &other)
    :Sprite(other), m_generator(other.m_generator), m_prev(other.m_prev),
    m_gentime(other.m_gentime), m_timing(other.m_timing), m_key(other.m_key),
    m_line(other.m_line), m_hitsound(other.m_hitsound),
    m_cleared(other.m_cleared), m_paused(other.m_paused)
    {
        ;
    }
    
    Note::~Note()
    {
        ;
    }
    
    Note &Note::operator=(const Note &other)
    {
        Sprite::operator=(other);
        this->m_generator = other.m_generator;
        this->m_prev = other.m_prev;
        this->m_gentime = other.m_gentime;
        this->m_timing = other.m_timing;
        this->m_key = other.m_key;
        this->m_hitsound = other.m_hitsound;
        this->m_cleared = other.m_cleared;
        this->m_paused = other.m_paused;
        return(*this);
    }
    
    void Note::Start()
    {
        Sprite::Start();
    }
    
    void Note::Update()
    {
        if(false == m_paused)
        {
            // check note cleared
            if(State::SEMI_CLEARED == m_cleared)
            {
                m_cleared = State::CLEARED;
                SetAvailable(false);
                return;
            }
            
            MoveNote();
            
            CheckHit();
            
            CheckOut();
        }
    }
    
    void Note::Init(const NoteGenerator *generator, const Note *prev,
                    GLint key, Uint8 line, const RhythmMap::Note &note)
    {
        // init info
        m_generator = generator;
        m_prev = prev;
        m_timing = note.timing + m_generator->GetWaitTime();
        m_gentime = m_timing - m_generator->GetSpeed();
        m_key = key;
        m_hitsound = note.hitsound;
        m_line = line;
        
        // set texture
        auto &textures = TextureStorage::GetGlobal();
        SetTexture(textures["atlas_ingame"_hash]);
        SetUV(note_uvs[m_hitsound]);
        SetScale(note_uvs[0].w, note_uvs[0].h);
    }
    
    void Note::Pause()
    {
        m_paused = true;
    }
    
    void Note::Resume()
    {
        m_paused = false;
    }
    
    Bool Note::isPrevCleared()const
    {
        return (nullptr == m_prev || State::CLEARED == m_prev->m_cleared);
    }
    
    const NoteGenerator *Note::GetGenerator()const
    {
        return m_generator;
    }
    
    Uint32 Note::GetGenTime()const
    {
        return m_gentime;
    }
    
    Uint32 Note::GetTiming()const
    {
        return m_timing;
    }
    
    GLint Note::GetKey()const
    {
        return m_key;
    }
    
    Uint8 Note::GetLine()const
    {
        return m_line;
    }
    
    Uint8 Note::GetHitSound()const
    {
        return m_hitsound;
    }
    
    Bool Note::isPaused()const
    {
        return m_paused;
    }
    
    void Note::MoveNote()
    {
        Uint32 time = m_generator->GetTime();
        int32 delta = time - m_gentime;
        if(0 < delta)
        {
            float64 proceed = (float32)delta/(m_timing-m_gentime);
            GLint window_h = 0, dummy=0;
            Application::Get().GetWindow().GetSize(&dummy, &window_h);
            float32 y = 0.9f*window_h*proceed;
            y = (float32)window_h*0.5f - y;
            SetLocalPosition(0.0f, y);
        }
    }
    
    void Note::CheckHit()
    {
        auto &input = GLFW::Input::Get();
        Score score = Score::NONE;
        if(isPrevCleared() && input.isKeyPressed(m_key))
        {
            Uint32 time = m_generator->GetTime();
            int32 delta = time - m_timing;
            delta = delta > 0 ? delta : -delta;
            score = Judge(delta);
        }
        
        if(score != Score::NONE)
        {
            // scoring
            RhythmScore::GetGlobal().Update(score);
            
            // play effects
            HitEffects::GetGlobal()->PlayHitEffect(GetLine(), GetLocalPosition().y);
            if(score > Score::MISS)
                SoundPlayer::GetGlobal().PlayHitSound(m_hitsound);
            SetSemiCleared();
        }
    }
    
    void Note::CheckOut()
    {
        Uint32 time = m_generator->GetTime();
        if(time >= m_timing + REMOVE_NOTE_AFTER)
        {
            RhythmScore::GetGlobal().Update(Score::MISS);
            SetSemiCleared();
        }
    }
    
    void Note::SetSemiCleared()
    {
        m_cleared = State::SEMI_CLEARED;
    }
    
    Note::Score Note::Judge(int32 delta)
    {
        delta = delta > 0 ? delta : -delta;
        if(delta <= JUDGE_RANGE)
        {
            if(delta <= judge[0])
                return Score::PERFECT;
            else if(delta <= judge[1])
                return Score::GOOD;
            else if(delta <= judge[2])
                return Score::BAD;
            else
                return Score::MISS;
        }
        return Score::NONE;
    }
    
    // LongNote classs
    
    const Math::IRect LongNote::end_uv[4] =
    {
        {200,524,100,100},
        {200,424,100,100},
        {200,324,100,100},
        {200,224,100,100}
    };
    
    const Math::IRect LongNote::mid_uv[4] =
    {
        {200,0,100,224},
        {300,0,100,224},
        {400,0,100,224},
        {500,0,100,224},
    };
    
    LongNote::LongNote(Uint32 id, Bool isStatic)
    :Note(id, isStatic),
    m_longnote_end(id + "end"_hash, isStatic),
    m_longnote_mid(id + "mid"_hash, isStatic),
    m_endtime(0), m_state(State::NOT_HITTED)
    {
        ;
    }
    
    LongNote::LongNote(Uint32 id, const GameObject *parent, Bool isStatic)
    :Note(id, parent, isStatic),
    m_longnote_end(id + "end"_hash, this, isStatic),
    m_longnote_mid(id + "mid"_hash, this, isStatic),
    m_endtime(0), m_state(State::NOT_HITTED)
    {
        ;
    }
    
    LongNote::~LongNote()
    {
        ;
    }
    
    void LongNote::Start()
    {
        auto &textures = TextureStorage::GetGlobal();
        auto *atlas = textures["atlas_ingame"_hash];
        
        GLint w=0, h=0;
        Application::Get().GetWindow().GetSize(&w, &h);
        float64 ratio = (float32)(m_endtime - GetTiming())/(GetGenerator()->GetSpeed());
        float32 len = ratio*0.9f*h;
        
        // setting mid note
        m_longnote_mid.SetTexture(atlas);
        m_longnote_mid.SetUV(mid_uv[GetHitSound()]);
        m_longnote_mid.SetScale(mid_uv[GetHitSound()].w, len);
        m_longnote_mid.SetLocalPosition(0.0f, len/2.0f);
        
        // setting end note
        m_longnote_end.SetTexture(atlas);
        m_longnote_end.SetUV(end_uv[GetHitSound()]);
        m_longnote_end.SetScale(end_uv[GetHitSound()].w, end_uv[GetHitSound()].h);
        m_longnote_end.SetLocalPosition(0.0f, len);
        
        Note::Start();
    }
    
    void LongNote::Update()
    {
        Note::Update();
        m_longnote_end.Update();
        m_longnote_mid.Update();
    }
    
    void LongNote::Draw()
    {
        m_longnote_mid.Draw();
        m_longnote_end.Draw();
        if(State::HITTING != m_state)
            Note::Draw();
    }
    
    void LongNote::Init(const NoteGenerator *generator, const Note *prev,
                        GLint m_key, Uint8 line, const RhythmMap::Note &note)
    {
        Note::Init(generator, prev, m_key, line, note);
        m_endtime = note.endtime + (GetGenerator()->GetWaitTime());
    }
    
    void LongNote::CheckHit()
    {
        if(State::NOT_HITTED == m_state)
            checkBegHit();
        else if(State::HITTING == m_state)
        {
            auto &input = GLFW::Input::Get();
            Uint32 t = GetGenerator()->GetTime();
            Score score = Score::NONE;
            if((t < m_endtime - JUDGE_RANGE) && (false == input.isKeyDown(GetKey())))
            {
                score = Score::BAD;
                RhythmScore::GetGlobal().Update(score);
                
                m_state = State::HIT_FAILED;
                HitEffects::GetGlobal()->EndLongNoteEffect(GetLine());
            }
            else
            {
                if(false == input.isKeyDown(GetKey()))
                {
                    // TODO : write score code
                    
                    int32 delta = m_endtime - t;
                    score = Judge(delta);
                    
                    RhythmScore::GetGlobal().Update(score);
                    
                    if(score > Score::MISS)
                    {
                        SoundPlayer::GetGlobal().PlayHitSound(GetHitSound());
                        HitEffects::GetGlobal()->PlayHitEffect(GetLine(), GetLocalPosition().y);
                    }
                    HitEffects::GetGlobal()->EndLongNoteEffect(GetLine());
                    SetSemiCleared();
                }
            }
            UpdateHittingPos();
        }
        else if(State::HIT_FAILED == m_state)
            UpdateHittingPos();
    }
    
    void LongNote::CheckOut()
    {
        Uint32 t = GetGenerator()->GetTime();
        if(State::NOT_HITTED ==  m_state)
        {
            if(GetTiming() + REMOVE_NOTE_AFTER < t)
            {
                RhythmScore::GetGlobal().Update(Note::Score::MISS);
                m_state = State::BEG_HIT_FAILED;
            }
        }
        else if(State::HITTING == m_state)
        {
            if(m_endtime + REMOVE_NOTE_AFTER < t)
            {
                RhythmScore::GetGlobal().Update(Note::Score::MISS);
                HitEffects::GetGlobal()->EndLongNoteEffect(GetLine());
                SetSemiCleared();
            }
        }
    }
    
    void LongNote::checkBegHit()
    {
        auto &input = GLFW::Input::Get();
        Uint32 t = GetGenerator()->GetTime();
        Score score = Score::NONE;
        
        if(isPrevCleared() && input.isKeyPressed(GetKey()))
        {
            int32 delta = GetTiming() - t;
            score = Judge(delta);
        }
        
        if(Score::NONE != score)
        {
            RhythmScore::GetGlobal().Update(score);
            
            if(score > Score::MISS)
            {
                SoundPlayer::GetGlobal().PlayHitSound(GetHitSound());
                HitEffects::GetGlobal()->StartLongNoteEffect(GetLine());
                m_state = State::HITTING;
            }
        }
    }
    
    void LongNote::UpdateHittingPos()
    {
        Uint32 t = GetGenerator()->GetTime();
        int32 delta = m_endtime - t;
        
        if(delta < 0)
            return;
        
        GLint w=0, h=0;
        Application::Get().GetWindow().GetSize(&w, &h);
        
        SetLocalPosition(0.0f, -0.4f*h);
        
        Uint32 speed = GetGenerator()->GetSpeed();
        float64 ratio = (float32)(m_endtime - t)/speed;
        float32 len = ratio*0.9f*h;
        
        m_longnote_mid.SetLocalPosition(0.0f, len/2.0f);
        m_longnote_mid.SetScale(mid_uv[GetHitSound()].w, len);
        m_longnote_end.SetLocalPosition(0.0f, len);
    }
}




