#include "Bks_Types.h"
#include "Bks_Model.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"

int main(const unsigned int argc, const char argv[][])
{
    //used for debugging infra
    eina_init();
    bks_model_init();
    bks_controller_init();
    bks_ui_init();

    bks_controller_run(controller);

    bks_ui_shutdown(ui);
    bks_controller_shutdown(controller);
    bks_model_shutdown(model);

    //used for debugging infra
    eina_shutdown();
    return 0;
}
