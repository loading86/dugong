#pragma once
#include <inttypes.h>
#include <algorithm>
#include <list>
namespace raftcore
{
    enum class ProgressStateType
    {
        ProgressProbe,
        ProgressReplicate,
        ProgressSnapshot
    };

    class Inflights
    {
        private:
            uint32_t m_count;
            uint32_t m_size;
            std::list<uint64_t> m_buffer;
        public:
            Inflights(uint32_t size);
            bool Full();
            bool Add(uint64_t index);
            void Freeto(uint64_t index);
            void FreeFirstOne();
            void Reset();
    };

    class Progress
    {
        public:
            uint64_t m_match;
            uint64_t m_next;
            ProgressStateType m_state;
            bool m_paused;
            bool m_active;
            Inflights m_inflights;
            bool learner;
            uint64_t m_pending_snapshot_index;
        
        public:
            void SetState(ProgressStateType state);
            void BecomeProbe();
            void BecomeReplicate();
            void BecomeSnapshot(uint64_t snapshot_index);
            void Pause();
            void Resume();
            bool MayUpdate(uint64_t match_index);
            bool SendStopped();
            void Reset();
    };

}