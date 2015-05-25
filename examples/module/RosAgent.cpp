#include "RosAgent.h"
#include <opencog/server/CogServer.h>
#include <opencog/server/Factory.h>
#include <opencog/util/Logger.h>
using namespace opencog;
// load/unload functions for the Module interface
extern "C" const char* opencog_module_id() { return RosModule::info().id.c_str(); }
extern "C" Module* opencog_module_load() { return new RosModule(); }
extern "C" void opencog_module_unload(Module* m) { delete m; }
/*RosModule::RosModule()
{
logger().info("Wessi [RosModule] constructor");
}*/
RosModule::~RosModule()
{
logger().info("Wessi [RosModule] destructor");
CogServer& cogserver = static_cast<CogServer&>(server());
cogserver.destroyAllAgents(RosAgent::info().id);
}//*/
void RosModule::init()
{
logger().info("[RosModule] init");
CogServer& cogserver = static_cast<CogServer&>(server());
cogserver.registerAgent(RosAgent::info().id, &factory);
cogserver.createAgent(RosAgent::info().id, true);
}
/*RosAgent::RosAgent() : Agent()//possible to give parameter for reapeted load of RosModule...
{
logger().info("[RosAgent] constructor");
}*/
RosAgent::~RosAgent() {
logger().info("[RosAgent] destructor");
}
void RosAgent::run(CogServer* server)
{
logger().info("[RosAgent] run");
}
