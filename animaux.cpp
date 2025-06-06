#include "animaux.hpp"
#include "univers.hpp"


void Animal::naissance(int nAge, char nSexe, int nFaim, int iPosX, int iPosY, char nNourriture, int nMaxAge, int nMaxFaim, int nMaxRepr, char nEspece){
    espece = nEspece;
    age = nAge;
    sexe = nSexe;
    faim = nFaim;
    posX = iPosX;
    posY = iPosY;
    nourriture = nNourriture;
    maxAge = nMaxAge;
    maxFaim = nMaxFaim;
    reproduction = false;
    maxAttenteReproduction = nMaxRepr;
    attenteReproduction = 0;
    maxAttenteNaissance = 2;
    attenteNaissance = maxAttenteNaissance;
    maxAttenteNourrir = 2;
    attenteNourrir = maxAttenteNourrir;
    prevX = iPosX;
    prevY = iPosY;
}

void Animal::seDeplacer(int nPosX, int nPosY){
    prevX = this->posX;
    prevY = this->posY;
    this->posX = nPosX;
    this->posY = nPosY;
}

void Animal::plusAge(){
    age++;
}
void Animal::plusFaim(){
    faim++;
}
void Animal::moinsAttenteReproduction(){
    if(attenteReproduction > 0) --attenteReproduction;
}
void Animal::moinsAttenteNaissance(){
    if(attenteNaissance > 0) --attenteNaissance;
}
void Animal::moinsAttenteNourrir(){
    if(attenteNourrir > 0) --attenteNourrir;
}
void Animal::seReproduire(int px, int py){
    partX = px;
    partY = py;
    attenteNaissance = maxAttenteNaissance;
    reproduction = true;
}
void Animal::finReproduction(){
    partX = -1;
    partY = -1;
    reproduction = false;
    attenteReproduction = maxAttenteReproduction;
    attenteNaissance = maxAttenteNaissance;
}
void Animal::seNourrir(){
    faim = 0;
    attenteNourrir = maxAttenteNourrir;
}
int Animal::estMort(){
    if(faim > maxFaim) return 1;
    if(age > maxAge) return 2;
    return 0;
}
int Animal::getPosX(){
    return posX;
}
int Animal::getPosY(){
    return posY;
}
char Animal::getEspece(){
    return this->espece;
}
bool Animal::getReproduction(){
    return reproduction;        
}
char Animal::getSexe(){
    return sexe;
}
int Animal::getPartX(){
    return partX;
}
int Animal::getPartY(){
    return partY;
}
int Animal::getFaim(){
    return faim;
}
int Animal::getAttenteReproduction(){
    return attenteReproduction;
}
int Animal::getAttenteNaissance(){
    return attenteNaissance;
}
int Animal::getAttenteNourrir(){
    return attenteNourrir;
}
bool Animal::backTrack(int newX, int newY){
    if(newX == prevX && newY == prevY) return true;
    return false;
}


Mouton::Mouton(int nPosX, int nPosY, char nSexe){
    Animal::naissance(0, nSexe, 0, nPosX, nPosY, HERBE, 50, 5, 4, MOUTON);
}

Loup::Loup(int nPosX, int nPosY, char nSexe){
    Animal::naissance(0, nSexe, 0, nPosX, nPosY, MOUTON, 60, 10, 6, LOUP);
}