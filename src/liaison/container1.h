#ifndef DRCTCGUI20130129H
#define DRCTCGUI20130129H


#include "config.pb.h"

#include "goby/common/liaison_container.h"
#include "goby/common/zeromq_service.h"
#include "goby/pb/protobuf_node.h"
#include "goby/pb/protobuf_pubsub_node_wrapper.h"


#include <zmq.hpp>

#include <Wt/WTimer>

extern "C"
{
    std::vector<goby::common::LiaisonContainer*> goby_liaison_load(
        const goby::common::protobuf::LiaisonConfig& cfg,
        boost::shared_ptr<zmq::context_t> zmq_context);
}

class ExtendContainer1 : public goby::common::LiaisonContainer
{
public:

    ExtendContainer1(goby::common::ZeroMQService& zeromq_service,
                     const Extend1Config& cfg,
                     Wt::WContainerWidget* parent = 0);

    ~ExtendContainer1()
    { }

private:
    void loop();

    goby::common::protobuf::PubSubSocketConfig get_internal_pubsub_config();

    void handle_test_message(const TestMessage& msg);

    
    // virtual from goby::common::LiaisonContainer
    void focus()   { timer_.start(); }
    void unfocus() { timer_.stop(); }
        
private:
    Extend1Config cfg_;
    goby::common::ZeroMQService& zeromq_service_;
    goby::pb::StaticProtobufNode protobuf_node_;
    goby::pb::StaticProtobufPubSubNodeWrapper pubsub_node_;

    Wt::WGroupBox* box1_;
    Wt::WText* rx_msg_;
    
    Wt::WTimer timer_;

};


#endif
