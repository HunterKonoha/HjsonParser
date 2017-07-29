#include "Value.h"
#include "detail/StringConverter.h"
#include <algorithm>

namespace Local {
  struct Visitor {
    std::ostream& stream;
    std::size_t indent;

    template<typename T>
    void operator()(const T& value) {
      this->stream << value;
    }

    template<>
    void operator()(const HjsonParser::Value::StringType& value) {
      this->stream << "\"";
      for (auto&&ch : value) {
        if (ch == '\n') {
          this->stream << "\\n";
        }
        else {
          this->stream << ch;
        }
      }
      this->stream << "\"";
    }

    template<>
    void operator()(const HjsonParser::Value::ArrayType& value) {
      ++this->indent;
      this->stream << "[" << std::endl;
      if (!value.empty()) {
        for (std::size_t i = 0; i < value.size() - 1; ++i) {
          this->OutputIndent();
          value[i].Visit(Local::Visitor{ this->stream ,this->indent });
          this->stream << "," << std::endl;
        }
        this->OutputIndent();
        value.back().Visit(Local::Visitor{ this->stream ,this->indent });
        this->stream << std::endl;
      }
      --this->indent;
      this->OutputIndent();
      this->stream << "]";
    }

    template<>
    void operator()(const HjsonParser::Value::ObjectType& value) {
      ++this->indent;
      this->stream << "{" << std::endl;
      if (!value.empty()) {
        auto it = value.begin();
        for (auto end = --value.end(); it != end; ++it) {
          this->OutputIndent();
          this->stream << "\"" << it->first << "\"";
          this->stream << ":" << " ";
          it->second.Visit(Local::Visitor{ this->stream ,this->indent });
          this->stream << "," << std::endl;
        }
        this->OutputIndent();
        this->stream << "\"" << it->first << "\"";
        this->stream << ":" << " ";
        it->second.Visit(Local::Visitor{ this->stream ,this->indent });
        this->stream << std::endl;
      }
      --this->indent;
      this->OutputIndent();
      this->stream << "}";
    }
    template<>
    void operator()(const std::monostate& value) {
      this->stream << "null" << std::endl;
    }

    void OutputIndent() {
      for (std::size_t i = 0; i < this->indent; ++i) {
        this->stream << "  ";
      }
    }

  };
}

HjsonParser::Value::Value(bool value) : data_(value) {

}

HjsonParser::Value::Value(double value) : data_(value) {

}

HjsonParser::Value::Value(const StringType & value, CharacterSet CharSet) : data_(detail::StringConverter()(value, CharacterSet::UTF8, CharSet)) {
  auto& str = std::get<Value::StringType>(this->data_);
  if (str.back() == '\0') {
    str.pop_back();
  }
}

HjsonParser::Value::Value(const ArrayType & value) : data_(value) {

}

HjsonParser::Value::Value(const ObjectType & value) : data_(value) {

}

bool HjsonParser::Value::IsNull() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::Null);
}

bool HjsonParser::Value::IsBoolean() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::Boolean);
}

bool HjsonParser::Value::IsNumber() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::Number);
}

bool HjsonParser::Value::IsString() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::String);
}

bool HjsonParser::Value::IsArray() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::Array);
}

bool HjsonParser::Value::IsObject() const {
  return this->data_.index() == static_cast<std::size_t>(ValueType::Object);
}

HjsonParser::Value::ValueType HjsonParser::Value::GetValueType() const {
  return static_cast<ValueType>(this->data_.index());
}

HjsonParser::Value & HjsonParser::Value::operator[](std::size_t Index) {
  return this->GetArray()[Index];
}

const HjsonParser::Value & HjsonParser::Value::operator[](std::size_t Index) const {
  return this->GetArray().at(Index);
}

HjsonParser::Value & HjsonParser::Value::operator[](const StringType & Key) {
  return this->GetObject()[Key];
}

const HjsonParser::Value & HjsonParser::Value::operator[](const StringType & Key) const {
  return this->GetObject().at(Key);
}

double & HjsonParser::Value::GetNumber() {
  if (!this->IsNumber()) {
    throw std::runtime_error("!IsNumber()");
  }
  return std::get<double>(this->data_);
}

const double & HjsonParser::Value::GetNumber() const {
  if (!this->IsNumber()) {
    throw std::runtime_error("!IsNumber()");
  }
  return std::get<double>(this->data_);
}

bool & HjsonParser::Value::GetBoolean() {
  if (!this->IsBoolean()) {
    throw std::runtime_error("!IsBoolean()");
  }
  return std::get<bool>(this->data_);
}

const bool & HjsonParser::Value::GetBoolean() const {
  if (!this->IsBoolean()) {
    throw std::runtime_error("!IsBoolean()");
  }
  return std::get<bool>(this->data_);
}

HjsonParser::Value::StringType & HjsonParser::Value::GetString() {
  if (!this->IsString()) {
    throw std::runtime_error("!IsString()");
  }
  return std::get<StringType>(this->data_);
}

const HjsonParser::Value::StringType & HjsonParser::Value::GetString() const {
  if (!this->IsString()) {
    throw std::runtime_error("!IsString()");
  }
  return std::get<StringType>(this->data_);
}

HjsonParser::Value::ArrayType & HjsonParser::Value::GetArray() {
  if (!this->IsArray()) {
    throw std::runtime_error("!IsArray()");
  }
  return std::get<ArrayType>(this->data_);
}

const HjsonParser::Value::ArrayType & HjsonParser::Value::GetArray() const {
  if (!this->IsArray()) {
    throw std::runtime_error("!IsArray()");
  }
  return std::get<ArrayType>(this->data_);
}

HjsonParser::Value::ObjectType & HjsonParser::Value::GetObject() {
  if (!this->IsObject()) {
    throw std::runtime_error("!IsObject()");
  }
  return std::get<ObjectType>(this->data_);
}

const HjsonParser::Value::ObjectType & HjsonParser::Value::GetObject() const {
  if (!this->IsObject()) {
    throw std::runtime_error("!IsObject()");
  }
  return std::get<ObjectType>(this->data_);
}

bool HjsonParser::Value::Contain(std::size_t Index) const {
  if (!this->IsArray()) {
    throw std::runtime_error("!IsArray()");
  }
  return Index < this->GetArray().size();
}

bool HjsonParser::Value::Contain(const StringType & Key) const {
  if (!this->IsObject()) {
    throw std::runtime_error("!IsObject()");
  }
  const auto& obj = this->GetObject();
  return obj.find(Key) != obj.end();
}

std::ostream & HjsonParser::operator<<(std::ostream & stream, const Value & value) {
  value.Visit(Local::Visitor{ stream ,0 });
  return stream;
}

bool HjsonParser::operator==(const Value & obj1, const Value & obj2) {
  if (obj1.GetValueType() != obj2.GetValueType()) {
    return false;
  }
  auto all_of = [](const auto& x, const auto& y) {
    return std::equal(x.begin(), x.end(), y.begin(), y.end());
  };
  switch (obj1.GetValueType()) {
    case Value::ValueType::Number:
      return std::fabs(obj1.GetNumber() - obj2.GetNumber()) <= DBL_EPSILON;
    case Value::ValueType::Boolean:
      return obj1.GetBoolean() == obj2.GetBoolean();
    case Value::ValueType::String:
      return obj1.GetString() == obj2.GetString();
    case Value::ValueType::Array:
      return all_of(obj1.GetArray(), obj2.GetArray());
    case Value::ValueType::Object:
      return all_of(obj1.GetObject(), obj2.GetObject());
    case Value::ValueType::Null:
      return true;
  }
  return true;
}
