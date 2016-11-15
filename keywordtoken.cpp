#ifndef KeywordToken_H
#define KeywordToken_H

#include "keywordtoken.h"

using namespace std;

KeywordToken::KeywordToken()
{
   this->set_token_type(TOKEN_KEYWORD);
}

KeywordToken::~KeywordToken()
{

}

KeywordToken::KeywordToken(keyword_attr_type   attr) : KeywordToken()
{
   this->set_attribute(attr);
}

void KeywordToken::set_attribute(keyword_attr_type   attr)
{
   this->attribute = attr;
}

keyword_attr_type  KeywordToken::get_attribute()
{
   return this->attribute;
}

string* KeywordToken::to_string() const
{
   string szKWWord = "";
   if (this->attribute == KW_BEGIN)
   {
      szKWWord = "KW_Begin";
   }
   else if (this->attribute == KW_BOOL)
   {
      szKWWord = "KW_Bool";
   }
   else if (this->attribute == KW_ELSE)
   {
      szKWWord = "KW_Else";
   }
   else if (this->attribute == KW_END)
   {
      szKWWord = "KW_End";
   }
   else if (this->attribute == KW_IF)
   {
      szKWWord = "KW_If";
   }
   else if (this->attribute == KW_INT)
   {
      szKWWord = "KW_Int";
   }
   else if (this->attribute == KW_LOOP)
   {
      szKWWord = "KW_Loop";
   }
   else if (this->attribute == KW_NOT)
   {
      szKWWord = "KW_Not";
   }
   else if (this->attribute == KW_NO_ATTR)
   {
      szKWWord = "KW_No_Attr";
   }
   else if (this->attribute == KW_PRINT)
   {
      szKWWord = "KW_Print";
   }
   else if (this->attribute == KW_PROCEDURE)
   {
      szKWWord = "KW_Procedure";
   }
   else if (this->attribute == KW_PROGRAM)
   {
      szKWWord = "KW_Program";
   }
   else if (this->attribute == KW_THEN)
   {
      szKWWord = "KW_Then";
   }
   else if (this->attribute == KW_WHILE)
   {
      szKWWord = "KW_While";
   }
   return new string(szKWWord);
}

#endif