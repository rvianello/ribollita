#include <memory>

#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/FileParsers/FileParsers.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>

PG_FUNCTION_INFO_V1(mol_from_smiles);
PG_FUNCTION_INFO_V1(mol_to_smiles);
PG_FUNCTION_INFO_V1(mol_from_molblock);
PG_FUNCTION_INFO_V1(mol_to_molblock);

}

#include "mol.hpp"

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
    std::unique_ptr<RDKit::ROMol> mol(RDKit::SmilesToMol(data, params));
    result = bytea_from_mol(mol.get());
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

  std::unique_ptr<RDKit::RWMol> mol(mol_from_bytea(data));
  std::string smiles = RDKit::MolToSmiles(
    *mol,
    isomeric, kekule, root_atom, canonical,
    all_bonds_explicit, all_hs_explicit, random
    );

  PG_RETURN_CSTRING(pnstrdup(smiles.c_str(), smiles.size()));
}

Datum
mol_from_molblock(PG_FUNCTION_ARGS)
{
  char *data = PG_GETARG_CSTRING(0);
  bool sanitize = PG_GETARG_BOOL(1);
  bool remove_hs = PG_GETARG_BOOL(2);
  bool strict_parsing = PG_GETARG_BOOL(3);

  bytea *result = nullptr;

  try {
    std::unique_ptr<RDKit::RWMol> mol(RDKit::MolBlockToMol(data, sanitize, remove_hs, strict_parsing));
    result = bytea_from_mol(mol.get());
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
mol_to_molblock(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  bool include_stereo = PG_GETARG_BOOL(1);
  int conf_id = PG_GETARG_INT32(2);
  bool kekulize = PG_GETARG_BOOL(3);
  bool allow_v2000 = PG_GETARG_BOOL(4);

  std::unique_ptr<RDKit::RWMol> mol(mol_from_bytea(data));
  std::string molblock = RDKit::MolToMolBlock(
    *mol, include_stereo, conf_id, kekulize, !allow_v2000
    );

  PG_RETURN_CSTRING(pnstrdup(molblock.c_str(), molblock.size()));
}
