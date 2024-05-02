cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_CXX_STANDARD 23)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "C:/Users/vboxuser/Documents/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(TeodorSynth3001
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "TeodorSynth3001 VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(TeodorSynth3001
    source/params.h
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
        source/common.hpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(TeodorSynth3001
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(TeodorSynth3001
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(TeodorSynth3001
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (TeodorSynth3001
    RESOURCES
        resource/95CD5696869050EE8F3A8D61955BEDF4_snapshot.png
        resource/95CD5696869050EE8F3A8D61955BEDF4_snapshot_2.0x.png
)

target_link_libraries(TeodorSynth3001
    PRIVATE
        sdk
)

smtg_target_configure_version_file(TeodorSynth3001)

if(SMTG_MAC)
    smtg_target_set_bundle(TeodorSynth3001
        BUNDLE_IDENTIFIER ua.edu.ucu.teodorsynths
        COMPANY_NAME "My Plug-in Company"
    )
    smtg_target_set_debug_executable(TeodorSynth3001
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(TeodorSynth3001 PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TeodorSynth3001)

        smtg_target_set_debug_executable(TeodorSynth3001
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
