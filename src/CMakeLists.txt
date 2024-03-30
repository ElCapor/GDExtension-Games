# SPDX-License-Identifier: Unlicense
# Grab class source files
file(GLOB_RECURSE class_sources
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/classes/src/*.cpp"
)

target_sources( ${PROJECT_NAME}
    PRIVATE
        ${class_sources}
        GDExtensionTemplate.h
        GDExtensionTemplate.cpp
        RegisterExtension.cpp
)


target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
        "classes/include"
)

# we have to disable this warning about converting double to real_t or whatever
target_compile_options(${PROJECT_NAME} PRIVATE "/wd4244")
