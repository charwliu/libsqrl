ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	LICENSE \
	README.md \
	THANKS

SUBDIRS = \
	src 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc

DISTCLEANFILES = $(pkgconfig_DATA)

