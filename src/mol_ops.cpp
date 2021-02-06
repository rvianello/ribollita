#include <GraphMol/MolOps.h>

extern "C" {

#include <postgres.h>
#include <fmgr.h>
#include <funcapi.h>
#include <miscadmin.h>
#include <utils/tuplestore.h>

PG_FUNCTION_INFO_V1(mol_formal_charge);
PG_FUNCTION_INFO_V1(mol_kekulize);
PG_FUNCTION_INFO_V1(mol_fragments);
PG_FUNCTION_INFO_V1(mol_add_hs);
PG_FUNCTION_INFO_V1(mol_remove_hs);

}

#include "mol.hpp"

Datum
mol_formal_charge(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  auto *mol = romol_from_bytea(data);
  int charge = RDKit::MolOps::getFormalCharge(*mol);
  delete mol;

  PG_RETURN_INT32(charge);
}

Datum
mol_kekulize(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  auto *mol = rwmol_from_bytea(data);

  RDKit::MolOps::Kekulize(*mol);

  bytea *result = bytea_from_mol(mol);
  delete mol;
  PG_RETURN_BYTEA_P(result);
}

Datum
mol_add_hs(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  bool explicit_only = PG_GETARG_BOOL(1);
  bool add_coords = PG_GETARG_BOOL(2);
  // only_on_atoms: not yet supported
  bool add_residue_info = PG_GETARG_BOOL(3);

  auto *mol = rwmol_from_bytea(data);

  RDKit::MolOps::addHs(
    *mol, explicit_only, add_coords, nullptr, add_residue_info);

  bytea *result = bytea_from_mol(mol);
  delete mol;
  PG_RETURN_BYTEA_P(result);
}

Datum
mol_remove_hs(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  bool implicit_only = PG_GETARG_BOOL(1);
  bool update_explicit_count = PG_GETARG_BOOL(2);
  bool sanitize = PG_GETARG_BOOL(3);

  auto *mol = rwmol_from_bytea(data);

  RDKit::MolOps::removeHs(
    *mol, implicit_only, update_explicit_count, sanitize);

  bytea *result = bytea_from_mol(mol);
  delete mol;
  PG_RETURN_BYTEA_P(result);
}

Datum
mol_fragments(PG_FUNCTION_ARGS)
{
  bytea *data = PG_GETARG_BYTEA_PP(0);
  bool sanitize_fragments = PG_GETARG_BOOL(1);
  bool copy_conformers = PG_GETARG_BOOL(2);

  ReturnSetInfo *rsinfo = (ReturnSetInfo *) fcinfo->resultinfo;
  TupleDesc tupdesc;
  MemoryContext per_query_ctx;
  MemoryContext oldcontext;

  /* check to see if caller supports us returning a tuplestore */
  if (rsinfo == NULL || !IsA(rsinfo, ReturnSetInfo)) {
    ereport(ERROR,
      errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
	  errmsg("set-valued function called in context that cannot accept a set"));
  }

  if (!(rsinfo->allowedModes & SFRM_Materialize) ||
      rsinfo->expectedDesc == NULL) {
    ereport(ERROR,
      errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
      errmsg("materialize mode required, but it is not allowed in this context"));
  }

  /*
   * Materialize mode works like this: the function creates a Tuplestore holding
   * the (possibly empty) result set, and returns it.  There are no multiple calls.
   * The function must also return a TupleDesc that indicates the tuple structure.
   * The Tuplestore and TupleDesc should be created in the context
   * econtext->ecxt_per_query_memory (note this will *not* be the context the
   * function is called in).  The function stores pointers to the Tuplestore and
   * TupleDesc into ReturnSetInfo, sets returnMode to indicate materialize mode,
   * and returns null.  isDone is not used and should be left at ExprSingleResult.
   *
   * The Tuplestore must be created with randomAccess = true if
   * SFRM_Materialize_Random is set in allowedModes, but it can (and preferably
   * should) be created with randomAccess = false if not.  Callers that can support
   * both ValuePerCall and Materialize mode will set SFRM_Materialize_Preferred,
   * or not, depending on which mode they prefer.
   *
   * If available, the expected tuple descriptor is passed in ReturnSetInfo;
   * in other contexts the expectedDesc field will be NULL.  The function need
   * not pay attention to expectedDesc, but it may be useful in special cases.
   */

  per_query_ctx = rsinfo->econtext->ecxt_per_query_memory;
  oldcontext = MemoryContextSwitchTo(per_query_ctx);

  /* get the requested return tuple description */
  tupdesc = CreateTupleDescCopy(rsinfo->expectedDesc);

  /*
   * Check to make sure we have a reasonable tuple descriptor
   *
   * Note we will attempt to coerce the values into whatever the return
   * attribute type is and depend on the "in" function to complain if
   * needed.
   */
  if (tupdesc->natts != 1) {
	ereport(ERROR,
			errcode(ERRCODE_SYNTAX_ERROR),
			errmsg("unexpected length return tuple"));
  }

  /* let the caller know we're sending back a tuplestore */
  rsinfo->returnMode = SFRM_Materialize;

  /* do we need a random access tuplestore? */
  bool random_access = rsinfo->allowedModes & SFRM_Materialize_Random;

  /* initialize our tuplestore (while still in query context!) */
  Tuplestorestate *tupstore = tuplestore_begin_heap(random_access, false, work_mem);

  auto *mol = romol_from_bytea(data);
  auto fragment_mols = RDKit::MolOps::getMolFrags(
      *mol, sanitize_fragments, nullptr, nullptr,
      copy_conformers);
  delete mol;

  Datum values[1];
  bool nulls[1];
  nulls[0] = false;

  for (auto frag : fragment_mols) {
    bytea *result = bytea_from_mol(*frag);
    values[0] = PointerGetDatum(result);
    HeapTuple tuple = heap_form_tuple(tupdesc, values, nulls);
    tuplestore_puttuple(tupstore, tuple);
  }

  /*
   * SFRM_Materialize mode expects us to return a NULL Datum. The actual
   * tuples are in our tuplestore and passed back through rsinfo->setResult.
   * rsinfo->setDesc is set to the tuple description that we actually used
   * to build our tuples with, so the caller can verify we did what it was
   * expecting.
   */
  rsinfo->setResult = tupstore;
  rsinfo->setDesc = tupdesc;
  MemoryContextSwitchTo(oldcontext);

  return (Datum) 0;
}