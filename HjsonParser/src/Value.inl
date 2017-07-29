#pragma once
template<typename Call>
void HjsonParser::Value::Visit(Call&& Callable) const {
  std::visit(Callable, this->data_);
}

template<class T>
inline std::enable_if_t<std::is_arithmetic<T>::value, T> HjsonParser::Value::Get() const {
  return static_cast<T>(this->GetNumber());
}

template<class T>
inline std::enable_if_t<!std::is_arithmetic<T>::value, T> HjsonParser::Value::Get() const {
  return to_string(this->GetString());
}

template<>
inline bool HjsonParser::Value::Get<bool>() const {
  return this->GetBoolean();
}

template<>
inline std::string HjsonParser::Value::Get<std::string>() const {
  return this->GetString();
}

template<class T>
inline std::enable_if_t<std::is_arithmetic<T>::value, std::optional<T>> HjsonParser::Value::GetOpt() const {
  if (!this->IsNumber()) {
    return std::nullopt;
  }
  return static_cast<T>(this->GetNumber())
}

template<class T>
inline std::enable_if_t<!std::is_arithmetic<T>::value, std::optional<T>> HjsonParser::Value::GetOpt() const {
  if (!this->IsString()) {
    return std::nullopt;
  }
  return to_string(this->GetString());
}

template<>
inline std::optional<bool> HjsonParser::Value::GetOpt<bool>() const {
  if (!this->IsBoolean()) {
    return std::nullopt;
  }
  return this->GetBoolean();
}

template<>
inline std::optional<std::string> HjsonParser::Value::GetOpt<std::string>() const {
  if (!this->IsString()) {
    return std::nullopt;
  }
  return this->GetString();
}

template<class T, class U>
T HjsonParser::Value::GetOr(U&& DefaultValue) const {
  return this->GetOpt<T>().value_or(std::forward<U>(DefaultValue));
}

inline bool HjsonParser::operator!=(const Value & obj1, const Value & obj2) {
  return !(obj1 == obj2);
}
