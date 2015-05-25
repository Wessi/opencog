#include <opencog/util/Logger.h>

#include "RosModule.h"

using namespace opencog;

// load/unload functions for the Module interface
DECLARE_MODULE(RosModule)

RosModule::RosModule(CogServer& cs) : Module(cs)
{
    logger().info("[RosModule] constructor");
}

RosModule::~RosModule()
{
    logger().info("[Wessi RosModule] destructor");
}

void RosModule::init()
{
    logger().info("[RosModule] init");
}

