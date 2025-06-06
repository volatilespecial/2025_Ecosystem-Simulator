#ifndef ANIMAUX_HPP
#define ANIMAUX_HPP

#define MOUTON 'M'
#define LOUP 'L'
#define MALE 'A'
#define FEMELLE 'F'


class Animal {
    protected:
        char espece; // Loup ou mouton
        int posX, posY; // Coordonnées de la case
        int age; // Age actuel de l'animal
        char sexe; // Male ou femelle
        int faim; // Nombre de tours passés sans avoir mangé
        char nourriture; // Mouton ou herbe
        int maxAge; // Age maximum de l'animal
        int maxFaim; // Nombre maximum de tours sans manger
        bool reproduction; // Si en reproduction (immobile)
        int partX, partY; // Coordonnées du partenaire si reproduction
        int attenteReproduction; // Cooldown pour qu'un animal puisse se reproduire à nouveau
        int maxAttenteReproduction; // Max cooldown pour la reproduction
        int attenteNaissance; // Cooldown pour qu'on animal puisse donner naissance
        int maxAttenteNaissance; // Max cooldown pour la naissance
        int attenteNourrir; // Cooldown pour qu'un animal puisse se nourrir à nouveau
        int maxAttenteNourrir; // Max cooldown pour la nourriture
        int prevX, prevY; // Position précédente pour éviter le backtracking

    public:
        void naissance(int nAge, char nSexe, int nFaim, int iPosX, int iPosY, char nNourriture, int nMaxAge, int nMaxFaim, int nMaxRepr, char nEspece);
        void seDeplacer(int nPosX, int nPosY);
        void plusAge();
        void plusFaim();
        void moinsAttenteReproduction();
        void moinsAttenteNaissance();
        void moinsAttenteNourrir();
        void seReproduire(int px, int py);
        void finReproduction();
        void seNourrir();
        int estMort();
        int getPosX();
        int getPosY();
        char getEspece();
        bool getReproduction();
        char getSexe();
        int getPartX();
        int getPartY();
        int getFaim();
        int getAttenteReproduction();
        int getAttenteNaissance();
        int getAttenteNourrir();
        bool backTrack(int newX, int newY);
};

class Mouton : public Animal{
    public:
        Mouton(int nPosX, int nPosY, char nSexe);
};

class Loup : public Animal {
    public:
        Loup(int nPosX, int nPosY, char nSexe);
};

#endif