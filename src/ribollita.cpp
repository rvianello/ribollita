#include <RDGeneral/versions.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

}

#include <algorithm>
#include <cstring>

extern "C" {

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(ribollita_version);
PG_FUNCTION_INFO_V1(rdkit_version);
PG_FUNCTION_INFO_V1(rdkit_build);
PG_FUNCTION_INFO_V1(boost_version);

}


Datum
ribollita_version(PG_FUNCTION_ARGS)
{
  (void) fcinfo; // unused
  PG_RETURN_CSTRING(pstrdup( RIBOLLITA_VERSION ));
}

Datum
rdkit_version(PG_FUNCTION_ARGS)
{
  (void) fcinfo; // unused
  PG_RETURN_CSTRING(pstrdup(RDKit::rdkitVersion));
}

Datum
rdkit_build(PG_FUNCTION_ARGS)
{
  (void) fcinfo; // unused
  PG_RETURN_CSTRING(pstrdup(RDKit::rdkitBuild));
}

Datum
boost_version(PG_FUNCTION_ARGS)
{
  (void) fcinfo; // unused
  PG_RETURN_CSTRING(pstrdup(RDKit::boostVersion));
}
