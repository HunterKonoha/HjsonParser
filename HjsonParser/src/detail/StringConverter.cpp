#include "StringConverter.h"
#include <Windows.h>
#include <vector>

namespace Conveter {
  std::string wide_to_multi_winapi(std::wstring const& src) {
    auto const dest_size = WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> dest(dest_size, '\0');
    if (WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
      throw std::system_error{ static_cast<int>(GetLastError()), std::system_category() };
    }
    return std::string(dest.begin(), dest.end());
  }

  std::wstring multi_to_wide_winapi(std::string const& src) {
    auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, nullptr, 0U);
    std::vector<wchar_t> dest(dest_size, L'\0');
    if (::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
      throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    return std::wstring(dest.begin(), dest.end());
  }

  std::wstring utf8_to_wide_winapi(std::string const& src) {
    auto const dest_size = MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, nullptr, 0U);
    std::vector<wchar_t> dest(dest_size, L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
      throw std::system_error{ static_cast<int>(GetLastError()), std::system_category() };
    }
    return std::wstring(dest.begin(), dest.end());
  }

  std::string wide_to_utf8_winapi(std::wstring const& src) {
    auto const dest_size = ::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> dest(dest_size, '\0');
    if (::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
      throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    return std::string(dest.begin(), dest.end());
  }

  std::string UTF8toShiftJis(const std::string & Str) {
    auto const wide = utf8_to_wide_winapi(Str);
    return wide_to_multi_winapi(wide);
  }

  std::string ShiftJisToUTF8(std::string const& src) {
    auto const wide = multi_to_wide_winapi(src);
    return wide_to_utf8_winapi(wide);
  }
}


std::string HjsonParser::detail::StringConverter::operator()(const std::string& Input, CharacterSet InputChar, CharacterSet OutputChar) {
  if (InputChar == OutputChar) {
    return Input;
  }
  if (InputChar == CharacterSet::Shift_JIS&&OutputChar == CharacterSet::UTF8) {
    return Conveter::ShiftJisToUTF8(Input);
  }
  else if (InputChar == CharacterSet::UTF8&&OutputChar == CharacterSet::Shift_JIS) {
    return Conveter::UTF8toShiftJis(Input);
  }
  else {
    return "";
  }
}

std::string HjsonParser::detail::StringConverter::operator()(const std::wstring& Input, CharacterSet InputChar, CharacterSet OutputChar) {
  if (InputChar == CharacterSet::Shift_JIS) {
    return StringConverter()(Conveter::wide_to_multi_winapi(Input), InputChar, OutputChar);
  }
  else if (InputChar == CharacterSet::UTF8) {
    return StringConverter()(Conveter::wide_to_utf8_winapi(Input), InputChar, OutputChar);
  }
  else {
    return "";
  }
}