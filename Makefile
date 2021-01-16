MODULES = ribollita
EXTENSION = ribollita
DATA = ribollita--1.0.sql
#DOCS = README.ribollita
#HEADERS_ribollita = ribollita.h

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
