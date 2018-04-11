#pragma once
#include "volatile_state.h"
#include "memory_storage.h"
#include "protocol.h"
namespace raftcore
{
    class RaftLog
    {
        private:
            std::shared_ptr<MemoryStorage> m_storage;
            std::shared_ptr<VolatileState> m_volatile_state;
            std::shared_ptr<Logger> m_logger;
            uint64_t m_commit;
            uint64_t m_applied; 
        
        public:
            RaftLog(std::shared_ptr<MemoryStorage> storage, std::shared_ptr<Logger> logger);
            void SetCommit(uint64_t commit){m_commit = commit;}
            void SetApplied(uint64_t applied){m_applied = applied;}
            uint64_t GetCommit(){return m_commit;}
            uint64_t GetApplied(){return m_applied;}
    };
}
