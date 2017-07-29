#pragma once

namespace HjsonParser {
  namespace detail {
    namespace Literal {
      constexpr char Space = ' ';//スペース
      constexpr char Tab = '\t';//タブ
      constexpr char LF = '\n';//改行
      constexpr char CR = '\r';//キャリッジリターン
      constexpr char BeginArray = '[';//配列の開始
      constexpr char EndArray = ']';//配列の終了
      constexpr char BeginObject = '{';//オブジェクトの開始
      constexpr char EndObject = '}';//オブジェクトの終了
      constexpr char NameSeparator = ':';//名前の分割トークン
      constexpr char CommaSeparator = ',';//カンマの分割トークン
      constexpr char LFSeparator = LF;//改行の分割トークン
      constexpr char StartBlockComment[] = "/*";//ブロックコメントの開始
      constexpr char EndBlockComment[] = "*/";//ブロックコメントの終了
      constexpr char True[] = "true";//真偽値のtrue
      constexpr char False[] = "false";//真偽値のfalse
      constexpr char Null[] = "null";//ヌル値
      constexpr char Escape = '\\';//エスケープ
      constexpr char QuotationMark = '"';//クォーテーションマーク
    }
  }
}