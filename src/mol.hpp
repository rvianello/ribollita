#ifndef RIBOLLITA_MOL_INCLUDED
#define RIBOLLITA_MOL_INCLUDED

bytea * bytea_from_mol(const RDKit::ROMol * mol);

RDKit::ROMol * romol_from_bytea(bytea *data);
RDKit::RWMol * rwmol_from_bytea(bytea *data);

#endif
