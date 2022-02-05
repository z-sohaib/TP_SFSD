#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "fonction.c"

void menu()
{
    char ChoixMenu1[2];
    int ChoixMenu1Int;
    char ChoixMenu2[2];
    int ChoixMenu2Int;

    while (strcmp(ChoixMenu1, "3") != 0) // tant que l'utilisateur n'a pas quitter
    {
        system("cls");

        // printf("======================menu1======================\n");
        affichemenu1();
        printf("\n");
        printf("       1 : Creer nouveau fichier(remarque : on fait la creation avec des cle paires pour tester l'insertion au milieu )\n");
        printf("       2 : Chargement d'un fichier existant\n");
        printf("       3 : Quitter\n\n");

        printf("donner votre choix : ");
        scanf("%s", ChoixMenu1);

        ChoixMenu1Int = atoi(ChoixMenu1);
        int Nbeng = 1; // nombre d'enregistrement a inserer
        LOVC *fichier; // declaration de notre pointeur fichier

        switch (ChoixMenu1Int)
        {
        case 1: // creation d'un nouveau fichier
            do
            {
                printf("donner le nombre d'enregistrement a insere : ");
                scanf("%d", &Nbeng);
            } while (Nbeng <= 0);
            creerFichier(&fichier, Nbeng);                      // creatoin du fichier de Nbeng enregistrement
            fermer_LOVC(fichier);                               // sauvegarde de l'entete dans le fichier
            fichier = ouvrir_LOVC("Livrets_National.bin", 'A'); // ouverture du fichier
            printf("\ncreation effectuee avec succes\n");
            printf("appyer sur entre pour continuer\n");
            getch();
            break;

        case 2:                                            // Chargement d'un fichier existant
            if (checkIfFileExists("Livrets_National.bin")) // si le fichier existe
            {
                fichier = ouvrir_LOVC("Livrets_National.bin", 'A'); // ouverture du fichier
                printf("\nchargement effectue avec succes\n");
                printf("appyer sur entre pour continuer\n");
                getch();
            }
            else // fichier n'exite pas
            {
                printf("fichier introuvable veuiller creer un nouveau fichier\n");
                printf("appyer sur entre pour continuer\n");
                getch();
            }
            break;
        case 3:                   // quitter le programme
            fermer_LOVC(fichier); // sauvegarde de l'entete dans le fichier
            break;
        }
        if (strcmp(ChoixMenu1, "3") != 0) // si l'utilisateur n'as pas choisi de quitter le programme
        {

            //================================menu2
            char cle[11];
            int trouv, i, j;
            char taillech[6]; // pour recuperer la taille du champ variable
            char Rechchaine[LOVC_MAX];
            BufferLOVC BuffLovc;
            long long int cle_int;
            int supprimer;
            char ChoixMenuTof[2];
            int ChoixMenuTofInt;
            int bouclewilaya = 1;
            TOF *fichierTof;
            int wilayaTof;
            char ChoixMenuLof[2];
            int ChoixMenuLofInt;
            LOF *fichierLof;
            LOF *Lofaffiche;
            LOF *Lofins;
            LOVC *fichierReorga;
            char rep[2];
            float sup, ins, res;
            char nom[37];
            char Nomfich[50];
            livretLOF livret;
            int maxindex;
            champIndex *tableIndex1;
            int stop;
            char nomwil[37];

            if (checkIfFileExists("Livrets_National.bin")) // si le fichier existe
            {
                strcpy(ChoixMenu2, "0");
                while (strcmp(ChoixMenu2, "9") != 0) // tantque l'utlisateur n'a pas quitt�
                {
                    system("cls");
                    // printf("======================menu2======================\n");
                    affichemenu2();
                    printf("\n");
                    printf("       1 :affichageLovc\n");
                    printf("       2 :recherche\n");
                    printf("       3 :insertion\n");
                    printf("       4 :suppression\n");
                    printf("       5 :Reorganisation\n");
                    printf("       6 :Menu Tof\n");
                    printf("       7 :Menu Lof\n");
                    printf("       9 :retour vers Menu1\n\n");

                    printf("donner votre choix : ");
                    scanf("%s", ChoixMenu2);

                    ChoixMenu2Int = atoi(ChoixMenu2);

                    switch (ChoixMenu2Int)
                    {
                    case 1:                     // affichage Lovc
                        affichageLOVC(fichier); // appel de la fonction d'affichage
                        printf("\naffichage effectue avec succes\n");
                        printf("appyer sur entre pour continuer\n");
                        getch();
                        break;
                    case 2: // recherche Lovc
                        printf("donner la cle que vous cherchez : ");
                        scanf("%lli", &cle_int);
                        nbtochar(cle_int, cle);                  // conveti la cle en chaine de caractre de 10 caractere
                        recherche(fichier, cle, &trouv, &i, &j); // appel a la fonction de recherche
                        if (trouv == 1)                          // si on a trouv� la cle
                        {
                            liredir_LOVC(fichier, i, &BuffLovc);                                  // on lit le bloc
                            RecupChamp(5, BuffLovc, fichier, i, j, taillech);                     // on recupere la taille du champ variable
                            RecupChamp(atoi(taillech) + 41, BuffLovc, fichier, i, j, Rechchaine); // on lit la chaine de caractere d'enregistrement recherch�
                            if (Rechchaine[5] == '0')                                             // si l'enregistrement n'est pas supprim�
                            {
                                printf("cle trouvee\n");
                                affichageChamps(Rechchaine); // affichage
                            }
                            else // enregistrement supprim�
                            {
                                printf("cle introuvable (supprimer)\n");
                            }
                        }
                        else // cle introuvable
                        {
                            printf("cle introuvable\n");
                        }
                        printf("\nrecherche effectuee avec succes\n");
                        printf("appyer sur entre pour continuer\n");
                        getch();
                        break;
                    case 3: // insertion
                        printf("donner la cle que vous voulez inserer : ");
                        scanf("%lli", &cle_int);
                        nbtochar(cle_int, cle); // conveti la cle en chaine de caractre de 10 caractere
                        inserer(&fichier, cle); // insertion

                        fermer_LOVC(fichier);                               // sauvegarde de l'entete dans le fichier
                        fichier = ouvrir_LOVC("Livrets_National.bin", 'A'); // ouverture du fichier

                        printf("appyer sur entre pour continuer\n");
                        getch();
                        break;
                    case 4: // suppression
                        printf("donner la cle que vous voulez supprimer : ");
                        scanf("%lli", &cle_int);
                        nbtochar(cle_int, cle); // conveti la cle en chaine de caractre de 10 caractere
                        supprimer = supressionLOVC(&fichier, cle);
                        if (supprimer == 0) // si l'enregistrement est supprim�
                        {
                            printf("\nsuppression effectuee avec succes\n");
                            fermer_LOVC(fichier);                               // sauvegarde de l'entete dans le fichier
                            fichier = ouvrir_LOVC("Livrets_National.bin", 'A'); // ouverture du fichier
                        }
                        else // cle introuvable
                        {
                            printf("cle introuvable\n");
                        }
                        sup = entete_LOVC(fichier, 5);
                        ins = entete_LOVC(fichier, 6);
                        res = sup / ins;
                        if (SEUIL_LOVC <= res) // pour faire la reoganisation automatique
                        {
                            reorganisation(fichier, &fichierReorga);
                            fermer_LOVC(fichierReorga);
                            printf("reorganisation automatique effectuee avec succes veulliez quitter le prgramme et renommer le fichier 'Livrets_National_v2' en 'Livrets_National'\n");
                        }
                        printf("appyer sur entre pour continuer\n");
                        getch();
                        break;
                    case 5: // reorganisation
                        reorganisation(fichier, &fichierReorga);
                        printf("reorganisation automatique effectuee avec succes veulliez quitter le prgramme et renommer le fichier 'Livrets_National_v2' en 'Livrets_National'\n");
                        printf("voulez vous afficher le fichier apres la reorganisation (O/N) : ");
                        scanf("%s", rep);
                        if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                        {
                            affichageLOVC(fichierReorga);
                        }

                        printf("\reorganisation effectuee avec succes\n");
                        fermer_LOVC(fichierReorga);
                        printf("appyer sur entre pour continuer\n");
                        getch();
                        break;
                        break;
                    case 6: // Menu Tof
                        strcpy(ChoixMenuTof, "1");
                        while (strcmp(ChoixMenuTof, "0") != 0)
                        {
                            system("cls");
                            // printf("======================Menu Tof======================\n");
                            affichemenutof();
                            printf("\n");
                            printf("       1 :creer fichier pour toutes les wilayas\n");
                            printf("       2 :creer fichier pour une wilaya donnee\n");
                            printf("       3 :affichage fichier wilaya\n");
                            printf("       0 :Retour au Menu 2\n");

                            printf("donner votre choix : ");
                            scanf("%s", ChoixMenuTof);

                            ChoixMenuTofInt = atoi(ChoixMenuTof);

                            switch (ChoixMenuTofInt)
                            {
                            case 1: // cree fichier tof pour toute les wilaya
                                bouclewilaya = 1;
                                while (bouclewilaya <= 58)
                                {
                                    creerFichierWilaya(bouclewilaya, fichier, &fichierTof);
                                    fermerTOF(fichierTof);
                                    bouclewilaya++;
                                }
                                printf("\ncreation effectuee avec succes\n");
                                printf("appyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 2: // creer fichier pour une wilaya donnee
                                do
                                {
                                    printf("donner le numero de la wilaya(entier)  : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58)); // si la wilaya scanner est inferieur a 1 ou superieur a 58
                                creerFichierWilaya(wilayaTof, fichier, &fichierTof);
                                fermerTOF(fichierTof);
                                printf("\ncreation effectuee avec succes\n");
                                printf("appyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 3: // affichage fichier wilaya
                                do
                                {
                                    printf("donner le numero de la wilaya que vous voulez afficher  : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));
                                nomFichierWilaya(wilayaTof, nom); // pour genere le nom de la wilaya
                                strcat(nom, ".bin");
                                if (checkIfFileExists(nom))
                                {
                                    fichierTof = ouvrirLOF(nom, 'A');
                                    affichageTOF(fichierTof);
                                    printf("\naffichage effectue avec succes\n");
                                }
                                else
                                {
                                    printf("veulliez cree le fichier d'abord\n");
                                }
                                // creerFichierWilaya(wilayaTof, fichier,&fichierTof);//en cas de mise a jour du fichier LOVC le fichier tof sera recree
                                printf("appyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 0:
                                break;
                            }
                        }
                        break;
                    case 7: // menu Lof
                        strcpy(ChoixMenuLof, "1");
                        while (strcmp(ChoixMenuLof, "0") != 0)
                        {
                            system("cls");
                            // printf("======================Menu Lof======================\n");
                            affichemenulof();
                            printf("\n");
                            printf("       1 :creer fichier Lof pour une wilaya donnee\n");
                            printf("       2 :insertion LOF (sauvgarde table d'index) +affichage table d'index + chargement table d'index \n");
                            printf("       3 :affichage fichier Lof\n");
                            printf("       4 :insertion LOF sans sauvgarde table d'index(dossier differents'fichier_lof_sans_sauv')\n");
                            printf("       5 :affichage table d'index\n");
                            printf("       6 :insertion LOF sans sauvgarde table d'index(le meme dossier)\n");
                            printf("       0 :Retour au Menu 2\n");

                            printf("donner votre choix : ");
                            scanf("%s", ChoixMenuLof);

                            ChoixMenuLofInt = atoi(ChoixMenuLof);
                            switch (ChoixMenuLofInt)
                            {
                            case 1: // creer fichier Lof pour une wilaya donnee
                                do
                                {
                                    printf("donner le numero de la wilaya que vous voulez cree  : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nomwil); // pour genere le nom de la wilaya
                                strcat(nomwil, ".bin");
                                if (checkIfFileExists(nomwil)) // si le fichier de la wilaya existe
                                {
                                    fichierTof = ouvrirLOF(nomwil, 'A');
                                }
                                else // si le fichier de la wilaya n'existe pas
                                {
                                    printf("\nfichier wilaya introuvable\n");
                                    printf("creation fichier wilaya en cours\n");
                                    creerFichierWilaya(wilayaTof, fichier, &fichierTof);
                                }

                                creeFichierTerrain(wilayaTof, fichierTof, &fichierLof); // en cas de mise a jour du fichier LOVC le fichier LOF sera mis a jour
                                fermerLOF(fichierLof);                                  // pour sauvgarder l'entete
                                fermerTOF(fichierTof);                                  // pour sauvgarder l'entete
                                printf("\ncreation du fichier LOF effectuee avec succes\n");
                                printf("appyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 2: // insertion LOf (sauvgarde table d'index) +affichage table d'index + chargement table d'index
                                do
                                {
                                    printf("donner le numero de la wilaya   : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nomwil); // pour genere le nom de la wilaya
                                strcat(nomwil, ".bin");
                                if (checkIfFileExists(nomwil)) // si le fichier wilaya existe
                                {
                                    fichierTof = ouvrirLOF(nomwil, 'A');
                                }
                                else
                                {
                                    printf("\nfichier wilaya introuvable\n");
                                    printf("creation fichier wilaya en cours\n");
                                    creerFichierWilaya(wilayaTof, fichier, &fichierTof);
                                }
                                nomFichierWilaya(wilayaTof, nom); // pour generer le nom du fichier LOF
                                strcat(nom, "_Terrain");
                                strcpy(Nomfich, nom);
                                strcat(Nomfich, "_Index.bin"); // Nomfich = nom du fichier index de la wilaya donnee
                                strcat(nom, ".bin");
                                if (checkIfFileExists(nom) && checkIfFileExists(Nomfich)) // si le fichier lof et le fichier index existe
                                {
                                    printf("voulez vous charger la table d'index (si non le fichier lof et sa table d'index sera recree et les insertion faites avant seront perdu )  (O/N)");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0)) // si l'utilisateur veux charger la table d'index
                                    {
                                        Lofins = ouvrirLOF(nom, 'A');                           // ouverture du fichier lof
                                        chargementFichierIndex(&maxindex, Nomfich, tableIndex); // chargment de la table d'index
                                    }
                                    else
                                    {                                                       // l'utilisateur ne veut pas charger la table d'index
                                        creeFichierTerrain(wilayaTof, fichierTof, &Lofins); // recreation du fichier lof et de la table d'index(la table d'index sera mise a jour automatiquement avec cette fonction
                                    }

                                    printf("donner le numero a insere(cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                }
                                else
                                { // si le fichier lof et le fichier index n'existe pas
                                    printf("table d'index introuvable \n");
                                    printf("\ncreation du fichier LOf avec sa table d'index en cours\n");
                                    creeFichierTerrain(wilayaTof, fichierTof, &Lofins); // recreation du fichier lof et de la table d'index

                                    printf("donner le numero a insere (cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                }
                                printf("\nvoulez vous afficher la table d'index (O/N): ");
                                scanf("%s", rep);
                                if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                {
                                    affichageTableIndex(tableIndex, enteteLOF(Lofins, 4));
                                }
                                sauvegardeFichierIndex(tableIndex, enteteLOF(Lofins, 4), Nomfich); // sauvgarde du fichier index
                                printf("\nsauvgarde effectuee avec succes\n");

                                fermerLOF(Lofins); // pour enregister l'entete
                                fermerTOF(fichierTof);
                                printf("\ninsertion effectuee avec succes\n");
                                printf("appuyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 3: // affichage lof
                                do
                                {
                                    printf("donner le numero de la wilaya que vous voulez afficher  : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nom); // gnere le nom du fichier
                                strcat(nom, "_Terrain");
                                strcat(nom, ".bin");
                                if (checkIfFileExists(nom)) // si le fichier lof existe
                                {
                                    Lofaffiche = ouvrirLOF(nom, 'A');

                                    affichageLOF(Lofaffiche);
                                    printf("\naffichage effectuee avec succes\n");
                                    printf("appyer sur entre pour continuer\n");
                                    getch();
                                }
                                else
                                {
                                    printf("\nfichier introuvable veuiller creer le fichier\n");
                                    printf("appyer sur entre pour continuer\n");
                                    getch();
                                }

                                break;
                            case 4:                              // insertion LOF sans sauvgarde table d'index
                                _mkdir("fichier_lof_sans_sauv"); // creation du dossier pour mettre le nouveau fichier lof
                                chdir("fichier_lof_sans_sauv");  // changement du repertoire
                                do
                                {
                                    printf("donner le numero de la wilaya   : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nomwil); // pour genere le nom de la wilaya
                                strcat(nomwil, ".bin");
                                if (checkIfFileExists(nomwil))
                                {
                                    fichierTof = ouvrirLOF(nomwil, 'A');
                                }
                                else
                                {
                                    printf("\nfichier wilaya introuvable\n");
                                    printf("creation fichier wilaya en cours\n");
                                    creerFichierWilaya(wilayaTof, fichier, &fichierTof);
                                }

                                creeFichierTerrain(wilayaTof, fichierTof, &Lofins); // il faut recree le fichier tof a chaque fois car de se repertoire en insert sans sauvgarder la table d'index
                                stop = 0;
                                while (stop == 0) // utlisateur veux arreter les insertion
                                {
                                    printf("donner le numero a insere (cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                    printf("\nvoulez vous afficher la table d'index (O/N):");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                    {
                                        affichageTableIndex(tableIndex, enteteLOF(Lofins, 4));
                                    }
                                    printf("\nvoulez vous afficher fichier lof (O/N): ");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                    {
                                        affichageLOF(Lofins);
                                    }
                                    printf("voulez vous continuer a inserer (O/N): ");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                    {
                                        stop = 0;
                                    }
                                    else
                                    {
                                        stop = 1;
                                    }
                                }
                                fermerLOF(Lofins);
                                fermerTOF(fichierTof);
                                chdir("../"); // retour a l'ancien repertoire
                                break;
                            case 5: // affichage table d'index
                                do
                                {
                                    printf("donner le numero de la wilaya   : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nom); // pour generer le nom du fichier LOF
                                strcat(nom, "_Terrain");
                                strcpy(Nomfich, nom);
                                strcat(Nomfich, "_Index.bin"); // Nomfich = nom du fichier index de la wilaya donnee
                                strcat(nom, ".bin");
                                if (checkIfFileExists(Nomfich)) // si le fichier lof et le fichier index existe
                                {
                                    chargementFichierIndex(&maxindex, Nomfich, tableIndex); // chargment de la table d'index
                                    affichageTableIndex(tableIndex, maxindex);
                                }
                                else
                                {
                                    printf("veuillez creer la table d'index et la sauvgarder (option2 menu lof) \n");
                                }
                                printf("\nappyer sur entre pour continuer\n");
                                getch();
                                break;
                            case 6:
                                do
                                {
                                    printf("donner le numero de la wilaya   : ");
                                    scanf("%d", &wilayaTof);
                                } while ((wilayaTof < 1) || (wilayaTof > 58));

                                nomFichierWilaya(wilayaTof, nomwil); // pour genere le nom de la wilaya
                                strcat(nomwil, ".bin");
                                if (checkIfFileExists(nomwil)) // si le fichier wilaya existe
                                {
                                    fichierTof = ouvrirLOF(nomwil, 'A');
                                }
                                else
                                {
                                    printf("\nfichier wilaya introuvable\n");
                                    printf("creation fichier wilaya en cours\n");
                                    creerFichierWilaya(wilayaTof, fichier, &fichierTof);
                                }
                                nomFichierWilaya(wilayaTof, nom); // pour generer le nom du fichier LOF
                                strcat(nom, "_Terrain");
                                strcpy(Nomfich, nom);
                                strcat(Nomfich, "_Index.bin"); // Nomfich = nom du fichier index de la wilaya donnee
                                strcat(nom, ".bin");
                                if (checkIfFileExists(nom) && checkIfFileExists(Nomfich)) // si le fichier lof et le fichier index existe
                                {
                                    printf("voulez vous charger la table d'index (si non le fichier lof et sa table d'index sera recree et les insertion faites avant seront perdu )  (O/N)");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0)) // si l'utilisateur veux charger la table d'index
                                    {
                                        Lofins = ouvrirLOF(nom, 'A');                           // ouverture du fichier lof
                                        chargementFichierIndex(&maxindex, Nomfich, tableIndex); // chargement de la table d'index
                                    }
                                    else
                                    { // l'utilisateur ne veut pas charger la table d'index

                                        creeFichierTerrain(wilayaTof, fichierTof, &Lofins); // recreation du fichier lof et de la table d'index(la table d'index sera mise a jour automatiquement avec cette fonction
                                    }

                                    printf("donner le numero a insere(cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                }
                                else
                                { // si le fichier lof et le fichier index n'existe pas
                                    printf("table d'index introuvable \n");
                                    printf("\ncreation du fichier LOf avec sa table d'index en cours\n");
                                    creeFichierTerrain(wilayaTof, fichierTof, &Lofins); // recreation du fichier lof et de la table d'index

                                    printf("donner le numero a insere (cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                }
                                printf("\nvoulez vous afficher la table d'index (O/N): ");
                                scanf("%s", rep);
                                if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                {
                                    affichageTableIndex(tableIndex, enteteLOF(Lofins, 4));
                                }

                                printf("\nvoulez vous afficher fichier lof (O/N): ");
                                scanf("%s", rep);
                                if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                {
                                    affichageLOF(Lofins);
                                }

                                printf("voulez vous faire d'autre insertion (O/N): ");
                                scanf("%s", rep);
                                while (strcmp(rep, "O") == 0 || strcmp(rep, "o") == 0)
                                {
                                    printf("donner le numero a insere (cle)(entier) : ");
                                    scanf("%lli", &cle_int);
                                    nbtochar(cle_int, livret.numero);
                                    printf("\ndonner la superficie : ");
                                    scanf("%lli", &livret.superficie);
                                    insertionIndex(tableIndex, enteteLOF(Lofins, 4), &Lofins, livret); // insertion dans le fichier LOf avec mise a jour de la tble d'index
                                    printf("\nvoulez vous afficher la table d'index (O/N): ");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                    {
                                        affichageTableIndex(tableIndex, enteteLOF(Lofins, 4));
                                    }

                                    printf("\nvoulez vous afficher fichier lof (O/N): ");
                                    scanf("%s", rep);
                                    if ((strcmp(rep, "o") == 0) || (strcmp(rep, "O") == 0))
                                    {
                                        affichageLOF(Lofins);
                                    }

                                    printf("voulez vous faire d'autre insertion(O/N) : ");
                                    scanf("%s", rep);
                                }
                                sauvegardeFichierIndex(tableIndex, enteteLOF(Lofins, 4), Nomfich); // sauvgarde du fichier index
                                fermerLOF(Lofins);                                                 // pour enregister l'entete
                                fermerTOF(fichierTof);                                             // pour enregister l'entete
                                printf("\ninsertion effectuee avec succes\n");
                                printf("appuyer sur entre pour continuer\n");
                                getch();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
