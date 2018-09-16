include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
   "${CMAKE_CURRENT_BINARY_DIR}/preferences"
)


file(GLOB_RECURSE HEADER_FILES *.h *.hpp)
file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE UI_FILES *.ui)
set(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${CPP_FILES} ${UI_FILES} ${HEADER_FILES} PARENT_SCOPE)

list(APPEND preferences_SRCS
   preferences/preferencesdialog.cpp
   preferences/settingsinterface.cpp
   preferences/settingsinterface.h
   preferences/regexpoptions.cpp
   preferences/regexpsettings.cpp
   preferences/regexppreferencespage.cpp
)

list(APPEND preferences_UIs
   preferences/preferencesdialog.ui
   preferences/regexppreferencespage.ui
)

list(APPEND preferences_MOC_HDRS
   preferences/preferencesdialog.h
   preferences/regexpsettings.h
   preferences/regexppreferencespage.h
)

list(APPEND qregexp-editor_SRCS
   ${preferences_SRCS}
   regexptextedit.cpp
   regexptextedit_p.cpp
   patternfilter.cpp
   parentheseshighlighter.cpp
   qregexp_editor-version.cpp
   node.cpp
   regexpmodel.cpp
   treeitem.cpp
   searchdata.cpp
   searchsettings.cpp
   mainwindow.cpp
   escapedpatterndialog.cpp
   aboutdialog.cpp
   main.cpp
)

set(qregexp-editor_UIs
   ${preferences_UIs}
   ui/aboutdialog.ui
   ui/escapedpatterndialog.ui
   ui/mainwindow.ui
)

set(qregexp-editor_MOC_HDRS
   ${preferences_MOC_HDRS}
   parentheseshighlighter.h
   aboutdialog.h
   escapedpatterndialog.h
   mainwindow.h
   regexpmodel.h
)

set(qregexp-editor_RCCS
   ../application.qrc
)

# generate rules for building source files from the resources
qt4_add_resources(qregexp-editor_RCC_SRCS ${qregexp-editor_RCCS})

qt4_wrap_ui(qregexp-editor_SRCS ${qregexp-editor_UIs})
qt4_wrap_cpp(qregexp-editor_SRCS ${qregexp-editor_MOC_HDRS})

qt4_generate_moc(
   ${CMAKE_CURRENT_SOURCE_DIR}/regexptextedit.h
   ${CMAKE_CURRENT_BINARY_DIR}/regexptextedit.moc
)
qt4_generate_moc(
   ${CMAKE_CURRENT_SOURCE_DIR}/preferences/settingsinterface.h
   ${CMAKE_CURRENT_BINARY_DIR}/preferences/settingsinterface.moc
)

list(APPEND manual_MOC_SRCS
   "regexptextedit.moc"
   "preferences/settingsinterface.moc"

)

if(APPLE)
   set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${QREGEXP-EDITOR_VERSION})
   set(MACOSX_BUNDLE_VERSION ${QREGEXP-EDITOR_VERSION})
   set(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${QREGEXP-EDITOR_VERSION}")
   set(CMAKE_OSX_ARCHITECTURES "i386")
endif(APPLE)

add_executable(
   ${PROGRAM_NAME}
   WIN32
   MACOSX_BUNDLE
   ${qregexp-editor_SRCS}
   ${manual_MOC_SRCS}
   ${qregexp-editor_RCC_SRCS}
)

target_link_libraries(${PROGRAM_NAME} ${QT_LIBRARIES})

install(TARGETS ${PROGRAM_NAME} DESTINATION bin)
add_dependencies(${PROGRAM_NAME} translations)

add_subdirectory(tests)
