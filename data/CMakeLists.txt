install (FILES "${PROJECT_SOURCE_DIR}/data/bksystem.sh"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/empty.sqlite"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/example.sqlite"
    DESTINATION "share/${CMAKE_PROJECT_NAME}")
install (FILES "${PROJECT_SOURCE_DIR}/data/images/accounts/locked.png"
    DESTINATION "share/${CMAKE_PROJECT_NAME}/images/accounts/")
install (FILES "${PROJECT_SOURCE_DIR}/data/bksystem.desktop"
    DESTINATION "share/applications")
