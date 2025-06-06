#include "animaux.hpp"
#include "univers.hpp"

using namespace std;

int main(){

    // Univers doit être minimum 5x5.
    // 4x5, 2x5, 5x4 incorrect
    // 5x5, 5x6, 6x5 correct

    srand(time(NULL));
    //Univers u1(15, 15, 30, 5);
    //Univers u1(1, 1, 5000, 500);
    //Univers u1(1, 1, 0, 0);
    Univers u1(5, 2, 2, 0);
    //Univers u1(12, 12, 30, 5);

    int compteur = 1000;

    for(;;){
        --compteur;
        u1.afficher();
        // if(compteur <= 1){
        //     u1.afficher();
        //     compteur = 1000;
        // }
        if(u1.estMort()){
            u1.afficher();
            cout << "L'univers est mort..." << endl;
            return 0;
        }
        cin.ignore();
        u1.simulerTour();
        u1.miseAJourNombreAnimaux();
    }

    return 0;
}