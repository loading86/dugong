#include <time.h>
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

    void Raft::Reset(uint64_t term)
    {
        if(m_cur_term != term)
        {
            m_cur_term = term;
            m_vote_for = 0;
        }
        m_transfer_leader = 0;
        m_heartbeat_elapsed = 0;
        m_election_elapsed = 0;
        m_random_election_tick = Random(m_election_tick) + m_election_tick;  
        m_votes.clear();
        m_pending_conf = false;
        for(auto& node_prog : m_peers)
        {
            Progress& prog = node_prog.second;
            //prog.Reset();
            prog.m_next = m_raftlog->LastIndex() + 1;
            prog.m_active = true;
            prog.m_pending_snapshot_index = 0;
            prog.m_inflights.Reset();
            prog.m_match = 0;
            if(node_prog.first == m_id)
            {
                prog.m_match = m_raftlog->LastIndex();
            }
        }
        for(auto& node_prog : m_learners)
        {
            Progress& prog = node_prog.second;
            prog.m_next = m_raftlog->LastIndex() + 1;
            prog.m_active = true;
            prog.m_pending_snapshot_index = 0;
            prog.m_inflights.Reset();
            prog.m_match = 0;
            if(node_prog.first == m_id)
            {
                prog.m_match = m_raftlog->LastIndex();
            }
        }
        m_pending_conf = false;

    }

    void Raft::BecomeFollower(uint64_t term, uint64_t leader)
    {
        Reset(term);
        m_cur_state = StateType::StateFollower;
        m_leader = leader;
    }

    int Raft::Random(int high)
    {
        static std::default_random_engine re {uint32_t(time(NULL))};
        using Dist = std::uniform_int_distribution<int>;
        static Dist uid {};
        return uid(re, Dist::param_type{0,high});
    }

    void Raft::StepMessage(Message& msg)
    {
        if(msg.m_term > m_cur_term)
        {
            if(msg.m_type == MessageType::MsgApp || msg.m_type == MessageType::MsgHeartbeat || msg.m_type == MessageType::MsgSnap)
            {
                BecomeFollower(msg.m_term, msg.m_from);
            }else if(msg.m_type == MessageType::MsgPreVote || msg.m_type == MessageType::MsgVote)
            {
                if(m_vote_for != 0)
                {
                    m_vote_for = msg.m_from;
                }
            }
        }
    }

    void Raft::StepLeader(Message& msg)
    {
        if(msg.m_term > m_cur_term)
        {
            
        }
    }
}