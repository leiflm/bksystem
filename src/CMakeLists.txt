include_directories(
    "${CMAKE_SOURCE_DIR}/src/include"
    "${CMAKE_SOURCE_DIR}/src/model/include"
    "${CMAKE_SOURCE_DIR}/src/ui/include"
    ${EINA_INCLUDE_DIRS}
    ${EFREET_INCLUDE_DIRS}
    ${EVAS_INCLUDE_DIRS}
    ${ECORE_INCLUDE_DIRS}
    ${ECORE_FILE_INCLUDE_DIRS}
    ${ECORE_X_INCLUDE_DIRS}
    ${EET_INCLUDE_DIRS}
    ${EDJE_INCLUDE_DIRS}
    ${EDBUS_INCLUDE_DIRS}
    ${ETHUMB_INCLUDE_DIRS}
    ${ELEMENTARY_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
    )

LIST(APPEND BKSYSTEM_CC_SOURCES
    common/bks_status.c
    common/bks_job.c
    model/bks_model_product.c
    model/bks_model_user_account.c
    model/bks_model_user_account.c
    model/bks_model_sale.c
    model/bks_model_path.c
    model/bks_model_sql.c
    model/bks_model.c
    ui/elm_utils.c
    ui/utils.c
    ui/win.c
    ui/singleton.c
    ui/controller.c
    ui/fileselector.c
    ui/notification.c
    ui/products_grid.c
    ui/products_list.c
    ui/products_selected.c
    ui/products_page.c
    ui/user_accounts_list.c
    ui/user_accounts_page.c
    ui/bks_ui.c
    controller/bks_controller.c
    main.c
    )

add_executable(bksystem ${BKSYSTEM_CC_SOURCES})
target_link_libraries(bksystem 
    ${EINA_LIBRARIES}
    ${EFREET_LIBRARIES}
    ${EVAS_LIBRARIES}
    ${ECORE_LIBRARIES}
    ${ECORE_FILE_LIBRARIES}
    ${ECORE_X_LIBRARIES}
    ${EET_LIBRARIES}
    ${EDJE_LIBRARIES}
    ${EDBUS_LIBRARIES}
    ${ETHUMB_LIBRARIES}
    ${ELEMENTARY_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )

install(TARGETS bksystem DESTINATION bin)
