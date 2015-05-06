#ifndef _OPENCOG_Example_MODULE_H
#define _OPENCOG_Example_MODULE_H

#include <opencog/server/Module.h>

namespace opencog
{
class CogServer;

class ExampleModule : public Module
{
private:

public:

    ExampleModule(CogServer&);
    virtual ~ExampleModule();
    virtual void init();
    virtual const char* id();
};

} // namespace opencog

#endif // _OPENCOG_Example_MODULE_H
