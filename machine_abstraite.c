#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=================================== Machine abstraite LOVC

int entete_LOVC(LOVC *f, int i)
{
    switch (i)
    {
    case 1:
        return f->entete.nbBloc;
    case 2:
        return f->entete.premierBloc;
    case 3:
        return f->entete.dernierBloc;
    case 4:
        return f->entete.posLibre;
    case 5:
        return f->entete.nbCarSupp;
    case 6:
        return f->entete.nbCarIns;
    default:
        break;
    }
}

void aff_entete_LOVC(LOVC *f, int i, int valeur)
{

    switch (i)
    {
    case 1:
    {
        f->entete.nbBloc = valeur; // nombre total de bloc allou�s dans le fichier
    }
    break;
    case 2:
    {
        f->entete.premierBloc = valeur; // numero du bloc representatnt la tete du fichier
    }
    break;
    case 3:
    {
        f->entete.dernierBloc = valeur; // numero du bloc representatnt la queue du ficheir
    }
    break;
    case 4:
    {
        f->entete.posLibre = valeur; // la postion libre dans le bloc de queue
    }
    break;
    case 5:
    {
        f->entete.nbCarSupp = valeur; // nombre de caract�res suprim� depuis la cr�ation du ficher afin de lancer la r�organiosation
    }
    break;
    case 6:
    {
        f->entete.nbCarIns = valeur; // nombre de caract�res inser�
    }
    };
}

void liredir_LOVC(LOVC *f, int i, BufferLOVC *buf)
{

    fseek(f->fichier, (sizeof(EnteteLOVC) + sizeof(TBlocLOVC) * (i - 1)), SEEK_SET); // positionnement au debut du bloc numero i
    fread(buf, sizeof(BufferLOVC), 1, f->fichier);                                   // lecture d'un bloc de caract�re correspondant a la taille du bloc dans le buffer
    rewind(f->fichier);                                                              // repositionnement endebut de fichier
}

void ecriredir_LOVC(LOVC *f, int i, BufferLOVC *buf)
{

    fseek(f->fichier, sizeof(EnteteLOVC) + sizeof(TBlocLOVC) * (i - 1), SEEK_SET); // positionnement au debut du bloc numero i
    fwrite(buf, sizeof(BufferLOVC), 1, f->fichier);                                // ecriture du contenu du buffer dans le bloc numero i du fichier
}

LOVC *ouvrir_LOVC(char nom_fich[], char mode)
{

    LOVC *f = malloc(sizeof(LOVC)); // allocation  de la structure
    BufferLOVC buf = {.tab = "", .suivant = -1};
    if ((mode == 'A') || (mode == 'a')) // mode ancien
    {
        f->fichier = fopen(nom_fich, "rb+");                    // ouverture du fichier en mode binaire lecture et ecriture
        fread(&(f->entete), sizeof(EnteteLOVC), 1, f->fichier); // chargement de l'entete enregistr�e en debut de fichier
    }
    else
    {
        if ((mode == 'N') || (mode == 'n')) // mode nouveau
        {
            f->fichier = fopen(nom_fich, "wb+");                     // ouverture du fichier en mode binaire  ecriture
            aff_entete_LOVC(f, 1, 0);                                // mise a zeo du nombre de bloc si entete(fichier,1)=0 alors le fichier est vide
            aff_entete_LOVC(f, 2, 1);                                //     // mettre tete au premier bloc
            aff_entete_LOVC(f, 3, 1);                                // mettre queue au premier bloc
            aff_entete_LOVC(f, 4, 0);                                // le premier caract�re du ficheir correspond a laposition libre puisqu'il est nouveau
            aff_entete_LOVC(f, 5, 0);                                // aucun caract�re n'a encore �t� supprim�
            aff_entete_LOVC(f, 6, 0);                                // aucun caract�re n'a encore �t� ajout�
            fwrite(&(f->entete), sizeof(EnteteLOVC), 1, f->fichier); // enregistrement de l'entete dans le fichier
            // buf.suivant=-1;                                 // le suivant du premier bloc a NULL
            // sprintf(buf.tab,"%s"," ");                       // initialisation du buffer a chaine vide
            ecriredir_LOVC(f, 1, &buf); // ecriture du premier bloc dans le fichier
        }
        else // format d'ouverture incorrecte
        {
            printf("Mode d'ouverture non valide");
        }
    }
    return (f); // renvoyer la structure cr��e
}

void fermer_LOVC(LOVC *f) // procedure de fermeture du fichier
{
    rewind(f->fichier);                                      // repositionnement du curseur en debut de fichier
    fwrite(&(f->entete), sizeof(EnteteLOVC), 1, f->fichier); // sauvegarde de la derni�re version de l'entete de la strucuture LOVC
    // dans le fichier
    fclose(f->fichier); // fermeture du fichier
    free(f);
}

void alloc_bloc_LOVC(LOVC *f)
{

    BufferLOVC buf;                               //???  // allocation du Buffer
    liredir_LOVC(f, entete_LOVC(f, 3), &buf);     // lecture du bloc correspondant a la queue
    buf.suivant = entete_LOVC(f, 3) + 1;          // mise a jour dui suvant de la queue au bloc correspondant a la nouvelle queue
    ecriredir_LOVC(f, entete_LOVC(f, 3), &buf);   // ecriture du bloc de queue dans le fichier
    aff_entete_LOVC(f, 3, entete_LOVC(f, 3) + 1); // mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
    buf = (BufferLOVC){.tab = "", .suivant = -1}; // vider la chaine du buffer
    ecriredir_LOVC(f, entete_LOVC(f, 3), &buf);   // ecriture du buffer dans le bloc representatnt la nouvelle queue
    aff_entete_LOVC(f, 1, entete_LOVC(f, 1) + 1); // incr�mentation du nombre de bloc allou�s
}

//=================================== Machine abstraite TOF

int enteteTOF(TOF *f, int i)
{
    switch (i)
    {
    case 1:
        return f->entete.nbBloc; // Nombre des blocs
    case 2:
        return f->entete.nbEnrg; // Nombre des enregistrements
    default:
        break;
    }
}

void affEnteteTOF(TOF *f, int i, int val)
{
    switch (i)
    {
    case 1:
        f->entete.nbBloc = val; // Nombre de blocs
    case 2:
        f->entete.nbEnrg = val; // Nombre des enregistrements
    default:
        break;
    }
}

void lireDirTOF(TOF *f, int i, BufferTOF *buff)
{
    fseek(f->fichier, (sizeof(EnteteTOF) + sizeof(TBlocTOF) * (i - 1)), SEEK_SET);
    fread(buff, sizeof(BufferTOF), 1, f->fichier);
    rewind(f->fichier);
}

void ecrireDirTOF(TOF *f, int i, BufferTOF *buff)
{
    fseek(f->fichier, sizeof(EnteteTOF) + sizeof(TBlocTOF) * (i - 1), SEEK_SET);
    fwrite(buff, sizeof(BufferTOF), 1, f->fichier);
}

TOF *ouvrirTOF(char nomFichier[], char mode)
{
    TOF *f = malloc(sizeof(TOF));
    BufferTOF buff = {.tab = 0, .nb = 0};
    if ((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+");
        fread(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
    }
    else if ((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+");
        affEnteteTOF(f, 1, 0);
        affEnteteTOF(f, 2, 0);
        fwrite(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
        ecrireDirTOF(f, 1, &buff);
    }
    else
    {
        printf("Mode d'ouverture non valide");
    }
    return (f);
}

void fermerTOF(TOF *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

void allocBlocTOF(TOF *f)
{
    affEnteteTOF(f, 1, enteteTOF(f, 1) + 1);
}

//=================================== Machine abstraite LOF

int enteteLOF(LOF *f, int i)
{
    switch (i)
    {
    case 1:
        return f->entete.premierBloc;
    case 2:
        return f->entete.dernierBloc;
    case 3:
        return f->entete.nbEnrg;
    case 4:
        return f->entete.nbBloc;
    default:
        break;
    }
}

void affEnteteLOF(LOF *f, int i, int val)
{
    switch (i)
    {
    case 1:
        f->entete.premierBloc = val;
        break;
    case 2:
        f->entete.dernierBloc = val;
        break;
    case 3:
        f->entete.nbEnrg = val;
        break;
    case 4:
        f->entete.nbBloc = val;
    default:
        break;
    }
}

void lireDirLOF(LOF *f, int i, BufferLOF *buff)
{
    fseek(f->fichier, (sizeof(EnteteLOF) + sizeof(TBlocLOF) * (i - 1)), SEEK_SET);
    fread(buff, sizeof(BufferLOF), 1, f->fichier);
    rewind(f->fichier);
}

void ecrireDirLOF(LOF *f, int i, BufferLOF *buff)
{
    fseek(f->fichier, sizeof(EnteteLOF) + sizeof(TBlocLOF) * (i - 1), SEEK_SET);
    fwrite(buff, sizeof(BufferLOF), 1, f->fichier);
}

LOF *ouvrirLOF(char nomFichier[], char mode)
{
    LOF *f = malloc(sizeof(LOF));
    BufferLOF buff = {.tab = 0, .nbEnrg = 0, .suivant = -1};
    if ((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+");
        rewind((f)->fichier);
        fread(&(f->entete), sizeof(EnteteLOF), 1, f->fichier);
    }
    else if ((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+");
        affEnteteLOF(f, 1, 1);
        affEnteteLOF(f, 2, 1);
        affEnteteLOF(f, 3, 0);
        affEnteteLOF(f, 4, 1);
        fwrite(&(f->entete), sizeof(EnteteLOF), 1, f->fichier);
        ecrireDirLOF(f, 1, &buff);
    }
    else
    {
        printf("Mode d'ouverture non valide");
    }
    return (f);
}

void fermerLOF(LOF *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(EnteteLOF), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

void allocBlocLOF(LOF *f)
{
    BufferLOF buff;
    lireDirLOF(f, enteteLOF(f, 2), &buff);
    buff.suivant = enteteLOF(f, 2) + 1;
    ecrireDirLOF(f, enteteLOF(f, 2), &buff);
    affEnteteLOF(f, 2, enteteLOF(f, 2) + 1);
    buff = (BufferLOF){.nbEnrg = 0, .tab = 0, .suivant = -1};
    ecrireDirLOF(f, enteteLOF(f, 2), &buff);
}
