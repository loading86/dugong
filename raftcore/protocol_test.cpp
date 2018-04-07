#include "protocol.h"
#include <iostream>
int main()
{
    Entry ent;
    ent.m_index = 1;
    ent.m_term = 2;
    ent.m_type = EntryType::EntryNormal;
    ent.m_data = "hello msgpack!";  
    msgpack::v2::sbuffer buffer;  
    msgpack::v2::pack(buffer, ent);  
  
    msgpack::v1::unpacked msg;  
    msgpack::v1::unpack(&msg, buffer.data(), buffer.size());  
  
    msgpack::v1::object obj = msg.get(); 
    Entry ent2;
    obj.convert(&ent2);
    std::cout << ent2.m_data << std::endl;


    Snapshot ss;
    ss.m_data = "hello dugong!";
    SnapshotMetadata ssmd;
    ssmd.m_conf_state.m_nodes = {1,2,3};
    ssmd.m_conf_state.m_learners = {4,5,6};
    ssmd.m_index = 1;
    ssmd.m_term = 2;
    ss.m_snapshot_metadata = ssmd;
    msgpack::v2::sbuffer buffer2;
    msgpack::v2::pack(buffer2, ss);
    msgpack::v1::unpacked msg2;
    msgpack::v1::unpack(&msg2, buffer2.data(), buffer2.size());
    Snapshot ss2;
    msgpack::v1::object obj2= msg2.get();
    obj2.convert(&ss2);
    std::cout<<ss2.m_data<<std::endl;
    for(int n : ss2.m_snapshot_metadata.m_conf_state.m_nodes) {
        std::cout << n << '\n';
    }
    return 0;
}