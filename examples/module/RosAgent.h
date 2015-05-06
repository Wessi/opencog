
#ifndef _OPENCOG_Ros_AGENT_H
#define _OPENCOG_Ros_AGENT_H

#include <string>

#include <opencog/server/Agent.h>
#include <opencog/server/Factory.h>
#include <opencog/server/Module.h>

namespace opencog
{

class CogServer;

class RosAgent : public Agent
{
public:

    virtual const ClassInfo& classinfo() const { return info(); }
    static const ClassInfo& info() {
        static const ClassInfo _ci("opencog::RosAgent");
        return _ci;
    }
    RosAgent();
    ~RosAgent();
    void run(CogServer*);
    bool connected();
    void printToScreen(char*);

}; // class

class ExampleModule : public Module
{
private:

    Factory<RosAgent, Agent> factory;

public:

    static const ClassInfo& info() {
        static const ClassInfo _ci("opencog::ExampleModule");
        return _ci;
    }

    ExampleModule();
    ~ExampleModule();
    void init();
};

} // namespace opencog

#endif // _OPENCOG_Ros_AGENT_H
