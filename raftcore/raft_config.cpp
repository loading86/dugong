#include "raft_config.h"
namespace raftcore
{
    bool RaftConfig::Validate() const
    {
        if(m_id == 0)
        {
            return false;
        }
        if(m_heartbeat_tick == 0)
        {
            return false;
        }
        if(m_election_tick < 10 * m_heartbeat_tick)
        {
            return false;
        }
        if(m_max_size_per_msg == 0 || m_max_inflight_msgs == 0)
        {
            return false;
        }
        if(m_readonly_option == ReadOnlyOption::ReadOnlyLease && !m_check_quorum)
        {
            return false;
        }
    }
};