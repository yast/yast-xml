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

#include "Y2CCXmlAgent.h"
#include "Y2XmlAgentComponent.h"


Y2CCXmlAgent::Y2CCXmlAgent()
    : Y2ComponentCreator(Y2ComponentBroker::BUILTIN)
{
}


bool
Y2CCXmlAgent::isServerCreator() const
{
    return true;
}


Y2Component *
Y2CCXmlAgent::create(const char *name) const
{
    if (!strcmp(name, "ag_xml")) return new Y2XmlAgentComponent();
    else return 0;
}


Y2CCXmlAgent g_y2ccag_xml;
