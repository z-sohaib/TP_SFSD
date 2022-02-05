#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//=================================== Tableaux

char *wilayas[58] = {"Adrar", "Chlef", "Laghouat", "Oum El Bouaghi", "Batna", "Bejaia", "Biskra", "Bechar", "Blida", "Bouira", "Tamanrasset", "Tebessa", "Tlemcen", "Tiaret", "Tizi Ouzou", "Alger", "Djelfa", "Jijel", "Setif", "Saida", "Skikda", "Sidi Bel Abbes", "Annaba", "Guelma", "Constantine", "Medea", "Mostaganem", "M'sila", "Masacra", "Ouargla", "Oran", "El Bayadh", "Illizi", "Bou Arreridj", "Boumerdes", "El Tarf", "Tindouf", "Tissemesilt", "El Oued", "Khenchela", "Souk Ahras", "Tipaza", "Mila", "Ain Defla", "Naama", "Ain Timouchent", "Ghardaia", "Relizane", "Timimoun", "Bordj Badji", "Ouled Djellal", "Beni Abbes", "In Salah", "In Guezzam", "Touggourt", "Djanet", "M'Ghair", "El Meniaa"};

char *Type[4] = {"T", "M", "A", "I"};

//=================================== Structures LOVC

#define LOVC_MAX 1024
#define SEUIL_LOVC 0.6

typedef struct TBlocLOVC
{
    char tab[LOVC_MAX];
    int suivant;
} TBlocLOVC;

typedef TBlocLOVC BufferLOVC;

typedef struct EnteteLOVC
{
    int nbBloc;
    int premierBloc;
    int dernierBloc;
    int posLibre;
    int nbCarSupp;
    int nbCarIns;
} EnteteLOVC;

typedef struct LOVC
{
    FILE *fichier;
    EnteteLOVC entete;
} LOVC;

//=================================== Structures TOF

#define TOF_MAX 8

typedef struct livretTOF
{
    char numero[11];
    char type;
    long long int superficie;
} livretTOF;

typedef struct TBlocTOF
{
    livretTOF tab[TOF_MAX];
    int nb;
} TBlocTOF;

typedef TBlocTOF BufferTOF;

typedef struct EnteteTOF
{
    int nbBloc;
    int nbEnrg;
} EnteteTOF;

typedef struct TOF
{
    FILE *fichier;
    EnteteTOF entete;
} TOF;

//=================================== Structures TOF

#define LOF_MAX 4
#define SEUIL_LOF 0.5

typedef struct livretLOF
{
    char numero[11];
    long long int superficie;
} livretLOF;

typedef struct TBlocLOF
{
    livretLOF tab[LOF_MAX];
    int suivant;
    int nbEnrg;
} TBlocLOF;

typedef TBlocLOF BufferLOF;

typedef struct EnteteLOF
{
    int premierBloc;
    int dernierBloc;
    int nbEnrg;
    int nbBloc;
} EnteteLOF;

typedef struct LOF
{
    FILE *fichier;
    EnteteLOF entete;
} LOF;

//=================================== Structures Index

typedef struct champIndex
{
    int numBloc;
    char numero[11];
} champIndex;

champIndex tableIndex[100000];

typedef struct Index
{
    FILE *fichier;
    int nombreChamps;
} Index;

//=================================== Machine abstraite LOVC

int entete_LOVC(LOVC *f, int i);
void aff_entete_LOVC(LOVC *f, int i, int valeur);
void liredir_LOVC(LOVC *f, int i, BufferLOVC *buf);
void ecriredir_LOVC(LOVC *f, int i, BufferLOVC *buf);
LOVC *ouvrir_LOVC(char nom_fich[], char mode);
void fermer_LOVC(LOVC *f);
void alloc_bloc_LOVC(LOVC *f);

//=================================== Machine abstraite TOF

int enteteTOF(TOF *f, int i);
void affEnteteTOF(TOF *f, int i, int val);
void lireDirTOF(TOF *f, int i, BufferTOF *buff);
void ecrireDirTOF(TOF *f, int i, BufferTOF *buff);
TOF *ouvrirTOF(char nomFichier[], char mode);
void fermerTOF(TOF *f);
void allocBlocTOF(TOF *f);

//=================================== Machine abstraite LOF

int enteteLOF(LOF *f, int i);
void affEnteteLOF(LOF *f, int i, int val);
void lireDirLOF(LOF *f, int i, BufferLOF *buff);
void ecrireDirLOF(LOF *f, int i, BufferLOF *buff);
LOF *ouvrirLOF(char nomFichier[], char mode);
void fermerLOF(LOF *f);
void allocBlocLOF(LOF *f);

//=================================== Fonctions LOVC

char *randstring(size_t length, int i);
void ChainePr(char chaine[], char cle[]);
void RecupChamp(int n, BufferLOVC buf, LOVC *f, int i, int j, char champ[]);
void recherche(LOVC *f, char cle[], int *trouv, int *i, int *j);
void diviserChaine(char chaine[], char chaine1[], char chaine2[], int n);
void nbtochar(long long int nb, char cle[]);

void creerFichier(LOVC **fichier, int nbEnrg);
void inserer(LOVC **fichier, char cle[]);
void affichageLOVC(LOVC *fichier);
void affichageLOVC(LOVC *fichier);
int supressionLOVC(LOVC **fichier, char cle[]);
void reorganisation(LOVC *fichier1, LOVC **fichier2);
int checkIfFileExists(const char *filename);

//=================================== Fonctions TOF

void nomFichierWilaya(int numWilaya, char nomFichier[]);
void recupererWilaya(char chaine[], char wilaya[]);
livretTOF recupererEnrgTOF(char chaine[]);
void creerFichierWilaya(int numWilaya, LOVC *fichier1, TOF **fichier2);
void affichageTOF(TOF *fichier);

//=================================== Fonctions LOF

void creeFichierTerrain(int numWilaya, TOF *fichier1, LOF **fichier2);
void affichageLOF(LOF *fichier);

//=================================== Fonctions Index

void affichageTableIndex(champIndex tableIndex[], int nombreChamps);
void insertionInterne(LOF **fichier, int numBloc, livretLOF livret);
void insertionIndex(champIndex tableIndex[], int nombreChamps, LOF **fichier, livretLOF livret);
int verfierExiste(LOF *fichier, int numBloc, char numero[]);
void sauvegardeFichierIndex(champIndex tableIndex[], int nombreChamps, char Nomfich[]);
// champIndex chargementFichierIndex(int *nombreChamps,char Nomfich[]);
void chargementFichierIndex(int *nombreChamps, char Nomfich[], champIndex tableIndex1[]);

//====================================affichage
void affichage1();
void affichemenu1();
void affichemenu2();
void affichemenutof();
void affichemenulof();
