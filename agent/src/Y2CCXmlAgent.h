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

#ifndef Y2CCXmlAgent_h
#define Y2CCXmlAgent_h

#include "Y2.h"

/**
 * @short And a component creator for the component
 */
class Y2CCXmlAgent : public Y2ComponentCreator
{
    public:
        /**
         * Enters this component creator into the global list of component creators.
         */
        Y2CCXmlAgent();
    
        /**
         * Specifies, whether this creator creates Y2Servers.
         */
        virtual bool isServerCreator() const;
    
        /**
         * Implements the actual creating of the component.
         */
        virtual Y2Component *create(const char *name) const;
};

#endif
