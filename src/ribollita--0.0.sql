--
-- version info
--

CREATE FUNCTION ribollita_version()
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'ribollita_version'
    LANGUAGE C IMMUTABLE;

CREATE FUNCTION rdkit_version()
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'rdkit_version'
    LANGUAGE C IMMUTABLE;

CREATE FUNCTION rdkit_build()
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'rdkit_build'
    LANGUAGE C IMMUTABLE;

CREATE FUNCTION boost_version()
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'boost_version'
    LANGUAGE C IMMUTABLE;

--
-- mol type
--

CREATE TYPE mol;

CREATE FUNCTION mol_in(cstring)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_out(mol)
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_out'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_recv(internal)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_recv'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_send(mol)
    RETURNS bytea
    PARALLEL SAFE
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

--
-- mol ops
--

CREATE FUNCTION mol_formal_charge(mol)
    RETURNS integer
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_formal_charge'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_kekulize(mol)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_kekulize'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_add_hs(
        mol,
        explicit_only bool DEFAULT false,
        add_coords bool DEFAULT false,
        add_residue_info bool DEFAULT false)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_add_hs'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_remove_hs(
        mol,
        implicit_only bool DEFAULT false,
        update_explicit_count bool DEFAULT false,
        sanitize bool DEFAULT true)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_remove_hs'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_remove_hs_ex(
        mol,
        remove_degree_zero bool DEFAULT false,
        remove_higher_degrees bool DEFAULT false,
        remove_only_neighbors bool DEFAULT false,
        remove_isotopes bool DEFAULT false,
        remove_and_track_isotopes bool DEFAULT false,
        remove_dummy_neighbors bool DEFAULT false,
        remove_defining_bond_stereo bool DEFAULT false,
        remove_with_wedge_bond bool DEFAULT true,
        remove_with_query bool DEFAULT false,
        remove_mapped bool DEFAULT true,
        remove_in_s_groups bool DEFAULT false,
        show_warnings bool DEFAULT true,
        implicit_only bool DEFAULT false,
        update_explicit_count bool DEFAULT false,
        remove_hydrides bool DEFAULT true,
        sanitize bool DEFAULT true)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_remove_hs_ex'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_remove_all_hs(
        mol,
        sanitize bool DEFAULT true)
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_remove_all_hs'
    LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION mol_fragments(
        mol,
        sanitize_fragments bool DEFAULT true,
        copy_conformers bool DEFAULT true)
    RETURNS TABLE (fragment mol)
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_fragments'
    LANGUAGE C STRICT IMMUTABLE;

--
-- mol formats
--

CREATE FUNCTION mol_from_smiles(
        smiles cstring,
        sanitize bool DEFAULT true,
        -- replacements: not yet supported
        allow_cxsmiles bool DEFAULT true,
        strict_cxsmiles bool DEFAULT true,
        parse_name bool DEFAULT false,
        remove_hs bool DEFAULT true,
        use_legacy_stereo bool DEFAULT true
        )
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

CREATE FUNCTION mol_from_molblock(
        molblock cstring,
        sanitize bool DEFAULT true,
        remove_hs bool DEFAULT true,
        strict_parsing bool DEFAULT true
        )
    RETURNS mol
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_from_molblock'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION mol_to_molblock(
        m mol,
        include_stereo bool DEFAULT true,
        conf_id integer DEFAULT -1,
        kekulize bool DEFAULT true,
        allow_v2000 bool DEFAULT false
        )
    RETURNS cstring
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_to_molblock'
    LANGUAGE C IMMUTABLE STRICT;

--
-- mol descriptors
--

CREATE FUNCTION mol_amw(mol)
    RETURNS float8
    PARALLEL SAFE
    AS 'MODULE_PATHNAME', 'mol_amw'
    LANGUAGE C IMMUTABLE STRICT;
