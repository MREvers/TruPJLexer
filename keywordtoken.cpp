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
	return new string("Keyword");
}

#endif