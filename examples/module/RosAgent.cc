
#include "RosAgent.h"

#include <opencog/server/CogServer.h>
#include <opencog/server/Factory.h>
#include <opencog/util/Logger.h>

/*Include to access action messages*/
#include <opencog/atomspace/TruthValue.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atomspace/Atom.h>
using namespace opencog;

// load/unload functions for the Module interface
extern "C" const char* opencog_module_id()
{ return ExampleModule::info().id.c_str(); }
extern "C" Module*     opencog_module_load()
{ return new ExampleModule(); }
extern "C" void        opencog_module_unload(Module* m)
{ delete m; }

ExampleModule::ExampleModule()
{
    logger().info("[ExampleModule] constructor");
}

ExampleModule::~ExampleModule()
{
    logger().info("[ExampleModule] destructor");
    CogServer& cogserver = static_cast<CogServer&>(server());
    cogserver.destroyAllAgents(RosAgent::info().id);
}

void ExampleModule::init()
{
    logger().info("[ExampleModule] init");
    CogServer& cogserver = static_cast<CogServer&>(server());
    cogserver.registerAgent(RosAgent::info().id, &factory);
    cogserver.createAgent(RosAgent::info().id, true);
}

RosAgent::RosAgent() : Agent(100)
{
    logger().info("[RosAgent] constructor");
}

RosAgent::~RosAgent() {
    logger().info("[RosAgent] destructor");
}

void RosAgent::run(CogServer* server)
{
    std::cout << "############## This is run function ##############" <<endl;
    AtomTable at = atomspace().getAtomTable();
    HandleSeq hs = atomspace().fetchAtom(at);
    int size = hs.size();
    while(size>0)
    {
        char msg[] = hs.data()->do_res(size--);
        printToConsole(msg);
    }

    logger().info("[RosAgent] run");
}
bool RosAgent::connected(){
    bool flag=false;
    //do something
    //flag=...
    return flag;
}

void RosAgent::printToConsole(char *msg[]){
    std::cout << "############## This is printToConsole function ##############" <<endl;
    std::cout << "This is the action message" <<endl;
    switch (msg) {
    case 'blink':
        std::cout << "the action message is "<< msg <<endl;
        break;
    case 'blink-relaxed':
        std::cout << "the action message is "<< msg <<endl;
        break;
    case 'blink-sleepy':
        std::cout << "the action message is "<< msg <<endl;
        break;
    default:
        break;
    }
    return;
}

