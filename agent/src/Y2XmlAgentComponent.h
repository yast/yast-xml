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

#ifndef Y2XmlAgentComponent_h
#define Y2XmlAgentComponent_h

#include "Y2.h"

class SCRInterpreter;
class XmlAgent;


class Y2XmlAgentComponent : public Y2Component
{
    private:
        SCRInterpreter *interpreter;
        XmlAgent *agent;
    
    public:
    
        /**
         * Default constructor
         */
        Y2XmlAgentComponent();
        
        /**
         * Destructor
         */
        ~Y2XmlAgentComponent();
        
        /**
         * Returns true: The scr is a server component
         */
        bool isServer() const;
        
        /**
         * Returns the name of the module.
         */
        virtual string name() const;
        
        /**
         * Starts the server, if it is not already started and does
         * what a server is good for: Gets a command, evaluates (or
         * executes) it and returns the result.
         * @param command The command to be executed. Any YCPValueRep
         * can be executed. The execution is performed by some
         * YCPInterpreter.
         */
        virtual YCPValue evaluate(const YCPValue& command);
};

#endif
