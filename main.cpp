//The main library OSLib
#include <oslib/oslib.h>
#include <time.h>
#include "numscene.h"

// The callbacks
PSP_MODULE_INFO("plusraPSP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main() {
    // Initialisation
    srand(time(0));
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslInitAudio();

    // Chargement du jeu
    NumScene *scene = new NumScene();
    if (!scene->loaded()) oslFatalError("There was an error when loading data files.\nPlease consider reinstalling the application.");

    // Boucle principale
    while (!osl_quit) {
        // Debut drawing
        oslStartDrawing();
        // Lecture des touches
        oslReadKeys();
        // Update
        scene->update();
        // Fin drawing
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
    }

    // Nettoyage de la memoire
    delete scene;

    // Leave the program
    oslDeinitAudio();
    oslEndGfx();
    oslQuit();
    return 0;
}