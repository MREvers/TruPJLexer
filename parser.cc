#include "parser.h"
#include <iostream>

using namespace std;

Parser::Parser(Scanner *the_scanner)
{
   /* Initialize the parser. */
   lex = the_scanner;
   word = lex->next_token();
}

Parser::~Parser()
{
   /* Delete the parser. */
   if (lex != NULL)
   {
      delete lex;
   }
   if (word != NULL)
   {
      delete word;
   }
}

// If we have parsed the entire program, then word
// should be the EOF Token.  This function tests
// that condition.
bool Parser::done_with_input()
{
   return word->get_token_type() == TOKEN_EOF;
}

bool Parser::parse_program()
{
   // PROGRAM -> program identifier ; DECL_LIST BLOCK ;
   // Predict (program identifier ; DECL_LIST BLOCK ;) == {program}

   // Match keyword program, first symbol on RHS
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROGRAM)
   {
      delete word;
      word = lex->next_token();
      cout << "test" << endl;
      if (word->get_token_type() == TOKEN_ID)
      {
         delete word;
         word = lex->next_token();
         cout << "test2" << endl;
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
         {
            delete word;
            word = lex->next_token();
            cout << "test3" << endl;
            if (parse_decl_list())
            {
               cout << "test4" << endl;
               if (parse_block())
               {
                  cout << "test5" << endl;
                  if (word->get_token_type() == TOKEN_PUNC
                     && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
                  {
                     delete word;
                     word = lex->next_token();
                     return true;
                  }
                  else
                  {
                     string *expected = new string("';'");
                     parse_error(expected, word);
                     return false;
                  }
               }
               else
               {
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("';'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         string *expected = new string("identifier");
         parse_error(expected, word);
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword program");
      parse_error(expected, word);
      return false;
   }
}


bool Parser::parse_decl_list()
{
   /* DECL_LIST -> VARIABLE_DECL_LIST PROCEDURE_DECL_LIST

      Predict(VARIABLE_DECL_LIST PROCEDURE_DECL_LIST)
        = First(VARIABLE_DECL_LIST)
       union First (PROCEDURE_DECL_LIST)
       union Follow (DECL_LIST) = {identifier, procedure, begin}

       Note that we don't actually need to check the predict set
       here.  The predict set is used to choose the correct
       production, but there isn't a choice here.

       In addition, we take advantage of C++'s short circuit
       evaluation of Boolean expressions. */

   return parse_variable_decl_list() && parse_procedure_decl_list();
}


bool Parser::parse_variable_decl_list()
{
   if (word->get_token_type() == TOKEN_ID)
   {

      if (parse_variable_decl())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
         {
            delete word;
            word = lex->next_token();
            if (parse_variable_decl_list())
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("';'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if ((word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE))
   {
      return true;
   }
   else
   {
      string *expected = new string("(identifier, keyword begin, keyword procedure)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_variable_decl()
{
   if (word->get_token_type() == TOKEN_ID)
   {
      if (parse_identifier_list())
      {
         if (parse_standard_type())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("identifier");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_procedure_decl_list()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
   {
      delete word;
      word = lex->next_token();
      if (parse_procedure_decl())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
         {
            delete word;
            word = lex->next_token();
            if (parse_procedure_decl_list())
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("';'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
   {

      return true;
   }
   else
   {
      string *expected = new string("(keyword procedure, keyword begin)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_identifier_list()
{
   if (word->get_token_type() == TOKEN_ID)
   {

      if (parse_identifier_list_prm())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("identifier");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_identifier_list_prm()
{
   delete word;
   word = lex->next_token();
   if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
   {
      if (word->get_token_type() == TOKEN_ID)
      {
         delete word;
         word = lex->next_token();
         if (parse_identifier_list_prm())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         string *expected = new string("identifier");
         parse_error(expected, word);
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(',', ':')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_standard_type()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_INT)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_BOOL)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(keyword int, keyword bool)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_block()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
   {
      delete word;
      word = lex->next_token();
      if (parse_stmt_list())
      {
         if (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_END)
         {
            delete word;
            word = lex->next_token();
            return true;
         }
         else
         {
            string *expected = new string("keyword end");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword begin");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_procedure_decl()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
   {
      delete word;
      word = lex->next_token();
      if (word->get_token_type() == TOKEN_ID)
      {
         delete word;
         word = lex->next_token();
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
         {
            delete word;
            word = lex->next_token();
            if (parse_procedure_args())
            {
               if (word->get_token_type() == TOKEN_PUNC
                  && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
               {
                  delete word;
                  word = lex->next_token();
                  if (parse_variable_decl_list())
                  {
                     if (parse_block())
                     {
                        return true;
                     }
                     else
                     {
                        return false;
                     }
                  }
                  else
                  {
                     return false;
                  }
               }
               else
               {
                  string *expected = new string("')'");
                  parse_error(expected, word);
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("'('");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         string *expected = new string("identifier");
         parse_error(expected, word);
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword procedure");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_procedure_args()
{
   if (word->get_token_type() == TOKEN_ID)
   {
      delete word;
      word = lex->next_token();
      if (parse_formal_parm_list())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(identifier, ')')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_formal_parm_list()
{
   if (word->get_token_type() == TOKEN_ID)
   {
      delete word;
      word = lex->next_token();
      if (parse_identifier_list_prm())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON)
         {
            delete word;
            word = lex->next_token();
            if (parse_standard_type())
            {
               if (parse_formal_parm_list_hat())
               {
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("':'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("identifier");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_formal_parm_list_hat()
{
   if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
   {
      delete word;
      word = lex->next_token();
      if (parse_formal_parm_list())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(';', ')')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_stmt_list()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT))
   {

      if (parse_stmt())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
         {
            delete word;
            word = lex->next_token();
            if (parse_stmt_list_prm())
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("';'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
   {
      delete word;
      word = lex->next_token();
      if (parse_stmt_list_prm())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifier, keyword if, keyword while, keyword print, ';')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_stmt_list_prm()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT))
   {
      if (parse_stmt())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
         {
            delete word;
            word = lex->next_token();
            if (parse_stmt_list_prm())
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("';'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_END)
   {
      return true;
   }
   else
   {
      string *expected = new string("(identifier, keyword if, keyword while, keyword print, keyword end)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_stmt()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
   {
      if (parse_if_stmt())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
   {
      if (parse_while_stmt())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
   {
      if (parse_print_stmt())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_ID)
   {
      delete word;
      word = lex->next_token();
      if (parse_ad_hoc_as_pc_tail())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(keyword if, keyword while, keyword print, identifier)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_ad_hoc_as_pc_tail()
{
   if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
         {
            return true;
         }
         else
         {
            string *expected = new string("')'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(':=', '(')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_if_stmt()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         if (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN)
         {
            delete word;
            word = lex->next_token();
            if (parse_block())
            {
               if (parse_if_stmt_hat())
               {
                  return true;
               }
               else
               {
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("keyword then");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword if");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_if_stmt_hat()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_ELSE)
   {
      delete word;
      word = lex->next_token();
      if (parse_block())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(keyword else, ';')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_while_stmt()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         if (word->get_token_type() == TOKEN_KEYWORD
            && static_cast<KeywordToken *>(word)->get_attribute() == KW_LOOP)
         {
            delete word;
            word = lex->next_token();
            if (parse_block())
            {
               return true;
            }
            else
            {
               return false;
            }
         }
         else
         {
            string *expected = new string("keyword loop");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword while");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_print_stmt()
{
   if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("keyword print");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_expr_list()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_NUM)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      delete word;
      word = lex->next_token();
      if (parse_actual_parm_list())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(identifer, num, '(', '+', '-', keyword not, ')')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_actual_parm_list()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_NUM)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         if (parse_actual_parm_list_hat())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifer, num, '(', '+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_actual_parm_list_hat()
{
   if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
   {
      delete word;
      word = lex->next_token();
      if (parse_actual_parm_list())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("(',', ')')");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_expr()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_NUM)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      if (parse_simple_expr())
      {
         if (parse_expr_hat())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifer, num, '(', '+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_expr_hat()
{
   if (word->get_token_type() == TOKEN_RELOP)
   {
      delete word;
      word = lex->next_token();
      if (parse_simple_expr())
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   else if ((word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_LOOP)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN))
   {
      return true;
   }
   else
   {
      string *expected = new string("(')', ',', ';', keyword loop, keyword then)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_simple_expr()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_NUM)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      if (parse_term())
      {
         if (parse_simple_expr_prm())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifer, num, '(', '+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_simple_expr_prm()
{
   if (word->get_token_type() == TOKEN_ADDOP)
   {
      delete word;
      word = lex->next_token();
      if (parse_term())
      {
         if (parse_simple_expr_prm())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if ((word->get_token_type() == TOKEN_RELOP)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_LOOP)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN))
   {
      return true;
   }
   else
   {
      string *expected = new string("(addop, relop, ';', ',', ')', keyword loop, keyword then)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_term()
{
   if ((word->get_token_type() == TOKEN_ID)
      || (word->get_token_type() == TOKEN_NUM)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      if (parse_factor())
      {
         if (parse_term_prm())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifer, num, '(', '+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_term_prm()
{
   if (word->get_token_type() == TOKEN_MULOP)
   {
      delete word;
      word = lex->next_token();
      if (parse_factor())
      {
         if (parse_term_prm())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if ((word->get_token_type() == TOKEN_ADDOP)
      || (word->get_token_type() == TOKEN_RELOP)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
      || (word->get_token_type() == TOKEN_PUNC
         && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_LOOP)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN))
   {
      return true;
   }
   else
   {
      string *expected = new string("(mulop, addop, relop, ';', ',', ')', keyword loop, keyword then)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_factor()
{
   if (word->get_token_type() == TOKEN_ID)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else if (word->get_token_type() == TOKEN_NUM)
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
   {
      delete word;
      word = lex->next_token();
      if (parse_expr())
      {
         if (word->get_token_type() == TOKEN_PUNC
            && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
         {
            delete word;
            word = lex->next_token();
            return true;
         }
         else
         {
            string *expected = new string("')'");
            parse_error(expected, word);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else if ((word->get_token_type() == TOKEN_ADDOP
      && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      delete word;
      word = lex->next_token();
      if (parse_sign())
      {
         if (parse_factor())
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      string *expected = new string("(identifier, num, '(', '+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

bool Parser::parse_sign()
{
   if ((word->get_token_type() == TOKEN_ADDOP
      && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
      || (word->get_token_type() == TOKEN_ADDOP
         && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
      || (word->get_token_type() == TOKEN_KEYWORD
         && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
   {
      delete word;
      word = lex->next_token();
      return true;
   }
   else
   {
      string *expected = new string("('+', '-', keyword not)");
      parse_error(expected, word);
      return false;
   }
}

void Parser::parse_error(string *expected, Token *found)
{
   cout << "Parse error: Expected \"" << *expected << "\", found " << *(word->to_string()) << endl;
   delete expected;
}
