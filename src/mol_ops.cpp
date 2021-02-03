#include <GraphMol/MolOps.h>
#include <GraphMol/MolPickler.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_formal_charge);
PG_FUNCTION_INFO_V1(mol_kekulize);

}

Datum
mol_formal_charge(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  std::string pkl(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::ROMol(pkl);
  int charge = RDKit::MolOps::getFormalCharge(*mol);
  delete mol;

  PG_RETURN_INT32(charge);
}

Datum
mol_kekulize(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  std::string pkl(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::RWMol(pkl);
  RDKit::MolOps::Kekulize(*mol);

  RDKit::MolPickler::pickleMol(
    mol, pkl,
    RDKit::PicklerOps::AllProps | RDKit::PicklerOps::CoordsAsDouble);
  delete mol;
  auto sz = pkl.size();
  bytea *result = (bytea *) palloc(VARHDRSZ + sz);
  memcpy(VARDATA(result), pkl.data(), sz);
  SET_VARSIZE(result, VARHDRSZ + sz);

  PG_RETURN_BYTEA_P(result);
}
