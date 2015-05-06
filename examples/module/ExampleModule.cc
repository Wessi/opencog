#include <opencog/util/Logger.h>

#include "ExampleModule.h"

using namespace opencog;

// load/unload functions for the Module interface
DECLARE_MODULE(ExampleModule)

ExampleModule::ExampleModule(CogServer& cs) : Module(cs)
{
    logger().info("[ExampleModule] constructor");
}

ExampleModule::~ExampleModule()
{
    logger().info("[Wessi ExampleModule] destructor");
}

void ExampleModule::init()
{
    logger().info("[ExampleModule] init");
}

