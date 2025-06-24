#pragma once

#ifdef _WIN32
  #ifdef BUILD_DLL
    #define MYLIB_API __declspec(dllexport)
  #else
    #define MYLIB_API __declspec(dllimport)
  #endif
#else
  #define MYLIB_API
#endif

#include <cstdint>
#include <string>
#include <vector>
using namespace std;

// Оголошуємо експортувані функції
extern "C" {
  class Coding{
    public:
    MYLIB_API static vector<uint8_t> encrypt(const vector<uint8_t>& bytes, int key);
    MYLIB_API static vector<uint8_t> decrypt(const vector<uint8_t>& bytes, int key);
  };
}
