#ifndef RIBOLLITA_MOL_INCLUDED
#define RIBOLLITA_MOL_INCLUDED

// RDKit::RWMol and RDKit::ROMol are basically the same type,
// but the former subclasses the latter. To streamline the 
// interoperability with the RDKit C++ API, coversion to bytea *
// takes an RDKit::ROMol *, and conversion from bytea * returns
// an RDKit::RWMol *.

bytea * bytea_from_mol(const RDKit::ROMol * mol);
RDKit::RWMol * mol_from_bytea(bytea *data);

#endif
