#pragma once

#ifdef _WIN64
  #ifdef _DEBUG
   #pragma comment(lib, "x64/HjsonParser_d")
  #else
   #pragma comment(lib, "x64/HjsonParser")
  #endif
#else
  #ifdef _DEBUG
   #pragma comment(lib, "x86/HjsonParser_d")
  #else
    #pragma comment(lib, "x86/HjsonParser")
  #endif
#endif