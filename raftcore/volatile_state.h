#include "logger.h"
#include <inttypes.h>
namespace raftcore
{
    class VolatileState
    {
    public:
        void Append(const std::deque<raftpb::Entry>& entries);
    private:
        std::shared_ptr<raftpb::Snapshot> m_snapshot;
        std::deque<raftpb::Entry> m_ents;
    };
}