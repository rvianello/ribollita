#include <GraphMol/Descriptors/MolDescriptors.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_amw);

}

#include "mol.hpp"

Datum
mol_amw(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  auto *mol = romol_from_bytea(data);
  double amw = RDKit::Descriptors::calcAMW(*mol);
  delete mol;

  PG_RETURN_FLOAT8(amw);
}
