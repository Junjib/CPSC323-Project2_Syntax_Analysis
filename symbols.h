#ifndef SYMBOLS_H
#define SYMBOLS_H

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

Symbols lexer(char c)
{
  if(isalpha(c))
  {
    return TS_I;
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
      case 'n': return TS_NUM;
      case 'e': return TS_EMPTY;
      case ',': return TS_COMMA;
      case ';': return TS_SEMI;
      case '@': return TS_EOS;
      default: return TS_INVALID;
    }
  }
}

#endif
