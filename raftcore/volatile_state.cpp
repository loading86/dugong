#include "volatile_state.h"
namespace raftcore
{
    VolatileState::VolatileState()
    {
        m_offset = 1;
        m_snapshot = nullptr;
    }

    bool VolatileState::LastIndex(uint64_t& index)
    {
        if(!m_ents.empty())
        {
            index = m_ents.rbegin()->index();
            returun true;
        }
        if(m_snapshot != nullptr)
        {
            index = m_snapshot->metadata().index();
            return true;
        }
        return false;
    }

    bool VolatileState::TermOf(uint64_t index, uint64_t& term)
    {
        if(!m_ents.empty())
        {
            uint64_t local_first_index = m_ents.begin()->index();
            uint64_t local_end_index = m_ents.rbegin()->index();
            if(index >= local_first_index && index <= local_end_index)
            {
                term =  m_ents[index - local_first_index].term();
                returun true;
            }
        }
        if(m_snapshot != nullptr)
        {
            if(m_snapshot->metadata().index() == index)
            {
                term =  m_snapshot->metadata().term();
                return true;
            }
        }
        return false;
    }

    bool VolatileState::ClearEntriesBefore(uint64_t index, uint64_t term)
    {
        uint64_t local_term;
        bool ret = TermOf(index, local_term);
        if(!ret || local_term != term)
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
        uint64_t import_first_index_should_be = m_offset + m_entries.size();
        uint64_t import_first_index = entries.begin()->index();
        uint64_t import_end_index = entries.rbegin()->index();
        if(import_first_index_should_be == import_first_index)
        {
            m_ents.insert(m_ents.end(), entries.begin(), entries.end()); 
            return;
        }
        if(import_first_index <= m_offset)
        {
            m_ents = entries;
            m_offset = import_first_index;
            return;
        }
        uint64_t local_end_index = LastIndex();
        if(import_first_index > local_end_index)
        {
            m_logger->Panic("gap between local entries and income entries");
        }
        uint64_t preserve = local_end_index - import_first_index;
        m_ents.erase(m_ents.begin() + preserve, m_ents.end());
        m_ents.insert(m_ents.end(), entries.begin(), entries.end());   
    }

    void VolatileState::Load(std::shared_ptr<Snapshot> snapshot)
    {
        m_snapshot = snapshot;
        m_ents.clear();
        m_offset = m_snapshot == nullptr ? 1 : m_snapshot->metadata().index() + 1;
    }

    bool VolatileState::FirstUnstableIndex(uint64_t& index)
    {
        if(m_snapshot != nullptr)
        {
            index = m_snapshot->metadata().index() + 1;
            return true;
        }
        return false;
    }
}