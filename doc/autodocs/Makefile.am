#
# Makefile.am for xml/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html)
CLEANFILES = $(html_DATA)

index.html: $(srcdir)/../../src/*.h $(srcdir)/../../ycp/*.ycp	\
		$(srcdir)/../../ycp/include/*.ycp
	@ybindir@/ycpdoc -d. $(srcdir)/../../ycp/*.ycp
	@ybindir@/ycpdoc -d. $(srcdir)/../../ycp/include/*.ycp
	cd $(srcdir)/../../src && @ybindir@/ydoc @RPMNAME@ *.h
	mv -f $(srcdir)/../../src/*.html .
