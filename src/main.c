int main(const unsigned int argc, const char argv[][])
{
    Bks_Model *model;
    Bks_Controller *controller;
    Bks_Ui *ui;

    model = bks_model_new();
    controller = bks_controller_new(model);
    ui = bks_ui_new(controller, model);

    bks_controller_run(controller);

    bks_ui_shutdown(ui);
    bks_controller_shutdown(controller);
    bks_model_shutdown(model);

    return 0;
}
