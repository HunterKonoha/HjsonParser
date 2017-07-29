#pragma once
#include <variant>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include "CharacterSet.h"

namespace HjsonParser {
  class Value {
    public:
      using StringType = std::string;
      using ArrayType = std::vector<Value>;
      using ObjectType = std::unordered_map<StringType, Value>;

      enum class ValueType {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
      };

    private:
      std::variant<std::monostate, bool, double, StringType, ArrayType, ObjectType> data_ = std::monostate{};

    public:
      Value() = default;
      Value(const Value& obj) = default;
      Value(Value&&) = default;
      explicit Value(bool value);
      explicit Value(double value);
      Value(const StringType& value, CharacterSet CharSet);
      explicit Value(const ArrayType& value);
      explicit Value(const ObjectType& value);
      Value& operator=(const Value& obj) = default;
      Value& operator=(Value&& obj) = default;
      template<typename Call>
      void Visit(Call&& Callable)const;

      //type_getter
      bool IsNull()const;
      bool IsBoolean()const;
      bool IsNumber()const;
      bool IsString()const;
      bool IsArray()const;
      bool IsObject()const;
      ValueType GetValueType()const;

      //getter
      Value& operator[](std::size_t Index);
      const Value& operator[](std::size_t Index)const;
      Value& operator[](const StringType& Key);
      const Value& operator[](const StringType& Key)const;
      double& GetNumber();
      const double& GetNumber()const;
      bool& GetBoolean();
      const bool& GetBoolean()const;
      StringType& GetString();
      const StringType& GetString()const;
      ArrayType& GetArray();
      const ArrayType& GetArray()const;
      ObjectType& GetObject();
      const ObjectType& GetObject()const;
      bool Contain(std::size_t Index)const;
      bool Contain(const StringType& Key)const;

      //template getter
      template<class T>
      typename std::enable_if_t<!std::is_arithmetic<T>::value, T> Get()const;
      template<class T>
      typename std::enable_if_t<std::is_arithmetic<T>::value, T> Get()const;
      template<>
      bool Get<bool>()const;
      template<>
      std::string Get<std::string>()const;
      template<class T>
      typename std::enable_if_t<!std::is_arithmetic<T>::value, std::optional<T>> GetOpt()const;
      template<class T>
      typename std::enable_if_t<std::is_arithmetic<T>::value, std::optional<T>> GetOpt()const;
      template<>
      std::optional<bool> GetOpt<bool>()const;
      template<class T, class U>
      T GetOr(U&& DefaultValue)const;
  };

  std::ostream& operator<<(std::ostream&, const Value&);
  bool operator==(const Value& obj1, const Value& obj2);
  bool operator!=(const Value& obj1, const Value& obj2);
}
#include "Value.inl"