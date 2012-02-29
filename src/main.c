#include "Bks_Types.h"
#include "Bks_Model.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"

int main(const unsigned int argc, const char argv[][])
{
    //used for debugging infra
    eina_init();
    //gimme threads!
    ecore_init();
    bks_model_init();
    bks_controller_init();
    bks_ui_init();

    //start mainloop
    bks_controller_run();

    bks_ui_shutdown();
    bks_controller_shutdown();
    bks_model_shutdown();

    //no more threads!
    ecore_shutdown();
    //used for debugging infra
    eina_shutdown();
    return 0;
}
