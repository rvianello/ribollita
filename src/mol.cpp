#include <memory>

#include <GraphMol/MolPickler.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>
#include <common/base64.h>
#include <lib/stringinfo.h>

PG_FUNCTION_INFO_V1(mol_in);
PG_FUNCTION_INFO_V1(mol_out);
PG_FUNCTION_INFO_V1(mol_recv);
PG_FUNCTION_INFO_V1(mol_send);

}

#include "mol.hpp"

Datum
mol_in(PG_FUNCTION_ARGS)
{
  char *text = PG_GETARG_CSTRING(0);
  int text_len = strlen(text);

  int data_len = pg_b64_dec_len(text_len);
  char *data = (char *) palloc(data_len);
  data_len = pg_b64_decode(text, text_len, data, data_len);

  if (data_len < 0) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not decode molecule data as base64"));
  }

  try {
    std::string pkl(data, data_len);
    std::unique_ptr<RDKit::ROMol> mol(new RDKit::ROMol(pkl));
  }
  catch (...) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  bytea *result = (bytea *) palloc(VARHDRSZ + data_len);
  memcpy(VARDATA(result), data, data_len);
  SET_VARSIZE(result, VARHDRSZ + data_len);

  PG_RETURN_BYTEA_P(result);
}

Datum
mol_out(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);

  int dstlen = pg_b64_enc_len(VARSIZE_ANY_EXHDR(data));
  char *result = (char *) palloc(dstlen + 1);

  dstlen = pg_b64_encode(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data), result, dstlen);
  if (dstlen < 0) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not encode molecule data as base64"));
  }
  result[dstlen] = '\0';

  PG_RETURN_CSTRING(result);
}

Datum
mol_recv(PG_FUNCTION_ARGS)
{
  StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);

  // the bytea receive function in postgres'
  // src/backend/utils/adt/varlena.c
  // seems to assume that the buf's cursor may
  // be not null, and that could point to the actual
  // input inside the StringInfo buffer.
  char *data = buf->data + buf->cursor;
  int size = buf->len - buf->cursor;

  try {
    std::string pkl(data, size);
    std::unique_ptr<RDKit::ROMol> mol(new RDKit::ROMol(pkl));
  }
  catch (...) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  bytea * result = (bytea *) palloc(VARHDRSZ + size);
  memcpy(VARDATA(result), data, size);
  SET_VARSIZE(result, VARHDRSZ + size);

  PG_RETURN_BYTEA_P(result);
}

Datum
mol_send(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_P_COPY(0);
  PG_RETURN_BYTEA_P(data);
}

bytea * bytea_from_mol(const RDKit::ROMol * mol)
{
  std::string pkl;
  RDKit::MolPickler::pickleMol(
    mol, pkl,
    RDKit::PicklerOps::MolProps | RDKit::PicklerOps::CoordsAsDouble);
  auto sz = pkl.size();
  bytea *data = (bytea *) palloc(VARHDRSZ + sz);
  memcpy(VARDATA(data), pkl.data(), sz);
  SET_VARSIZE(data, VARHDRSZ + sz);
  return data;
}

RDKit::RWMol * mol_from_bytea(bytea *data)
{
  std::string pkl(VARDATA_ANY(data), VARSIZE_ANY_EXHDR(data));
  return new RDKit::RWMol(pkl);
}
