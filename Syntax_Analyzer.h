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
void traverse(string &line, int &i, string &temp);

// Grabs the relevant strings to analyze
void linesToAnalyze(vector<string>& lines, int& skippedLines)
{
ifstream readFile;
string temp;
string discard;
string operatorslist= "*+-=/><%";

readFile.open("input2.txt");

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
table[NTS_S][TS_IF] = 3;
table[NTS_S][TS_WHILE] = 4;
table[NTS_S][TS_I] = 1;
table[NTS_S][TS_EMPTY] = 2;
table[NTS_S][TS_INT] = 2;
table[NTS_S][TS_FLOAT] = 2;
table[NTS_S][TS_BOOL] = 2;

table[NTS_I][TS_IF] = 5;

table[NTS_W][TS_WHILE] = 6;

table[NTS_A][TS_I] = 7;

table[NTS_E][TS_I] = 8;
table[NTS_E][TS_OPAREN] = 8;
table[NTS_E][TS_NUM] = 8;

table[NTS_EP][TS_PLUS] = 9;
table[NTS_EP][TS_MINUS] = 10;
table[NTS_EP][TS_CPAREN] = 11;
table[NTS_EP][TS_SEMI] = 11;

table[NTS_T][TS_I] = 12;
table[NTS_T][TS_OPAREN] = 12;
table[NTS_T][TS_NUM] = 12;

table[NTS_TP][TS_PLUS] = 16;
table[NTS_TP][TS_MINUS] = 16;
table[NTS_TP][TS_MULTI] = 13;
table[NTS_TP][TS_DIV] = 14;
table[NTS_TP][TS_MOD] = 15;
table[NTS_TP][TS_CPAREN] = 16;
table[NTS_TP][TS_SEMI] = 16;

table[NTS_F][TS_I] = 18;
table[NTS_F][TS_OPAREN] = 17;
table[NTS_F][TS_NUM] = 19;

table[NTS_D][TS_EMPTY] = 21;
table[NTS_D][TS_INT] = 20;
table[NTS_D][TS_FLOAT] = 20;
table[NTS_D][TS_BOOL] = 20;

table[NTS_TYPE][TS_INT] = 22;
table[NTS_TYPE][TS_FLOAT] = 23;
table[NTS_TYPE][TS_BOOL] = 24;

table[NTS_MOREID][TS_EMPTY] = 26;
table[NTS_MOREID][TS_COMMA] = 25;

table[NTS_C][TS_TRUE] = 27;
table[NTS_C][TS_FALSE] = 28;
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

  case 2: // S --> D
    cout << "Executing case: S --> D" << endl;
    writeFile << "\tS --> D\n";
    ss.pop();
    ss.push(NTS_D);
    break;

  case 3: // S --> I
    cout << "Executing case: S --> I" << endl;
    writeFile << "\tS --> I\n";
    ss.pop();
    ss.push(NTS_I);
    break;

  case 4: // S --> W
    cout << "Executing case: S --> W" << endl;
    writeFile << "\tS --> W\n";
    ss.pop();
    ss.push(NTS_W);
    break;

  case 5: // I --> if C then S else S endif
    cout << "Executing case: I --> if C then S else S endif" << endl;
    writeFile << "\tI --> if C then S else S endif\n";
    ss.pop();
    ss.push(TS_ENDIF);
    ss.push(NTS_S);
    ss.push(TS_ELSE);
    ss.push(NTS_S);
    ss.push(TS_THEN);
    ss.push(NTS_C);
    ss.push(TS_IF);
    break;

  case 6: // W --> while C do S whileend
    cout << "Executing case: W --> while C do S whileend" << endl;
    writeFile << "\tW --> while C do S whileend\n";
    ss.pop();
    ss.push(TS_WHILEEND);
    ss.push(NTS_S);
    ss.push(TS_DO);
    ss.push(NTS_C);
    ss.push(TS_WHILE);
    break;

  case 7: // A --> i=E;
    cout << "Executing case: A --> i=E;" << endl;
    writeFile << "\tA --> i=E;\n";
    ss.pop();
    ss.push(TS_SEMI); // ;
    ss.push(NTS_E); // E
    ss.push(TS_EQUAL); // =
    ss.push(TS_I); // i
    break;

  case 8: // E --> TE'
    cout << "Executing case: E --> TE'" << endl;
    writeFile << "\tE --> TE'\n";
    ss.pop();
    ss.push(NTS_EP); // E'
    ss.push(NTS_T); // T
    break;

  case 9: // E' --> +TE'
    cout << "Executing case: E' --> +TE'" << endl;
    writeFile << "\tE' --> +TE'\n";
    ss.pop();
    ss.push(NTS_EP); // E'
    ss.push(NTS_T); // T
    ss.push(TS_PLUS); // +
    break;

  case 10: // E' --> -TE'
    cout << "Executing case: E' --> -TE'" << endl;
    writeFile << "\tE' --> -TE'\n" << endl;
    ss.pop();
    ss.push(NTS_EP); // E'
    ss.push(NTS_T); // T
    ss.push(TS_MINUS); // -
    break;

  case 11: // E' --> epsilon
    cout << "Executing case: E' --> epsilon" << endl;
    writeFile << "\tE' --> epsilon\n";
    ss.pop();
    break;

  case 12: // T --> FT'
    cout << "Executing case: T --> FT'" << endl;
    writeFile << "\tT --> FT'\n";
    ss.pop();
    ss.push(NTS_TP); // T'
    ss.push(NTS_F); // F
    break;

  case 13: // T' --> *FT'
    cout << "Executing case: T' --> *FT'" << endl;
    writeFile << "\tT' --> *FT'\n";
    ss.pop();
    ss.push(NTS_TP); // T'
    ss.push(NTS_F); // F
    ss.push(TS_MULTI); // *
    break;

  case 14: // T' --> /FT'
    cout << "Executing case: T' --> /FT'" << endl;
    writeFile << "\tT' --> /FT'\n";
    ss.pop();
    ss.push(NTS_TP); // T'
    ss.push(NTS_F); // F
    ss.push(TS_DIV); // /
    break;

  case 15: // T' --> %FT'
    cout << "Executing case: T' --> %FT'" << endl;
    writeFile << "\tT' --> %FT'\n";
    ss.pop();
    ss.push(NTS_TP); // T'
    ss.push(NTS_F); // F
    ss.push(TS_MOD); // %
    break;

  case 16: // T' --> epsilon
    cout << "Executing case: T' --> epsilon" << endl;
    writeFile << "\tT' --> epsilon\n";
    ss.pop();
    break;

  case 17: // F --> (E)
    cout << "Executing case: F --> (E)" << endl;
    writeFile << "\tF --> (E)\n";
    ss.pop();
    ss.push(TS_CPAREN); // )
    ss.push(NTS_E); // E
    ss.push(TS_OPAREN); // (
    break;

  case 18: // F --> i
    cout << "Executing case: F --> i" << endl;
    writeFile << "\tF --> i\n";
    ss.pop();
    ss.push(TS_I); // i
    break;

  case 19: // F --> n
    cout << "Executing case: F --> n" << endl;
    writeFile << "\tF --> n\n";
    ss.pop();
    ss.push(TS_NUM);
    break;

  case 20: // D --> Ty i Mi;
    cout << "Executing case: D --> Ty i Mi;" << endl;
    writeFile << "\tD --> Ty i Mi;\n";
    ss.pop();
    ss.push(NTS_MOREID);
    ss.push(TS_I);
    ss.push(NTS_TYPE);
    break;

  case 21: // D --> e;
    cout << "Executing case: D --> e;" << endl;
    writeFile << "\tD --> e;\n";
    ss.pop();
    ss.push(TS_EMPTY);
    break;

  case 22: // Ty --> int
    cout << "Executing case: Ty --> int" << endl;
    writeFile << "\tTy --> int\n";
    ss.pop();
    ss.push(TS_INT);
    break;

  case 23: // Ty --> float
    cout << "Executing case: Ty --> float" << endl;
    writeFile << "\tTy --> float\n";
    ss.pop();
    ss.push(TS_FLOAT);
    break;

  case 24: // Ty --> bool
    cout << "Executing case: Ty --> bool" << endl;
    writeFile << "\tTy --> bool\n";
    ss.pop();
    ss.push(TS_BOOL);
    break;

  case 25: // Mi --> , i Mi
    cout << "Executing case: Mi --> , i Mi" << endl;
    writeFile << "\tMi --> , i Mi\n";
    ss.pop();
    ss.push(NTS_MOREID);
    ss.push(TS_I);
    ss.push(TS_COMMA);
    break;

  case 26: // Mi --> e
    cout << "Executing case: Mi --> e" << endl;
    writeFile << "\tMi --> e\n";
    ss.pop();
    ss.push(TS_EMPTY);
    break;

  case 27: // C --> true
    cout << "Executing case: C --> true" << endl;
    writeFile << "\tC --> true\n";
    ss.pop();
    ss.push(TS_TRUE);
    break;

  case 28: // C --> false
    cout << "Executing case: C --> false" << endl;
    writeFile << "\tC --> false\n";
    ss.pop();
    ss.push(TS_FALSE);
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
bool check = false;
string temp = "";
ss.push(TS_EOS);
ss.push(NTS_S);
s.push_back('@');

while(ss.size() > 0)
{
  if(!check && (isalpha(*p) || isdigit(*p)))
  {
    traverse(s, i, temp);
    check = true;
    p = &s[i];
    //cout << *p << endl;

  }
  if(lexer(*p,temp) == ss.top())
  {
    if (*p != '@'){
      writeRecord(lex, writeFile);
      //cout << "call write ";
    }
    if(temp.size() != 0)
    {
      cout << "Matched symbols: " << temp << endl;
      temp.clear();
      check = false;
    }
    else
    {
      cout << "Matched symbols: " << *p << endl;
    // writeFile << "Matched symbols: " << *p << endl;
    }
    ss.pop();
    i++;
    p = &s[i];
  }
  else
  {
    cout << "Rule: " << table[ss.top()][lexer(*p,temp)] << endl;
  // writeFile << "Rule: " << table[ss.top()][lexer(*p)] << endl;
    if(!pushToStack(table[ss.top()][lexer(*p, temp)], ss, writeFile)){
      return false;
    }
  }
}
return true;
}

// This function is here to test the logic for string traversal if the
// identifier is greater than one character (EX: xa1$). Logic similar to this will
// probably have to be implemented in the syntax analysis function.
/*void testTraverse(vector<string>& lines)
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
}*/

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

// This function will deal with words (identifiers/keywords) and numbers that
// have a length greater than 1
void traverse(string &line, int &i, string &temp)
{
  bool check = false;
  bool dot = false;
  char hold;

  for(int j = i; j < line.size(); j++)
  {
    if(!check && isalpha(line[j]))
    {
      check = true;
      temp += line[j];
      hold = line[j];
    }
    else if(!check && isdigit(line[j]))
    {
      check = true;
      temp += line[j];
      hold = line[j];
    }
    else if(check && isdigit(hold) && (isdigit(line[j])) || line[j] == '.')
    {
      if(!dot && line[j] == '.')
      {
        temp += line[j];
        dot = true;
        i = j;
      }
      else if(dot && line[j] == '.')
      {
        cout << "SYNTAX ERROR: Only one dot (.) is allowed in a number\n";
        break;
      }
      else
      {
        temp += line[j];
        i = j;
      }
    }
    else if(check && isdigit(hold) && !isdigit(line[j]))
    {
      check = false;
      //cout << temp << endl;
      break;
    }
    else if(check && (isalpha(line[j]) || isdigit(line[j]) || line[j] == '$'))
    {
      temp += line[j];
      i = j;
    }
    else if(check && (!isalpha(line[j]) || !isdigit(line[j]) || line[j] != '$'))
    {
      check = false;
      //cout << temp << endl;
      break;
    }
  }
}
