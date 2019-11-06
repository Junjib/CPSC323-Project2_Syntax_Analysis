#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <fstream>
#include <cctype>
#include "record.h"
#include "symbols.h"
#include <iomanip>
#include <map>

using namespace std;

void linesToAnalyze(vector<string>& lines);
void setupTable(map< Symbols, map<Symbols, int> > &table);
void pushToStack(int rule, stack<Symbols> &ss);
void testTraverse(vector<string>& lines);

// Grabs the relevant strings to analyze
void linesToAnalyze(vector<string>& lines)
{
  ifstream readFile;
  string temp;
  string discard;
  string operatorslist= "*+-=/><%";

  readFile.open("input.txt");

  while(getline(readFile, temp))
  {
    if(temp[0] == '!' || temp == "")
    {
      discard = temp;
    }
    else if(operatorslist.find(temp[0]) != string::npos)
    {
      discard = temp;
    }
    else
    {
      lines.push_back(temp);
    }
  }
  readFile.close();
}

// Constructs the table
void setupTable(map< Symbols, map<Symbols, int> > &table)
{
  table[NTS_S][TS_I] = 1;
  table[NTS_A][TS_I] = 2;
  table[NTS_E][TS_I] = 3;
  table[NTS_E][TS_OPAREN] = 3;
  table[NTS_EP][TS_PLUS] = 4;
  table[NTS_EP][TS_MINUS] = 5;
  table[NTS_EP][TS_CPAREN] = 6;
  table[NTS_EP][TS_SEMI] = 6;
  table[NTS_T][TS_I] = 7;
  table[NTS_T][TS_OPAREN] = 7;
  table[NTS_TP][TS_PLUS] = 11;
  table[NTS_TP][TS_MINUS] = 11;
  table[NTS_TP][TS_MULTI] = 8;
  table[NTS_TP][TS_DIV] = 9;
  table[NTS_TP][TS_MOD] = 10;
  table[NTS_TP][TS_CPAREN] = 11;
  table[NTS_TP][TS_SEMI] = 11;
  table[NTS_F][TS_I] = 12;
  table[NTS_F][TS_OPAREN] = 13;
}

// Based on the rule will push rule to stack in reverse order
void pushToStack(int rule, stack<Symbols> &ss)
{
  switch(rule)
  {
    case 1: // S --> A
      ss.pop();
      ss.push(NTS_A); // A
      break;

    case 2: // A --> i=E;
      ss.pop();
      ss.push(TS_SEMI); // ;
      ss.push(NTS_E); // E
      ss.push(TS_EQUAL); // =
      ss.push(TS_I); // i
      break;

    case 3: // E --> TE'
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      break;

    case 4: // E' --> +TE'
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_PLUS); // +
      break;

    case 5: // E' --> -TE'
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_MINUS); // -
      break;

    case 6: // E' --> epsilon
      ss.pop();
      break;

    case 7: // T --> FT'
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      break;

    case 8: // T' --> *FT'
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MULTI); // *
      break;

    case 9: // T' --> /FT'
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_DIV); // /
      break;

    case 10: // T' --> %FT'
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MOD); // %
      break;

    case 11: // T' --> epsilon
      ss.pop();
      break;

    case 12: // F --> i
      ss.pop();
      ss.push(TS_I); // i
      break;

    case 13: // F --> (E)
      ss.pop();
      ss.push(TS_CPAREN); // )
      ss.push(NTS_E); // E
      ss.push(TS_OPAREN); // (
      break;

    default:
      cout << "SYNTAX ERROR!\n";
      break;
  }
}

// This function is here to test the logic for string traversal if the
// identifier is greater than one character (EX: xa1$). Logic similar to this will
// probably have to be implemented in the syntax analysis function.
void testTraverse(vector<string>& lines)
{
  char *c = nullptr;

  string test = lines[0];
  bool isI = false;

  for(int i = 0; i < test.size(); i++)
  {
    c = &test[i];

    if(!isI && lexer(*c) == TS_I)
    {
      isI = true;
      cout << "TS_I\n";

    }
    else if(isI && (isalpha(*c) || isdigit(*c) || *c == '$'))
    {
      cout << "Still i\n";
    }
    else if(isI && (lexer(*c) == TS_EQUAL || lexer(*c) == TS_PLUS || lexer(*c) == TS_SEMI))
    {
      isI = false;
      cout << "not i\n";
      cout << *c << endl;
    }
  }
}
