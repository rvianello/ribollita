SELECT mol_to_smiles(mol_from_smiles('C1OCCCC1'));

SELECT mol_to_smiles(mol_from_smiles('OC=CC'));
SELECT mol_to_smiles(mol_from_smiles('OC=CC'), canonical => false);

SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'));
SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'), isomeric => false);
SELECT mol_to_smiles(mol_from_smiles('F[C@](Cl)(Br)I'), all_bonds_explicit => true);

SELECT mol_to_molblock(mol_from_smiles(''));
SELECT mol_to_smiles(mol_from_molblock(
    (Chr(10) || Chr(10) || Chr(10) ||
    '  0  0  0  0  0  0  0  0  0  0999 V2000' || Chr(10) ||
    'M  END')::cstring
    ));
