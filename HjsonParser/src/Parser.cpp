#include "Parser.h"
#include "detail/ParserGrammar.h"
#include "detail/StringConverter.h"

HjsonParser::Value HjsonParser::Parse(const std::string & Input, CharacterSet InputCharSet, CharacterSet OutputCharSet) {
  static detail::ParserGrammar<std::string::const_iterator> parser;
  std::string tmp;
  std::string::const_iterator it, end;
  if (InputCharSet != CharacterSet::UTF8) {
    tmp = detail::StringConverter()(Input, InputCharSet, CharacterSet::UTF8);
    it = tmp.cbegin(), end = tmp.cend();
  }
  else {
    it = Input.cbegin(), end = Input.cend();
  }
  parser.charset_ = OutputCharSet;
  Value ret;
  boost::spirit::qi::parse(it, end, parser, ret);
  return ret;
}

HjsonParser::Value HjsonParser::Parse(const std::wstring & Input, CharacterSet InputCharSet, CharacterSet OutputCharSet) {
  static detail::ParserGrammar<std::string::const_iterator> parser;
  std::string tmp = detail::StringConverter()(Input, InputCharSet, CharacterSet::UTF8);
  std::string::const_iterator it, end;
  it = tmp.cbegin(), end = tmp.cend();
  parser.charset_ = OutputCharSet;
  Value ret;
  boost::spirit::qi::parse(it, end, parser, ret);
  return ret;
}