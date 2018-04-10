#include <inttypes.h>
#include "memory_storage.h"

namespace raftcore
{
    MemoryStorage::MemoryStorage()
    {
        Entry ent;
        m_entries.push_back(ent);
    }

    bool MemoryStorage::ApplySnapshot(std::shared_ptr<Snapshot> snapshot)
    {
        if(m_snapshot->m_snapshot_metadata->m_index > snapshot->m_snapshot_metadata->m_index)
        {
            return false;
        }
        
    }
}