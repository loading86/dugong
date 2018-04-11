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
}