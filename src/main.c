#include <Eina.h>
#include <Ecore.h>
#include "Bks_System.h"
#include "Bks_Thread_Queue.h"
#include "Bks_Model.h"
#include "Bks_Controller.h"
#include "Bks_Ui.h"

int main(int argc, char *argv[])
{
    //used for debugging infra and locks
    eina_init();
    //gimme threads!
    ecore_init();

    bks_thread_queue_init();
    bks_model_init();
    bks_controller_init();
    bks_ui_init(argc, argv);

    //start mainloop
    bks_controller_run();

    bks_thread_queue_shutdown();
    bks_ui_shutdown();
    bks_controller_shutdown();
    bks_model_shutdown();

    //no more threads!
    ecore_shutdown();
    //used for debugging infra and locks
    eina_shutdown();
    return 0;
}
