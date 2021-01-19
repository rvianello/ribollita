MODULES = ribollita
EXTENSION = ribollita
DATA = ribollita--0.0.sql
#DOCS = README.ribollita
#HEADERS_ribollita = ribollita.h

REGRESS = copytext

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
