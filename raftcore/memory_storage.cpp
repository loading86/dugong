#include <inttypes.h>
#include "memory_storage.h"

namespace raftcore
{
    
    MemoryStorage::MemoryStorage()
    {
        Entry ent;
        m_entries.push_back(ent);
    }

    HardState MemoryStorage::GetHardState()
    {
        return m_hard_state;
    }

    ConfState MemoryStorage::GetConfState()
    {
        if(m_snapshot == nullptr)
        {
            ConfState cf;
            return cf;
        }
        return m_snapshot->m_snapshot_metadata.m_conf_state;
    }

    bool MemoryStorage::ApplySnapshot(std::shared_ptr<Snapshot> snapshot)
    {
        if(m_snapshot != nullptr && m_snapshot->m_snapshot_metadata.m_index > snapshot->m_snapshot_metadata.m_index)
        {
            return false;
        }
        m_snapshot = snapshot;
        m_entries.clear();
        Entry ent;
        ent.m_term = snapshot->m_snapshot_metadata.m_term;
        ent.m_index = snapshot->m_snapshot_metadata.m_index;
        m_entries.push_back(ent);
        return true;
    }

    uint64_t MemoryStorage::FirstIndex()
    {
        return m_entries[0].m_index + 1;
    }

    uint64_t MemoryStorage::LastIndex()
    {
        return m_entries[0].m_index + m_entries.size() - 1;
    }

    bool MemoryStorage::TermOf(uint64_t index, uint64_t& term)
    {
        uint64_t offset = m_entries[0].m_index;
        if(index < offset)
        {
            return false;
        }
        if(index - offset >= m_entries.size())
        {
            return false;
        }
        term = m_entries[index - offset].m_term;
        return true;
    }
}