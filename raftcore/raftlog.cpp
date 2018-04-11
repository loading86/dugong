#include "raftlog.h"

namespace raftcore
{
    RaftLog::RaftLog(std::shared_ptr<MemoryStorage> storage, std::shared_ptr<Logger> logger)
    {
        m_storage = storage;
        m_logger = logger;
        m_volatile_state = std::shared_ptr<VolatileState>(new VolatileState(logger));
        uint64_t first_index = storage->FirstIndex();
        uint64_t last_index = storage->LastIndex();
        m_volatile_state->SetOffset(last_index + 1);
        m_commit = first_index - 1;
        m_applied = first_index - 1;
    }

    bool RaftLog::Match(uint64_t index, uint64_t term)
    {
        uint64_t term_of_index;
        bool ret = TermOf(index, term_of_index);
        if(!ret)
        {
            return false;
        }
        return term == term_of_index;
    }

    bool RaftLog::TermOf(uint64_t index, uint64_t& term)
    {
        bool ret = m_volatile_state->TermOf(index, term);
        if(ret)
        {
            return true;
        }
        ret = storage->TermOf(index, term);
        return ret;
    }

    uint64_t RaftLog::FirstIndex()
    {
        uint64_t index;
        bool ret = m_volatile_state->FirstUnstableIndex(index);
        if(ret)
        {
            return index;
        }
        return storage->FirstIndex();
    }

    uint64_t RaftLog::LastIndex()
    {
        uint64_t index;
        bool ret = m_volatile_state->LastIndex(index);
        if(ret)
        {
            return index;
        }
        return storage->LastIndex();
    }

    bool RaftLog::Append(uint64_t last_index, uint64_t last_term, uint64_t commit, const std::deque<Entry>& ents)
    {
        bool ret = 
    }
}