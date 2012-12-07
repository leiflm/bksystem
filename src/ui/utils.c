#include <Ecore_X.h>
#include "Bks_Ui_Private.h"

Ecore_X_Window get_win_by_name(Ecore_X_Window parent, const char *name)
{
    Ecore_X_Window *windows, win = 0;
    int nwin, i;
    char *nname;

    ecore_x_icccm_name_class_get(parent, &nname, NULL);
    if (nname && (strcmp(nname, name) == 0))
    {
        free(nname);
        return parent;
    }
    free(nname);

    windows = ecore_x_window_children_get(parent, &nwin);
    if (windows)
    {
        for (i = 0; i < nwin; i++)
        {
            win = get_win_by_name(windows[i], name); 
            if (win != 0)
                break;
        }
        free(windows);
    }
    return win;
}
