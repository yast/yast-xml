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

#include "XmlAgent.h"
#include <stdarg.h>
#include <string>

int ParseError = 0;
std::string ErrorMessage("");

/**
 * Constructor
 */
XmlAgent::XmlAgent() : SCRAgent()
{
    xmlInitParser ();
}

/**
 * Destructor
 */
XmlAgent::~XmlAgent()
{
    xmlCleanupParser ();
}


bool XmlAgent::isCDATA(YCPString section)
{
    for (int i=0; i < Cdata->size(); i++) {
	if (section->value() == Cdata->value(i)->asString()->value())
	{
	    return true;
	}
    }
    return false;
}

YCPValue XmlAgent::SetType(xmlNodePtr node) {

    const xmlChar *confAttr;
    confAttr = xmlGetNsProp(node, (const xmlChar *) "type", (const xmlChar *) "http://www.suse.com/1.0/configns");
    xmlNodePtr lastChild = xmlGetLastChild (node);
    if  ( confAttr ==NULL ) {
	y2debug("no attribute");
    }
    else if (!xmlStrcmp(confAttr, (const xmlChar *)"boolean")  )
    {
	y2debug("Boolean found");
	return  (YCPBoolean((const char *)lastChild->content));
    }
    else if (!xmlStrcmp(confAttr, (const xmlChar *)"symbol"))
    {
	y2debug("Symbol found");
	YCPSymbol sym((const char *)lastChild->content);
	return  (YCPSymbol(sym));
    }
    else if (!xmlStrcmp(confAttr, (const xmlChar *)"integer")  )
    {
	y2debug("Integer found");
	return  (YCPInteger(atoi((const char *)lastChild->content)));
    }

    return  (YCPString((const char *)lastChild->content));
}




/**
 * ParseNodesAsMap
 */
YCPValue XmlAgent::ParseNodesAsMap(xmlNodePtr tree) {
    YCPValue result = YCPVoid();
    YCPValue ret = YCPVoid();
    YCPMap resultMap;
    YCPList resultList;

    xmlNodePtr nodes;
    nodes = tree->children;
    xmlKeepBlanksDefault(0);

    while (nodes != NULL) {

	xmlNodePtr lastChild = xmlGetLastChild (nodes);
    xmlChar *confAttr;
    confAttr = xmlGetNsProp(nodes, (const xmlChar *) "type", (const xmlChar *) "http://www.suse.com/1.0/configns");

	if (lastChild==NULL)
	{
	    y2debug("ignoring empty tag");
	}
	else if ( nodes!=NULL  &&  lastChild->type == XML_TEXT_NODE && xmlStrcmp(confAttr, (const xmlChar *)"list"))
	{
	    y2debug("Parsing text node: %s", (const char *)nodes->name);
	    y2debug("Parsing text node (Child): %s:%s", (const char *)lastChild->name, (const char *)lastChild->content);
	    resultMap->add(YCPString((const char *)nodes->name),  SetType(nodes));
	}
	else if ( nodes!=NULL  &&  lastChild->type == XML_CDATA_SECTION_NODE )
	{
	    y2debug("Cdata Section");
	    resultMap->add(YCPString((const char *)nodes->name), YCPString((const char *)lastChild->content));
	}
	else
	{

	    if ( !xmlStrcmp(confAttr, (const xmlChar *)"list"))
	    {
		y2debug("Parsing List: %s",  (const char *)nodes->name);
		ret = ParseNodesAsList(nodes);
		resultMap->add(YCPString((const char *)nodes->name), ret);
	    }
	    else
	    {
		y2debug("Parsing Map: %s",  (const char *)nodes->name);
		ret = ParseNodesAsMap(nodes);
		resultMap->add(YCPString((const char *)nodes->name), ret);
	    }
	}
	nodes = nodes->next;
    }
    return resultMap;

}




/**
 * ParseNodesAsList
 */

YCPValue XmlAgent::ParseNodesAsList(xmlNodePtr tree) {

    YCPValue ret = YCPVoid();
    YCPList resultList;

    xmlNodePtr nodes;
    nodes = tree->children;
    xmlKeepBlanksDefault(0);

    while (nodes != NULL) {

	xmlNodePtr lastChild = xmlGetLastChild (nodes);
    const xmlChar *confAttr;
    confAttr = xmlGetNsProp(nodes, (const xmlChar *) "type", (const xmlChar *) "http://www.suse.com/1.0/configns");

	if (lastChild==NULL) {
	    y2debug("empty tag");
	}
	else if (  nodes!=NULL  &&  lastChild->type == XML_TEXT_NODE && xmlStrcmp(confAttr, (const xmlChar *)"list") ) {
	    y2debug("text Section");
	    resultList->add(SetType(nodes) );
	}
	else if ( nodes!=NULL  &&  lastChild->type == XML_CDATA_SECTION_NODE) {
	    y2debug("Cdata Section");
	    resultList->add(YCPString((const char *)lastChild->content));

	} else	{
	    if ( !xmlStrcmp(confAttr, (const xmlChar *)"list"))
	    {
		y2debug("Parsing List: %s",  (const char *)nodes->name);
		ret = ParseNodesAsList(nodes);
		resultList->add(ret);
	    }
	    else
	    {
		y2debug("Parsing Map: %s",  (const char *)nodes->name);
		ret = ParseNodesAsMap(nodes);
		resultList->add(ret);
	    }
	}
	nodes = nodes->next;
    }
    return resultList;

}

YCPMap XmlAgent::getStruct(xmlNodePtr structNode)
{
    YCPMap result;
    xmlNodePtr members = structNode->children;
    while (members != NULL) {
	y2debug("Parsing struct member");
	xmlNodePtr children = members->children;
	YCPValue key = YCPVoid();
	while (children!=NULL) {
	    y2debug("Parsing struct member key/value");
	    xmlNodePtr lastChild = xmlGetLastChild (children);

	    if ( !xmlStrcmp(children->name,(const xmlChar *)"name")  ) {
		y2debug("key: %s", (const char *)lastChild->content);
		key = YCPString((const char *)lastChild->content);
	    }
	    children = children->next;
	}
	result->add(key,getValue(members));
	members = members->next;
    }

    return result;
}


YCPValue XmlAgent::getArray(xmlNodePtr arrayNode)
{
    YCPList result;
    xmlNodePtr data = arrayNode->children;
    y2debug("List data: %s", (const char *)data->name);
    YCPValue ret = getValue(data);
    if (ret->isList()) {
	y2debug("Got a list back");
	return ret;
    }
    else
    {
	y2debug("Got one array item back");
	YCPList newList;
	newList->add(ret);
	return newList;
    }
}


YCPValue XmlAgent::getValue( xmlNodePtr paramNode)
{
    YCPValue result = YCPVoid();
    YCPList arrayItems;

    xmlNodePtr ValueNode;
    xmlNodePtr children = paramNode->children;

    while (children!=NULL) {

	y2debug("Value: %s", (const char *)children->name );
	if  ( !xmlStrcmp(children->name, (const xmlChar *)"value"))
	{

	    ValueNode = xmlGetLastChild (children);
	    if ( !xmlStrcmp(ValueNode->name, (const xmlChar *)"int") ||
		 !xmlStrcmp(ValueNode->name, (const xmlChar *)"i4"))
	    {
		y2debug("Got integer (int)");
		xmlNodePtr lastChild = xmlGetLastChild (ValueNode);
		if (lastChild==NULL) {
		    y2error("empty tag");

		}
		else if ( lastChild->type == XML_TEXT_NODE  ) {
		    result = YCPInteger(atoi((const char *)lastChild->content));
		}
	    }
	    else if ( !xmlStrcmp(ValueNode->name, (const xmlChar *)"string"))
	    {
		y2debug("Got string ");
		xmlNodePtr lastChild = xmlGetLastChild (ValueNode);
		if (lastChild==NULL) {
		    y2error("empty tag");

		}
		else if ( lastChild->type == XML_TEXT_NODE  ) {
		    result = YCPString((const char *)lastChild->content);
		}
	    }
	    else if ( !xmlStrcmp(ValueNode->name, (const xmlChar *)"boolean"))
	    {
		y2debug("Got Boolean ");
		xmlNodePtr lastChild = xmlGetLastChild (ValueNode);
		if (lastChild==NULL) {
		    y2error("empty tag");

		}
		else if ( lastChild->type == XML_TEXT_NODE  ) {
		    if (!xmlStrcmp(lastChild->content, (const xmlChar *)"1"))
			result = YCPBoolean(true);
		    else
			result = YCPBoolean(false);
		}
	    }
	    else if ( !xmlStrcmp(ValueNode->name, (const xmlChar *)"struct"))
	    {
		result = getStruct(ValueNode);
	    }
	    else if ( !xmlStrcmp(ValueNode->name, (const xmlChar *)"array"))
	    {
		y2debug("Got array ");
		result = getArray(ValueNode);
	    }
	    arrayItems->add(result);
	}
	children = children->next;
    }
    if (arrayItems->size() > 1) {
	return arrayItems;
    }
    else
    {
	return result;
    }

}


YCPList XmlAgent::getParams (xmlNodePtr paramsNode)
{
    YCPList params;
    xmlNodePtr childrenNode;
    childrenNode = paramsNode->children;
    while (childrenNode != NULL) {
	y2debug("Parsing params");
	if ( !xmlStrcmp(childrenNode->name, (const xmlChar *)"param") )
	{
	    params->add(getValue(childrenNode));
	}
	else
	{
	    y2error("<param> needed here");
	}

	childrenNode = childrenNode->next;
    }
    return params;

}


YCPValue XmlAgent::Xmlrpc(xmlNodePtr tree)
{
    YCPMap result;
    xmlNodePtr childrenNode;

    childrenNode = tree->children;
    y2debug("Parsing xmlrpc....");
    while (childrenNode != NULL) {

	y2debug("Parsing %s:%s", (const char *)childrenNode->name, xmlGetLastChild(childrenNode)->content);
	if ( !xmlStrcmp(childrenNode->name, (const xmlChar *)"methodName") )
	{
	    result->add(YCPString("methodName"), YCPString((const char *)xmlGetLastChild(childrenNode)->content));
	}
	else if ( !xmlStrcmp(childrenNode->name, (const xmlChar *)"params") )
	{
	    result->add(YCPString("params"), getParams(childrenNode));
	}
	else if ( !xmlStrcmp(childrenNode->name, (const xmlChar *)"fault") )
	{
	    y2error("fault");
	    result->add(YCPString("fault"), getValue(childrenNode));
	}
	childrenNode = childrenNode->next;
    }
    return result;
}




/*
 * Get single values from map
 */
const char * getMapValue ( const YCPMap map, const string key)
{

    for (YCPMapIterator i = map->begin(); i != map->end (); i++)
    {
	if (!i.key()->isString())   // key must be a string
	{
	    y2error("Invalid key %s, must be a string",
		    i.value()->toString().c_str());
	}
	else // everything OK
	{
	    string variablename = i.key()->asString()->value();
	    if ( variablename == key )
	    {
		if (i.value()->isString()) {
		    YCPString ret = i.value()->asString();
		    return  (const char *)ret->value().c_str();
		}
	    }
	}
    }
    return (const char *)"";
}

/*
 * Get single values (list) from map
 */
YCPList getMapValueAsList ( const YCPMap map, const string key)
{
    for (YCPMapIterator i = map->begin(); i != map->end (); i++)
    {
	if (!i.key()->isString())   // key must be a string
	{
	    y2error("Cannot write invalid key %s, must be a string",
		    i.value()->toString().c_str());
	}
	else        // everything OK
	{
	    string variablename = i.key()->asString()->value();
	    if ( variablename == key )
	    {
		if (i.value()->isString() )
		{
		    y2error("Invalid value %s. Key %s requires one value",
			    i.value()->toString().c_str(), key.c_str());
		    break;
		}
		YCPList value = i.value()->asList();
		return ( value );
	    }
	}
    }
    YCPList *norefs = new YCPList;
    return ( *norefs );
}

/*
 * Get single values (list) from map
 */
YCPMap getMapValueAsMap ( const YCPMap map, const string key)
{
    for (YCPMapIterator i = map->begin(); i != map->end (); i++)
    {
	if (!i.key()->isString())   // key must be a string
	{
	    y2error("Cannot write invalid key %s, must be a string",
		    i.value()->toString().c_str());
	}
	else        // everything OK
	{
	    string variablename = i.key()->asString()->value();
	    if ( variablename == key )
	    {
		if (i.value()->isString() )
		{
		    y2error("Invalid value %s. Key %s requires one value",
			    i.value()->toString().c_str(), key.c_str());
		    break;
		}
		YCPMap value = i.value()->asMap();
		return ( value );
	    }
	}
    }
    YCPMap *norefs = new YCPMap;
    return ( *norefs );
}



xmlDocPtr ParseYCPMethodCall( YCPMap map, xmlDocPtr  doc)
{

    for (YCPMapIterator i = map->begin (); i != map->end (); i++)
    {
	const char *key = i.key()->asString()->value().c_str();
	y2debug("Key: %s", key);
	if (!strcmp(key, "methodName"))
	{
	    y2debug("methodName");
	    xmlNewChild(doc->children, NULL, (const xmlChar *)"methodName",(const xmlChar *)i.value()->asString()->value().c_str() );
	}
	else if (!strcmp(key,"params"))
	{
	    xmlNodePtr params = xmlNewChild(doc->children, NULL, (const xmlChar *)"params",NULL);
	    y2debug("params");
	    if (i.value()->isList())
	    {
		YCPList paramList = i.value()->asList();
		for (int i=0; i<paramList->size(); i++)
		{
		    y2debug("Adding param");
		    xmlNodePtr param = xmlNewChild(params, NULL, (const xmlChar *)"param", NULL);
		    if (param == NULL) {
			y2error("Error");
		    }

		    if (paramList->value(i)->isInteger())
		    {
			y2debug("integer found");

			xmlNodePtr val = xmlNewChild(param, NULL, (const xmlChar *)"value", NULL);
			xmlNewChild(val, NULL, (const xmlChar *)"int", (const xmlChar *)paramList->value(i)->toString().c_str());
		    }
		    else if (paramList->value(i)->isString())
		    {
			y2debug("string found");

			xmlNodePtr val = xmlNewChild(param, NULL, (const xmlChar *)"value", NULL);
			xmlNewTextChild(val, NULL, (const xmlChar *)"string", (const xmlChar *)paramList->value(i)->asString()->value().c_str());
		    }
		}
	    }
	}


    }
    return doc;
}




xmlNodePtr XmlAgent::ParseYCPList(YCPList list, xmlNodePtr parent, const  char * listname,  xmlDocPtr doc)
{
    const char *entry = getMapValue( ListEntries, listname);
    if (!entry || !*entry) {
	entry = "listentry";
    }
    for (int i = 0;i<list->size();i++)
    {
	if (list->value(i)->isString())
	{
	    // do not interpret entities, #187618
	    xmlNewTextChild(parent, NULL,
			(const xmlChar *)entry,
			(const xmlChar *)list->value(i)->asString()->value().c_str());
	}
	else if (list->value(i)->isInteger())
	{
	    y2debug("int");
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)entry,
					     (const xmlChar *)list->value(i)->toString().c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"integer");
	}
	else if (list->value(i)->isSymbol())
	{
	    int length = list->value(i)->toString().length();
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)entry,
					     (const xmlChar *)list->value(i)->toString().substr(1,length).c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"symbol");
	}
	else if (list->value(i)->isBoolean())
	{
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)entry,
					     (const xmlChar *)list->value(i)->toString().c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"boolean");
	}
	else if (list->value(i)->isMap())
	{
	    xmlNodePtr mapNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)entry,
					     NULL);
	    ParseYCPMap(list->value(i)->asMap(), mapNode, doc);

	}
	else if (list->value(i)->isList())
	{
	    xmlNodePtr listNode = xmlNewChild(parent, NULL,
					      (const xmlChar *)entry,
					      NULL);
	    xmlNewNsProp(listNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"list");
	    ParseYCPList(list->value(i)->asList(), listNode,  entry, doc);
	}
        else
        {
            y2debug("break");
            break;
        }
    }

    return parent;
}

xmlNodePtr XmlAgent::ParseYCPMap(YCPMap map, xmlNodePtr parent, xmlDocPtr doc) {

    for (YCPMapIterator i = map->begin (); i != map->end (); i++)
    {
	if (!i.key()->isString()) {
	    y2error( "The value to be stored must be a string, skipping that entry");
	    continue;
	}
	if ( i.value()->isString() &&  !isCDATA(i.key()->asString()))
	{
            y2debug("not cdata");
	    xmlNewTextChild(parent, NULL,
			(const xmlChar *)(const xmlChar *)i.key()->asString()->value().c_str(),
			(const xmlChar *)i.value()->asString()->value().c_str());
            continue;
	}

	if (i.value()->isString() &&  isCDATA(i.key()->asString()))
	{
	    y2debug("cdata section");
	    xmlNodePtr cdataNode = xmlNewChild(parent, NULL,
					       (const xmlChar *)(const xmlChar *)i.key()->asString()->value().c_str(),
					       NULL);
	    xmlNodePtr cdataBlock = xmlNewCDataBlock( doc,
						      (const xmlChar *)i.value()->asString()->value().c_str(),
						      strlen(i.value()->asString()->value().c_str()));
	    xmlAddChild(cdataNode,cdataBlock);
	}
	else if (i.value()->isInteger())
	{
            y2debug("integer");
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)i.key()->asString()->value().c_str(),
					     (const xmlChar *)i.value()->toString().c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"integer");
	}
	else if (i.value()->isSymbol())
	{
            y2debug("symbol");
	    int length = i.value()->toString().length();
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)i.key()->asString()->value().c_str(),
					     (const xmlChar *)i.value()->toString().substr(1,length).c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"symbol");
	}
	else if (i.value()->isBoolean())
	{
            y2debug("boolean");
	    xmlNodePtr intNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)i.key()->asString()->value().c_str(),
					     (const xmlChar *)i.value()->toString().c_str());
	    xmlNewNsProp(intNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"boolean");
	}
	else if (i.value()->isMap())
	{
            y2debug("map");
	    xmlNodePtr mapNode = xmlNewChild(parent, NULL,
					     (const xmlChar *)i.key()->asString()->value().c_str(),
					     NULL);
	    ParseYCPMap(i.value()->asMap(), mapNode, doc);

	}
	else if (i.value()->isList())
	{
            y2debug("list");
	    xmlNodePtr listNode = xmlNewChild(parent, NULL,
					      (const xmlChar *)i.key()->asString()->value().c_str(),
					      NULL);
	    xmlNewNsProp(listNode, configNamespace, (const xmlChar *)"type", (const xmlChar *)"list");
	    ParseYCPList(i.value()->asList(), listNode, i.key()->asString()->value().c_str(), doc);

	}
        else
        {
            y2debug("break");
            break;
        }

    }

    return parent;

}





static void
xmlagentError(void *ctx, const char *msg, ...)
{
    char *result;
    va_list args;
    char buffer[50000];

    ParseError = 1;
    buffer[0] = 0;
    va_start(args, msg);
    vsprintf(&buffer[strlen(buffer)], msg, args);
    va_end(args);
    result = (char *) xmlEncodeEntitiesReentrant(NULL, BAD_CAST buffer);
    if (result) {
	// xmlGenericError(xmlGenericErrorContext, "%s", result);
	y2error(result);
	ErrorMessage += std::string(result);
	xmlFree(result);
    }
    buffer[0] = 0;
}






/**
 * Read
 */
YCPValue XmlAgent::Read(const YCPPath &path, const YCPValue& arg, const YCPValue& opt )
{

    string content;
    string input;
    const char *xmlData;

    xmlDocPtr doc;
    xmlNodePtr tree;
    int ret, xi;

    ParseError = 0; // reset previous error state

    // Parse Path
    for (int i=0; i<path->length(); i++) {
	if (path->component_str (i)=="xmlrpc") {
	    content = path->component_str (i);
	}
	else if (path->component_str (i) == "string") {
	    input = path->component_str (i);
	}
        else if (path->component_str (i) == "error_message") {
            return YCPString(ErrorMessage);
        }
    }

    ErrorMessage = std::string();

    // Get file name/string content from arguments

    if ( !arg.isNull() && arg->isString())
        xmlData =  arg->asString()->value().c_str();
    else {
        return YCPError (string ("File argument or data string missing"));
    }


    xmlKeepBlanksDefault	(0);
    xmlDoValidityCheckingDefaultValue = 0;
    xmlGetWarningsDefaultValue = 1;
    xmlParserCtxtPtr ctxt;
    xmlSAXHandler silent, *old;


    if (input == "string")
    {
	y2debug("Parsing from memory");
	ctxt = xmlCreateMemoryParserCtxt(xmlData,strlen(xmlData));
    }
    else
    {
	y2debug("Parsing a file");
	ctxt = xmlCreateFileParserCtxt(xmlData);

    }

    if (ctxt == NULL)
    {
	doc = NULL;
    }
    else
    {

	y2milestone( "Setting XML generic error handler" );
	xmlSetGenericErrorFunc (ctxt, (xmlGenericErrorFunc) xmlagentError);

	memcpy(&silent, ctxt->sax, sizeof(silent));

	old = ctxt->sax;
	silent.error = xmlagentError;
	if (xmlGetWarningsDefaultValue)
	    silent.warning = xmlagentError;
	else
	    silent.warning = NULL;

	silent.fatalError = xmlagentError;
	ctxt->sax = &silent;
	ctxt->vctxt.error = xmlagentError;
	if (xmlGetWarningsDefaultValue)
	    ctxt->vctxt.warning = xmlagentError;
	else
	    ctxt->vctxt.warning = NULL;

	xmlParseDocument(ctxt);

	ret = ctxt->wellFormed;
	doc = ctxt->myDoc;

	xi = xmlXIncludeProcess(doc);
	if (xi == -1)
	{
	    y2milestone( "Freeing generic XML error handler");
	    xmlSetGenericErrorFunc( NULL, NULL );
	    return YCPError(string ("Failed while processing XIncludes"));
	}

	ctxt->sax = old;

	y2milestone( "Freeing generic XML error handler");
	xmlSetGenericErrorFunc( NULL, NULL );

	xmlFreeParserCtxt(ctxt);

	if (!ret)
	{
	    xmlFreeDoc(doc);
	    doc = NULL;
	    return YCPError(string("Error while parsing...."));
	}
	if (ParseError)
	{
	    return YCPError(string("Error while parsing...."));
	}
    }


    if (doc == NULL) {
	y2error("Error opening the file");
	xmlFreeDoc(doc);
	return YCPVoid();
    }

    tree = xmlDocGetRootElement(doc);

    if (tree == NULL) {
        y2error("empty document");
	xmlFreeDoc(doc);
	return YCPVoid();
    }

    YCPValue result = YCPVoid();
    if (!strcmp(content.c_str(),"xmlrpc"))
    {
	result = Xmlrpc(tree);
    }
    else
    {
	result = ParseNodesAsMap(tree);
    }

    xmlFreeDoc(doc);
    return result;;


}


/**
 * Write
 */
YCPBoolean XmlAgent::Write(const YCPPath &path, const YCPValue& value,
    const YCPValue& arg)
{
    y2error("Wrong path '%s' in Write().", path->toString().c_str());
    return YCPBoolean(false);
}


/**
 * Execute  XML from YCP data
 */
YCPValue XmlAgent::Execute(const YCPPath &path, const YCPValue& value, const YCPValue& arg )
{
    const char *filename;

    YCPValue result = YCPVoid();
    int size;
    const char * input = "";
    const char *content = "";

    xmlDocPtr doc, newDoc;
    xmlChar *mem;

    for (int i=0; i<path->length(); i++)
    {
	if (path->component_str (i)=="xmlrpc")
	{
	    content = (const char *)path->component_str (i).c_str();
	}
	else if (path->component_str (i) == "string") {
	    input = (const char *)path->component_str (i).c_str();
	}
    }

    YCPMap argMap = arg->asMap();
    YCPMap options = value->asMap();

    Cdata = getMapValueAsList ( options,"cdataSections" );
    ListEntries = getMapValueAsMap(options,"listEntries");

    const char *rootElement	= getMapValue ( options,"rootElement");
    const char *systemID	= getMapValue ( options,"systemID" );
    const char *typeNS		= getMapValue ( options,"typeNamespace" );
    const char *fileName	= getMapValue ( options,"fileName");
    const char *nameSpace	= getMapValue ( options,"nameSpace");


    // Get file name from arguments
    if (fileName && *fileName )
    {
        filename =  fileName;
    }
    else if (input && *input)
    {
	y2milestone("String handling");
    }
    else
    {
        y2error("File argument missing");
        return YCPVoid();
    }


    doc = xmlNewDoc((const xmlChar *)"1.0");

    if (!strcmp(content,"xmlrpc"))
    {
	y2milestone("XML-RPC handling");
	doc->children = xmlNewDocNode(doc, NULL, (const xmlChar *)"methodCall", NULL);
	newDoc =  ParseYCPMethodCall(argMap, doc);
    }
    else
    {
	xmlNodePtr root = xmlNewDocNode(doc, NULL, (const xmlChar *)rootElement, NULL);
	xmlDocSetRootElement (doc, root);

	if (nameSpace && *nameSpace)
	{
	xmlNewNs (root,  (const xmlChar *)nameSpace, NULL);
	}

	if (typeNS && *typeNS) {
	configNamespace = xmlNewNs (root,  (const xmlChar *)typeNS, (const xmlChar *)"config");
	}
	else {
	    configNamespace = NULL;
	}


	doc->children = ParseYCPMap(argMap, root, doc);
	doc->intSubset = xmlCreateIntSubset (doc,
					     (const xmlChar *)rootElement,
					     NULL,
					     (const xmlChar *)systemID);
	newDoc = xmlCopyDoc (doc,1);
    }
    xmlIndentTreeOutput  = 1;
    xmlKeepBlanksDefault	(0);

    if (!strcmp(input,"string"))
    {
	xmlDocDumpFormatMemory (newDoc, &mem, &size, 1);

	result = YCPString((const char *)mem);

	xmlFree(mem);
    }
    else
    {
	result = YCPBoolean( xmlSaveFormatFile(filename, newDoc, 1) != -1 );
    }
    xmlFreeDoc(doc);
    if (strcmp(input,"string"))
    {
	xmlFreeDoc(newDoc);
    }
    return result;

}


/**
 * Dir
 */
YCPList XmlAgent::Dir(const YCPPath& path)
{
    y2error("Wrong path '%s' in Read().", path->toString().c_str());
    return YCPNull();
}


/**
 * otherCommand
 */
YCPValue XmlAgent::otherCommand(const YCPTerm& term)
{
    string sym = term->name();

    if (sym == "XmlAgent") {
        /* Your initialization */
        return YCPVoid();
    }

    return YCPNull();
}

