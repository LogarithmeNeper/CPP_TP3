/*************************************************************************
Main  -  Classe principale servant d'interface entre l'utilisateur et le
système de gestion du catalogue
-------------------
début                : 20/11/2019
copyright            : (C) 2019 par Charles Javerliat
e-mail               : charles.javerliat@insa-lyon.fr, pierre.sibut-bourde@insa-lyon.fr
*************************************************************************/

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système

#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

//------------------------------------------------------ Include personnel
#include "Catalogue.h"
#include "TrajetSimple.h"
#include "TrajetCompose.h"

// Contrat : Affiche le catalogue dans le terminal
static void afficherCatalogue(const Catalogue & catalogue)
{
  cout << endl << " ======  CATALOGUE ====== " << endl << endl;
  catalogue.afficher(cout);
  cout << endl << endl << " === FIN DU CATALOGUE === " << endl << endl;
}

// Contrat : Ajoute un trajet simple au Catalogue en demandant à l'utilisateur
// de renseigner les informations du trajet.
static void ajouterTrajetSimple(Catalogue & catalogue)
{
  cout << endl << " ===  AJOUT D'UN TRAJET SIMPLE === " << endl << endl;
  TrajetSimple* trajetSimple = nullptr;

  do
  {
    char villeDepart[100];
    char villeArrivee[100];
    char typeTransport[100];

    //Prompt de la ville de départ
    do {
      villeDepart[0] = '\0';
      cout << "Ville de départ: ";
      cin.clear();
      cin.getline(villeDepart, sizeof(villeDepart));
    } while(cin.fail());

    //Prompt de la ville d'arrivée
    do {
      villeArrivee[0] = '\0';
      cout << "Ville d'arrivée: ";
      cin.clear();
      cin.getline(villeArrivee, sizeof(villeArrivee));
    } while(cin.fail());

    //Prompt du type de transport
    do {
      typeTransport[0] = '\0';
      cout << "Type de transport: ";
      cin.clear();
      cin.getline(typeTransport, sizeof(typeTransport));
    } while(cin.fail());

    trajetSimple = new TrajetSimple(villeDepart, villeArrivee, typeTransport);

    //On essaie d'ajouter le trajet, si il n'est pas valide on recommence
  } while(!catalogue.ajouter(trajetSimple));

  cout << endl << " ======== FIN DE L'AJOUT ======== " << endl << endl;
}

// Contrat : Ajoute un trajet composé au Catalogue en demandant à l'utilisateur
// de renseigner les informations du trajet, aka les différents trajets simples
// qui le constitue.
static void ajouterTrajetCompose(Catalogue & catalogue)
{
  cout << endl << " ===  AJOUT D'UN TRAJET COMPOSE === " << endl;
  TrajetCompose* trajetCompose = nullptr;

  //Variable contenant la valeur de retour (o/n) à la question de si
  //on souhaite rajouter un autre trajet simple
  char ajouterTrajetSimple;

  do
  {
    delete trajetCompose;
    trajetCompose = new TrajetCompose();

    do
    {
      TrajetSimple* trajetSimple = nullptr;

      //-------------------- Ajout d'un sous-trajet simple --------------------
      do
      {
        char villeDepart[100];
        char villeArrivee[100];
        char typeTransport[100];
        cout << endl << ">>> Ajout d'un trajet simple <<<" << endl;

        //Prompt de la ville de départ
        do {
          villeDepart[0] = '\0';
          cout << "Ville de départ: ";
          cin.clear();
          cin.getline(villeDepart, sizeof(villeDepart));
        } while(cin.fail());

        //Prompt de la ville d'arrivée
        do {
          villeArrivee[0] = '\0';
          cout << "Ville d'arrivée: ";
          cin.clear();
          cin.getline(villeArrivee, sizeof(villeArrivee));
        } while(cin.fail());

        //Prompt du type de transport
        do {
          typeTransport[0] = '\0';
          cout << "Type de transport: ";
          cin.clear();
          cin.getline(typeTransport, sizeof(typeTransport));
        } while(cin.fail());

        trajetSimple = new TrajetSimple(villeDepart, villeArrivee, typeTransport);

        //On essaie d'ajouter le trajet, si il n'est pas valide on recommence
      } while(!trajetCompose->ajouter(trajetSimple));
      //------------------------------------------------------------------------

      do
      {
        cout << endl << "Ajouter un autre trajet simple (o/n) ? ";
        cin.clear();
        cin >> ajouterTrajetSimple;
        cin.ignore(10000, '\n');

        if(cin.fail() || (ajouterTrajetSimple != 'o' && ajouterTrajetSimple != 'n'))
        {
          cout << "Veuillez écrire o ou n." << endl;
        }

        //Tant que l'utilisateur ne répond pas 'o' ou 'n', on recommence
      } while(cin.fail() || (ajouterTrajetSimple != 'o' && ajouterTrajetSimple != 'n'));

    } while(ajouterTrajetSimple == 'o');

    //On essaie d'ajouter le trajet, si il n'est pas valide on recommence
  } while(!catalogue.ajouter(trajetCompose));

  cout << " ======== FIN DE L'AJOUT ======== " << endl << endl;
}

// Contrat : Supprime un trajet du Catalogue en demandant à l'utilisateur
// de renseigner le numéro du trajet à supprimer.
static void supprimerTrajet(Catalogue & catalogue)
{
  cout << endl << " ===  SUPPRESSION D'UN TRAJET === " << endl;

  if(catalogue.estVide())
  {
    cout << endl << "Aucun trajet à supprimer." << endl;
  }
  else
  {
    unsigned int numeroTrajet;

    do
    {
      cout << endl << "Entrez le numéro de trajet à supprimer (entre 1 et " << catalogue.getTaille() << "): ";
      cin.clear();
      cin >> numeroTrajet;

      if(cin.fail() || numeroTrajet < 1 || numeroTrajet > catalogue.getTaille())
      {
        cout << "Numéro de trajet invalide." << endl;
        cin.clear();
      }
    } while(numeroTrajet < 1 || numeroTrajet > catalogue.getTaille());

    cin.ignore(10000, '\n');

    catalogue.supprimer(catalogue.get(numeroTrajet - 1)->getTrajet());
  }
  cout << endl << " ====== FIN DE LA SUPPRESSION ====== " << endl << endl;
}

// Contrat : Liste tous les trajets allant d'une ville à l'autre sans tester
// de combinaisons possibles de trajets. Demande à l'utilisateur d'entrée la
// ville de départ et d'arrivée.
static void rechercheTrajetSimple(Catalogue & catalogue)
{
  cout << endl << " =====  RECHERCHE DE TRAJET SIMPLE ===== " << endl << endl;
  char villeDepart[100];
  char villeArrivee[100];

  //Prompt de la ville de départ
  do
  {
    villeDepart[0] = '\0';
    cout << "Ville de départ: ";
    cin.clear();
    cin.getline(villeDepart, 100);

    if(cin.fail())
    {
      cout << "Entrée invalide." << endl;
    }
  } while(cin.fail());

  //Prompt de la ville d'arrivée
  do
  {
    villeArrivee[0] = '\0';
    cout << "Ville d'arrivée: ";
    cin.clear();
    cin.getline(villeArrivee, 100);

    if(cin.fail())
    {
      cout << "Entrée invalide." << endl;
    }

  } while(cin.fail());

  bool found = catalogue.rechercheTrajetSimple(villeDepart, villeArrivee);

  if(!found)
  {
    cout << endl << "Aucun trajet trouvé entre " << villeDepart << " et " << villeArrivee;
  }

  cout << endl << endl << " ==  FIN DE RECHERCHE DE TRAJET SIMPLE == " << endl << endl;
}

// Contrat : Liste tous les trajets allant d'une ville à l'autre en testant
// les combinaisons possibles de trajets. Demande à l'utilisateur d'entrée la
// ville de départ et d'arrivée.
static void rechercheTrajetAvancee(Catalogue & catalogue)
{
  cout << endl << " =====  RECHERCHE DE TRAJET AVANCEE ===== " << endl << endl;
  char villeDepart[100];
  char villeArrivee[100];

  //Prompt de la ville de départ
  do
  {
    villeDepart[0] = '\0';
    cout << "Ville de départ: ";
    cin.clear();
    cin.getline(villeDepart, 100);

    if(cin.fail())
    {
      cout << "Entrée invalide." << endl;
    }
  } while(cin.fail());

  //Prompt de la ville d'arrivée
  do
  {
    villeArrivee[0] = '\0';
    cout << "Ville d'arrivée: ";
    cin.clear();
    cin.getline(villeArrivee, 100);

    if(cin.fail())
    {
      cout << "Entrée invalide." << endl;
    }
  } while(cin.fail());

  bool found = catalogue.rechercheTrajetAvancee(villeDepart, villeArrivee);

  if(!found)
  {
    cout << endl << "Aucun trajet trouvé entre " << villeDepart << " et " << villeArrivee;
  }

  cout << endl << endl << " ==  FIN DE RECHERCHE DE TRAJET AVANCEE == " << endl << endl;
}

static void chargement(Catalogue & catalogue)
{
    cout << endl << " ======= CHARGEMENT =======" << endl << endl;

    int trajetsCharges = 0;

    string nomFichier;
    cout << "Veuillez entrer un nom de fichier: ";
    cin >> nomFichier;

    ifstream fichier("data/"+nomFichier, ios::in);

    if (fichier.is_open())
    {
        string line;
        string villeDep;
        string villeArr;
        string moyTransport;

        bool estTrajetCompose=false;
        TrajetCompose* trajcmp;

        while(!fichier.eof())
        {
            getline(fichier, line, '\n');
            if(line == "")
            {
                if (!estTrajetCompose)
                {
                    trajcmp = new TrajetCompose();
                } else {
                    catalogue.ajouter(trajcmp);
                    ++trajetsCharges;
                }
                estTrajetCompose=!estTrajetCompose;
            }
            else
            {
                if(!estTrajetCompose)
                {
                    villeDep=line;
                    getline(fichier, villeArr, '\n');
                    getline(fichier, moyTransport, '\n');
                    TrajetSimple* trajet = new TrajetSimple(villeDep.c_str(),villeArr.c_str(),moyTransport.c_str());
                    catalogue.ajouter(trajet);
                    ++trajetsCharges;
                }
                else
                {
                    villeDep=line;
                    getline(fichier, villeArr, '\n');
                    getline(fichier, moyTransport, '\n');
                    TrajetSimple* trajet = new TrajetSimple(villeDep.c_str(),villeArr.c_str(),moyTransport.c_str());
                    trajcmp->ajouter(trajet);
                }
                
            }
        }
    }
    else
    {
        cout << "Impossible d'ouvrir le fichier." << endl;
    }
    
    fichier.close();

    cout << trajetsCharges << " trajet(s) chargé(s)." << endl;
    cout << endl << " ===== FIN CHARGEMENT =====" << endl << endl;
}

static void sauvegarde(const Catalogue & catalogue, const unsigned int* indices, const unsigned int & taille)
{
    if (catalogue.estVide())
    {
        cout << "Le catalogue est vide, rien à faire." << endl;
    }
    else if (taille == 0)
    {
        cout << "Il n'y a aucun trajet à sauvegarder." << endl;
    }
    else
    {
        string nomFichier;
        cout << "Veuillez entrer un nom de fichier: ";
        cin >> nomFichier;

        ofstream fichier("data/"+nomFichier, ios::out);

        if (fichier.is_open())
        {
            cout << "Taille: " << taille << endl;
            for (unsigned int i=0; i<taille; ++i)
            {
                MaillonListeChaineeTrajets* maillonAct = catalogue.get(indices[i]);
                Trajet* trajet = maillonAct->getTrajet();
                trajet->sauvegarde(fichier);
            }
            fichier.close();
            cout << "Sauvegarde terminée (data/" << nomFichier << ")." << endl;
        }
        else cout << "Impossible d'ouvrir le fichier." << endl;
    }

    cout << endl << " ===== FIN SAUVEGARDE =====" << endl << endl;
}

static void sauvegardeMenu(const Catalogue & catalogue) {
    
    unsigned short choix;

    char typeTrajet;
    string villeDepart, villeArrivee;
    unsigned int indiceDebut, indiceFin;

    unsigned int* indices = new unsigned int[100];
    unsigned int taille = 0;

    cout << endl << " ======= SAUVEGARDE =======" << endl << endl;
    cout << "Comment désirez-vous effectuer la sauvegarde ?" << endl;
    cout << "\t1 - Sans critère de sélection" << endl;
    cout << "\t2 - Selon le type de trajet" << endl;
    cout << "\t3 - Selon la ville de départ et/ou d'arrivée" << endl;
    cout << "\t4 - Selon une sélection de trajets" << endl;
    cout << "\t5 - Annuler" << endl;

    do {
        cout << "Entrez votre choix: ";
        cin >> choix;
        if(cin.fail() || choix < 1 || choix > 5) {
            cout << "Choix invalide." << endl;
            cin.clear();
        }
        cin.ignore(10000, '\n');
    } while(choix < 1 || choix > 5);

    switch(choix) {
        case 1:
            {
                for (unsigned int i=0; i<catalogue.getTaille(); ++i)
                {
                    indices[taille++] = i;
                }
                sauvegarde(catalogue, indices, taille);
                break;
            }
        case 2:
            {
                do
                {
                    cout << "Sauvegarder les trajets simples (S) ou composés (C): ";
                    cin >> typeTrajet;
                } while (typeTrajet != 'C' && typeTrajet != 'S');

                unsigned int i = 0;
                MaillonListeChaineeTrajets* maillonAct = catalogue.getPremierMaillon();
                while(maillonAct != nullptr)
                {
                    if (maillonAct->getTrajet()->getTypeTrajet() == typeTrajet)
                    {
                        indices[taille++] = i;
                    }
                    ++i;
                    maillonAct = maillonAct->getMaillonSuivant();
                }
                sauvegarde(catalogue, indices, taille);
                break;
            }
        case 3:
            {
                // Le sujet implique que l'utilisateur doit pouvoir
                // choisir la ville de départ ET/OU la ville d'arrivée.
                // Ici, les deux champs sont obligatoires.
                cout << "Ville de départ: ";
                cin >> villeDepart;
                cout << "Ville d'arrivée: ";
                cin >> villeArrivee;

                unsigned int i = 0;
                MaillonListeChaineeTrajets* maillonAct = catalogue.getPremierMaillon();
                while(maillonAct != nullptr)
                {
                    if (maillonAct->getTrajet()->getVilleDepart() == villeDepart
                     && maillonAct->getTrajet()->getVilleArrivee() == villeArrivee)
                    {
                        indices[taille++] = i;
                    }
                    ++i;
                    maillonAct = maillonAct->getMaillonSuivant();
                }
                sauvegarde(catalogue, indices, taille);
                break;
            }
        case 4:
            {
                do
                {
                    cout << "Indice de début: ";
                    cin >> indiceDebut;
                    cout << "Indice de fin: ";
                    cin >> indiceFin;
                } while ((indiceDebut > indiceFin) || (indiceFin > catalogue.getTaille()-1));
                for (unsigned int i=indiceDebut; i<=indiceFin; ++i)
                {
                    indices[taille++] = i;
                }
                sauvegarde(catalogue, indices, taille);
                break;
            }
        default: break;
    }

    delete[] indices;
}

int main ()
{
  //Instance unique du Catalogue sur la pile
  Catalogue catalogue;

  unsigned short choix = 0;

  cout << " === PROGRAMME DE GESTION DE CATALOGUE === " << endl;

  do
  {
    cout << "Que désirez-vous faire ?" << endl;
    cout << "\t1 - Afficher le catalogue" << endl;
    cout << "\t2 - Ajouter un trajet simple" << endl;
    cout << "\t3 - Ajouter un trajet composé" << endl;
    cout << "\t4 - Supprimer un trajet" << endl;
    cout << "\t5 - Recherche de trajet simple" << endl;
    cout << "\t6 - Recherche de trajet avancée" << endl;
    cout << "\t7 - Chargement d'un catalogue" << endl;
    cout << "\t8 - Sauvegarde d'un catalogue" << endl;
    cout << "\t9 - Quitter" << endl;

    //Prompt de l'action à effectuer sur la Catalogue
    do {
      cout << "Entrez votre choix: ";
      cin >> choix;

      if(cin.fail() || choix < 1 || choix > 9) {
        cout << "Choix invalide." << endl;
        cin.clear();
      }

      cin.ignore(10000, '\n');

    } while(choix < 1 || choix > 9);

    switch(choix) {

      case 1:
      afficherCatalogue(catalogue);
      break;
      case 2:
      ajouterTrajetSimple(catalogue);
      break;
      case 3:
      ajouterTrajetCompose(catalogue);
      break;
      case 4:
      supprimerTrajet(catalogue);
      break;
      case 5:
      rechercheTrajetSimple(catalogue);
      break;
      case 6:
      rechercheTrajetAvancee(catalogue);
      break;
      case 7:
      chargement(catalogue);
      break;
      case 8:
      sauvegardeMenu(catalogue);
      break;
      default:
      break;
    }

  } while(choix != 9);

  cout << endl << " === FERMETURE DU PROGRAMME === " << endl;

  return 0;
}
