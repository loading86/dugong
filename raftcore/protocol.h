#pragma once
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
    std::vector<uint64_t> m_peers;
    std::vector<uint64_t> m_learners;

public:
    MSGPACK_DEFINE(m_peers, m_learners);
};

struct SnapshotMetadata
{  
public:
    ConfState m_conf_state;
    uint64_t m_index = 0;
    uint64_t m_term = 0;

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
    uint64_t m_index = 0;
    uint64_t m_term = 0;
    EntryType m_type = EntryType::EntryNormal;
    std::string m_data;

public:
    MSGPACK_DEFINE(m_index, m_term, (int&)m_type, m_data); 

};

struct Message 
{
public:
	MessageType m_type = MessageType::MsgApp;
	uint64_t m_to = 0;
	uint64_t m_from = 0;
	uint64_t m_term = 0;
	uint64_t m_log_term = 0;
	uint64_t m_index = 0;
	std::deque<Entry> m_entries;
	uint64_t m_commit = 0;
	Snapshot m_snapshot;
	bool m_reject = false;
	uint64_t m_reject_hint = 0;
	std::string m_context;

public:
    MSGPACK_DEFINE((int&)m_type, m_to, m_from, m_term, m_log_term, m_index, m_entries, m_commit, m_snapshot, m_reject, m_reject_hint, m_context); 
};

struct HardState
{
public:
    uint64_t m_term = 0;
    uint64_t m_vote = 0;
    uint64_t m_commit = 0;

public:
    MSGPACK_DEFINE(m_term, m_vote, m_commit);
};

struct ConfChange 
{
public:
	uint64_t m_id = 0;
	ConfChangeType m_type = ConfChangeType::ConfChangeAddLearnerNode;
	uint64_t m_node_id = 0;
	std::string m_context;

public:
    MSGPACK_DEFINE(m_id, (int&)m_type, m_node_id, m_context);
};
}
// MSGPACK_ADD_ENUM(EntryType::EntryNormal);
// MSGPACK_ADD_ENUM(EntryType::EntryConfChange);

