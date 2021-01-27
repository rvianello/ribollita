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