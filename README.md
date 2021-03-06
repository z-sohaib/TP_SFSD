École national Supérieure d’Informatique
2ème année cycle préparatoire 
Structures de fichiers et structures de données (SFSD)
TP : Gestion des livrets fonciers
Pour les groupes : G02 et G09 (2021 - 2022)
Proposé par : Mme EL ALLIA

Énoncé :
On souhaite manipuler les livrets fonciers de la conservation foncière sur tout le territoire national dans un 
fichier binaire nommé « Livrets_National.bin ».

Chaque livret contient les informations suivantes selon cet ordre :
- Un numéro unique sur 10 caractères
- Wilaya où se trouve le bien sur 14 caractères
- Type : il peut être : un terrain (T), une maison (M), un appartement (A), un immeuble (I) … Cette liste 
n’est pas exhaustive, on se contente que de ces 4 types dans ce TP, donc on le représente par un seul caractère
- Superficie du bien sur 10 caractères (en mètre carré m2)
- Observation : c’est un champ de taille variable qui sert à décrire le bien en désignant toutes ces caractéristiques ainsi que les noms des propriétaires du bien.

Ce fichier est organisé selon la méthode LOVC : fichier vu comme une Liste de blocs dont les données sont 
Ordonnées selon leurs numéros et sont de taille Variable avec Chevauchement. Le bloc contient B 
caractères (fixe) et la taille maximale de la donnée ne doit pas dépasser la taille d’un bloc.

Les caractéristiques du fichier binaire « Livrets_National.bin » doit contenir au moins les caractéristiques 
suivantes :
- L’adresse du premier bloc du fichier.
- Le nombre global de caractères insérés (mis à jour lors de l’insertion).
- Le nombre global de caractères supprimés (mis à jour lors de la suppression logique).
- …

Pour réaliser tous vos traitements des deux parties, développez (en langage C) les modules suivants :

PARTIE 1 :

1) Le modèle de la machine abstraite vu en cours (AllocBloc, LireDir, EcrireDir, Aff-entete, Entete, Ouvrir et 
Fermer) pour chaque type de fichier.

2) Création du fichier « Livrets_National.bin » contenant un ensemble de livrets aléatoires. Vous devrez le 
réaliser avec des valeurs aléatoires comme suit :
• Numéro est généré aléatoirement de tels sorte que le fichier n’accepte pas de doublons càd on ne 
peut pas trouver deux livrets avec le même numéro.
• Wilaya est généré aléatoirement à partir d’un ensemble contenant toutes les wilayas du pays, soit 
de entiers de 01 à 58 ou chaines {Adrar, Chlef, Laghouat,…}
• Type aussi généré aléatoirement à partir de l’ensemble {T,M, A, I}
• Superficie du bien ne doit pas dépasser la taille des 10 caractères et aussi générer aléatoirement.
• Observation c’est un champ qui contiendra n’importe quoi comme caractère généré aléatoirement.

3) Recherche d’un livret à partir de son numéro. Ce module doit retourner l’adresse du bloc ainsi que la 
position dans le bloc où le livret se trouve ou doit se trouver pour qu’on puisse l’insérer par la suite s’il n’existe 
pas dans le bon ordre (fichier ordonné).

4) Insertion d’un nouveau livret donné dans le fichier de données tout en respectant l’ordre (d’après son 
numéro et pas de doublons dans le fichier).

5) Suppression logique d’un livret donnée dans le fichier de données.

6) Réorganisation du fichier de données tout en épurant les livrets supprimés logiquement. Cette 
réorganisation peut être exécutée soit manuellement à partir du menu soit automatiquement (en arrière-plan) 
lors de la suppression logique (d’après un seuil donné) 
PARTIE 2 :

7) On désire faire certains traitements sur les livrets (décentralisation des livrets d’après les wilayas). On veut 
donc créer pour chaque wilaya un fichier contenant toutes les informations propres au livret sauf le champ
Observation n’est pas pris en compte et des champs non utiles. Ces fichiers sont organisés selon la 
méthode TOF : fichier vu comme tableau dont les enregistrements sont ordonnés et de taille fixe.
Création du fichier « Livrets_NomWilaya.bin » à partir du fichier « Livrets_National.bin ». L’utilisateur doit 
choisir à partir de la table de Wilaya qui sera affichée à l’écran la wilaya qui désire. Le fichier crée doit 
porter le nom de la wilaya choisie. Le champ non utile dans ce cas est wilaya. Il nous reste que le numéro, 
type et superficie.

8) On désire à partir du fichier « Livrets_NomWilaya.bin » charger dans un nouveau fichier uniquement des 
données ayant pour type « terrain ». Ce fichier de données nommé « Livrets_NomWilaya_Terrain.bin »
crée est de type LOF : fichier vu en liste avec des données ordonnées selon leurs numéros et de tailles
fixes. Les blocs de taille maximum b enregistrements sont remplis à u%.
Tout en chargeant ce fichier de données, on crée en parallèle une table index non dense. Cette table 
contient uniquement le plus grand numéro de chaque bloc de données ainsi le numéro de bloc.

9) Au fur et à mesure, on effectue des insertions d’un livret dans ce fichier de données
« Livrets_NomWilaya_Terrain.bin » tout en exploitant la table index. 
Développez le module insertion d’un livret tout en vérifiant son existence (le fichier n’accepte pas des 
doublons). Si le bloc est rempli à 100% et pour éviter les décalages entre les blocs, une allocation d’un 
nouveau bloc est nécessaire et la table d’index est mise à jour.

10) - Sauvegarde de la table index dans un fichier index (dans le menu).
- Chargement de la table index à partir du fichier index (dans le menu).
- Affichage de la table index (dans le menu)
Points importants :
• Le TP est en binôme.
• Tous les modules demandés doivent être exécutés via un menu principal comportant les deux parties.
• Il faut remettre un fichier compressé contenant uniquement : vos codes sources (tous vos 
programmes et le programme principal, ne pas remettre d’executable) et les fichiers de données et 
index. Il faut respecter le format du fichier compressé : 
Nom1_Nom2_TP_SFSD_2022_Groupexx.zip où xx représente le numéro de votre groupe. 
Je vous transmettrai au moment opportun le lien de dépôt de votre fichier (pas de mail, svp).
• La durée maximale du TP est fixée à 4 semaines. La date de remise est prévue pour le 27 décembre
2021 avant minuit. Aucun retard ne sera toléré. 
• Plusieurs aspects seront pris en considération lors de l’évaluation du TP (la démonstration, les codes 
sources (les structures, les commentaires, l’indentation, etc.), l’interface homme-machine (IHM), 
etc…).
• Un ZERO pour tout plagiat. 