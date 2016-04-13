#include "liaison_extend/protobuf/test.pb.h"

#include "goby/pb/application.h"
#include "config.pb.h"

class Echo : public goby::pb::Application
{
public:
    Echo(protobuf::EchoConfig* cfg);
    ~Echo() { }
    
private:
    void loop();
    void handle_test_message(const TestMessage& msg);
};

int main(int argc, char* argv[])
{
    protobuf::EchoConfig cfg;
    goby::run<Echo>(argc, argv, &cfg);
}

Echo::Echo(protobuf::EchoConfig* cfg) : goby::pb::Application(cfg)
{
    subscribe(&Echo::handle_test_message, this, "testgroup");
}


void Echo::loop()
{
}

void Echo::handle_test_message(const TestMessage& msg)
{
    std::cout << msg.DebugString() << std::endl;
    TestMessage msgout = msg;
    static int a = 0;
    static int b = 0;
    msgout.set_a(a++);
    msgout.set_b(b++);
    publish(msgout, "testgroup");
}

