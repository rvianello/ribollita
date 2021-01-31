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

CREATE FUNCTION mol_recv(internal)
    RETURNS mol
    AS 'MODULE_PATHNAME', 'mol_recv'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_send(mol)
    RETURNS bytea
    AS 'MODULE_PATHNAME', 'mol_send'
    LANGUAGE C STRICT IMMUTABLE;

CREATE TYPE mol (
   internallength = variable,
   input = mol_in,
   output = mol_out,
   receive = mol_recv,
   send = mol_send,
   storage = extended
);

CREATE FUNCTION mol_amw(mol)
    RETURNS float8
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_amw'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_from_smiles(cstring)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_from_smiles'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_to_smiles(
        m mol,
        isomeric bool DEFAULT true,
        kekule bool DEFAULT false,
        root_atom integer DEFAULT -1,
        canonical bool DEFAULT true,
        all_bonds_explicit bool DEFAULT false,
        all_hs_explicit bool DEFAULT false,
        random bool DEFAULT false 
        )
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_to_smiles'
    LANGUAGE C IMMUTABLE STRICT;
