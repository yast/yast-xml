/*
 * YaST2: Core system
 *
 * Description:
 *   YaST2 SCR: Xml agent implementation
 *
 * Authors:
 *   Anas Nashif <nashif@suse.de>
 *
 * $Id$
 */

#include "Y2XmlAgentComponent.h"
#include <scr/SCRInterpreter.h>
#include "XmlAgent.h"


Y2XmlAgentComponent::Y2XmlAgentComponent()
    : interpreter(0),
      agent(0)
{
}


Y2XmlAgentComponent::~Y2XmlAgentComponent()
{
    if (interpreter) {
        delete interpreter;
        delete agent;
    }
}


bool
Y2XmlAgentComponent::isServer() const
{
    return true;
}

string
Y2XmlAgentComponent::name() const
{
    return "ag_xml";
}


YCPValue Y2XmlAgentComponent::evaluate(const YCPValue& value)
{
    if (!interpreter) {
        agent = new XmlAgent();
        interpreter = new SCRInterpreter(agent);
    }
    
    return interpreter->evaluate(value);
}

