#ifndef ASSERT_HPP
#define ASSERT_HPP
// taken from https://raw.githubusercontent.com/vorlac/godot-roguelite/main/src/core/assert.hpp
#pragma once

#include <cstdio>
#include <iostream>

#include <godot_cpp/core/error_macros.hpp>

#define assertion(condition, message)                                             \
    do                                                                            \
    {                                                                             \
        if constexpr(!(condition)) [[unlikely]]                                            \
        {                                                                         \
            ::godot::_err_print_error(__FUNCTION__, __FILE__, __LINE__,           \
                                    message " => condition: (" #condition ")"); \
            ::godot::_err_flush_stdout();                                         \
            GENERATE_TRAP();                                                      \
        }                                                                         \
    }                                                                             \
    while (false)

#define error_msg(message)                                                    \
    do                                                                        \
    {                                                                         \
        ::godot::_err_print_error(__FUNCTION__, __FILE__, __LINE__, message); \
        ::godot::_err_flush_stdout();                                         \
        GENERATE_TRAP();                                                      \
    }                                                                         \
    while (false)

#define runtime_assert(condition) assertion(condition, "validation check failed")

#endif