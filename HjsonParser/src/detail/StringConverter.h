#pragma once
#include "../CharacterSet.h"
#include <string>

namespace HjsonParser {
  namespace detail {
    struct StringConverter {
      std::string operator()(const std::string& Input, CharacterSet InputChar, CharacterSet OutputChar);
      std::string operator()(const std::wstring& Input, CharacterSet InputChar, CharacterSet OutputChar);
    };
  }
}