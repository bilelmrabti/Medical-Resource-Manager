#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Structure pour représenter une ressource */
typedef struct resource {
    int id;
    char nom_personnel[50];
    int matricule_ambulance;
    char disponibilite[20];
    char localisation[50];
} resource;

typedef struct listeResource {
    resource value;
    struct listeResource *next;
} listeResource;

typedef struct patient {
    int id;
    char nom_patient[50];
    char prenom_patient[50];
    char gouvernorat[50];
    int num_telephone;
} patient;

typedef struct listePatient {
    patient value;
    struct listePatient *next;
} listePatient;

// Fonction pour comparer deux ressources
bool compareRessources(resource a, resource b) {
    return a.id == b.id || a.matricule_ambulance == b.matricule_ambulance;
}

// Fonction pour vérifier l'unicité d'une ressource dans la liste
bool isUniqueRessource(listeResource *li, resource data) {
    listeResource *current = li;
    while (current != NULL) {
        if (compareRessources(current->value, data)) {
            return false; // Ressource déjà présente
        }
        current = current->next;
    }
    return true; // Ressource unique
}

// Créer une ressource
resource creation(listeResource *li) {
    resource r;
    do {
        printf("Donner l'id: ");
        scanf("%d", &r.id);
    } while (!isUniqueRessource(li, r));

    do {
        printf("Donner le matricule: ");
        scanf("%d", &r.matricule_ambulance);
    } while (!isUniqueRessource(li, r));

    printf("Donner le nom du personnel: ");
    scanf("%s", r.nom_personnel);
    printf("Donner la localisation: ");
    scanf("%s", r.localisation);
    strcpy(r.disponibilite, "indisponible");
    FILE *fic=fopen("ressource.txt", "a");
    fprintf(fic, "%d %s %d %s %s\n", r.id, r.nom_personnel, r.matricule_ambulance, r.disponibilite, r.localisation);
    fclose(fic);
    return r;
}

listeResource* createNode(int id, char* nom_personnel, int matricule_ambulance, char* disponibilite, char* localisation) {
    listeResource *newNode =malloc(sizeof(listeResource));
    newNode->value.id = id;
    strcpy(newNode->value.nom_personnel,nom_personnel);
    newNode->value.matricule_ambulance =matricule_ambulance;
    strcpy(newNode->value.disponibilite, disponibilite);
    strcpy(newNode->value.localisation, localisation);
    newNode->next = NULL;
    return newNode;
}
//Insère un nouveau nœud à la fin de la liste chaînée des ressources
listeResource* insertAtEnd(listeResource *head, int id, char* nom, int matricule, char* disponibilite, char* localisation) {
    listeResource *newNode = createNode(id, nom, matricule, disponibilite, localisation);
    if (head == NULL) {
        head = newNode;
    } else {
        listeResource *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    return head;
}
//Restaure les ressources
listeResource* restaurer(){
    FILE *file = fopen("ressource.txt", "r");
    if (file == NULL) {
        printf("il n ya pas de ressources disponible pour le moment\n");
        return ;
    }

    listeResource *head = NULL;
    int id, matricule;
    char nom[50], disponibilite[20], localisation[50];

    while (fscanf(file, "%d %49s %d %19s %49s", &id, nom, &matricule, disponibilite, localisation) != EOF) {
        head=insertAtEnd(head, id, nom, matricule, disponibilite, localisation);
    }
    fclose(file);
    return head;
}
// Ajouter une ressource
listeResource* ajoutResource(listeResource *li, resource r) {
    listeResource *element = malloc(sizeof(listeResource));
    if (element == NULL) {
        fprintf(stderr, "Erreur: probleme allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }
    element->value = r;
    element->next = NULL;
    if (li == NULL) {
        return element;
    }
    listeResource *temp = li;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = element;
    return li;
}

// Afficher les ressources
void afficher(listeResource *li) {
    listeResource *temp = li;
    while (temp != NULL) {
        printf("Ressource: %d %s %d %s %s\n", temp->value.id, temp->value.nom_personnel, temp->value.matricule_ambulance, temp->value.disponibilite, temp->value.localisation);
        temp = temp->next;
    }
}

// Vérifier la disponibilité d'une ressource dans un gouvernorat
int disponibilite(listeResource *li, char gouvernorat[50]) {
    listeResource *temp = li;
    while (temp != NULL) {
        if (strcmp(toupper(temp->value.localisation), toupper(gouvernorat)) == 0) {
            if (strcmp(toupper(temp->value.disponibilite), toupper("disponible")) == 0) {
                strcpy(temp->value.disponibilite, "indisponible");
                return 1;
            }
        }
        temp = temp->next;
    }
    return 0;
}

// Vérifier si une ressource est disponible dans un gouvernorat
int est_disponible(listeResource *li, char gouvernorat[50]) {
    listeResource *temp = li;
    while (temp != NULL) {
        if (strcmp(toupper(temp->value.localisation), toupper(gouvernorat)) == 0) {
            if (strcmp(toupper(temp->value.disponibilite), toupper("disponible")) == 0) {
                return 1;
            }
        }
        temp = temp->next;
    }
    return 0;
}

// Fonction pour comparer deux patients
bool comparePatients(patient a, patient b) {
    return a.id == b.id;
}

// Fonction pour vérifier l'unicité d'un patient dans la liste
bool isUniquePatient(listePatient *li, patient data) {
    listePatient *current = li;
    while (current != NULL) {
        if (comparePatients(current->value, data)) {
            return false; // Patient déjà présent
        }
        current = current->next;
    }
    return true; // Patient unique
}

// Créer un patient
patient creationPatient(listePatient *li) {
    patient p;
    do {
        printf("Donner l'id: ");
        scanf("%d", &p.id);
    } while (!isUniquePatient(li, p));

    printf("Donner le nom du patient: ");
    scanf("%s", p.nom_patient);
    printf("Donner le prenom du patient: ");
    scanf("%s", p.prenom_patient);
    printf("Donner le gouvernorat du patient: ");
    scanf("%s", p.gouvernorat);
    printf("Donner le numero de telephone du patient: ");
    scanf("%d", &p.num_telephone);
    return p;
}

// Ajouter un patient
listePatient* ajoutPatient(listePatient *li, patient p) {
    listePatient *element = malloc(sizeof(listePatient));
    if (element == NULL) {
        fprintf(stderr, "Erreur: probleme allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }
    element->value = p;
    element->next = NULL;
    if (li == NULL) {
        return element;
    }
    listePatient *temp = li;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = element;
    return li;
}

// Gérer la demande d'un patient
void demande(listeResource *li, patient p) {
    int test = disponibilite(li, p.gouvernorat);
    FILE *fic;
    if (test == 1) {
        fic = fopen("affecte.txt", "a");
    } else {
        fic = fopen("liste_dattente.txt", "a");
    }
    fprintf(fic, "%d %s %s %s %d\n", p.id, p.nom_patient, p.prenom_patient, p.gouvernorat, p.num_telephone);
    fclose(fic);
}

// Afficher une liste de patients
void afficher_list(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("\033[31mla liste est vide\033[0m\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fichier)) {
        printf("%s", buffer);
    }

    fclose(fichier);
}

// Gérer les ressources
void gestion(listeResource *li) {
    listeResource *temp = li;
    int dispo = 0, indispo = 0;
    while (temp != NULL) {
        if (strcmp(toupper(temp->value.disponibilite), toupper("disponible")) == 0) {
            dispo++;
        } else {
            indispo++;
        }
        temp = temp->next;
    }
    printf("\033[32mLe nombre de ressources disponibles est: %d\033[0m\n", dispo);
    printf("\033[31mLe nombre de ressources indisponibles est: %d\033[0m\n", indispo);
}

// Supprimer les lignes contenant un mot spécifique
void supprimerLignesAvecMot(const char *nomFichier, const char *mot) {
    FILE *fichierSource, *fichierTemporaire;
    char buffer[256];

    fichierSource = fopen(nomFichier, "r");
    if (fichierSource == NULL) {
        printf("la liste est vide");
        return;
    }

    fichierTemporaire = fopen("temp.txt", "w");
    if (fichierTemporaire == NULL) {

        fclose(fichierSource);
        return;
    }

    while (fgets(buffer, sizeof(buffer), fichierSource)) {
        if (strstr(buffer, mot) == NULL) {
            fputs(buffer, fichierTemporaire);
        } else {
            FILE *fichier = fopen("affecte.txt", "a");
            fputs(buffer, fichier);
            fclose(fichier);
        }
    }

    fclose(fichierSource);
    fclose(fichierTemporaire);

    if (remove(nomFichier) != 0) {
        printf("Erreur lors de la suppression du fichier source");
        return;
    }

    if (rename("temp.txt", nomFichier) != 0) {
        printf("Erreur lors du renommage du fichier temporaire");
    }
}

// Mettre à jour les ressources
void mettre_a_jour(listeResource *li, const char *nomFichier) {
    listeResource *temp = li;
    while (temp != NULL) {
        if (strcmp(toupper(temp->value.disponibilite), toupper("disponible")) != 0) {
            supprimerLignesAvecMot(nomFichier, temp->value.localisation);
            strcpy(temp->value.disponibilite, "disponible");
        }
        temp = temp->next;
    }
}

// Vérifier l'état d'un patient
void verifierEtat(int id) {
    FILE *fic = fopen("affecte.txt", "r");
    char buffer[256];
    char chaine[50];
    char premierMot[50];
    sprintf(chaine, "%d", id);

    while (fgets(buffer, sizeof(buffer), fic)) {
        strcpy(premierMot, strtok(buffer, " "));
        if (strcmp(premierMot, chaine) == 0) {
            printf("\033[32mVous etes deja affecte\033[0m\n");
            fclose(fic);
            return;
        }
    }

    fclose(fic);

    fic = fopen("liste_dattente.txt", "r");
    while (fgets(buffer, sizeof(buffer), fic)) {
        strcpy(premierMot, strtok(buffer, " "));
        if (strcmp(premierMot, chaine) == 0) {
            printf("\033[33mVous etes dans la liste d'attente\033[0m\n");
            fclose(fic);
            return;
        }
    }

    fclose(fic);
    printf("\033[31mVous n'etes pas inscrit\033[0m\n");
}

// Modifier une ressource
void modifierResource(listeResource *li, int id) {
    listeResource *current = li;
    while (current != NULL) {
        if (current->value.id == id) {
            printf("Donner le nouveau nom du personnel: ");
            scanf("%s", current->value.nom_personnel);
            printf("Donner la nouvelle localisation: ");
            scanf("%s", current->value.localisation);
            printf("\033[32mRessource modifiee avec succes.\033[0m\n");
            return;
        }
        current = current->next;
    }
    printf("\033[31mRessource avec l'ID %d non trouvee.\033[0m\n", id);
}

// Menu principal
void menu() {
    printf("\033[1;32m ======================================\033[0m\n");
    printf("\033[1;32m|          Menu Principal              |\033[0m\n");
    printf("\033[1;32m ======================================\033[0m\n");
    printf("\033[1;32m| \033[1;31m1. EMPLOYEE                          |\033[0m\033[0m\n");
    printf("\033[1;32m|\033[1;34m 2. PATIENT                           |\033[0m\n");
    printf("\033[1;32m| 3. Quitter                           |\033[0m\n");
    printf("\033[1;32m ======================================\033[0m\n");
    printf("Choisissez une option: ");
}

// Gérer le personnel
listeResource* gererPersonnel(listeResource *li) {
    int choix;
    do {
        printf("\n\033[1;31m ===================================================================================\033[0m\n");
        printf("\033[1;31m|                               Menu Personnel                                      |\033[0m\n");
        printf("\033[1;31m ===================================================================================\033[0m\n");
        printf("\033[1;31m|               1. restorer les ressources                                          |\033[0m\n");
        printf("\033[1;31m|               2. Ajouter une ressource                                            |\033[0m\n");
        printf("\033[1;31m|               3. Afficher les ressources                                          |\033[0m\n");
        printf("\033[1;31m|               4. Determiner le nombre de ressources  disponibles et indisponibles |\033[0m\n");
        printf("\033[1;31m|               5. Afficher la liste des patients                                   |\033[0m\n");
        printf("\033[1;31m|               6. Determiner la disponibilite d'une ressource                      |\033[0m\n");
        printf("\033[1;31m|               7. Modifier une ressource                                           |\033[0m\n");
        printf("\033[1;31m|               8. Mettre a jour                                                    |\033[0m\n");
        printf("\033[1;31m|               9. Retour au menu principal                                         |\033[0m\n");
        printf("\033[1;31m ===================================================================================\033[0m\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                li=restaurer();
                break;
            case 2: {
                resource r = creation(li);
                li = ajoutResource(li, r);
                break;
            }
            case 3:
                afficher(li);
                break;
            case 4:
                gestion(li);
                break;
            case 5: {
                int a;
                do {
                    printf("1. Afficher la liste des patients affectes\n");
                    printf("2. Afficher la liste des patients en attente\n");
                    printf("3. Retourner\n");
                    printf("Choisissez une option: ");
                    scanf("%d", &a);
                    switch (a) {
                        case 1:
                            afficher_list("affecte.txt");
                            break;
                        case 2:
                            afficher_list("liste_dattente.txt");
                            break;
                        case 3:
                            break;
                        default:
                            printf("Option invalide. Veuillez reessayer.\n");
                            break;
                    }
                } while (a != 3);
                break;
            }
            case 6: {
                char gouvernorat[50];
                printf("Donner le nom d'un gouvernorat: ");
                scanf("%s", gouvernorat);
                int t = est_disponible(li, gouvernorat);
                if (t == 1) {
                    printf("Il y a une ressource disponible a %s\n", gouvernorat);
                } else {
                    printf("Il n'y a pas de ressources disponibles a %s\n", gouvernorat);
                }
                break;
            }
            case 7: {
                int id;
                printf("Donner l'id de la ressource a modifier: ");
                scanf("%d", &id);
                modifierResource(li, id);
                break;
            }
            case 8:
                mettre_a_jour(li, "liste_dattente.txt");
                FILE *fichier = fopen("liste_dattente.txt", "r");
                int c = fgetc(fichier);
                if (c != EOF) {
                    printf("Il y a certains gouvernorats qui n'ont pas de ressources.\n");
                }
                fclose(fichier);
                break;
            case 9:
                break;
            default:
                printf("Option invalide. Veuillez reessayer.\n");
        }
    } while (choix != 9);
return li;
}

// Gérer les patients
listePatient* gererPatient(listeResource *li, listePatient *pi) {
    int choix;
    do {
        printf("\n\033[1;34m ========================================\033[0m\n");
        printf("\033[1;34m|          Menu Patient                  |\033[0m\n");
        printf("\033[1;34m ========================================\033[0m\n");
        printf("\033[1;34m| 1. Demander une ressource              |\033[0m\n");
        printf("\033[1;34m| 2. Verifier l'etat                     |\033[0m\n");
        printf("\033[1;34m| 3. Retour au menu principal            |\033[0m\n");
        printf("\033[1;34m ========================================\033[0m\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                patient p = creationPatient(pi);
                pi = ajoutPatient(pi, p);
                demande(li, p);
                printf("\033[32mVotre demande est enregistree.\033[0m\n");
                break;
            }
            case 2: {
                int id;
                printf("Donner votre id: ");
                scanf("%d", &id);
                verifierEtat(id);
                break;
            }
            case 3:
                break;
            default:
                printf("Option invalide. Veuillez reessayer.\n");
        }
    } while (choix != 3);
return pi;
}

int main() {
    listeResource *li = NULL;
    listePatient *pi = NULL;
    int choix;
    do {
        menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                 li=gererPersonnel(li);
                break;
            case 2:
                 pi=gererPatient(li, pi);
                break;
            case 3:
                printf("Au revoir!\n");
                break;
            default:
                printf("Option invalide. Veuillez reessayer.\n");
        }
    } while (choix != 3);

    return 0;
}
