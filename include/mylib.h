#pragma once
#include <cstdint>
#include <vector>

#ifdef _WIN32
  #ifdef BUILD_DLL
    #define MYLIB_API __declspec(dllexport)
  #else
    #define MYLIB_API __declspec(dllimport)
  #endif
#else
  #define MYLIB_API
#endif

using namespace std;

extern "C" {
    MYLIB_API vector<uint8_t> encrypt(const vector<uint8_t>& bytes, int key);
    MYLIB_API vector<uint8_t> decrypt(const vector<uint8_t>& bytes, int key);
}
