#pragma once
#include "GrammarTemplate.h"
#include "Config.h"
#include "../Value.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>

namespace HjsonParser {
  namespace detail {
    template<typename Iterator>
    struct ParserGrammar : GrammarTemplate<Iterator, Value> {
      CharacterSet charset_ = CharacterSet::Shift_JIS;
      //Hjsonテキスト規則
      rule<Value> hjson_text_;
      //コメント規則
      rule<void> line_comment_, block_comment_, comment_;
      //特殊文字規則
      rule<void> ws_c_, ws_;
      //数字規則
      rule<double> number_;
      //統合文字列規則
      rule<Value> string_;
      //Json用文字列規則
      rule<Value::StringType> json_string_, char_;
      //クォーテーションマークなし用文字列規則
      rule<Value::StringType> ql_string_, ql_char_;
      rule<void> ql_end_;
      //複数行文字列用規則
      rule<Value::StringType> ml_string_, ml_char_;
      rule<void> ml_seq_, ml_ignore_;
      //値規則
      rule<Value> value_, literal_;
      rule<void> literal_end_;
      //値分割規則
      rule<void> value_separator_;
      //配列規則
      rule<Value::ArrayType> array_;
      //オブジェクト規則
      rule<Value::ObjectType> object_;
      rule<std::pair<Value::StringType, Value>> member_;
      //ルートオブジェクト規則
      rule<Value::ObjectType> root_object_;
      //オブジェクト名規則
      rule<Value::StringType> name_, keyname_, non_punctuator_char_;

      ParserGrammar() : GrammarTemplate<Iterator, Value>(this->hjson_text_) {
        namespace qi = boost::spirit::qi;
        namespace phx = boost::phoenix;
        //Hjsonテキスト規則の記述
        this->hjson_text_ %= this->ws_c_ >> (this->root_object_ | this->value_) >> this->ws_c_;
        //コメント規則の記述
        this->comment_ %= this->line_comment_ | this->block_comment_;
        this->line_comment_ %= (qi::lit('#') | qi::lit("//")) >> *(qi::char_ - qi::lit(Literal::LF)) >> qi::lit(Literal::LF);
        this->block_comment_ %= qi::lit(Literal::StartBlockComment) >> *(!qi::lit(Literal::EndBlockComment) >> qi::char_) >> qi::lit(Literal::EndBlockComment);
        //特殊文字規則の記述
        this->ws_c_ %= *(this->comment_ | this->ws_);
        this->ws_ %= +(qi::lit(Literal::Space) | qi::lit(Literal::Tab) | qi::lit(Literal::LF) | qi::lit(Literal::CR));
        //数字規則の記述
        this->number_ %= qi::double_ | qi::int_;
        //統合文字列規則の記述
        this->string_ = (this->json_string_ | this->ml_string_ | this->ql_string_)[qi::_val = phx::construct<Value>(qi::_1, phx::cref(this->charset_))];
        //Json用文字列規則の記述
        this->json_string_ = (qi::lit(Literal::QuotationMark) >> *this->char_ >> qi::lit(Literal::QuotationMark));
        this->char_ = (qi::char_ - (qi::lit(Literal::Escape) | qi::lit(Literal::QuotationMark)))[phx::push_back(qi::_val, qi::_1)]
          | (qi::lit(Literal::Escape) >> (qi::lit(Literal::QuotationMark)[qi::_val = "\""]
            | qi::lit("\\")[qi::_val = "\\"]
            | qi::lit("/")[qi::_val = "/"]
            | qi::lit("b")[qi::_val = "\b"]
            | qi::lit("f")[qi::_val = "\f"]
            | qi::lit("n")[qi::_val = "\n"]
            | qi::lit("r")[qi::_val = "\r"]
            | qi::lit("t")[qi::_val = "\t"]
            | (qi::lit("u") >> qi::repeat(4)[qi::xdigit])[phx::bind(&ParserGrammar::FormatUnicodeEscapeSequence, qi::_val, qi::_1)]));
        //クォーテーションマークなし用文字列規則の記述
        this->ql_string_ %= this->non_punctuator_char_ >> *(!this->ql_end_ >> this->ql_char_);
        this->ql_end_ %= *(qi::lit(Literal::Space) | qi::lit(Literal::Tab) | qi::lit(Literal::CR)) >> qi::lit(Literal::LF);
        this->ql_char_ %= qi::char_(Literal::Space) | qi::char_(Literal::Tab) | qi::char_;
        //複数行文字列用規則の記述
        this->ml_string_ = qi::raw[this->ml_seq_ >>
          *(!this->ml_seq_ >> (this->ml_char_ | this->ml_ignore_))
          >> this->ml_seq_][phx::bind(&ParserGrammar::FormatMultilLineString, qi::_val, qi::_1)];
        this->ml_seq_ %= qi::lit("'''");
        this->ml_char_ %= qi::char_(Literal::Space) | qi::char_(Literal::Tab) | qi::char_(Literal::LF) | qi::char_;
        this->ml_ignore_ %= qi::lit(Literal::CR);
        //値規則の記述
        this->value_ %= this->literal_ | this->object_ | this->array_ | this->number_ | this->string_;
        this->literal_ %= (qi::lit(Literal::False)[qi::_val = phx::construct<Value>(false)]
          | qi::lit(Literal::True)[phx::construct<Value>(true)]
          | qi::lit(Literal::Null)[qi::_val = phx::construct<Value>()])
          >> !this->literal_end_;
        this->literal_end_ %= *(qi::lit(Literal::Space) | qi::lit(Literal::Tab))
          >> (qi::char_ - (qi::lit('#') | qi::lit('/') | qi::lit(Literal::CommaSeparator)
            | qi::lit(Literal::BeginArray) | qi::lit(Literal::EndArray) | qi::lit(Literal::BeginObject) | qi::lit(Literal::EndObject)));
        //値分割規則の記述
        this->value_separator_ %= (this->ws_c_ >> qi::lit(Literal::CommaSeparator) >> this->ws_c_)
          | (*(this->comment_ | +(qi::lit(Literal::Space) | qi::lit(Literal::Tab) | qi::lit(Literal::CR))) >> qi::lit(Literal::LFSeparator) >> this->ws_c_);
        //配列規則の記述
        this->array_ = qi::lit(Literal::BeginArray) >> this->ws_c_ >>
          -(this->value_[phx::push_back(qi::_val, qi::_1)] >> *(this->value_separator_ >> this->value_[phx::push_back(qi::_val, qi::_1)]) >> -this->value_separator_)
          >> this->ws_c_ >> qi::lit(Literal::EndArray);
        //オブジェクト規則の記述
        this->object_ %= qi::lit(Literal::BeginObject) >> this->ws_c_ >>
          -(this->member_ >> *(this->value_separator_ >> this->member_) >> -this->value_separator_)
          >> this->ws_c_ >> qi::lit(Literal::EndObject);
        this->member_ = this->name_ >> this->ws_c_ >> qi::lit(Literal::NameSeparator) >> this->ws_c_ >> this->value_;
        //ルートオブジェクト規則の記述
        this->root_object_ %= this->member_ >> *(this->value_separator_ >> this->member_) >> -this->value_separator_;
        //オブジェクト名規則の記述
        this->name_ %= this->json_string_ | this->keyname_;
        this->keyname_ %= +this->non_punctuator_char_;
        this->non_punctuator_char_ = qi::char_
          - (qi::lit(Literal::CommaSeparator) | qi::lit(Literal::NameSeparator) | qi::lit(Literal::BeginArray) | qi::lit(Literal::EndArray) | qi::lit(Literal::BeginObject) | qi::lit(Literal::EndObject) | this->ws_);
      }
      

      static void InsertString(Value::StringType& Str, std::size_t Position, const Value::StringType& Value) {
        Str.insert(Position, Value);
      }

      static void FormatUnicodeEscapeSequence(Value::StringType& Val, const std::vector<char>& Array) {
        uint16_t ch = 0;
        ch = (Array[2] - '0') << 4;
        ch += (Array[3] - '0') << 0;
        if (Array[0] != 0 || Array[1] != 0) {
          ch += (Array[0] - '0') << 12;
          ch += (Array[1] - '0') << 8;
        }
        std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> converter;
        Val += converter.to_bytes(ch);
      }

      static void FormatMultilLineString(Value::StringType& Val, boost::iterator_range<Iterator> It) {
        auto idx = It.begin(), end = It.end() - 1;
        int indent = 0;
        while (*idx != '\n') {
          if (*idx == ' ') {
            ++indent;
          }
          --idx;
        }
        while (*end != '\n') {
          --end;
        }
        idx = It.begin();
        while (*idx != '\n') {
          ++idx;
        }
        ++idx;
        bool push = false;
        int space = 0;
        for (; idx != end; ++idx) {
          if (*idx == '\n') {
            push = false, space = 0;
            Val.push_back('\n');
          }
          else if (*idx != ' ') {
            push = true;
            Val.push_back(*idx);
          }
          else {
            if (push) {
              Val.push_back(*idx);
            }
            else {
              ++space;
              if (space == indent) {
                push = true;
              }
            }
          }
        }
      }
    };
  }
}