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
    auto *mol = new RDKit::ROMol(pkl);
    delete mol;
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

  try {
    std::string pkl(buf->data, buf->len);
    auto *mol = new RDKit::ROMol(pkl);
    delete mol;
  }
  catch (...) {
    ereport(ERROR,
            errcode(ERRCODE_DATA_EXCEPTION),
            errmsg("could not construct molecule"));
  }

  bytea * result = (bytea *) palloc(VARHDRSZ + buf->len);
  memcpy(VARDATA(result), buf->data, buf->len);
  SET_VARSIZE(result, VARHDRSZ + buf->len);

  PG_RETURN_BYTEA_P(result);
}

Datum
mol_send(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_P_COPY(0);
  PG_RETURN_BYTEA_P(data);
}
