#include "progress.h"

namespace raftcore
{
    Inflights::Inflights(uint32_t size)
    {
        m_size = size;
        m_count = 0;
    }

    bool Inflights::Full()
    {
        return m_size == m_count;
    }

    bool Inflights::Add(uint64_t index)
    {
        if(Full())
        {
            return false;
        }
        m_buffer.push_back(index);
        m_count++;
        return true;
    }

    void Inflights::Freeto(uint64_t index)
    {
        while(m_count > 0 && index >= *m_buffer.begin())
        {
            m_buffer.pop_front();
            m_count--;
        }
    }

    void Inflights::FreeFirstOne()
    {
        if(m_count > 0)
        {
            m_buffer.pop_front();
            m_count--;
        }
    }

    void Inflights::Reset()
    {
        m_size = 0;
        m_count = 0;
        m_buffer.clear();
    }

    void Progress::SetState(ProgressStateType state)
    {
        m_state = state;
        m_paused = false;
        m_inflights.Reset();
    }

    void Progress::BecomeProbe()
    {
        if(m_state == ProgressStateType::ProgressSnapshot)
        {
            uint64_t pending_index = m_pending_snapshot_index;
            SetState(ProgressStateType::ProgressProbe);
            m_next = std::max(m_match + 1, pending_index + 1);
        }else
        {
            SetState(ProgressStateType::ProgressProbe);
            m_next = m_match + 1;
        }
    }

    void Progress::BecomeReplicate()
    {
        SetState(ProgressStateType::ProgressReplicate);
        m_next = m_match + 1;
    }

    void Progress::BecomeSnapshot(uint64_t snapshot_index)
    {
        SetState(ProgressStateType::ProgressSnapshot);
        m_pending_snapshot_index = snapshot_index;
    }

    void Progress::Pause()
    {
        m_paused = true;
    }

    void Progress::Resume()
    {
        m_paused = false;
    }

    bool Progress::MayUpdate(uint64_t match_index)
    {
        bool update_match = false;
        if(match_index > m_match)
        {
            m_match = match_index;
            update_match = true;
            Resume();
        }
        m_next = std::max(m_next, match_index + 1);
        return update_match;
    }

    bool Progress::SendStopped()
    {
        switch(m_state)
        {
            case ProgressStateType::ProgressProbe:
                return m_paused;
            case ProgressStateType::ProgressReplicate:
                return m_inflights.Full();
            default:
                return true;
        }
    }

}