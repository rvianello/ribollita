#include <GraphMol/Descriptors/MolDescriptors.h>
#include <GraphMol/MolPickler.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_amw);

}

Datum
mol_amw(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  std::string pkl;
  pkl.assign(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::ROMol(pkl);
  double amw = RDKit::Descriptors::calcAMW(*mol);
  delete mol;

  PG_RETURN_FLOAT8(amw);
}
