#
# Makefile.am for xml/agent/src
#

AM_CXXFLAGS = -DY2LOG=\"agent-xml\"

plugin_LTLIBRARIES = libpy2ag_xml.la
noinst_LTLIBRARIES = liby2ag_xml.la

liby2ag_xml_la_SOURCES =                    \
	XmlAgent.cc XmlAgent.h

liby2ag_xml_la_LDFLAGS = -version-info 2:0
liby2ag_xml_la_LIBADD = @AGENT_LIBADD@ $(LIBXMLPP_LIBS) $(XML_LIBS)

libpy2ag_xml_la_SOURCES =                           \
        $(liby2ag_xml_la_SOURCES)                   \
        Y2CCXmlAgent.cc
libpy2ag_xml_la_LDFLAGS = -version-info 2:0
libpy2ag_xml_la_LIBADD = @AGENT_LIBADD@ $(LIBXMLPP_LIBS) $(XML_LIBS)

INCLUDES = -I$(includedir) $(XML_CPPFLAGS)
