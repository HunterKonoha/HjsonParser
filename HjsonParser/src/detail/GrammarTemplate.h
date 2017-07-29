#pragma once
#include <boost/spirit/include/qi.hpp>
#include "../Value.h"

namespace HjsonParser {
  namespace detail {
    template<typename Iterator, typename ResultType>
    struct GrammarTemplate : boost::spirit::qi::grammar<Iterator, ResultType(void)>{
      template<typename Result>
      using rule = boost::spirit::qi::rule<Iterator, Result(void)>;

      template<typename T>
      GrammarTemplate(const T& Expr) : GrammarTemplate::base_type(Expr) {

      }
    };
  }
}