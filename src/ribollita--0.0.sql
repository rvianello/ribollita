CREATE FUNCTION copytext(text) RETURNS text
     AS 'MODULE_PATHNAME', 'copytext'
     LANGUAGE C STRICT;

CREATE FUNCTION reversetext(text) RETURNS text
     AS 'MODULE_PATHNAME', 'reversetext'
     LANGUAGE C STRICT;


CREATE TYPE mol;

CREATE FUNCTION mol_in(cstring)
    RETURNS mol
    AS 'MODULE_PATHNAME', 'mol_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_out(mol)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'mol_out'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE mol (
   internallength = variable,
   input = mol_in,
   output = mol_out,
   storage = extended
);

CREATE FUNCTION mol_amw(mol)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'mol_amw'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_from_smiles(cstring)
    RETURNS mol
    AS 'MODULE_PATHNAME', 'mol_from_smiles'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_to_smiles(mol)
    RETURNS cstring
    AS 'MODULE_PATHNAME', 'mol_to_smiles'
    LANGUAGE C IMMUTABLE STRICT;
