#pragma once
#include <ctime>
#include <random>
#include "raftlog.h"
#include "raft_config.h"
#include "protocol.h"
namespace raftcore
{

    enum class StateType
    {
        StateFollower,
        StatePreCandidate,
        StateCandidate,
        StateLeader
    };

    enum class CampaignType
    {   
        CampaignPreElection,
        CampaignElection,
        CampaignTransfer
    };

    class Raft
    {
        private:
            uint64_t m_id;
            uint64_t m_cur_term;
            uint64_t m_vote_for;
            std::shared_ptr<RaftLog> m_raftlog;
            uint64_t m_max_size_per_msg;
            uint64_t m_max_inflight_msgs;
            StateType m_cur_state;
            bool m_is_learner;
            std::list<Message> m_messages;
            uint64_t m_leader;
            uint64_t m_transfer_leader;
            uint64_t m_heartbeat_tick;
            uint64_t m_election_tick;
            uint64_t m_heartbeat_elapsed;
            uint64_t m_election_elapsed;
            uint64_t m_random_election_tick;
            bool m_disable_proposal_forwarding;
            ReadOnlyOption m_readonly_option;

            unsigned m_seed;
            std::mt19937_64* m_rng;
            std::uniform_int_distribution<int>* m_uni;

        
        public:
            Raft(const RaftConfig& cfg, std::shared_ptr<MemoryStorage> storage, std::shared_ptr<Logger> logger);
            bool LoadState(const HardState& hs);
            void StepLeader(Message& msg);
            void StepFollower(Message& msg);
            void StepCandidate(Message& msg);
    };

}