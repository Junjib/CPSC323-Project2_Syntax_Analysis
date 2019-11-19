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
#include <algorithm>

using namespace std;

void linesToAnalyze(vector<string>& lines);
void setupTable(map< Symbols, map<Symbols, int> > &table);
bool pushToStack(int rule, stack<Symbols> &ss, ofstream &writeFile);
bool syntaxAnalysis(queue<Record> &lex, string &s, stack<Symbols> &ss);
void lexemeQueue(vector<Record> &finalRecords, queue<Record> &lex, map< Symbols, map<Symbols, int> > &table, ofstream &writeFile);
void testTraverse(vector<string>& lines);
void removeSpaces(string *str);
void writeRecord(queue<Record> &lex, ofstream &writeFile);

// Grabs the relevant strings to analyze
void linesToAnalyze(vector<string>& lines, int& skippedLines)
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
      skippedLines++;
    }
    else if(operatorslist.find(temp[0]) != string::npos)
    {
      discard = temp;
      skippedLines++;
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
      writeFile << "\tS --> A\n";
      ss.pop();
      ss.push(NTS_A); // A
      break;

    case 2: // A --> i=E;
      cout << "\tExecuting case: A --> i=E" << endl;
      writeFile << "\tA --> i=E\n";
      ss.pop();
      ss.push(TS_SEMI); // ;
      ss.push(NTS_E); // E
      ss.push(TS_EQUAL); // =
      ss.push(TS_I); // i
      break;

    case 3: // E --> TE'
      cout << "Executing case: E --> TE'" << endl;
      writeFile << "\tE --> TE'\n";
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      break;

    case 4: // E' --> +TE'
      cout << "Executing case: E' --> +TE'" << endl;
      writeFile << "\tE' --> +TE'\n";
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_PLUS); // +
      break;

    case 5: // E' --> -TE'
      cout << "Executing case: E' --> -TE'" << endl;
      writeFile << "\tE' --> -TE'\n" << endl;
      ss.pop();
      ss.push(NTS_EP); // E'
      ss.push(NTS_T); // T
      ss.push(TS_MINUS); // -
      break;

    case 6: // E' --> epsilon
      cout << "Executing case: E' --> epsilon" << endl;
      writeFile << "\tE' --> epsilon\n";
      ss.pop();
      break;

    case 7: // T --> FT'
      cout << "Executing case: T --> FT'" << endl;
      writeFile << "\tT --> FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      break;

    case 8: // T' --> *FT'
      cout << "Executing case: T' --> *FT'" << endl;
      writeFile << "\tT' --> *FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MULTI); // *
      break;

    case 9: // T' --> /FT'
      cout << "Executing case: T' --> /FT'" << endl;
      writeFile << "\tT' --> /FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_DIV); // /
      break;

    case 10: // T' --> %FT'
      cout << "Executing case: T' --> %FT'" << endl;
      writeFile << "\tT' --> %FT'\n";
      ss.pop();
      ss.push(NTS_TP); // T'
      ss.push(NTS_F); // F
      ss.push(TS_MOD); // %
      break;

    case 11: // T' --> epsilon
      cout << "Executing case: T' --> epsilon" << endl;
      writeFile << "\tT' --> epsilon\n";
      ss.pop();
      break;

    case 12: // F --> i
      cout << "Executing case: F --> i" << endl;
      writeFile << "\tF --> i\n";
      ss.pop();
      ss.push(TS_I); // i
      break;

    case 13: // F --> (E)
      cout << "Executing case: F --> (E)" << endl;
      writeFile << "\tF --> (E)\n";
      ss.pop();
      ss.push(TS_CPAREN); // )
      ss.push(NTS_E); // E
      ss.push(TS_OPAREN); // (
      break;

    default:
      cout << "SYNTAX ERROR!\n";
      writeFile << "SYNTAX ERROR!\n";
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
    lex.push(finalRecords[i]);
    cout << finalRecords[i].token << " " << finalRecords[i].lexem << endl;
  }
}

// This function will commence syntax analysis of a line in the input
bool syntaxAnalysis(queue<Record> &lex, string &s, stack<Symbols> &ss, map< Symbols, map<Symbols, int> > &table, ofstream &writeFile)
{
  char *p = &s[0];
  int i = 0;
  ss.push(TS_EOS);
  ss.push(NTS_S);
  s.push_back('@');

  while(ss.size() > 0)
  {
    if(lexer(*p) == ss.top())
    {
      if (*p != '@'){
        writeRecord(lex, writeFile);
        cout << "call write";
      }
      cout << "Matched symbols: " << *p << endl;
	  // writeFile << "Matched symbols: " << *p << endl;
      ss.pop();
      i++;
      p = &s[i];
    }
    else
    { 
      cout << "Rule: " << table[ss.top()][lexer(*p)] << endl;
	  // writeFile << "Rule: " << table[ss.top()][lexer(*p)] << endl;
      if(!pushToStack(table[ss.top()][lexer(*p)], ss, writeFile)){
        return false;
      }
    }
  }
  return true;
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

// Function to remove all spaces from a given string 
void removeSpaces(string *str) 
{ 
  str->erase(remove(str->begin(), str->end(), ' '), str->end());
}

// Funtion to write current record we are working on
void writeRecord(queue<Record> &lex, ofstream &writeFile){
  if(lex.size()<=0){
    return;
  }
  Record tempRec = lex.front();
  if(tempRec.lexem == "="){
    writeFile << "Token: " << tempRec.token << "\t\tLexeme: " << tempRec.lexem << endl;
    lex.pop();
  }
  else if(tempRec.lexem == "%"){ 
    lex.pop();
    if(lex.front().lexem == tempRec.lexem){
      writeFile << "Token: " << tempRec.token << "\t\tLexeme: "<< tempRec.lexem << tempRec.lexem << endl;
    }
    writeRecord(lex, writeFile);
  }
  else if(tempRec.lexem == "!"){
    lex.pop();
    writeRecord(lex, writeFile);
  }
  else{
    writeFile << "Token: " << tempRec.token << "\t\tLexeme: " << tempRec.lexem << endl;
    lex.pop();
  }
}