#
# Makefile.am for liby2util/doc/autodocs
#

htmldir = $(docdir)/autodocs

html_DATA = index.html $(wildcard *.html *.png) doxygen.css @RPMNAME@.tag

CLEANFILES = $(html_DATA) doxygen.log

header_dir = $(srcdir)/../../src/
index.html: $(header_dir)/*.h 
	@ybindir@/ydoxygen PROJECT_NAME=@RPMNAME@ TAGFILES=
