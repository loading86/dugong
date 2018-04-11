#pragma once
#include <inttypes.h>
#include <vector>
#include "protocol.h"
namespace raftcore
{
    enum class ReadOnlyOption
    {
        ReadOnlySafe,
        ReadOnlyLease
    };

    struct RaftConfig
    {
        public:
            uint64_t m_id;
            std::vector<uint64_t> m_peers;
            std::vector<uint64_t> m_learners;
            uint64_t m_election_tick;
            uint64_t m_heartbeat_tick;
            uint64_t m_applied;
            uint64_t m_max_size_per_msg;
            uint64_t m_max_inflight_msgs;
            bool m_check_quorum;
            bool m_prevote;
            ReadOnlyOption m_readonly_option;
            bool m_disable_proposal_forwarding;
        
        public:
            bool Validate() const;
    };
}
