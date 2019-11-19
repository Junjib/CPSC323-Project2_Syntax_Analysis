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
bool pushToStack(int rule, stack<Symbols> &ss, ofstream &writeFile);
void syntaxAnalysis(queue<Record> &lex, string &s, stack<Symbols> &ss);
void lexemeQueue(vector<Record> &finalRecords, queue<Record> &lex, map< Symbols, map<Symbols, int> > &table, ofstream &writeFile);
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
// returns true if successful, Returns false if error occurs
bool pushToStack(int rule, stack<Symbols> &ss, ofstream &writeFile)
{
  switch(rule)
  {
    case 1: // S --> A
      cout << "Executing case: S --> A" << endl;
      writeFile << "Executing case: S --> A\n";
      ss.pop();
      ss.push(NTS_A); // A
      break;

    case 2: // A --> i=E;
      cout << "Executing case: A --> i=E" << endl;
      writeFile << "Executing case: A --> i=E\n";
      ss.pop();
      ss.push(TS_SEMI); // ;
      ss.push(NTS_E); // E
      ss.push(TS_EQUAL); // =
      ss.push(TS_I); // i
      break;

    case 3: // E --> TE'
      cout << "Executing case: E --> TE'" << endl;
      writeFile << "Executing case: E --> TE'\n";
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      break;

    case 4: // E' --> +TE'
      cout << "Executing case: E' --> +TE'" << endl;
      writeFile << "Executing case: E' --> +TE'\n";
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_PLUS); // +
      break;

    case 5: // E' --> -TE'
      cout << "Executing case: E' --> -TE'" << endl;
      writeFile << "Executing case: E' --> -TE'\n" << endl;
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_MINUS); // -
      break;

    case 6: // E' --> epsilon
      cout << "Executing case: E' --> epsilon" << endl;
      writeFile << "Executing case: E' --> epsilon\n";
      ss.pop();
      break;

    case 7: // T --> FT'
      cout << "Executing case: T --> FT'" << endl;
      writeFile << "Executing case: T --> FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      break;

    case 8: // T' --> *FT'
      cout << "Executing case: T' --> *FT'" << endl;
      writeFile << "Executing case: T' --> *FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MULTI); // *
      break;

    case 9: // T' --> /FT'
      cout << "Executing case: T' --> /FT'" << endl;
      writeFile << "Executing case: T' --> /FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_DIV); // /
      break;

    case 10: // T' --> %FT'
      cout << "Executing case: T' --> %FT'" << endl;
      writeFile << "Executing case: T' --> %FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MOD); // %
      break;

    case 11: // T' --> epsilon
      cout << "Executing case: T' --> epsilon" << endl;
      writeFile << "Executing case: T' --> epsilon\n";
      ss.pop();
      break;

    case 12: // F --> i
      cout << "Executing case: F --> i" << endl;
      writeFile << "Executing case: F --> i\n";
      ss.pop();
      ss.push(TS_I); // i
      break;

    case 13: // F --> (E)
      cout << "Executing case: F --> (E)" << endl;
      writeFile << "Executing case: F --> (E)\n";
      ss.pop();
      ss.push(TS_CPAREN); // )
      ss.push(NTS_E); // E
      ss.push(TS_OPAREN); // (
      break;

    default:
      cout << "SYNTAX ERROR!\n";
      return false;
      break;
  }
  return true;
}

// This function will load the elements in the vector finalRecords into a queue
void lexemeQueue(vector<Record> &finalRecords, queue<Record> &lex)
{
  for(int i = 0; i < finalRecords.size(); i++)
  {
    cout << finalRecords[i].token << " " << finalRecords[i].lexem << endl;
    lex.push(finalRecords[i]);
  }
}

// This function will commence syntax analysis of a line in the input
void syntaxAnalysis(queue<Record> &lex, string &s, stack<Symbols> &ss, map< Symbols, map<Symbols, int> > &table, ofstream &writeFile)
{
  char *p = &s[0];
  int i = 0;
  ss.push(TS_EOS);
  ss.push(NTS_S);
  s.push_back('@');

  // TODO: Don't know where this should be done in the function but we need to write
  // the token and lexeme of the symbol we are analyzing to the output file. One of the
  // parameters to this function is a queue of Record however there are ! and %
  // (at least in this input file) that we need to ignore. Can't say I'm sure what
  // the logic should be to get this done.
  // TODO: Everything needs to be written to an output file. I already set up the
  // the file to be written to all that needs to be done is to reformat it so that
  // it looks similar to the professor's sample output file.
  while(ss.size() > 0)
  {
    if(lexer(*p) == ss.top())
    {
      cout << "Matched symbols: " << *p << endl;
	  writeFile << "Matched symbols: " << *p << endl;
      ss.pop();
      i++;
      p = &s[i];
      // TODO: Need to be able to traverse the string pointer by one character
      // while avoiding whitespace. Currently pointer will traverse fine if there is
      // no whitespace in the string.
      // Optional: Include logic for identifiers that are more than one character
      // long. An example of this logic can be found in the testTraverse() function.
    }
    else
    {
      cout << "Rule: " << table[ss.top()][lexer(*p)] << endl;
	  writeFile << "Rule: " << table[ss.top()][lexer(*p)] << endl;
      if(!pushToStack(table[ss.top()][lexer(*p)], ss, writeFile)){
        break;
      }
      // TODO: If there is no valid rule then we have an error while the pushToStack()
      // function will display an error message the function will continue parsing.
      // Find a way to implement logic that will break out of the loop and exit the
      // function.
      // Optional: include an error handler function that will point out the error
      // and either write it to the text file or print it on the screen.
      // Optional(Probably not worth doing right now): include logic to where upon
      // an error, the error will written to the file and screen, but the function
      // will keep on parsing incase there are other syntax errors.
    }
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
