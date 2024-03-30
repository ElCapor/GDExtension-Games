# SPDX-License-Identifier: Unlicense
add_subdirectory("classes/src")

target_sources( ${PROJECT_NAME}
    PRIVATE
        GDExtensionTemplate.h
        GDExtensionTemplate.cpp
        RegisterExtension.cpp
)


target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
        "classes/include"
)
