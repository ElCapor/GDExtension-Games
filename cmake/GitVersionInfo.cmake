# SPDX-License-Identifier: Unlicense

find_program( GIT_PROGRAM git )

if ( GIT_PROGRAM )
    # get version information
    execute_process(
        COMMAND "${GIT_PROGRAM}" --version
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message( STATUS "Using git: ${GIT_PROGRAM} (${GIT_VERSION})" )

    include( GetGitRevisionDescription )

    get_git_head_revision( GIT_REFSPEC GIT_SHA1 )
    git_describe( GIT_SHORT )

    string( TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME )

    message( STATUS "${PROJECT_NAME} version: ${GIT_SHORT}" )
    
    unset( GIT_VERSION )
endif()
