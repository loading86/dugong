#include "msgpack.hpp"
#include <inttypes.h>
#include <string>
#include <vector>
namespace raftcore
{
enum class EntryType 
{
    EntryNormal,
    EntryConfChange 
};

enum class MessageType 
{
	MsgHup,
	MsgBeat,
	MsgProp,
	MsgApp,
	MsgAppResp,
	MsgVote,
	MsgVoteResp,
	MsgSnap,
	MsgHeartbeat,
	MsgHeartbeatResp,
	MsgUnreachable,
	MsgSnapStatus,
	MsgCheckQuorum,
	MsgTransferLeader,
	MsgTimeoutNow,
	MsgReadIndex,
	MsgReadIndexResp,
	MsgPreVote,
	MsgPreVoteResp    
};

enum class ConfChangeType 
{
	ConfChangeAddNode,
	ConfChangeRemoveNode,
	ConfChangeUpdateNode,
	ConfChangeAddLearnerNode
};

struct ConfState
{     
public:
    std::vector<uint64_t> m_nodes;
    std::vector<uint64_t> m_learners;

public:
    MSGPACK_DEFINE(m_nodes, m_learners);
};

struct SnapshotMetadata
{  
public:
    ConfState m_conf_state;
    uint64_t m_index;
    uint64_t m_term;

public:
    MSGPACK_DEFINE(m_conf_state, m_index, m_term);
};

struct Snapshot
{
public:
    SnapshotMetadata m_snapshot_metadata;
    std::string m_data;

public:
    MSGPACK_DEFINE(m_snapshot_metadata, m_data);
};

struct Entry 
{
public:
    uint64_t m_index;
    uint64_t m_term;
    EntryType m_type;
    std::string m_data;

public:
    // Entry() = delete;
    // Entry(Entry&&) = default;
    // Entry(const Entry&) = default;   
    // Entry& Entry::operator=(Entry&&) = default;
    // Entry& Entry::operator=(const Entry&) = default;
    MSGPACK_DEFINE(m_index, m_term, (int&)m_type, m_data); 

};

struct Message 
{
public:
	MessageType m_type;
	uint64_t m_to;
	uint64_t m_from;
	uint64_t m_term;
	uint64_t m_log_term;
	uint64_t m_index;
	Entry m_entries;
	uint64_t m_commit;
	Snapshot m_snapshot;
	bool m_reject;
	uint64_t m_reject_hint;
	std::string m_context;

public:
    MSGPACK_DEFINE((int&)m_type, m_to, m_from, m_term, m_log_term, m_index, m_entries, m_commit, m_snapshot, m_reject, m_reject_hint, m_context); 
};

struct HardState
{
public:
    uint64_t m_term;
    uint64_t m_vote;
    uint64_t m_commit;

public:
    MSGPACK_DEFINE(m_term, m_vote, m_commit);
};

struct ConfChange 
{
public:
	uint64_t m_id;
	ConfChangeType m_type;
	uint64_t m_node_id;
	std::string m_context;

public:
    MSGPACK_DEFINE(m_id, (int&)m_type, m_node_id, m_context);
};
}
// MSGPACK_ADD_ENUM(EntryType::EntryNormal);
// MSGPACK_ADD_ENUM(EntryType::EntryConfChange);

