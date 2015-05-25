#ifndef _OPENCOG_Ros_MODULE_H
#define _OPENCOG_Ros_MODULE_H

#include <opencog/server/Module.h>

namespace opencog
{
class CogServer;

class RosModule : public Module
{
private:

public:

    RosModule(CogServer&);
    virtual ~RosModule();
    virtual void init();
    virtual const char* id();
};

} // namespace opencog

#endif // _OPENCOG_Ros_MODULE_H
