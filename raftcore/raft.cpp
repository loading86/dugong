#include "raft.h"

namespace raftcore
{
    Raft::Raft(const RaftConfig& cfg, std::shared_ptr<MemoryStorage> storage, std::shared_ptr<Logger> logger)
    {
        if(!cfg.Validate())
        {
            logger->Panic("cfg is not validate");
        }
        m_id = cfg.m_id;
        m_cur_term = 0;
        m_vote_for = 0;
        m_max_size_per_msg = cfg.m_max_size_per_msg;
        m_max_inflight_msgs = cfg.m_max_inflight_msgs;
        StateType m_cur_state = StateType::StateFollower;
        m_is_learner = false;
        m_leader = 0;
        m_transfer_learder = 0;
        m_heartbeat_tick = cfg.m_heartbeat_tick;
        m_election_tick = cfg.m_election_tick;
        m_heartbeat_elapsed = 0;
        m_election_elapsed = 0;
        m_random_election_tick = 0;
        m_disable_proposal_forwarding = cfg.m_disable_proposal_forwarding;
        m_readonly_option = cfg.m_readonly_option;
        m_raftlog = std::shared_ptr<RaftLog>(new RaftLog(storage, logger));
        HardState hs = storage->GetHardState();
        ConfState cs = storage->GetConfState();
        std::vector<uint64_t> peers = cfg.m_peers;
        std::vector<uint64_t> learners = cfg.m_learners;
        if(cs.m_peers.size() != 0 || cs.m_learners.size() != 0)
        {
            if(peers.size() != 0 || learners.size() != 0)
            {
                logger->Panic("when peers and learners config stored in db, cant specify it in config");
            }
            peers = cs.m_peers;
            learners = cs.m_learners;
        }
        LoadState(hs);
    }

    bool Raft::LoadState(const HardState& hs)
    {
        if(hs.m_commit < m_raftlog->GetCommit())
        {
            return false;
        }
        m_raftlog->SetCommit(hs.m_commit);
        m_cur_term = hs.m_term;
        m_vote_for = hs.m_vote;
        return true; 
    }
}