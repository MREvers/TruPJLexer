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
	return new string("Punctuation");
}