#include <memory>
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

  std::unique_ptr<RDKit::RWMol> mol(mol_from_bytea(data));
  double amw = RDKit::Descriptors::calcAMW(*mol);

  PG_RETURN_FLOAT8(amw);
}
