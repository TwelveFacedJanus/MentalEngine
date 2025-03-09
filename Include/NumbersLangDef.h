#pragma once
#ifndef NUMBERS_LANG_DEP_H_
#define NUMBERS_LANG_DEP_H_

#include <stdint.h>

// #define ENABLE_VULKAN_RENDERER
#define ENABLE_OPENGL_RENDERER
// #define ENABLE_GLFW
#define MENTAL_BETA

#if !defined(ENABLE_VULKAN_RENDERER)
#define ENABLE_OPENGL_RENDERER
#elif !defined(ENABLE_OPENGL_RENDERER)
#define ENABLE_VULKAN_RENDERER
#endif

#if defined(ENABLE_OPENGL_REDENRER)
#include "GL/glew.h"
#elif defined(ENABLE_VULKAN_RENDERER)
#define GLFW_INCLUDE_VULKAN
#endif

#if defined(ENABLE_GLFW)
#include <GLFW/glfw3.h>
#endif

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;



#endif // NUMBERS_LANG_DEP_H_