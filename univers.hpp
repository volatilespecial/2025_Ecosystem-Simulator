#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include "animaux.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

#define HERBE 'H'
#define MINERAUX 'S'
#define VIDE ' '



class Case {
    bool estHerbe; // Si la case contient de l'herbe
    bool estMineral; // Si la case contient des minéraux
    Animal *occupant; // L'animal qui occupe la case, ou sinon nullptr si aucun animal
    int attentePousseHerbe; // Cooldown avant pousse herbe sur une case
    public:
        Case();
        bool aHerbe() const;
        void retirerHerbe();
        bool pousserHerbe();
        bool aMineraux() const;
        void deposerMineraux();
        bool estLibre();
        void ajouterAnimal(Animal *a);
        void retirerAnimal();
        Animal* getOccupant() const;
        void miseAJour();
        void moinsAttentePousseHerbe();
        int getAttentePousseHerbe();
};

class Univers {
    int m, n; //dimensions
    std::vector<std::vector<Case>> grille; //grille de cases
    std::vector<Animal*> animaux; //liste dynamique d'animaux
    int tour; //tour en cours
    int compteur; //compteur pour ajouter des animaux
    int nbMoutons, nbLoups; //nombre de moutons et de loups
    std::vector<std::string> Messages; // Les messages à afficher
    public:
        Univers(int largeur, int hauteur, int nbM, int nbL);
        void initialiser(int nbM, int nbL);
        void simulerTour();
        bool positionValide(int x, int y);
        bool estMort();
        bool estRempli();
        void miseAJourNombreAnimaux();
        char getCaseChar(int x, int y);
        void lignesSeparation();
        void afficher();
        ~Univers();

        private:
            void reduitePousseHerbe();
            void ajoutAnimauxAleatoire();
            void avancerVie();
            void deplacerEtNourrir();      
            void reproductionAnimaux();
            void naissanceAnimaux();
            void checkHerbe();
};

#endif