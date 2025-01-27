#pragma once

#if defined(_MSC_VER)
#if defined(EXPORT_GAMEPLAY)  
#define GAMEPLAY_API __declspec(dllexport)
#else
#define GAMEPLAY_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define GAMEPLAY_API __attribute__((visibility("default")))
#else
#define GAMEPLAY_API  
#endif
