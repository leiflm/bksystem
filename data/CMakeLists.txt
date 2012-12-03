add_custom_command(
    OUTPUT "elm_overlay.edj"
    DEPENDS "${PROJECT_SOURCE_DIR}/data/theme/elm_overlay.edc"
    COMMAND "edje_cc"
    ARGS "-id" "${PROJECT_SOURCE_DIR}/data/theme/images" "${PROJECT_SOURCE_DIR}/data/theme/elm_overlay.edc" "${CMAKE_CURRENT_BINARY_DIR}/elm_overlay.edj"
    )
add_custom_target(
    elm_overlay ALL
    DEPENDS "elm_overlay.edj"
    )

install (FILES "${PROJECT_SOURCE_DIR}/data/empty.sqlite"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/example.sqlite"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/elm_overlay.edj"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/images/accounts/locked.png"
    DESTINATION "share/${CMAKE_PROJECT_NAME}/images/accounts/")
install (FILES "${PROJECT_SOURCE_DIR}/data/bksystem.desktop"
    DESTINATION "share/applications")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/backup.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/bksystem.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/export_bill_basic.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/export_bill_complete_comb.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/export_bill_complete_sep.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/export_bill_html.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/insert_bill_reference_date_into_db.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/layout.css"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/restore_db.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/scripts/sub_txt_tmpl"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
