#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>
using namespace std;

enum Symbols
{
  // Terminal Symbols
  TS_IF, // if
  TS_THEN, // then
  TS_ELSE, // else
  TS_ENDIF, // endif
  TS_WHILE, // while
  TS_DO, // do
  TS_WHILEEND, // whileend
  TS_I, // i
  TS_EQUAL, // =
  TS_PLUS, // +
  TS_MINUS, // -
  TS_MULTI, // *
  TS_DIV, // /
  TS_MOD, // %
  TS_OPAREN, // (
  TS_CPAREN, // )
  TS_NUM, // n
  TS_EMPTY, // e
  TS_INT, // int
  TS_FLOAT, // float
  TS_BOOL, // bool
  TS_TRUE, // (true)
  TS_FALSE, // (false)
  TS_COMMA, // ,
  TS_SEMI, // ;
  TS_EOS, // @
  TS_INVALID, // invalid token

  //Non-terminal Symbols
  NTS_S, // S
  NTS_I, // I
  NTS_W, // W
  NTS_A, // A
  NTS_E, // E
  NTS_EP, // E'
  NTS_T, // T
  NTS_TP, // T'
  NTS_F, // F
  NTS_D, // D
  NTS_TYPE, // Ty
  NTS_MOREID, // Mi
  NTS_C // C
};

Symbols lexer(char c, string temp, Symbols topSymbol)
{
  if(temp == "if")
  {
    return TS_IF;
  }
  else if(temp == "then")
  {
    return TS_THEN;
  }
  else if(temp == "else")
  {
    return TS_ELSE;
  }
  else if(temp == "endif")
  {
    return TS_ENDIF;
  }
  else if(temp == "while")
  {
    return TS_WHILE;
  }
  else if(temp == "do")
  {
    return TS_DO;
  }
  else if(temp == "whileend")
  {
    return TS_WHILEEND;
  }
  else if(temp == "int")
  {
    return TS_INT;
  }
  else if(temp == "float")
  {
    return TS_FLOAT;
  }
  else if(temp == "bool")
  {
    return TS_BOOL;
  }
  else if(temp == "true" || temp == "TRUE" || temp == "True")
  {
    return TS_TRUE;
  }
  else if(temp == "false" || temp == "FALSE" || temp == "False")
  {
    return TS_FALSE;
  }
  else if(isalpha(c) || c == '$')
  {
    return TS_I;
  }
  else if(temp.size() != 0 && !isdigit(temp[0]) && isdigit(c))
  {
    return TS_I;
  }
  else if(temp.size() != 0 && isdigit(temp[0]) && isdigit(c))
  {
    return TS_NUM;
  }
  else if (topSymbol == NTS_MOREID && c == ';')
  {
    return TS_EMPTY;
  }
  else
  {
    switch(c)
    {
      case '=': return TS_EQUAL;
      case '+': return TS_PLUS;
      case '-': return TS_MINUS;
      case '*': return TS_MULTI;
      case '/': return TS_DIV;
      case '%': return TS_MOD;
      case '(': return TS_OPAREN;
      case ')': return TS_CPAREN;
      case ',': return TS_COMMA;
      case ';': return TS_SEMI;
      case '@': return TS_EOS;
      default: return TS_EMPTY;
      //default: return TS_INVALID;
    }
  }
}

#endif
