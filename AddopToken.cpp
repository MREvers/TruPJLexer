#ifndef AddopToken_H
#define AddopToken_H

#include "addoptoken.h"

using namespace std;

AddopToken::AddopToken()
{
	this->set_token_type(TOKEN_ADDOP);
}

AddopToken::~AddopToken()
{

}

AddopToken::AddopToken(addop_attr_type   attr) : AddopToken()
{
	this->set_attribute(attr);
}

void AddopToken::set_attribute(addop_attr_type   attr)
{
	this->attribute = attr;
}

addop_attr_type  AddopToken::get_attribute()
{
	return this->attribute;
}

string* AddopToken::to_string() const
{
	return new string("Addop");
}

#endif