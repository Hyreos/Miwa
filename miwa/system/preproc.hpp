#pragma once

#ifndef GLFW_NO_INCLUDE
#	define GLFW_NO_INCLUDE
#endif

#define _USE_MSC 1912

#ifdef __cplusplus
#ifdef _MSVC_LANG
#if (_MSC_VER < _USE_MSC)
#pragma message("Your compiler is too ancient for use, please update this")
#endif
#elif (__cplusplus < _CPP_VERSION)
#error "Your C++ version is oldest than version used on this project(C++17)"
#endif
#endif