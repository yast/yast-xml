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

#ifndef _XmlAgent_h
#define _XmlAgent_h

#include <Y2.h>
#include <scr/SCRAgent.h>
#include <scr/SCRInterpreter.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/parserInternals.h>
#include <libxml/xinclude.h>
#include <libxml/xmlerror.h>

/**
 * @short An interface class between YaST2 and Xml Agent
 */
class XmlAgent : public SCRAgent
{
    private:
        /**
         * Agent private variables
         */

    public:
        /**
         * Default constructor.
         */
        XmlAgent();
        /** 
         * Destructor.
         */
        virtual ~XmlAgent();

        /**
         * Provides SCR Read ().
         * @param path Path that should be read.
         * @param arg Additional parameter.
         */
        virtual YCPValue Read(const YCPPath &path, const YCPValue& arg=YCPNull ());

        /**
         * Provides SCR Write ().
         */
        virtual YCPValue Execute(const YCPPath &path, const YCPValue& value, const YCPValue& arg );

        /**
         * Provides SCR Write ().
         */
        virtual YCPValue Dir(const YCPPath& path);

        /**
         * Provides SCR Write ().
         */
        virtual YCPValue Write(const YCPPath &path,
			   const YCPValue& value,
			   const YCPValue& arg = YCPNull());

        /**
         * Used for mounting the agent.
         */    
        virtual YCPValue otherCommand(const YCPTerm& term);

protected:
    YCPList Cdata;
    YCPMap ListEntries;
    xmlNsPtr configNamespace;
   
    virtual YCPValue ParseNodesAsMap(xmlNodePtr tree);
    virtual YCPValue ParseNodesAsList(xmlNodePtr tree);
    virtual YCPValue SetType(xmlNodePtr node);
    virtual YCPValue Xmlrpc(xmlNodePtr tree);
    virtual YCPList getParams (xmlNodePtr paramsNode);
    virtual YCPValue getValue( xmlNodePtr paramNode);
    virtual YCPMap getStruct(xmlNodePtr structNode);
    virtual YCPValue getArray(xmlNodePtr structNode);
    virtual xmlNodePtr ParseYCPMap (YCPMap map, xmlNodePtr parent, xmlDocPtr doc);
    virtual xmlNodePtr ParseYCPList (YCPList list, xmlNodePtr parent, const char *listname, xmlDocPtr doc);
    virtual bool isCDATA(YCPString section);
};

#endif /* _XmlAgent_h */
