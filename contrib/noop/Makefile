# contrib/noop/Makefile

MODULE_big = noop
OBJS = noop.o

EXTENSION = noop
DATA = noop--1.0.sql
PGFILEDESC = "noop access method - informational NO-OP index"

REGRESS = noop

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/noop
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
