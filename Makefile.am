 AUTOMAKE_OPTIONS = subdir-objects
 ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

 bin_PROGRAMS = chess
 chess_SOURCES = clock.h clock.cc movement.h movement.cc main.cc Clockwindow.cc Clockwindow.h TimeControlDialog.cc

chess_LDADD = @MYAPP_LIBS@
AM_CPPFLAGS = @MYAPP_CFLAGS@ -std=gnu++0x
AM_LDFLAGS=-lpthread
# dist_noinst_SCRIPTS = autogen.sh

