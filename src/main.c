#include <Eina.h>
#include <Ecore.h>
#include "Bks_System.h"
#include "Bks_Model.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"

int main(int argc, char *argv[])
{
    //used for debugging infra and locks
    eina_init();
    //gimme threads!
    ecore_init();

    memset(&mdl, 0, sizeof(Bks_Model));
    memset(&ui, 0, sizeof(Bks_Ui));
    memset(&ctrl, 0, sizeof(Bks_Controller));

    bks_model_init();
    bks_ui_init(argc, argv);
    bks_controller_init();

    //start mainloop
    bks_controller_run();

    bks_ui_shutdown();
    bks_controller_shutdown();
    bks_model_shutdown();

    //no more threads!
    ecore_shutdown();
    //used for debugging infra and locks
    eina_shutdown();
    return 0;
}
