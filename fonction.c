#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fonction.h"
#include "machine_abstraite.c"

//============================== Fonctions LOVC

//============================== Random string

char *randstring(size_t length, int i) // fontion qui genere une chaine de caractre contenant l'alphabet ou des chiffres  (si  i = 0 alphabet sinon chiffre)
{

    static char charset1[] = "abcdefghijklmnopqrstuvwxyz"; // initialisation de chaeset1
    static char charset2[] = "0123456789";                 // initialisation de chaeset2

    char *randomString = NULL;

    if (length)
    {
        randomString = malloc(sizeof(char) * (length + 1)); // allocation de la chaine de caractere dynamique

        if (randomString)
        {
            for (int n = 0; n < length; n++) // boucle pour genrer une chaine de taille length donné comme parametre
            {
                if (i == 0) // generer une chaine de caractere qui contient l'alphabet
                {
                    int key = rand() % (int)(sizeof(charset1) - 1);
                    randomString[n] = charset1[key];
                }
                else // generer une chaine de caractere qui contient des chiffres
                {
                    int key = rand() % (int)(sizeof(charset2) - 1);
                    randomString[n] = charset2[key];
                }
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

//==============================genere la chaine a insert
void ChainePr(char chaine[], char cle[]) // procedure qui premet de gnerer unne chaine de caractre a inserer directement dans les buffers
// la chaine aura la structure suivant (taille(du champ observation)+suppression+cle+wilaya+type+superfecie+observation
{
    char Taille[6]; // la taille est sur 5 carateres mais j'ai ajouter une case de plus en cas de debordement
    char Tailletmp[6];
    char Wilaya[15];     // le chmp wilaya est sur 14 caracteres mais j'ai ajouter une case de plus en cas de debordement
    char type[2];        // le type est sur 1 caractere mais j'ai ajouter une case de plus en cas de debordement
    char Superfecie[11]; // la superficie est sur 10 caratere mais j'ai ajouter une case de plus en cas de debordement
    char Observation[LOVC_MAX - 41];

    strcpy(Wilaya, wilayas[rand() % 58]); // genrer un wilaya aleatoirement et la copie dans la variable wilaya
    while (strlen(Wilaya) < 14)           // ajouter "@" pour les wilayas dont la longeur est inferieur a 14 caracteres
    {
        strcat(Wilaya, "@");
    }
    strcpy(type, Type[rand() % 3]);                                   // genrer un type aleatoirement et le copie dans la variable type
    strcpy(Superfecie, randstring(10, 1));                            // genrer une superfecie aleatoirement et la copie dans la variable superfecie
    strcpy(Observation, randstring(1 + rand() % (LOVC_MAX - 42), 0)); // genrer une observatoin aleatoirement et la copie dans la variable observation
    itoa(strlen(Observation), Taille, 10);                            // copier la taille de l'observatoin dans la variable taille
    int lengthTaille = strlen(Taille);                                // variable sera utiliser pour ajouter des 0 au debut de la chaine taille en cas ou la variable taille ne contient pas 5 caracteres
    strcpy(Tailletmp, "");                                            // initialisation de la variable tailletmp
    while (lengthTaille < 5)
    {
        strcat(Tailletmp, "0");
        lengthTaille++;
    }
    strcat(Tailletmp, Taille);
    strcpy(Taille, Tailletmp);
    sprintf(chaine, "%s%s%s%s%s%s%s", Taille, "0", cle, Wilaya, type, Superfecie, Observation); // concatenation des  champs
}

//============================
void RecupChamp(int n, BufferLOVC buf, LOVC *f, int i, int j, char champ[]) // permet de recuperer un champ de taille n qui se trouve a la position j du buffer
// on passe le pointeur du fichier comme parametre en cas de chevauchement
{
    int k = 0; // indice de parcours
    BufferLOVC buf1;
    buf1 = buf;
    while (k < n) // tantque on n'est pas affecté toute la taille du mot
    {
        champ[k] = buf1.tab[j];
        k++;
        j++;
        if ((j >= LOVC_MAX)) // en cas de chevauchement
        {
            if (buf1.suivant != -1) // en verifie si on n'est pas dans le dernier bloc
            {

                i = buf1.suivant;          // i recoit le numero du prochain bloc
                j = 0;                     // indice de parcours de la chaine
                liredir_LOVC(f, i, &buf1); // lire le prochain bloc
            }
            else // cas ou on est dans le dernier bloc
            {
                champ[k] = '\0';
                break;
            }
        }
    }
    champ[k] = '\0';
}
//=========================================recherche lovc
void recherche(LOVC *f, char cle[], int *trouv, int *i, int *j) // permet de charcher une cle et sa position
{
    int taille, jtmp = 0, itmp;
    char taillech[6];
    char cch[11];
    BufferLOVC buf, buf1;
    int stop = 0;
    *trouv = 0;
    *i = entete_LOVC(f, 2);
    *j = 0;
    liredir_LOVC(f, *i, &buf);           // on lit le premier bloc
    while ((*trouv == 0) && (stop == 0)) // tant que on n'a pas trouvé la cle et on n'a pas depassé notre cle
    {
        RecupChamp(5, buf, f, *i, *j, taillech); // on recupere la taille du champ variable
        jtmp = *j;                               // pour que on change pas l'indice de parcours
        jtmp = jtmp + 6;                         // on saute 6 cases puor trouve le champ de cle
        buf1 = buf;
        itmp = *i;            // pour que on change pas l'indice de parcours des bloc
        if (jtmp >= LOVC_MAX) // on verifie si le champ cle commence dans le prochain bloc
        {
            jtmp = jtmp % LOVC_MAX;
            itmp = buf.suivant;
            liredir_LOVC(f, itmp, &buf1); // on lit le prochain bloc
        }
        RecupChamp(10, buf1, f, itmp, jtmp, cch); // on recupere le champ cle
        taille = atoi(taillech);                  // on convetit le taille en entier
        if (strcmp(cle, cch) == 0)                // si la cle recuperer = la cle qu'on cherche
        {
            *trouv = 1;
        }
        else
        {
            if (strcmp(cle, cch) < 0) // si on a depassé la cle qu'on cherche
            {
                stop = 1;
            }
            else // on continue la recherche
            {
                *j = (*j) + taille + 41; // on incremonte l'indice de recherche en lui ajoutant la taille deu champ variable + taille d'autre champ "taille +suppression + cle + wilaya + type + superficie =41 "
                if (*j >= LOVC_MAX)      // on verifie si l'indice de recherche depasse la taille du bloc
                {
                    *j = *j - LOVC_MAX;
                    if (buf.suivant != -1) // si on est pas arrivé au dernier bloc
                    {
                        *i = buf.suivant;
                        liredir_LOVC(f, *i, &buf); // on lit le prochain bloc
                    }
                    else // sinon on s'arrete
                    {
                        stop = 1;
                    }
                }
                if (((*j) == entete_LOVC(f, 4)) && ((*i) == entete_LOVC(f, 3))) // si on est arrive la la derniier position du dernier bloc on s'arrete
                {
                    stop = 1;
                }
            }
        }
    }
}
//============================== diviser une chaine de caracteres

void diviserChaine(char chaine[], char chaine1[], char chaine2[], int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    { // Recuprer la premi�re partie
        chaine1[i] = chaine[i];
    }
    chaine1[i] = '\0';

    for (i = n, j = 0; i < strlen(chaine); i++, j++)
    { // Recuperer la deuxi�me partie
        chaine2[j] = chaine[i];
    }
    chaine2[j] = '\0';
}

//============================== Convertir la clé en chaine de taille 10

void nbtochar(long long int nb, char cle[]) // permet de convertir un long long int en chaine de carateres de 10 caracteres
{
    char str[11];
    strcpy(cle, "");
    int cpt;
    lltoa(nb, str, 10);
    cpt = strlen(str);
    while (cpt < 10) // pour avoir exactement 10 caracteres
    {
        strcat(cle, "0");
        cpt++;
    }
    strcat(cle, str);
}
//============================== creation du fichier lovc

void creerFichier(LOVC **fichier, int nbEnrg)
{
    int blocActuel = 1, espace;
    char chaineTemporaire[LOVC_MAX], partie1[LOVC_MAX], partie2[LOVC_MAX];
    BufferLOVC buff;
    char cle[11];
    int cpt = 1;
    long long int cle_int = 0;
    *fichier = ouvrir_LOVC("Livrets_National.bin", 'n'); // Ouvrir le fichier
    liredir_LOVC(*fichier, blocActuel, &buff);           // Recuprer le bloc dans le buffer
    for (int i = 0; i < nbEnrg; i++)
    {
        strcpy(chaineTemporaire, "");
        espace = LOVC_MAX - entete_LOVC(*fichier, 4); // Recuperer l'espace restant dans le bloc
        cle_int++;
        cle_int++;
        nbtochar(cle_int, cle);
        ChainePr(chaineTemporaire, cle); // Genrer la chaine qu'on va ajouter
                                         // liredir_LOVC(*fichier, blocActuel, &buff); // Recuprer le bloc dans le buffer
        if (strlen(chaineTemporaire) > espace)
        {
            strcpy(partie1, "");
            strcpy(partie2, "");
            // Verifier si l'espace restant dans le bloc n'est pas suffisant
            diviserChaine(chaineTemporaire, partie1, partie2, espace);                         // Diviser la chaine en deux parties
                                                                                               // printf("partie1 %s\n partie2 %s\n",partie1,partie2 );
            strcat(buff.tab, partie1);                                                         // Mettre la premi�re partie dans l'espace restant
            alloc_bloc_LOVC(*fichier);                                                         // Allouer un nouveau bloc
            buff.suivant = entete_LOVC(*fichier, 3);                                           // Mettre a jour le suivant dans le bloc actuel
            ecriredir_LOVC(*fichier, blocActuel, &buff);                                       // Ecrire le contenu du buffer dans le fichier
            liredir_LOVC(*fichier, ++blocActuel, &buff);                                       // Recuperer le nouveau bloc dans le buffer
            strcat(buff.tab, partie2);                                                         // Ajouter la deuxi�me partie dans le nouveau bloc
            ecriredir_LOVC(*fichier, blocActuel, &buff);                                       // Ecrire le contenu du buffer dans le fichier
            aff_entete_LOVC(*fichier, 6, entete_LOVC(*fichier, 6) + strlen(chaineTemporaire)); // Ajouter la taille de la chaine dans l'entete
            aff_entete_LOVC(*fichier, 4, strlen(partie2));
        }

        else
        {                                       // Le cas ou l'espace restant dans le bloc est suffisant
            strcat(buff.tab, chaineTemporaire); // Ajouter la chaine complete dans le buffer

            aff_entete_LOVC(*fichier, 6, entete_LOVC(*fichier, 6) + strlen(chaineTemporaire)); // Ajouter la taille de la chaine dans l'entete
            aff_entete_LOVC(*fichier, 4, entete_LOVC(*fichier, 4) + strlen(chaineTemporaire));
        }
    }
    ecriredir_LOVC(*fichier, blocActuel, &buff); // Ecrire le contenu du buffer dans le fichier
}

//============================== Insertion lovc

void inserer(LOVC **fichier, char cle[])
{
    int trouv = 0, i = 1, j = 0, h = 1, rest;

    BufferLOVC buff;
    char key[LOVC_MAX];                // chaine a inserer
    char key1[LOVC_MAX];               // la chaine temporaire
    if (entete_LOVC(*fichier, 6) == 0) // cas fichier vide
    {
        liredir_LOVC(*fichier, 1, &buff);                                     // on lit le premier bloc
        ChainePr(key, cle);                                                   // genere une chaine a inseret
        strcpy(buff.tab, key);                                                // on l'affect au bloc
        aff_entete_LOVC(*fichier, 6, entete_LOVC(*fichier, 6) + strlen(key)); // incrementation de nombre de caractere inserer
        aff_entete_LOVC(*fichier, 4, strlen(key));                            // mise a jour de la derniere position
        ecriredir_LOVC(*fichier, 1, &buff);
        printf("\ninsertion effectuee avec succes\n");
    }
    else
    {
        recherche(*fichier, cle, &trouv, &i, &j); // on cherche la cle donnée

        if (trouv == 1) // si la cle est trouvé
        {
            liredir_LOVC(*fichier, i, &buff); // on lit le bloc ou elle se trouve notre cle
            if (buff.tab[j + 5] == '1')       // enregistrement supprimé
            {
                buff.tab[j + 5] = '0';              // mise a jour du champ supprimer de l'enregistrement
                ecriredir_LOVC(*fichier, i, &buff); // mise a jour du bloc
                printf("\ninsertion effectuee avec succes\n");
            }
            else // cle deja existant
            {
                printf("\ncle deja existant\n");
            }
        }
        else // cle non trouvé
        {
            ChainePr(key, cle);                                                   // on genere notre chaine a inserer
            aff_entete_LOVC(*fichier, 6, entete_LOVC(*fichier, 6) + strlen(key)); // mise a jour de nombre de caractere inseré

            liredir_LOVC(*fichier, i, &buff); // on lit notre bloc
            int stop = 0;
            while (stop <= 0)
            {
                if (strlen(key) <= (LOVC_MAX - j)) // si l'insetion de la chaine se fait dans le meme bloc
                {
                    h = 0;                                      // indice pour la chaine temporaire
                    for (int f = j; f < (j + strlen(key)); f++) // on parcour toute la chaine
                    {
                        key1[h] = buff.tab[f]; // on copie la chaine a inserer dans la prochaine iteration
                        buff.tab[f] = key[h];  // on ecrit notre chaine
                        h++;
                    }
                    key1[h] = '\0';
                    j = (j + strlen(key)); // mise a jour de l'indice de parcours
                    strcpy(key, key1);     // on copie la chaine temporaire dans key
                    // cpt = cpt + (strlen(key));
                }
                else // si l'insetion de la chaine se fait dans deux blocs
                {
                    rest = strlen(key) + j - LOVC_MAX; // nombre de cararctre a insere dans le deuxieme bloc
                    h = 0;
                    //  cpt = cpt + (strlen(key));
                    while (j < LOVC_MAX) // insertion dans le premier bloc
                    {
                        key1[h] = buff.tab[j];
                        buff.tab[j] = key[h];
                        j++;
                        h++;
                    }

                    if ((i + 1) > entete_LOVC(*fichier, 3)) // si on est arrive au dernier bloc
                    {
                        alloc_bloc_LOVC(*fichier);               // on alloue un nouveau bloc
                        buff.suivant = entete_LOVC(*fichier, 3); // on chaine l'ancien baff avec le nouveau bloc alloué
                        ecriredir_LOVC(*fichier, i, &buff);      // on ecrire notre buff
                                                                 // la mise a jour du numero du dernier bloc et nombre de bloc se fait automatiqument dans la machine abstraite
                                                                 // la mise a jour du numero du dernier bloc et nombre de bloc se fait automatiqument dans la machine abstraite

                        // la mise a jour du numero du dernier bloc et nombre de bloc se fait automatiqument dans la machine abstraite
                    }
                    else // si on est pas arrivé au dernier bloc
                    {
                        ecriredir_LOVC(*fichier, i, &buff);
                    }
                    i++; // on incremente l'indice de parcour des bloc cette instrucion est equivalante a i=buff.suivant;
                    // i=buff.suivant;
                    liredir_LOVC(*fichier, i, &buff); // on lit le nouveau bloc

                    for (int f = 0; f < rest; f++) // insertion des caractre restant
                    {
                        key1[h] = buff.tab[f];
                        buff.tab[f] = key[h];
                        h++;
                    }

                    j = rest; // mise a jour d'indice de parcours
                    key1[h] = '\0';
                    strcpy(key, key1); // on copie la chaine temporaire dans key
                }

                if (strlen(key) == 0) // si la taille de la nouvelle chaine a inserer ==0 donc on s'arrete
                {
                    stop = 1;
                }
                // cette methode d'arret est efficace contrairement a la methode d'arret en utilisant la position du dernier bloc car cette dernier donne des carateres ne plus a la fin du bloc et ne permet pas de savoir la dernier position dans le bloc
            }

            aff_entete_LOVC(*fichier, 4, entete_LOVC(*fichier, 6) % LOVC_MAX); // mise a jour de la postion libre dans le bloc de queue
            ecriredir_LOVC(*fichier, i, &buff);                                // ecriture du dernier blloc
            printf("\ninsertion effectuee avec succes\n");
        }
    }
}

//============================== Affichage des champs

void affichageChamps(char chaine[])
{
    printf("\n============================================================\n");
    printf("Numero : ");
    for (int i = 6; i < 16; i++) // Affichage du numero
    {
        printf("%c", chaine[i]);
    }
    printf("\n------------------------------\n");
    printf("Wilaya : ");
    for (int i = 16; i < 30; i++) // Affichage de la wilaya
    {
        if (chaine[i] != '@')
        {
            printf("%c", chaine[i]);
        }
    }
    printf("\n------------------------------\n");
    printf("Type : %c", chaine[30]); // Affichage du type
    printf("\n------------------------------\n");
    printf("Superficie : ");
    for (int i = 31; i < 41; i++) // Affichage de la superficie
    {
        printf("%c", chaine[i]);
    }
    printf("\n------------------------------\n");
    printf("L'observation : ");
    for (int i = 41; i < strlen(chaine); i++) // Affichage de l'observation
    {
        printf("%c", chaine[i]);
    }
}

//============================== Affichage LOVC

void affichageLOVC(LOVC *fichier)
{
    int nbCar = 0, tailleTemporaire, blocActuel = entete_LOVC(fichier, 2), posActuelle = 0;
    char chaineTemporaire[LOVC_MAX], tailleObs[6], supression;
    BufferLOVC buffer;

    if (entete_LOVC(fichier, 6) == 0)
    {
        printf("le fichier est vide \n");
    }
    else
    {
        liredir_LOVC(fichier, blocActuel, &buffer); // Lecture du bloc dans le buffer
        while (nbCar < (entete_LOVC(fichier, 6)))
        {
            RecupChamp(5, buffer, fichier, blocActuel, posActuelle, tailleObs);                       // Recuperer la taille de l'observation
            tailleTemporaire = 41 + atoi(tailleObs);                                                  // Trouver la taille total de l'enregistrement
            RecupChamp(tailleTemporaire, buffer, fichier, blocActuel, posActuelle, chaineTemporaire); // Recuperer l'enregistrement
            supression = chaineTemporaire[5];                                                         // Récupéer le champ de supression
            if (supression == '0')
            {                                      // Verifier si la chaine est supprimée
                affichageChamps(chaineTemporaire); // Affichage de l'enregistrement
            }
            posActuelle += tailleTemporaire; // Passer au prochain enregistrement
            nbCar += tailleTemporaire;       // Ajouter la taille totale de l'enregistrement au nombre de caracteres parcourus
            if (posActuelle >= LOVC_MAX)
            {                                // Verifier si on a dépassé la taille du bloc
                posActuelle -= LOVC_MAX;     // Passer au nouvelle position dans le prochain bloc
                blocActuel = buffer.suivant; // Passer au prochain bloc
                if (blocActuel != -1)
                {
                    liredir_LOVC(fichier, blocActuel, &buffer); // Passer au bloc suivant
                }
                else
                {
                    break;
                }
            }
        }
    }
}

//============================== Supression LOVC

int supressionLOVC(LOVC **fichier, char cle[])
{
    char cleTemporaire[11], tailleObs[6];
    int numBloc, pos, trouv, taille, possup, numblocsup;
    BufferLOVC buffer, buffersup;

    recherche(*fichier, cle, &trouv, &numBloc, &pos); // Trouver le bloc qui contient la clé et sa position
    if (trouv == 1)
    { // Verifier si la clé existe
        liredir_LOVC(*fichier, numBloc, &buffer);
        buffersup = buffer;
        numblocsup = numBloc;
        possup = pos + 5;
        if (possup >= LOVC_MAX) // Si on depassa la taille du bloc
        {
            possup = possup - LOVC_MAX;
            numblocsup = buffer.suivant;
            liredir_LOVC(*fichier, numblocsup, &buffersup); // Lire le bloc suivant
        }

        buffersup.tab[possup] = '1';                              // Mettre le champs de suprssion a vrai
        RecupChamp(5, buffer, *fichier, numBloc, pos, tailleObs); // Recuperer champ qui contient la taille de l'observation
        ecriredir_LOVC(*fichier, numblocsup, &buffersup);
        aff_entete_LOVC(*fichier, 5, entete_LOVC(*fichier, 5) + atoi(tailleObs) + 41); // Mise a jour des caracteres suprimés dans l'entete
        return 0;
    }
    else
    {
        return -1;
    }
}

//============================== Reorganisation

void reorganisation(LOVC *fichier1, LOVC **fichier2)
{
    int bloc1 = entete_LOVC(fichier1, 2), pos1 = 0, nbCar1 = 0, taille1;
    int espace, bloc2, pos2 = 0, taille2;
    BufferLOVC buffer1, buffer2;
    char tailleObs1[6], chaine[LOVC_MAX], supression;
    char partie1[LOVC_MAX], partie2[LOVC_MAX];

    *fichier2 = ouvrir_LOVC("Livrets_National_v2.bin", 'N'); // Ouvrir le nouveau fichier
    bloc2 = entete_LOVC(*fichier2, 2);                       // Recuperer le numero du premier bloc
    liredir_LOVC(fichier1, bloc1, &buffer1);                 // Lire le premier bloc d'ancien fichier dans le buffer
    liredir_LOVC(*fichier2, bloc2, &buffer2);                // Lire le premier bloc du nouveau fichier dans le buffer
    while (nbCar1 < entete_LOVC(fichier1, 6))
    {                                                                // Tant que on a pas atteint le nombre des caracteres total dans l'ancien fichier
                                                                     //  printf("%d ",pos1);
        RecupChamp(5, buffer1, fichier1, bloc1, pos1, tailleObs1);   // Recuperer la taille de l'observation
                                                                     //  printf("ll %s ",tailleObs1);
        taille1 = 41 + atoi(tailleObs1);                             // Trouver la taille total de l'enregistrement
        RecupChamp(taille1, buffer1, fichier1, bloc1, pos1, chaine); // Recuperer tout l'enregistrement
        supression = chaine[5];                                      // Recuperer le champ de supression
        if (supression == '0')
        {                             // Verifier si l'enregistrement n'est pas supprimé
            espace = LOVC_MAX - pos2; // Recuperer l'espace libre dans le bloc
            if (strlen(chaine) > espace)
            { // Cas ou la taille de la chaine recupérée dépassé l'espace restant dans le bloc

                diviserChaine(chaine, partie1, partie2, espace); // Diviser la chaine en deux parties
                strcat(buffer2.tab, partie1);                    // Ajouter la premiere partie dans le bloc actuel
                alloc_bloc_LOVC(*fichier2);                      // Allouer un nouveau bloc
                aff_entete_LOVC(*fichier2, 1, entete_LOVC(*fichier2, 1) + 1);
                buffer2.suivant = entete_LOVC(*fichier2, 3); // Mettre a jour le suivant de l'ancien bloc
                ecriredir_LOVC(*fichier2, bloc2, &buffer2);  // Ecrire l'ancien bloc dans le nouveau fichier
                bloc2 = buffer2.suivant;                     // Recuperer le numero du dernier bloc ou bloc suivant
                liredir_LOVC(*fichier2, bloc2, &buffer2);    // Lire le dernier bloc
                strcat(buffer2.tab, partie2);                // Ajouter la 2eme partie
                pos2 = strlen(partie2);                      // Mettre a jour la position actuelle dans le bloc
            }
            else
            {
                strcat(buffer2.tab, chaine); // Ajouter la chaine dans le buffer
                pos2 += strlen(chaine);      // Mettre a jour la position actuelle dans le bloc
            }
        }
        pos1 += taille1;   // Mettre a jour la position dans le bloc dans l'ancien fichier
        nbCar1 += taille1; // Mettre a jour le nombre de caracteres parcourus dans l'ancien fichier
        if (pos1 >= LOVC_MAX)
        {                                            // Verifier si on a dépassé la taille du bloc
            pos1 -= LOVC_MAX;                        // Mettre a jour la position
            bloc1++;                                 // buffer1.suivant; // Recuperer le numero du bloc suivant
            liredir_LOVC(fichier1, bloc1, &buffer1); // Recuperer le contenu du bloc suivant
        }
    }
    ecriredir_LOVC(*fichier2, bloc2, &buffer2); // Ecrire le contenu du buffer dans le nouveau fichier
    aff_entete_LOVC(*fichier2, 4, pos2);
    aff_entete_LOVC(*fichier2, 6, entete_LOVC(fichier1, 6) - entete_LOVC(fichier1, 5));
    // la mise a jour du numero du bloc representatnt la queue du fichier est faite automatiqument dans la macgine abstaite
}

//============================== Fonctions TOF

//============================== Generer nom du fichier a partir du numero de la wilaya

void nomFichierWilaya(int numWilaya, char nomFichier[]) // Generer le nom du fichier a partir du numero de la wilaya
{
    char wilaya[15];

    strcpy(nomFichier, "");
    strcpy(wilaya, wilayas[numWilaya - 1]);
    strcat(nomFichier, "Livrets_");
    strcat(nomFichier, wilaya);
    // strcat(nomFichier, ".bin");
}

//============================== Recuperer le champ de la wilaya

void recupererWilaya(char chaine[], char wilaya[]) // Recuperation de la wilaya a partir de la chaine LOVC
{
    strcpy(wilaya, "");
    for (int i = 16, j = 0; i < 30; i++)
    {
        if (chaine[i] != '@')
        {
            wilaya[j++] = chaine[i];
        }
        else
        {
            wilaya[j++] = '\0';
        }
    }
    wilaya[14] = '\0';
}

//============================== Recuperer enregistrement

livretTOF recupererEnrgTOF(char chaine[]) // Convertir une chaine LOVC en un enregistrement TOF
{
    livretTOF enrg;
    char chaineTemporaire1[11], chaineTemporaire2[11];

    // Recuperation du numero a partir de la chaine LOVC
    for (int i = 6, j = 0; i < 16; i++, j++)
    {
        chaineTemporaire1[j] = chaine[i];
        chaineTemporaire1[j + 1] = '\0';
    }
    strcpy(enrg.numero, chaineTemporaire1);
    // Recuperation du type a partir de la chaine LOVC
    enrg.type = chaine[30];
    // Recuperation de la superficie a partir de la chaine LOVC
    for (int i = 31, j = 0; i < 41; i++, j++)
    {
        chaineTemporaire2[j] = chaine[i];
        chaineTemporaire2[j + 1] = '\0';
    }
    enrg.superficie = atoll(chaineTemporaire2);
    return enrg;
}

//============================== Creer le fichier d'une wilaya

void creerFichierWilaya(int numWilaya, LOVC *fichier1, TOF **fichier2)
{
    int nbCar = 0, tailleTemporaire, blocActuel = entete_LOVC(fichier1, 2), posActuelle = 0, blocTOF, posTOF = 0, cpt = 0;
    char chaineTemporaire[LOVC_MAX], tailleObs[6], supression, wilaya[15], nom[27];
    BufferLOVC buffer1;
    BufferTOF buffer2;

    nomFichierWilaya(numWilaya, nom); // Recuperer le nom du fichier special au wilaya demandée
    strcat(nom, ".bin");
    *fichier2 = ouvrirTOF(nom, 'N'); // Ouvrir le fichier comme un nouveau fichier
    allocBlocTOF(*fichier2);
    blocTOF = enteteTOF(*fichier2, 1); // Recuperer le nombre des blocs
    lireDirTOF(*fichier2, blocTOF, &buffer2);
    liredir_LOVC(fichier1, blocActuel, &buffer1);
    while (nbCar < (entete_LOVC(fichier1, 6)))
    { // Tant que on a pas atteint le nombre des caracteres insérés dans le fichier LOVC
        // Recuperer la taille de l'observation puis la taille de la chaine ensuite la chaine
        RecupChamp(5, buffer1, fichier1, blocActuel, posActuelle, tailleObs);
        tailleTemporaire = 41 + atoi(tailleObs);
        RecupChamp(tailleTemporaire, buffer1, fichier1, blocActuel, posActuelle, chaineTemporaire);
        supression = chaineTemporaire[5];
        recupererWilaya(chaineTemporaire, wilaya);
        // printf("%s\n",wilaya);
        if ((supression == '0') && (strcmp(wilayas[numWilaya - 1], wilaya) == 0))
        { // Verifier si la chaine n'est pas supprimée et la wilaya si elle la wilaya demandée
            if (buffer2.nb < TOF_MAX)
            { // Si on a pas atteint le max d'un bloc TOF
                buffer2.tab[posTOF] = recupererEnrgTOF(chaineTemporaire);
                posTOF++;
                buffer2.nb++;
                cpt++;
                // affEnteteTOF(*fichier2, 2, enteteTOF(*fichier2, 2) + 1);
            }
            else
            { // Si on a atteint le max d'un bloc TOF
                ecrireDirTOF(*fichier2, blocTOF, &buffer2);
                // On ecrit le buffer dans le fichier et on alloue un nouveau bloc TOF
                allocBlocTOF(*fichier2);
                blocTOF++;
                posTOF = 0;
                lireDirTOF(*fichier2, blocTOF, &buffer2);
                buffer2.tab[posTOF] = recupererEnrgTOF(chaineTemporaire);
                posTOF++;
                buffer2.nb = 1;
                cpt++;
                // affEnteteTOF(*fichier2, 2, enteteTOF(*fichier2, 2) + 1);
            }
        }
        posActuelle += tailleTemporaire;
        nbCar += tailleTemporaire;
        if (posActuelle >= LOVC_MAX)
        { // Si on a depassé la taille du bloc LOVC
            posActuelle -= LOVC_MAX;
            blocActuel = buffer1.suivant;
            liredir_LOVC(fichier1, blocActuel, &buffer1);
        }
    }
    ecrireDirTOF(*fichier2, blocTOF, &buffer2); // Ecrire le contenu du dernier buffer dans le fichier TOF
    affEnteteTOF(*fichier2, 2, cpt);            // Mettre a jour le nombre des enregistrements dans l'entete
}

//============================== Affichage TOF

void affichageTOF(TOF *fichier)
{
    BufferTOF buffer;

    if (enteteTOF(fichier, 2) == 0) // Cas ou le fichier est vide
    {
        printf("le fichhier est vide\n");
    }
    for (int i = 1; i <= enteteTOF(fichier, 1); i++)
    { // Parcours du fichier jusqu'au dernier bloc
        lireDirTOF(fichier, i, &buffer);
        for (int j = 0; j < buffer.nb; j++)
        { // Parcours du bloc jusqu'au nombre des enregistrements dans le bloc
            printf("\n============================");
            printf("\nNumero : %s", buffer.tab[j].numero);
            printf("\nType : %c", buffer.tab[j].type);
            printf("\nSuperficie : %lld", buffer.tab[j].superficie);
        }
    }
}

//============================== Fonctions LOF

//============================== Creer fichier qui contient les terrains seulement dans une wilaya

void creeFichierTerrain(int numWilaya, TOF *fichier1, LOF **fichier2)
{
    char nom[37];
    BufferTOF bufftof;
    BufferLOF bufflof;
    int j = 1, jtemp, itof = 1, ilof = 1, jlof = 0, nbeng = 0, indice = 0;

    // tableIndex = malloc(sizeof(champIndex));
    nomFichierWilaya(numWilaya, nom); // pour genere le nom de la wilaya
    strcat(nom, "_Terrain");
    strcat(nom, ".bin");
    *fichier2 = ouvrirLOF(nom, 'N');
    affEnteteLOF(*fichier2, 1, 1);     // initialisation de l'entete de LOF
    affEnteteLOF(*fichier2, 2, 1);     // initialisation de l'entete de LOF
    affEnteteLOF(*fichier2, 3, nbeng); // initialisation de l'entete de LOF
    lireDirLOF(*fichier2, ilof, &bufflof);
    jlof = 0; // indice pour parcours fichier Tof

    while (j <= enteteTOF(fichier1, 2)) // tant que on n'a pas lu tout les enregistrments du fichier TOf
    {

        lireDirTOF(fichier1, itof, &bufftof); // on lit le buffer numero itof du fichier tof
        jtemp = 0;                            // indice de parcours a l'interieur du buffer tof
        while (jtemp < TOF_MAX)               // tant que on n'a pas lu tout les enregistrments du buffer tof
        {
            if (bufftof.tab[jtemp].type == 'T')
            {
                if (bufflof.nbEnrg < (LOF_MAX * SEUIL_LOF)) // si le buffer lof n'est pas rempli
                {
                    strcpy(bufflof.tab[jlof].numero, bufftof.tab[jtemp].numero);  // on copie le numero (cle) dans le buffer lof
                    bufflof.tab[jlof].superficie = bufftof.tab[jtemp].superficie; // con copie la superfecie dans le buffer lof
                    bufflof.nbEnrg++;                                             // on incermonte le nombre d'enregistrement dans le buffer lof
                    nbeng++;                                                      // on incermonte le nombre d'ergistrment inserer pour mittre a jour l'entete a la fin
                    tableIndex[indice].numBloc = ilof;                            // on copie le numero du bloc dans la table d'endex
                    strcpy(tableIndex[indice].numero, bufflof.tab[jlof].numero);  // on copie le numero dans la table d'index
                }
                else // si le buffer lof est rempli
                {

                    allocBlocLOF(*fichier2);                   // on alloue un nouveau buffer lof
                    bufflof.suivant = enteteLOF(*fichier2, 2); // on chaine les buffer
                                                               //  la mise a jour du dernier bloc se fait automatique dans la machine abstaite
                    ecrireDirLOF(*fichier2, ilof, &bufflof); // on ecrit l'ancien buffer lof
                    jlof = 0;                                // initialisation de l'indice de parcours dans lof
                    ilof++;                                  // incrementation de lindice de parcours des bloc lof(cette instruction est ecuivalante a ilof = bufflof.suivant

                    lireDirLOF(*fichier2, ilof, &bufflof);                        // on lit le nouveau buffer lof
                    strcpy(bufflof.tab[jlof].numero, bufftof.tab[jtemp].numero);  // on met la cle dans le buffer lof
                    bufflof.tab[jlof].superficie = bufftof.tab[jtemp].superficie; // on met la superficie dans le buffer lof
                    bufflof.nbEnrg = 1;                                           // on initialiser le nombre d'enregistrment dans bufferlof
                    nbeng++;                                                      // on incremente le nombre d'enregistrement inserer
                    // realloc(tableIndex, sizeof(champIndex) * ++indice);//on alloue une nouvelle cas dans le tableau d'index
                    indice++;
                    tableIndex[indice].numBloc = ilof;                           // on mis le numero du bloc dans le tableau d'index
                    strcpy(tableIndex[indice].numero, bufflof.tab[jlof].numero); // on mis la cle dans la table d'index
                }
                jlof++; // on incremente l'indice de parcours buffer lof
            }
            jtemp++;                        // on incermonte l'indice de parcous des buffer tof
            j++;                            // on incermente le nombre d'enregistrement mu
            if (j > enteteTOF(fichier1, 2)) // dans le cas ou le dernier bloc n'est pas rempli au seuil on sort des que on a lu tout les enregistrement
            {
                ecrireDirLOF(*fichier2, ilof, &bufflof); // on ecrit le buffer lof
                affEnteteLOF(*fichier2, 3, nbeng);       // on met a jour le nombre d'enregistrment dans lof
                affEnteteLOF(*fichier2, 4, ilof);        // on met a jour le nombre de bloc lof
                break;                                   // on sor de la fonction
            }
        }
        itof++; // on incermonte l'indice de parcours des bloc TOf
    }
    ecrireDirLOF(*fichier2, ilof, &bufflof); // on ecrit le buffer lof
    affEnteteLOF(*fichier2, 3, nbeng);       // on met a jour le nombre d'enregistrment dans lof
    affEnteteLOF(*fichier2, 4, ilof);        // on met a jour le nombre de bloc lof
}

//============================== Affichage du fichier LOF

void affichageLOF(LOF *fichier)
{
    BufferLOF buff;

    int j = 1, i = 1;

    if (enteteLOF(fichier, 3) == 0) // si le nombre d'enregistrment =0
    {
        printf("le fichier est vide\n");
    }
    while (j <= enteteLOF(fichier, 3)) // on parcours tout les enregistrment lof
    {
        lireDirLOF(fichier, i, &buff);
        for (int jtemp = 0; jtemp < buff.nbEnrg; jtemp++) // on parcours tout les enregistrment du buffer
        {
            printf("=====================\n");
            printf("numero : %s\n", buff.tab[jtemp].numero);
            printf("superficie : %lli\n", buff.tab[jtemp].superficie);
            j++;
            if (j > enteteLOF(fichier, 3)) // dans le cas ou le dernier buffer n'est pas remplis a 100%
            {
                break;
            }
        }

        i = buff.suivant;
    }
    printf("=====================\n");
}

//============================== Fonctions Index

//============================== Affichage de la table d'index

void affichageTableIndex(champIndex tableIndex[], int nombreChamps)
{
    for (int i = 0; i < nombreChamps; i++)
    {
        printf("\n================================");
        printf("\nNumero bloc : %d", tableIndex[i].numBloc);
        printf("\nPlus grand numero : %s", tableIndex[i].numero);
    }
}

//=================================================verifie si le fichier existe

int checkIfFileExists(const char *filename)
{
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if (exist == 0)
        return 1;
    else
        return 0;
}

//============================== Verifier existance

int verfierExiste(LOF *fichier, int numBloc, char numero[])
{
    BufferLOF buffer;

    lireDirLOF(fichier, numBloc, &buffer);
    // Parcourir le bloc
    for (int i = 0; i < buffer.nbEnrg; i++)
    {
        if (strcmp(numero, buffer.tab[i].numero) == 0)
        {
            return 1; // Quand on trouve le numero recherché
        }
    }
    return 0; // Si on trouve pas le numero recherché
}

//============================== Insertion dans le bloc avec les décalages

void insertionInterne(LOF **fichier, int numBloc, livretLOF livret)
{
    BufferLOF buffer;
    int pos = 0;
    livretLOF livrettmp;

    lireDirLOF(*fichier, numBloc, &buffer); // on lit le buffer ou il faut inserer la valeur
    while ((strcmp(livret.numero, buffer.tab[pos].numero) > 0) && (pos < buffer.nbEnrg))
    { // Trouver la position dans le buffer
        pos++;
    }

    if (pos == LOF_MAX - 1)
    { // Le cas ou la position est a la fin du bloc
        // buffer.tab[pos] = livret;
        strcpy(buffer.tab[pos].numero, livret.numero);
        buffer.tab[pos].superficie = livret.superficie;
    }
    else
    { // Les autres positions

        int max = buffer.nbEnrg;
        if (buffer.nbEnrg == LOF_MAX) // si le buffer est remplis a 100%
        {
            max--; // pour avoir la dernier position
        }
        for (int i = max; i > pos; i--) // Decaler tout les enregistrement d'une seule case
        {
            strcpy(buffer.tab[i].numero, buffer.tab[i - 1].numero);
            buffer.tab[i].superficie = buffer.tab[i - 1].superficie;
        }
        // Affecter le nouveau enregistrement
        strcpy(buffer.tab[pos].numero, livret.numero);
        buffer.tab[pos].superficie = livret.superficie;
    }
    if (buffer.nbEnrg < LOF_MAX) // si le buffer n'est pas rempli on incremente le nombre d'enregistrement
    {
        buffer.nbEnrg++;
    }

    ecrireDirLOF(*fichier, numBloc, &buffer);
}

//============================== Insertion avec table d'index

void insertionIndex(champIndex tableIndex[], int nombreChamps, LOF **fichier, livretLOF livret)
{
    int i = 0, numBloc, stop = 0, exist = 0;
    BufferLOF buffer, buffer2;
    livretLOF livretTemp;

    if (enteteLOF(*fichier, 3) == 0) // si le fichier lof est vide
    {
        lireDirLOF(*fichier, 1, &buffer);             // on lit le premier buffer
        strcpy(buffer.tab[0].numero, livret.numero);  // affectation du numero au buffer
        buffer.tab[0].superficie = livret.superficie; // affectation de la superficie au buffer
        buffer.nbEnrg = 1;
        tableIndex[0].numBloc = 1; // affectation du numero du bloc a la table d'index
        strcpy(tableIndex[0].numero, livret.numero);
        affEnteteLOF(*fichier, 3, 1);       // nombre d'enregistrement inserer
        ecrireDirLOF(*fichier, 1, &buffer); // ecrire le buffer
    }

    else
    {
        // Trouver le numero du bloc qui est supposé de contenir le livret
        while ((i < nombreChamps) && (stop == 0))
        {
            if (strcmp(livret.numero, tableIndex[i].numero) <= 0)
            {
                numBloc = tableIndex[i].numBloc;
                if (strcmp(livret.numero, tableIndex[i].numero) == 0)
                { // Verifier si le numero existe dans la table d'index
                    exist = 1;
                }
                stop = 1;
            }
            i++;
        }
        if (stop != 1) // dans le cas ou on insere au dernier bloc
        {

            numBloc = tableIndex[nombreChamps - 1].numBloc;
        }
        if (exist != 1)
        {
            if (verfierExiste(*fichier, numBloc, livret.numero) != 1) // Verifier si la numero du livret existe deja dans le bloc
            {

                lireDirLOF(*fichier, numBloc, &buffer);
                if (buffer.nbEnrg < LOF_MAX)
                { // Cas ou il y a de l'espace dans le bloc

                    if ((strcmp(livret.numero, tableIndex[nombreChamps - 1].numero) > 0) && (i == nombreChamps)) // si on insert dans le dernier bloc a la dernier position
                    {
                        strcpy(tableIndex[nombreChamps - 1].numero, livret.numero); // mettre a jour la table d'index
                    }
                    insertionInterne(&(*fichier), numBloc, livret); // la fonction qui insert a l'interier du bloc
                    buffer.nbEnrg++;                                // on incremente le nombre d'enregistrement
                }
                else
                {
                    if (buffer.nbEnrg == LOF_MAX)
                    {                                                                  // Cas ou le bloc est plein
                        if (strcmp(buffer.tab[LOF_MAX - 1].numero, livret.numero) > 0) // si on insert pas a la dernier position du dernier bloc
                        {

                            affEnteteLOF(*fichier, 4, enteteLOF(*fichier, 4) + 1);  // on incremente le nombre de de bloc
                            lireDirLOF(*fichier, enteteLOF(*fichier, 4), &buffer2); // on lit le bloc ajouter dans buffer2
                            buffer2.suivant = buffer.suivant;                       // on chaine les buffer
                            buffer.suivant = enteteLOF(*fichier, 4);

                            strcpy(buffer2.tab[0].numero, buffer.tab[LOF_MAX - 1].numero); // on copie la dernier valeur du buffer dans le nouveau buffer alloue
                            buffer2.tab[0].superficie = buffer.tab[LOF_MAX - 1].superficie;

                            insertionInterne(&(*fichier), numBloc, livret); // fonction qui insert a l'interier du buffer

                            lireDirLOF(*fichier, numBloc, &buffer);  // on lit le buffer pour le chainer
                            buffer.suivant = enteteLOF(*fichier, 4); // on chaine le buffer
                            buffer2.nbEnrg = 1;
                            ecrireDirLOF(*fichier, enteteLOF(*fichier, 4), &buffer2); // on ecrit le bufffer 2
                            ecrireDirLOF(*fichier, numBloc, &buffer);                 // on ecrit le buffer
                            char tmp[11], tmp2[11];
                            int Nubloctmp1, Nubloctmp2;

                            strcpy(tmp, tableIndex[i - 1].numero); // pour faire les decalge dans la table d'index
                            Nubloctmp1 = tableIndex[i - 1].numBloc;

                            strcpy(tmp2, buffer.tab[LOF_MAX - 1].numero);
                            Nubloctmp2 = Nubloctmp1;
                            for (int j = i - 1; j < nombreChamps; j++) // faire les decalge dans la table d'index
                            {
                                strcpy(tableIndex[j].numero, tmp2);
                                tableIndex[j].numBloc = Nubloctmp2;
                                strcpy(tmp2, tmp);
                                Nubloctmp2 = Nubloctmp1;
                                if ((j + 1) < nombreChamps)
                                {
                                    strcpy(tmp, tableIndex[j + 1].numero);
                                    Nubloctmp1 = tableIndex[j + 1].numBloc;
                                }
                            }

                            tableIndex[i].numBloc = enteteLOF(*fichier, 4); // le numero du nouveau bloc est affecte a la case correspendanteau bloc inserer

                            // realloc(tableIndex, sizeof(champIndex) * nombreChamps + 1);//on alloue une nouvelle case pour inserer la dernier valeur

                            tableIndex[nombreChamps].numBloc = Nubloctmp1;
                            if ((i == nombreChamps)) // si on a inserser dans le dernier bloc
                            {
                                tableIndex[i].numBloc = enteteLOF(*fichier, 4);
                            }

                            strcpy(tableIndex[nombreChamps].numero, tmp);
                        }
                        else // si on insert  a la dernier position du dernier bloc
                        {
                            affEnteteLOF(*fichier, 4, enteteLOF(*fichier, 4) + 1);  // on incermente le nombre de bloc
                            lireDirLOF(*fichier, enteteLOF(*fichier, 4), &buffer2); // on lit le nouveau bloc
                            buffer2.suivant = buffer.suivant;                       // on chaine

                            buffer.suivant = enteteLOF(*fichier, 4); // on chaine

                            strcpy(buffer2.tab[0].numero, livret.numero); // on colle la valeur a inseret dans le nouveau buffer allouer
                            buffer2.tab[0].superficie = livret.superficie;

                            // realloc(tableIndex, sizeof(champIndex) * nombreChamps + 1);//on alloue une nouvelle case dans la table d'index

                            tableIndex[nombreChamps].numBloc = enteteLOF(*fichier, 4); // on met a jour la table d'index

                            strcpy(tableIndex[nombreChamps].numero, livret.numero);
                            ecrireDirLOF(*fichier, numBloc, &buffer);                 // on ecrit notre buffer
                            buffer2.nbEnrg = 1;                                       // on initialiser le nombre d'enregistrement du buffer 2
                            ecrireDirLOF(*fichier, enteteLOF(*fichier, 4), &buffer2); // on ecrit le buffer 2
                        }
                    }
                }
                affEnteteLOF(*fichier, 3, enteteLOF(*fichier, 3) + 1); // on met a jour le nombre d'enregistrement insert dans lof
            }
        }
    }
}

//============================== Sauvegarde de la table d'index dans un fichier index

void sauvegardeFichierIndex(champIndex tableIndex[], int nombreChamps, char Nomfich[])
{
    Index f;
    // Sauvegarde du nombre des blocs dans le fichier
    f.fichier = fopen(Nomfich, "wb+");
    f.nombreChamps = nombreChamps;
    fwrite(&(f.nombreChamps), sizeof(int), 1, f.fichier);
    // Sauvegarde de la table d'index dans le fichier
    fseek(f.fichier, sizeof(int), SEEK_SET);
    // fwrite(tableIndex, sizeof(champIndex) * nombreChamps, 1, f->fichier);
    fwrite(tableIndex, sizeof(champIndex) * nombreChamps, 1, f.fichier);

    // Fermeture du fichier index
    rewind(f.fichier);
    fclose(f.fichier);
}

//============================== Chargement de la table d'index a partir du fichier index

void chargementFichierIndex(int *nombreChamps, char Nomfich[], champIndex tableIndex1[])
{

    Index f;
    int nbChamps;

    // Recuperation du nombre des blocs
    f.fichier = fopen(Nomfich, "rb+");

    fread(&nbChamps, sizeof(int), 1, f.fichier);

    *nombreChamps = nbChamps;
    // Chargement de la table d'index dans la mémoire centrale
    fseek(f.fichier, sizeof(int), SEEK_SET);
    fread(tableIndex1, sizeof(champIndex) * nbChamps, 1, f.fichier);

    // Fermeture du fichier
    rewind(f.fichier);
    fclose(f.fichier);
}

void affichage1()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                         _______   _____           _____   ______    _____   _____  \n");
    printf("                        |__   __| |  __ \         / ____| |  ____|  / ____| |  __ \ \n");
    printf("                           | |    | |__) |       | (___   | |__    | (___   | |  | |\n");
    printf("                           | |    |  ___/         \___ \    |  __|    \___ \    | |  | |\n");
    printf("                           | |    | |             ____) | | |       ____) | | |__| |\n");
    printf("                           |_|    |_|            |_____/  |_|      |_____/  |_____/ \n");
    printf("\n\n                                    TP : Gestion des livrets fonciers\n");
    printf("\n\n                                                                            realiser par : -Chalal Lyes\n");
    printf("                                                                                           -Louggani Abderaouf \n");
}
void affichemenu1()
{

    printf("                                           __ \n");
    printf("                                          /_ |\n");
    printf("  _ __ ___     ___   _ __    _   _         | |\n");
    printf(" | '_ ` _ \   / _ \ | '_ \  | | | |           | |\n");
    printf(" | | | | | | |  __/| | | | | |_| |         | |\n");
    printf(" |_| |_| |_|  \___| |_| |_|   \__,_|         |_|\n");
}

void affichemenu2()
{

    printf("                                           ___  \n");
    printf("                                          |__ \ \n");
    printf("  _ __ ___     ___   _ __    _   _           ) |\n");
    printf(" | '_ ` _ \   / _ \ | '_ \  | | | |            / / \n");
    printf(" | | | | | | |  __/| | | | | |_| |         / /_ \n");
    printf(" |_| |_| |_|  \___| |_| |_|   \__,_|        |____|\n");
}

void affichemenutof()
{
    printf("                                           _______    ____    ______ \n");
    printf("                                          |__   __|  / __ \  |  ____|\n");
    printf("  _ __ ___     ___   _ __    _   _           | |    | |  | || |__   \n");
    printf(" | '_ ` _ \   / _ \ | '_ \  | | | |             | |    | |  || |  __|  \n");
    printf(" | | | | | | |  __/| | | | | |_| |           | |    | |__|| | |     \n");
    printf(" |_| |_| |_|  \___| |_| |_|  \__,_|            |_|     \____/  |_|     \n");
}

void affichemenulof()
{
    printf("                                           _         ____    ______ \n");
    printf("                                          | |       / __ \  |  ____|\n");
    printf("  _ __ ___     ___   _ __    _   _        | |      | |  | || |__   \n");
    printf(" | '_ ` _ \   / _ \ | '_ \    | | | |        | |      | |  | ||  __|  \n");
    printf(" | | | | | | |  __/| | | | | |_| |        | |____  | |__| || |     \n");
    printf(" |_| |_| |_|  \___| |_| |_|   \__,_|        |______|  \____/  |_|     \n");
}
