#pragma once
#include "library.h"
#include "Value.h"
#include "CharacterSet.h"
#include <string>

namespace HjsonParser {
  Value Parse(const std::string& Input, CharacterSet InputCharSet = CharacterSet::Shift_JIS, CharacterSet OutputCharSet = CharacterSet::Shift_JIS);
  Value Parse(const std::wstring& Input, CharacterSet InputCharSet = CharacterSet::Shift_JIS, CharacterSet OutputCharSet = CharacterSet::Shift_JIS);
}