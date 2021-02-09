select mol_to_smiles(mol_kekulize(mol_from_smiles('c1ccccc1')));

select mol_to_smiles(fragment) from mol_fragments(mol_from_smiles('C.CC.COC'));

select mol_formal_charge(mol_from_smiles('CC(=O)[O-]'));


