#include <GraphMol/RDKitBase.h>
#include <GraphMol/MolPickler.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_in);
PG_FUNCTION_INFO_V1(mol_out);
PG_FUNCTION_INFO_V1(mol_recv);
PG_FUNCTION_INFO_V1(mol_send);

}


Datum
mol_in(PG_FUNCTION_ARGS)
{
  char *data = PG_GETARG_CSTRING(0);
  bytea *result;

  try {
    auto *mol = RDKit::SmilesToMol(data);
    std::string pkl;
    RDKit::MolPickler::pickleMol(mol, pkl);
    delete mol;
    auto sz = pkl.size();
    result = (bytea *) palloc(VARHDRSZ + sz);
    memcpy(VARDATA(result), pkl.data(), sz);
    SET_VARSIZE(result, VARHDRSZ + sz);
  }
  catch (...) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  PG_RETURN_BYTEA_P(result);
}

Datum
mol_out(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  std::string pkl;
  pkl.assign(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::ROMol(pkl);
  std::string smiles = RDKit::MolToSmiles(*mol, true);
  delete mol;

  PG_RETURN_CSTRING(pnstrdup(smiles.c_str(), smiles.size()));
}

Datum
mol_recv(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  std::string pkl;
  pkl.assign(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::ROMol(pkl);

  if (!mol) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  PG_RETURN_BYTEA_P(data);
}

Datum
mol_send(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  PG_RETURN_BYTEA_P(data);
}
