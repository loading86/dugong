#include "volatile_state.h"
namespace raftcore
{
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

    uint64_t VolatileState::FirstIndex()
    {

    }

    void VolatileState::Append(const std::deque<raftpb::Entry>& entries)
    {
        if(entries.empty())
        {
            return;
        }
        uint64_t import_first_index = entries.begin()->index();
        uint64_t import_end_index = entries.rbegin()->index();
        uint64_t local_end_index = LastIndex();
        if(import_first_index > local_end_index + 1)
        {
            return;
        }
        if(import_end_index > )
    }
}