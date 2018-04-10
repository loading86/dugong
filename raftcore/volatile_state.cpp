#include "volatile_state.h"
namespace raftcore
{
    VolatileState::VolatileState()
    {
        m_offset = 1;
        m_snapshot = nullptr;
    }

    uint64_t VolatileState::LastIndex()
    {
        if(!m_ents.empty())
        {
            return m_ents.rbegin()->index();
        }
        if(m_snapshot != nullptr)
        {
            return m_snapshot->metadata().index();
        }
        return 0;
    }

    uint64_t VolatileState::TermOf(uint64_t index)
    {
        if(!m_ents.empty())
        {
            uint64_t local_first_index = m_ents.begin()->index();
            uint64_t local_end_index = m_ents.rbegin()->index();
            if(index >= local_first_index && index <= local_end_index)
            {
                return m_ents[index - local_first_index].term();
            }
        }
        if(m_snapshot != nullptr)
        {
            if(m_snapshot->metadata().index() == index)
            {
                return m_snapshot->metadata().term();
            }
        }
        return UINT64_MAX;
    }

    bool VolatileState::Compact(uint64_t index, uint64_t term)
    {
        uint64_t local_term = TermOf(index);
        if(UINT64_MAX == local_term || local_term != term)
        {
            return false;
        }
        uint64_t local_first_index = m_ents.begin()->index();
        uint64_t local_end_index = m_ents.rbegin()->index();
        if(index > local_end_index || index < local_first_index)
        {
            return false;
        }
        uint64_t need_remove = index - local_first_index + 1;
        m_ents.erase(m_ents.begin(), m_ents.begin() + need_remove);
        m_offset = i + 1;
        return true;
    }

    bool VolatileState::ClearSnapshot(uint64_t index)
    {
        if(m_snapshot == nullptr)
        {
            return false;
        }
        if(m_snapshot->metadata().index() != index)
        {
            return false;
        }
        m_snapshot.reset();
        m_snapshot = nullptr;
        return true;
    }

    void VolatileState::Append(const std::deque<Entry>& entries)
    {
        if(entries.empty())
        {
            return;
        }
        
        uint64_t import_first_index = entries.begin()->index();
        uint64_t import_end_index = entries.rbegin()->index();
        if(import_first_index <= m_offset)
        {
            m_ents = entries;
            m_offset = import_first_index;
            return;
        }
        uint64_t local_end_index = LastIndex();
        if(import_first_index > local_end_index)
        {
            return;
        }
        uint64_t preserve = import_first_index - import_end_index;
        m_ents.erase(m_ents.begin() + preserve, m_ents.end());
        m_ents.insert(m_ents.end(), entries.begin(), entries.end());   
    }

    void VolatileState::Load(std::shared_ptr<Snapshot> snapshot)
    {
        m_snapshot = snapshot;
        m_ents.clear();
        m_offset = m_snapshot == nullptr ? 1 : m_snapshot->metadata().index() + 1;
    }
}