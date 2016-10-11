#include "PuncToken.h"



PuncToken::PuncToken()
{
	this->set_token_type(TOKEN_PUNC);
}


PuncToken::~PuncToken()
{
}


PuncToken::PuncToken(punc_attr_type attr) : PuncToken()
{
	this->set_attribute(attr);
}

punc_attr_type PuncToken::get_attribute()
{
	return this->attribute;
}

void PuncToken::set_attribute(punc_attr_type attr)
{
	this->attribute = attr;
}

string* PuncToken::to_string() const
{
   string szKWPunc = "";
   if (this->attribute == PUNC_ASSIGN)
   {
      szKWPunc = "PUNC_Assign";
   }
   else if (this->attribute == PUNC_CLOSE)
   {
      szKWPunc = "PUNC_Close";
   }
   else if (this->attribute == PUNC_COLON)
   {
      szKWPunc = "PUNC_Colon";
   }
   else if (this->attribute == PUNC_COMMA)
   {
      szKWPunc = "PUNC_Comma";
   }
   else if (this->attribute == PUNC_NO_ATTR)
   {
      szKWPunc = "PUNC_No_Attr";
   }
   else if (this->attribute == PUNC_OPEN)
   {
      szKWPunc = "PUNC_Open";
   }
   else if (this->attribute == PUNC_SEMI)
   {
      szKWPunc = "PUNC_Semi";
   }
	return new string(szKWPunc);
}