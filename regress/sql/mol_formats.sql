SELECT mol_to_smiles(mol_from_smiles('C1OCCCC1'));

SELECT mol_to_smiles(mol_from_smiles('OC=CC'));
SELECT mol_to_smiles(mol_from_smiles('OC=CC'), canonical => false);

SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'));
SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'), isomeric => false);
SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'), all_bonds_explicit => true);
