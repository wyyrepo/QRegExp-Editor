project(QRegExp-Editor)

cmake_minimum_required(VERSION 2.6)

if(APPLE)
   set(PROGRAM_NAME ${PROJECT_NAME})
else(APPLE)
   set(PROGRAM_NAME "qregexp-editor")
endif(APPLE)

# Set the defaut version. Will be used if version file
# isn't available and if it's impossible to get the version
# from git.
set(QREGEXP-EDITOR_DEFAULT_VERSION "0.2.1")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})
enable_testing()
include(QRegExpEditorVersion)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# must be placed before any add_subdirectory
set(FILES_TO_TRANSLATE)

GetQRegExpEditorVersion(QREGEXP-EDITOR_VERSION)
message(STATUS "QREGEXP-EDITOR_VERSION: " ${QREGEXP-EDITOR_VERSION})
file(WRITE "${BINARY_DIR}/VERSION_FILE" ${QREGEXP-EDITOR_VERSION})

configure_file(
   "${SOURCE_DIR}/qregexp_editor-version.cpp.in"
   "${CMAKE_CURRENT_BINARY_DIR}/src/qregexp_editor-version.cpp"
   @ONLY
)
include("CPackConfig.cmake")
add_subdirectory(src)

################ translation stuff ################

file(GLOB TRANSLATIONS_FILES translations/*.ts)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts
files(WARNING: make clean will delete the source .ts files! Danger!)")

if (UPDATE_TRANSLATIONS)
   qt4_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TRANSLATIONS_FILES})
else (UPDATE_TRANSLATIONS)
   qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
endif (UPDATE_TRANSLATIONS)

add_custom_target(translations DEPENDS ${QM_FILES})

install(FILES ${QM_FILES} DESTINATION "share/${PROGRAM_NAME}/translations")


################ uninstall target #################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


###### install images & icon & desktop menu #######

install(DIRECTORY "images"
   DESTINATION "share/${PROGRAM_NAME}"
   PATTERN "*.sh" EXCLUDE)

install(FILES dist/linux/qregexp-editor.desktop DESTINATION share/applications)
install(FILES images/qregexp-editor-logo-32.png DESTINATION share/pixmaps)
