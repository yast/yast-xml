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

#include <scr/Y2AgentComponent.h>
#include <scr/Y2CCAgentComponent.h>

#include "XmlAgent.h"


typedef Y2AgentComp <XmlAgent> Y2XmlAgentComp;

Y2CCAgentComp <Y2XmlAgentComp> g_y2ccag_xml ("ag_xml");

/*
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


Y2CCXmlAgent g_y2ccag_xml;*/
