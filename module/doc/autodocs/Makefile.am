#
# Makefile.am for y2c_xml/doc/autodocs
#
# $Id$
#

htmldir = $(docdir)/autodocs

index.html: $(srcdir)/../../src/*.ycp $(srcdir)/../../src/include/*.ycp
	@ybindir@/ycpdoc -d. $(srcdir)/../../src/*.ycp
	@ybindir@/ycpdoc -d. $(srcdir)/../../src/include/*.ycp

html_DATA = index.html $(wildcard *.html)

CLEANFILES = $(html_DATA)
