#
# Makefile.am for xml/agent/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html)
CLEANFILES = $(html_DATA)

index.html: $(srcdir)/../../src/*.h
	@ybindir@/ydoc agent-xml -l libycp -l liby2 -l libscr --url $(htmldir) $^
