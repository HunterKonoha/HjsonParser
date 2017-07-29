#pragma once

namespace HjsonParser {
  namespace detail {
    namespace Literal {
      constexpr char Space = ' ';//�X�y�[�X
      constexpr char Tab = '\t';//�^�u
      constexpr char LF = '\n';//���s
      constexpr char CR = '\r';//�L�����b�W���^�[��
      constexpr char BeginArray = '[';//�z��̊J�n
      constexpr char EndArray = ']';//�z��̏I��
      constexpr char BeginObject = '{';//�I�u�W�F�N�g�̊J�n
      constexpr char EndObject = '}';//�I�u�W�F�N�g�̏I��
      constexpr char NameSeparator = ':';//���O�̕����g�[�N��
      constexpr char CommaSeparator = ',';//�J���}�̕����g�[�N��
      constexpr char LFSeparator = LF;//���s�̕����g�[�N��
      constexpr char StartBlockComment[] = "/*";//�u���b�N�R�����g�̊J�n
      constexpr char EndBlockComment[] = "*/";//�u���b�N�R�����g�̏I��
      constexpr char True[] = "true";//�^�U�l��true
      constexpr char False[] = "false";//�^�U�l��false
      constexpr char Null[] = "null";//�k���l
      constexpr char Escape = '\\';//�G�X�P�[�v
      constexpr char QuotationMark = '"';//�N�H�[�e�[�V�����}�[�N
    }
  }
}