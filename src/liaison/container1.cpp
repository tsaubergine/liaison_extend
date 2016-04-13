#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WBreak>
#include <Wt/WSlider>
#include <Wt/WSpinBox>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
#include <Wt/WPushButton>
#include <Wt/WCssDecorationStyle>
#include <Wt/WColor>
#include <Wt/WLineEdit>
#include <Wt/WPanel>
#include <Wt/WGroupBox>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "goby/common/logger.h"

#include "liaison_extend/protobuf/test.pb.h"
#include "container1.h"

using namespace Wt;
using namespace goby::common::logger;
using goby::glog;

std::vector<boost::shared_ptr<goby::common::ZeroMQService> > services_;

extern "C"
{
    std::vector<goby::common::LiaisonContainer*> goby_liaison_load(
        const goby::common::protobuf::LiaisonConfig& cfg,
        boost::shared_ptr<zmq::context_t> zmq_context)
    {
        std::vector<goby::common::LiaisonContainer*> containers;

        // replicate for multiple new containers
        services_.push_back(boost::shared_ptr<goby::common::ZeroMQService>(new goby::common::ZeroMQService(zmq_context)));
        goby::common::LiaisonContainer* container1 = new ExtendContainer1(*services_.back().get(), cfg.GetExtension(extend1_config));
        containers.push_back(container1);

        // goby::common::LiaisonContainer* container2 = new ExtendContainer2(cfg.GetExtension(extend2_config));
        // containers.push_back(container2);

        return containers;
    }
    
}

ExtendContainer1::ExtendContainer1(goby::common::ZeroMQService& zeromq_service,
                                   const Extend1Config& cfg,
                                   Wt::WContainerWidget* parent)
    : LiaisonContainer(parent),
      cfg_(cfg),
      zeromq_service_(zeromq_service),
      protobuf_node_(&zeromq_service_),
      pubsub_node_(&protobuf_node_, get_internal_pubsub_config()),
      box1_(new Wt::WGroupBox("Box!", this)),
      rx_msg_(new Wt::WText("Placeholder", box1_))
{
    set_name("ExtendExample1");
    
    goby::common::protobuf::ZeroMQServiceConfig inproc_sockets;
    goby::common::protobuf::PubSubSocketConfig pubsub_config = get_internal_pubsub_config();
    inproc_sockets.add_socket()->CopyFrom(pubsub_config.publish_socket());
    inproc_sockets.add_socket()->CopyFrom(pubsub_config.subscribe_socket());
    zeromq_service_.merge_cfg(inproc_sockets);

    pubsub_node_.subscribe<TestMessage>(boost::bind(&ExtendContainer1::handle_test_message, this, _1), "testgroup");
    
    // setInterval is in milliseconds
    timer_.setInterval(1000/cfg_.timer_freq());
    timer_.timeout().connect(this, &ExtendContainer1::loop);
    timer_.start();
}

goby::common::protobuf::PubSubSocketConfig ExtendContainer1::get_internal_pubsub_config()
{
    using namespace goby::common;
    
    protobuf::PubSubSocketConfig pubsub_config;
    protobuf::ZeroMQServiceConfig::Socket* internal_subscribe_socket = pubsub_config.mutable_subscribe_socket();
    internal_subscribe_socket->set_socket_type(protobuf::ZeroMQServiceConfig::Socket::SUBSCRIBE);
    internal_subscribe_socket->set_socket_id(LIAISON_INTERNAL_SUBSCRIBE_SOCKET);
    internal_subscribe_socket->set_transport(protobuf::ZeroMQServiceConfig::Socket::INPROC);
    internal_subscribe_socket->set_connect_or_bind(protobuf::ZeroMQServiceConfig::Socket::CONNECT);
    internal_subscribe_socket->set_socket_name(liaison_internal_publish_socket_name());

    protobuf::ZeroMQServiceConfig::Socket* internal_publish_socket = pubsub_config.mutable_publish_socket();
    internal_publish_socket->set_socket_type(protobuf::ZeroMQServiceConfig::Socket::PUBLISH);
    internal_publish_socket->set_socket_id(LIAISON_INTERNAL_PUBLISH_SOCKET);
    internal_publish_socket->set_transport(protobuf::ZeroMQServiceConfig::Socket::INPROC);
    internal_publish_socket->set_connect_or_bind(protobuf::ZeroMQServiceConfig::Socket::CONNECT);
    internal_publish_socket->set_socket_name(liaison_internal_subscribe_socket_name());

    return pubsub_config;
    
}


void ExtendContainer1::loop()
{
    while(zeromq_service_.poll(0)) { }

    TestMessage msg;
    msg.set_a(1);
    msg.set_b(10);

    //    pubsub_node_.publish(msg); // with no group
    pubsub_node_.publish(msg, "testgroup"); // with group
    
}

void ExtendContainer1::handle_test_message(const TestMessage& msg)
{
    rx_msg_->setText("<pre>" + msg.DebugString() + "</pre>");
}

