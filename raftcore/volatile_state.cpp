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
        if(!m_entries.empty())
        {
            index = m_entries.rbegin()->m_index;
            return true;
        }
        if(m_snapshot != nullptr)
        {
            index = m_snapshot->m_snapshot_metadata.m_index;
            return true;
        }
        return false;
    }

    bool VolatileState::TermOf(uint64_t index, uint64_t& term)
    {
        if(!m_entries.empty())
        {
            uint64_t local_first_index = m_entries.begin()->m_index;
            uint64_t local_end_index = m_entries.rbegin()->m_index;
            if(index >= local_first_index && index <= local_end_index)
            {
                term =  m_entries[index - local_first_index].m_term;
                return true;
            }
        }
        if(m_snapshot != nullptr)
        {
            if(m_snapshot->m_snapshot_metadata.m_index == index)
            {
                term =  m_snapshot->m_snapshot_metadata.m_term;
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
        uint64_t local_first_index = m_entries.begin()->m_index;
        uint64_t local_end_index = m_entries.rbegin()->m_index;
        if(index > local_end_index || index < local_first_index)
        {
            return false;
        }
        uint64_t need_remove = index - local_first_index + 1;
        m_entries.erase(m_entries.begin(), m_entries.begin() + need_remove);
        m_offset = index + 1;
        return true;
    }

    bool VolatileState::ClearSnapshot(uint64_t index)
    {
        if(m_snapshot == nullptr)
        {
            return false;
        }
        if(m_snapshot->m_snapshot_metadata.m_index != index)
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
        uint64_t import_first_index = entries.begin()->m_index;
        uint64_t import_end_index = entries.rbegin()->m_index;
        if(import_first_index_should_be == import_first_index)
        {
            m_entries.insert(m_entries.end(), entries.begin(), entries.end()); 
            return;
        }
        if(import_first_index <= m_offset)
        {
            m_entries = entries;
            m_offset = import_first_index;
            return;
        }
        uint64_t local_end_index = m_entries.rbegin()->m_index;
        if(import_first_index > local_end_index)
        {
            m_logger->Panic("gap between local entries and income entries");
        }
        uint64_t preserve = local_end_index - import_first_index;
        m_entries.erase(m_entries.begin() + preserve, m_entries.end());
        m_entries.insert(m_entries.end(), entries.begin(), entries.end());   
    }

    void VolatileState::Load(std::shared_ptr<Snapshot> snapshot)
    {
        m_snapshot = snapshot;
        m_entries.clear();
        m_offset = m_snapshot == nullptr ? 1 : m_snapshot->m_snapshot_metadata.m_index + 1;
    }

    bool VolatileState::FirstUnstableIndex(uint64_t& index)
    {
        if(m_snapshot != nullptr)
        {
            index = m_snapshot->m_snapshot_metadata.m_index + 1;
            return true;
        }
        return false;
    }
}