#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/MolPickler.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_from_smiles);
PG_FUNCTION_INFO_V1(mol_to_smiles);

}


Datum
mol_from_smiles(PG_FUNCTION_ARGS)
{
  char *data = PG_GETARG_CSTRING(0);
  bool sanitize = PG_GETARG_BOOL(1);
  // replacements: not yet supported
  bool allow_cxsmiles = PG_GETARG_BOOL(2);
  bool strict_cxsmiles = PG_GETARG_BOOL(3);
  bool parse_name = PG_GETARG_BOOL(4);
  bool remove_hs = PG_GETARG_BOOL(5);
  bool use_legacy_stereo = PG_GETARG_BOOL(6);

  bytea *result = nullptr;

  RDKit::SmilesParserParams params = {
    0, // debugParse: not used
    sanitize,
    nullptr, // replacements: not yet supported
    allow_cxsmiles,
    strict_cxsmiles,
    parse_name,
    remove_hs,
    use_legacy_stereo
  };

  try {
    auto *mol = RDKit::SmilesToMol(data, params);
    std::string pkl;
    RDKit::MolPickler::pickleMol(mol, pkl);
    delete mol;
    auto sz = pkl.size();
    result = (bytea *) palloc(VARHDRSZ + sz);
    memcpy(VARDATA(result), pkl.data(), sz);
    SET_VARSIZE(result, VARHDRSZ + sz);
  }
  catch (...) {
    ereport(WARNING,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  if (result) {
    PG_RETURN_BYTEA_P(result);
  }
  else {
    PG_RETURN_NULL();
  }
}

Datum
mol_to_smiles(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  bool isomeric = PG_GETARG_BOOL(1);
  bool kekule = PG_GETARG_BOOL(2);
  int root_atom = PG_GETARG_INT32(3);
  bool canonical = PG_GETARG_BOOL(4);
  bool all_bonds_explicit = PG_GETARG_BOOL(5);
  bool all_hs_explicit = PG_GETARG_BOOL(6);
  bool random = PG_GETARG_BOOL(7);

  std::string pkl;
  pkl.assign(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  auto *mol = new RDKit::ROMol(pkl);
  std::string smiles = RDKit::MolToSmiles(
    *mol,
    isomeric, kekule, root_atom, canonical,
    all_bonds_explicit, all_hs_explicit, random
    );
  delete mol;

  PG_RETURN_CSTRING(pnstrdup(smiles.c_str(), smiles.size()));
}

