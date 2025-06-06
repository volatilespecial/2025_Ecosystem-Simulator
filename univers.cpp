#include "univers.hpp"
#include "animaux.hpp"

Case::Case() : estHerbe(true), estMineral(false), occupant(nullptr), attentePousseHerbe(2) {};
bool Case::aHerbe() const {
    return estHerbe;
}
void Case::retirerHerbe() {
    if(estHerbe) estHerbe = false;
}
bool Case::pousserHerbe() {
    if(aMineraux() && getAttentePousseHerbe() <= 0){
        estMineral = false;
        estHerbe = true;
        attentePousseHerbe = 2;
        return true;
    }else if(!estHerbe && (rand() % 100 < 10)){
        estHerbe = true;
        estMineral = false;
        attentePousseHerbe = 2;
        return true;
    }
    return false;
}
bool Case::aMineraux() const {
    return estMineral;
}
void Case::deposerMineraux() {
    estHerbe = false;
    estMineral = true;
}
bool Case::estLibre() {
    return occupant == nullptr;
}
void Case::ajouterAnimal(Animal *a){
    occupant = a;
}
void Case::retirerAnimal(){
    occupant = nullptr;
}
Animal* Case::getOccupant() const {
    return occupant;
}
void Case::miseAJour(){
    if(estMineral && !estHerbe) pousserHerbe();
}
void Case::moinsAttentePousseHerbe(){
    if(aMineraux() && attentePousseHerbe > 0)
        --attentePousseHerbe;
}
int Case::getAttentePousseHerbe(){
    return attentePousseHerbe;
}

Univers::Univers(int largeur, int hauteur, int nbM, int nbL){
    if(largeur < 5) {
       // largeur = 5;
    }
    if(hauteur < 5) {
       // hauteur = 5;
    }
    m = hauteur;
    n = largeur;
    tour = 1;
    nbMoutons = nbM;
    nbLoups = nbL;
    compteur = 1;

    grille.resize(m, std::vector<Case>(n));
    for(int r = 0; r<m; ++r){
        for(int c = 0; c<n; ++c){
            grille[r][c] = Case(); //c = x, r = y
        }
    }
    initialiser(nbMoutons, nbLoups);
}
void Univers::initialiser(int nbM, int nbL){
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distributionx(0, n-1);
    std::uniform_int_distribution<size_t> distributiony(0, m-1);
    int totalAnimaux = nbM + nbL;
    int maxCases = m * n;
    if(totalAnimaux > maxCases){
        std::cout << "Trop d'animaux pour l'univers" << std::endl;
        nbM /= 2;
        nbL /= 2;

        if(nbM < 2) nbM = 2 + (rand() % nbM);
        if(nbL < 2) nbL = 2 + (rand() % nbL);
        if(nbM >= maxCases) {
            nbM = 2 + (rand() % ((m > n) ? m : n));
        }
        if(nbL >= maxCases) {
            nbL = 2 + (rand() % ((m > n) ? m : n));
        }
    }
    
    nbMoutons = nbM;
    nbLoups = nbL;

    int femelleL = 0, femelleM = 0;
    char sexe;

    while(nbM > 0 || nbL > 0){
        int x = distributionx(generator);
        int y = distributiony(generator);

        if(grille[y][x].estLibre()) {
            if(nbM > 0){
                if(femelleM == 0){
                    sexe = FEMELLE;
                    femelleM = 1;
                }else{
                    sexe = MALE;
                    femelleM = 0;
                }
                Mouton *mouton = new Mouton(x, y, sexe);
                animaux.push_back(mouton);
                grille[y][x].ajouterAnimal(mouton);
                nbM--;
            }
            else if(nbL > 0){
                if(femelleL == 0){
                    sexe = FEMELLE;
                    femelleL = 1;
                }else{
                    sexe = MALE;
                    femelleL = 0;
                }
                Loup *loup = new Loup(x, y, sexe);
                animaux.push_back(loup);
                grille[y][x].ajouterAnimal(loup);
                nbL--;
            }
        }
    }
}
void Univers::simulerTour(){
    Messages.clear();
    reduitePousseHerbe();
    avancerVie();
 //   ajoutAnimauxAleatoire();
    reproductionAnimaux();
    deplacerEtNourrir(); 
    naissanceAnimaux();
    checkHerbe();
    
    tour++;
}

bool Univers::positionValide(int x, int y){
    if(x >= 0 && x < n && y >= 0 && y < m) return true;
    return false;
}
bool Univers::estMort(){
    return animaux.empty();
}

bool Univers::estRempli(){
    for(int y = 0; y<m; ++y){
        for(int x = 0; x<n; ++x){
            if(grille[y][x].estLibre()) return false;
        }
    }
    return true;
}

void Univers::miseAJourNombreAnimaux(){
    nbLoups = 0;
    nbMoutons = 0;
    for(Animal *a : animaux){
        if(a != nullptr){
            if(a->getEspece() == MOUTON) ++nbMoutons;
            else if(a->getEspece() == LOUP) ++nbLoups;
        }
    }
}

char Univers::getCaseChar(int x, int y){
    Animal *a = grille[y][x].getOccupant();
    if(a == nullptr){
        if(grille[y][x].aHerbe()) return ' ';
        //if(grille[y][x].aHerbe()) return HERBE;
        if(grille[y][x].aMineraux()) return MINERAUX;
        return VIDE;
    }
    if(a->getEspece() == MOUTON) return MOUTON;
    else return LOUP;
}

void Univers::lignesSeparation(){
    for(int colonne=0; colonne<n; ++colonne){
        if(colonne==0 && colonne==n-1){
            std::cout << "  +___+" << std::endl;
        }else if(colonne == n-1){
            std::cout << "+___+" << std::endl;
        }else if(colonne == 0){
            std::cout << "  +___";
        }else{
            std::cout << "+___";
        }
    }
}
void Univers::afficher(){
    for(int colonne=0; colonne<n; ++colonne){
        if(colonne==0){
            std::cout << "    " << colonne << "   ";
        }else if(colonne==n-1){
            std::cout << colonne << "  " << std::endl;
        }else{
            std::cout << colonne << "   ";
        }
    }
    for(int ligne=0; ligne<m; ++ligne){
        lignesSeparation();
        for(int colonne=0; colonne<n; ++colonne){
            if(colonne == 0 && colonne == n-1){
                std::cout << ligne << " | " << getCaseChar(colonne, ligne) <<  " |" << std::endl;
            }else if(colonne == 0){
                std::cout << ligne << " | " << getCaseChar(colonne, ligne) << " ";
            }else if(colonne == n - 1){
                std::cout << "| " << getCaseChar(colonne, ligne) << " |" << std::endl;
            }else{
                std::cout << "| " << getCaseChar(colonne, ligne) << " ";
            }
        }
    }
    lignesSeparation();
    std::cout << "Tour " << tour << " | Loups : " << nbLoups << " | Moutons : " << nbMoutons << std::endl;

    for(std::string message : Messages){
        std::cout << message << std::endl;
    }
}
Univers::~Univers(){
    for(Animal *a: animaux){
        free(a);
    }
}

void Univers::reduitePousseHerbe(){
    for(int ligne = 0; ligne < m; ++ligne){
        for(int colonne = 0; colonne < n; ++colonne){
            grille[ligne][colonne].moinsAttentePousseHerbe();
        }
    }
}

void Univers::ajoutAnimauxAleatoire(){
    
    if(compteur > 15){
        std::mt19937 generator(std::random_device{}());
        int moutonAjout, loupAjout;

        //moutonAjout = std::max(2, nbMoutons / 2);
        //loupAjout = std::max(2, nbLoups / 2);


        // std::uniform_int_distribution<int> distributionM(1, moutonAjout);
        // moutonAjout = distributionM(generator);

        // std::uniform_int_distribution<int> distributionL(1, loupAjout);
        // loupAjout = distributionL(generator);

        moutonAjout = 2;
        loupAjout = 2;

        //std::cout << loupAjout << ", " << moutonAjout << std::endl;

        std::uniform_int_distribution<size_t> distributionx(0, n-1);
        std::uniform_int_distribution<size_t> distributiony(0, m-1);
        int femelleL = 0, femelleM = 0;
        char sexe;

        while((moutonAjout > 0 && nbMoutons < 50) || loupAjout > 0){
            if(estRempli()) break;
            int x = distributionx(generator);
            int y = distributiony(generator);

            if(grille[y][x].estLibre()) {
                if(moutonAjout > 0 && nbMoutons < 50){
                    if(femelleM == 0){
                        sexe = FEMELLE;
                        femelleM = 1;
                    }else{
                        sexe = MALE;
                        femelleM = 0;
                    }
                    Mouton *mouton = new Mouton(x, y, sexe);
                    animaux.push_back(mouton);
                    grille[y][x].ajouterAnimal(mouton);
                    --moutonAjout;
                }
                else if(loupAjout > 0){
                    if(femelleL == 0){
                        sexe = FEMELLE;
                        femelleL = 1;
                    }else{
                        sexe = MALE;
                        femelleL = 0;
                    }
                    Loup *loup = new Loup(x, y, sexe);
                    animaux.push_back(loup);
                    grille[y][x].ajouterAnimal(loup);
                    --loupAjout;
                }
            }
        }
        compteur = 0;
    }else{
        ++compteur;
    }
}

void Univers::avancerVie(){
    int codeMort;
    
    std::vector<std::string> coordsMoutonFaim;
    std::vector<std::string> coordsMoutonVieillese;
    std::vector<std::string> coordsLoupFaim;
    std::vector<std::string> coordsLoupVieillese;


    for(std::vector<Animal*>::iterator it = animaux.begin(); it != animaux.end(); ){
        Animal* a = *it;
        a->plusAge();
        a->plusFaim();
        a->moinsAttenteReproduction();
        a->moinsAttenteNaissance();
        a->moinsAttenteNourrir();

        if((codeMort = a->estMort()) != 0){

            if(a->getEspece() == MOUTON){
                std::string coords = "(" + std::to_string(a->getPosX()) + "," + std::to_string(a->getPosY()) + ")";
                if(codeMort ==  1) coordsMoutonFaim.push_back(coords);
                else coordsMoutonVieillese.push_back(coords);
            }else{
                std::string coords = "(" + std::to_string(a->getPosX()) + "," + std::to_string(a->getPosY()) + ")";
                if(codeMort ==  1) coordsLoupFaim.push_back(coords);
                else coordsLoupVieillese.push_back(coords);
            }

            grille[a->getPosY()][a->getPosX()].ajouterAnimal(nullptr);
            grille[a->getPosY()][a->getPosX()].deposerMineraux();
            delete a;
            //it = animaux.erase(it);
            *it = nullptr;
            
        }
        ++it;
    }

    animaux.erase(std::remove(animaux.begin(), animaux.end(), nullptr), animaux.end());

    if(!coordsMoutonFaim.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsMoutonFaim.size(); ++i){
            message += coordsMoutonFaim[i] + " ";
            if(i!=coordsMoutonFaim.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs moutons est mort de faim.";
        Messages.push_back(message);
    }

    if(!coordsMoutonVieillese.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsMoutonVieillese.size(); ++i){
            message += coordsMoutonVieillese[i] + " ";
            if(i!=coordsMoutonVieillese.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs moutons est mort de vieillese.";
        Messages.push_back(message);
    }

    if(!coordsLoupFaim.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsLoupFaim.size(); ++i){
            message += coordsLoupFaim[i] + " ";
            if(i!=coordsLoupFaim.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs loups est mort de faim.";
        Messages.push_back(message);
    }

    if(!coordsLoupVieillese.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsLoupVieillese.size(); ++i){
            message += coordsLoupVieillese[i] + " ";
            if(i!=coordsLoupVieillese.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs loups est mort de vieillese.";
        Messages.push_back(message);
    }     
    
}

void Univers::deplacerEtNourrir(){
    int loupX, loupY, newLoupX, newLoupY;
    int moutonX, moutonY, newMoutonX, newMoutonY;
    
    std::vector<int> dx = {-1, 0, 1};
    std::vector<int> dy = {-1, 0, 1};
    std::vector<std::string> coordsLoupManger;
    //vector<Animal*> copie = animaux;


    std::mt19937 generator(std::random_device{}());

    //Loup : déplacement + gestion nourriture
    for(std::vector<Animal*>::iterator it = animaux.begin(); it != animaux.end(); ){

        if(*it != nullptr){
            shuffle(dx.begin(), dx.end(), generator);
            shuffle(dy.begin(), dy.end(), generator);

            Animal* a = *it;
            // std::cout << "Animal actuel : " << std::endl;
            // std::cout << "Espece: " << (a->getEspece() == LOUP ? "LOUP" : "MOUTON") << std::endl;
            // std::cout << "Position: (" << a->getPosX() << ", " << a->getPosY() << ")" << std::endl;

            if(a->getEspece() == LOUP && !(a->getReproduction())){
                loupX = a->getPosX();
                loupY = a->getPosY();
                bool aChasse = false;
                
                if(a->getAttenteNourrir() == 0){

                
                    for(int ny : dy){
                        for(int nx : dx){
                            if(nx == 0 && ny == 0) continue;
                            newLoupX = loupX + nx;
                            newLoupY = loupY + ny;
                            if(!positionValide(newLoupX, newLoupY)) continue;
                            if(grille[newLoupY][newLoupX].estLibre()) continue;
                            Animal *occupant = grille[newLoupY][newLoupX].getOccupant();
                            if(occupant && occupant->getEspece() == MOUTON && !(occupant->getReproduction())){
                                std::string coord = "(" + std::to_string(newLoupX) + "," + std::to_string(newLoupY) + ")";
                                coordsLoupManger.push_back(coord);
                                // std::cout << "a Manger Mouton pos : " << newLoupX << "," << newLoupY << std::endl;
                                aChasse = true;
                                delete occupant;
                                auto itOcc = std::find(animaux.begin(), animaux.end(), occupant);
                                if(itOcc != animaux.end()) *itOcc = nullptr;
                                grille[newLoupY][newLoupX].retirerAnimal();
                                grille[loupY][loupX].retirerAnimal();
                                grille[newLoupY][newLoupX].ajouterAnimal(a);
                                a->seDeplacer(newLoupX, newLoupY);
                                a->seNourrir();
                            }
                            if(aChasse) break;
                        }
                        if(aChasse) break;
                    }
                } 
                // if(aChasse) std::cout << "a bien chasse" << std::endl;
                if(!aChasse){
                    bool positionTrouve = false;
                    int essais = 0;
                    std::uniform_int_distribution<int> distribution(-1, 1);
                    for(int essais = 0; essais < 8 && !positionTrouve; ++essais){
                        newLoupX = loupX + distribution(generator);
                        newLoupY = loupY + distribution(generator);
                        if(positionValide(newLoupX, newLoupY) && !a->backTrack(newLoupX, newLoupY)){
                            if(grille[newLoupY][newLoupX].estLibre() || grille[newLoupY][newLoupX].getOccupant() == a){
                                if(grille[newLoupY][newLoupX].getOccupant() != a){
                                    grille[loupY][loupX].retirerAnimal();
                                    grille[newLoupY][newLoupX].ajouterAnimal(a);
                                    a->seDeplacer(newLoupX, newLoupY);
                                }
                                // std::cout << "(" << std::to_string(newLoupX) << "," << std::to_string(newLoupY) << ") : " << "Loup has random position" << std::endl;
                                positionTrouve = true;
                            }
                        }
                    }
                }
            }
        }
        ++it;
    }

    if(!coordsLoupManger.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsLoupManger.size(); ++i){
            message += coordsLoupManger[i] + " ";
            if(i!=coordsLoupManger.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs moutons a servi de repas pour des loups.";
        Messages.push_back(message);
    }


    animaux.erase(std::remove(animaux.begin(), animaux.end(), nullptr), animaux.end());

    //Mouton : déplacement + gestion nourriture
    for(std::vector<Animal*>::iterator it = animaux.begin(); it != animaux.end(); ){
        
        shuffle(dx.begin(), dx.end(), generator);
        shuffle(dy.begin(), dy.end(), generator);
        if(*it != nullptr){

            Animal *a = *it;

            if(a->getEspece() == MOUTON && !a->getReproduction()){
                // std::cout << "MOUTON!";
                moutonX = a->getPosX();
                moutonY = a->getPosY();
                bool aMange = false;
                bool posAlea = true;

                if(grille[moutonY][moutonX].aHerbe()){
                    // std::cout << "mange de l'herbe" << std::endl;
                    a->seNourrir();
                    grille[moutonY][moutonX].retirerHerbe();
                    aMange = true;
                }

                if(!aMange){
                    // std::cout << "na pas mange" << std::endl;
                    // Verifie herbe a manger
                    for(int ny : dy){
                        for(int nx : dx){
                            if(nx == 0 && ny == 0) continue;
                            newMoutonX = moutonX + nx;
                            newMoutonY = moutonY + ny;
                            if(!positionValide(newMoutonX, newMoutonY)) continue;
                            if(!grille[newMoutonY][newMoutonX].estLibre()) continue;
                            if(grille[newMoutonY][newMoutonX].aHerbe()){
                                std::string coord = "(" + std::to_string(newMoutonX) + "," + std::to_string(newMoutonY) + ")";
                                // std::cout << "mouton va sur une case avec herbe " << coord << std::endl;
                                posAlea = false;
                                grille[moutonY][moutonX].retirerAnimal();
                                grille[newMoutonY][newMoutonX].ajouterAnimal(a);
                                a->seDeplacer(newMoutonX, newMoutonY);
                                //a->seNourrir();
                            }
                            if(!posAlea) break;
                        }
                        if(!posAlea) break;
                    }
                    //if(!posAlea) break;

                    if(posAlea){
                        // std::cout << "cherche une position alea" << std::endl;
                        bool positionTrouve = false;
                        int essais = 0;
                        std::uniform_int_distribution<size_t> distribution(-1, 1);
                        while(!positionTrouve){
                            newMoutonX = moutonX + distribution(generator);
                            newMoutonY = moutonY + distribution(generator);
                            if(positionValide(newMoutonX, newMoutonY)){
                                if(grille[newMoutonY][newMoutonX].estLibre() || grille[newMoutonY][newMoutonX].getOccupant() == a){
                                    if(grille[newMoutonY][newMoutonX].getOccupant() != a){
                                        // std::cout << "nouvelle position" << std::endl;
                                        grille[moutonY][moutonX].retirerAnimal();
                                        grille[newMoutonY][newMoutonX].ajouterAnimal(a);
                                        a->seDeplacer(newMoutonX, newMoutonY);
                                    }
                                    // else std::cout << "reste position" << std::endl;
                                    positionTrouve = true;
                                }
                            }
                            //++essais;
                        }
                    }
                }
            }
        }
        ++it;
    }   
}

void Univers::reproductionAnimaux(){

    std::vector<std::string>coordsLoupReproduction;    
    std::vector<std::string>coordsMoutonReproduction; 

    std::mt19937 generator(std::random_device{}());

    std::vector<int> dx = {-1, 0, 1};
    std::vector<int> dy = {-1, 0, 1};

    //Loup : reproduire
    for(Animal *a: animaux){
        if(a->getEspece() == LOUP && !a->getReproduction() && a->getAttenteReproduction() == 0){
            bool reproduit = false;
            for(Animal *b: animaux){
                if(a != b && b->getEspece() == LOUP && a->getSexe() != b->getSexe() && !b->getReproduction() && b->getAttenteReproduction() == 0){
                    for(int ny: dy){
                        for(int nx: dx){
                            if(nx == 0 && ny == 0) continue;
                            if(positionValide(nx, ny) && a->getPosX() + nx == b->getPosX() && a->getPosY() + ny == b->getPosY()){
                                a->seReproduire(b->getPosX(), b->getPosY());
                                b->seReproduire(a->getPosX(), a->getPosY());
                                reproduit = true;
                                std::string coords = "(" +  std::to_string(a->getPosX()) + "," + std::to_string(a->getPosY()) + ") + (" + \
                                    std::to_string(b->getPosX()) + "," + std::to_string(b->getPosY()) + ")";
                                coordsLoupReproduction.push_back(coords);
                                break;
                            }
                        }
                        if(reproduit) break;
                    }
                }
                if(reproduit) break;
            }
        }
    }


    //Mouton : reproduire
    for(Animal *a: animaux){
        if(a->getEspece() == MOUTON && !a->getReproduction() && a->getAttenteReproduction() == 0){
            bool reproduit = false;
            for(Animal *b: animaux){
                if(a != b && b->getEspece() == MOUTON && a->getSexe() != b->getSexe() && !b->getReproduction() && b->getAttenteReproduction() == 0){
                    for(int ny: dy){
                        for(int nx: dy){
                            if(nx == 0 && ny == 0) continue;
                            if(positionValide(nx, ny) && a->getPosX() + nx == b->getPosX() && a->getPosY() + ny == b->getPosY()){
                                a->seReproduire(b->getPosX(), b->getPosY());
                                b->seReproduire(a->getPosX(), a->getPosY());
                                reproduit = true;
                                std::string coords = "(" +  std::to_string(a->getPosX()) + "," + std::to_string(a->getPosY()) + ")+(" + \
                                    std::to_string(b->getPosX()) + "," + std::to_string(b->getPosY()) + ")";
                                coordsMoutonReproduction.push_back(coords);
                                //cout << b->getPosX() << ", " << b->getPosY() << " -> " << a->getPosX() << ", " << a->getPosY() << endl;
                                //cout << newMessage << endl;
                                break;
                            }
                        }
                        if(reproduit) break;
                    }
                }
                if(reproduit) break;
            }
        }
    }

    if(!coordsLoupReproduction.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsLoupReproduction.size(); ++i){
            message += coordsLoupReproduction[i] + " ";
            if(i!=coordsLoupReproduction.size() - 1) message += ", ";
        }
        message += "] : Des loups vont se reproduire.";
        Messages.push_back(message);
    }

    if(!coordsMoutonReproduction.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsMoutonReproduction.size(); ++i){
            message += coordsMoutonReproduction[i] + " ";
            if(i!=coordsMoutonReproduction.size() - 1) message += ", ";
        }
        message += "] : Des moutons vont se reproduire.";
        Messages.push_back(message);
    }

}
void Univers::naissanceAnimaux(){

    int loupX, loupY;
    int moutonX, moutonY;
    std::vector<Animal*> nouveauAnimaux;
    std::vector<std::string> coordsLoupNaissance;
    std::vector<std::string> coordsMoutonNaissance;

    std::mt19937 generator(std::random_device{}());

    std::vector<int> dx = {-1, 0, 1};
    std::vector<int> dy = {-1, 0, 1};
    shuffle(dx.begin(), dx.end(), generator);
    shuffle(dy.begin(), dy.end(), generator); 

    //Loup : naissance
    for(Animal *a : animaux){
        if(a->getEspece() == LOUP && a->getReproduction() && a->getAttenteNaissance() == 0){
            bool estNe = false;
            Animal *b = grille[a->getPartY()][a->getPartX()].getOccupant();
            if(b == nullptr) continue;
            Animal *mere = (a->getSexe() == FEMELLE) ? a : b;
            for(int ny : dy){
                for(int nx : dx){
                    if(nx == 0 && ny == 0) continue;
                    loupX = mere->getPosX() + nx;
                    loupY = mere->getPosY() + ny;
                    if(positionValide(loupX, loupY) && grille[loupY][loupX].estLibre()){
                        std::string coords = "(" +  std::to_string(loupX) + "," + std::to_string(loupY) + ")";
                        coordsLoupNaissance.push_back(coords);
                        char sexe = (rand() % 2 == 0) ? MALE : FEMELLE;
                        Loup *loup = new Loup(loupX, loupY, sexe);
                        nouveauAnimaux.push_back(loup);
                        grille[loupY][loupX].ajouterAnimal(loup);
                        estNe = true;
                        break;
                    }
                }
                if(estNe) break;
            }
            a->finReproduction();
            b->finReproduction();
        }
    }

    shuffle(dx.begin(), dx.end(), generator);
    shuffle(dy.begin(), dy.end(), generator); 

    //Mouton : naissance
    for(Animal *a : animaux){
        //cout << a->getEspece() << "," << a->getPosX() << "," << a->getPosY() << endl;
        if(a->getEspece() == MOUTON && a->getReproduction()  && a->getAttenteNaissance() == 0){
            bool estNe = false;
            Animal *b = grille[a->getPartY()][a->getPartX()].getOccupant();
            if(b == nullptr) continue;
            Animal *mere = (a->getSexe() == FEMELLE) ? a : b;
            for(int ny : dy){
                for(int nx : dx){
                    if(nx == 0 && ny == 0) continue;
                    moutonX = mere->getPosX() + nx;
                    moutonY = mere->getPosY() + ny;
                    if(positionValide(moutonX, moutonY) && grille[moutonY][moutonX].estLibre()){
                        std::string coords = "(" +  std::to_string(moutonX) + "," + std::to_string(moutonY) + ")";
                        coordsMoutonNaissance.push_back(coords);
                        char sexe = (rand() % 2 == 0) ? MALE : FEMELLE;
                        Mouton *mouton = new Mouton(moutonX, moutonY, sexe);
                        nouveauAnimaux.push_back(mouton);
                        grille[moutonY][moutonX].ajouterAnimal(mouton);
                        estNe = true;
                        break;
                    }
                }
                if(estNe) break;
            }
            a->finReproduction();
            b->finReproduction();
        }
    }

    // std::cout << "test1" << std::endl;
    // Ajout des nouveaux animaux dans animaux
    for(Animal* nouveau : nouveauAnimaux){
        animaux.push_back(nouveau);
    }

    // std::cout << "test2" << std::endl;

    if(!coordsLoupNaissance.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsLoupNaissance.size(); ++i){
            message += coordsLoupNaissance[i] + " ";
            if(i!=coordsLoupNaissance.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs loup vient de naitre.";
        Messages.push_back(message);
    }

    if(!coordsMoutonNaissance.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsMoutonNaissance.size(); ++i){
            message += coordsMoutonNaissance[i] + " ";
            if(i!=coordsMoutonNaissance.size() - 1) message += ", ";
        }
        message += "] : Un ou plusieurs mouton vient de naitre.";
        Messages.push_back(message);
    }
}
void Univers::checkHerbe(){
    //Pousse des herbes si c'est possible
    std::vector<std::string> coordsHerbes;
    int ligne, colonne;
    for(ligne = 0; ligne < m; ++ligne){
        for( colonne = 0; colonne < n; ++colonne){
            if(grille[ligne][colonne].pousserHerbe()){
                std::string coords = "(" +  std::to_string(colonne) + "," + std::to_string(ligne) + ")";
                coordsHerbes.push_back(coords);
            }                            
        }
    }

    if(!coordsHerbes.empty()){
        std::string message = "[ ";
        for(int i = 0; i<coordsHerbes.size(); ++i){
            message += coordsHerbes[i] + " ";
            if(i!=coordsHerbes.size() - 1) message += ", ";
        }
        message += "] : De l'herbe a poussé.";
        Messages.push_back(message);
    }
    //nettoyageAnimaux();
}