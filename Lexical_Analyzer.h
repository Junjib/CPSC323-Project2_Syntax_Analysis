#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <cctype>
#include "record.h"
#include <iomanip>

using namespace std;

void readKeywords(set<string>& keywords);
void readInput(vector<char>& input);
void extractLexeme(vector<char>& input, queue<string>& lexemes);
Record lexer(string lexem, set<string>& keywords, vector<Record>& finalRecords);
bool isKeyword(string lexem, set<string>& keywords);
void writeToFile(queue<string>& lexemes, set<string>& keywords, vector<Record>& finalRecords);
bool isIdentifier(string lexem);
bool isNumber(string lexem);
bool isOperator(string lexem);
bool isSeparator(string lexem);

void readKeywords(set<string>& keywords)
{
ifstream read;
string word;

read.open("keywords.txt");

while (read >> word)
{
	keywords.insert(word);
}

read.close();
}

void readInput(vector<char>& input)
{
ifstream read;
char in;

read.open("input3.txt");

while (read >> noskipws >> in)
{
	input.push_back(in);
}

read.close();
}
// ================================================================
// This function will take the input file that is stored in the vector and extract
// any potential lexemes into queue.
void extractLexeme(vector<char>& input, queue<string>& lexemes)
{
char* punctuation = nullptr;
char* keyword_identifier = nullptr;
char* number = nullptr;
bool comment = false;
string kw_idStr;
string punctStr;
string numStr;

for (int i = 0; i < input.size(); i++)
{
	keyword_identifier = &input[i];
	punctuation = &input[i];
	number = &input[i];
	// ================================================================
	// This block of code will extract lexemes that are either keywords or identifiers.
	// Once the lexeme has been extracted into the string it will be pushed to the queue.
	// Once pushed the string will clear itself and prepare for the next lexeme.
	if (!isspace(*keyword_identifier) && !ispunct(*keyword_identifier) && !isdigit(*keyword_identifier))
	{
		kw_idStr += *keyword_identifier;
	}
	else if (!kw_idStr.empty() && isdigit(*keyword_identifier))
	{
		kw_idStr += *keyword_identifier;
	}
	else if (*keyword_identifier == '$')
	{
		kw_idStr += *keyword_identifier;
	}
	else if(!kw_idStr.empty() && comment == false)
	{
		lexemes.push(kw_idStr);
		kw_idStr.clear();
	}
	// If the string kw_idStr is not empty and this is the last iteration of the for loop
	// then push the string kw_idStr into the queue.
	if (!kw_idStr.empty() && i == (input.size() - 1))
	{
		lexemes.push(kw_idStr);
		kw_idStr.clear();
	}
	// ================================================================
	// ================================================================
	// This block of code will extract and push any operators or separators
	// into the queue.
	if (ispunct(*punctuation) && *punctuation != '$')
	{
		// If an ! is being pointed to then there is going to be a block comment;
		// therefore comment will be set to true. So everything after the ! will be
		// ignored.
		if (*punctuation == '!' && comment == false)
		{
			comment = true;
			punctStr = *punctuation;
			lexemes.push(punctStr);
			punctStr.clear();
		}
		// If an ! is being pointed and comment is set to true, then the block comment
		// is at an end. Therefore comment can be set back to false and the vector can be read
		// normally.
		else if (*punctuation == '!' && comment == true )
		{
			comment = false;
			kw_idStr.clear();
		}
		// If there is a floating point being read, then this if statement will ensure that the
		// . symbol is included into the floating point value.
		if (!numStr.empty() && *punctuation == '.')
		{
			numStr += *number;
		}
		else
		{
			punctStr = *punctuation;
			// This if ensures that if you have something like <= or += then punctStr is not
			// overwritten.
			if (!punctStr.empty() && ispunct(input[i+1]) && comment == false)
			{
				lexemes.push(punctStr);
				punctStr.clear();
			}
		}
	}
	// This will ensure that the separator or operator is pushed into the queue in the correct order
	// if you have something like x+y.
	else if (!punctStr.empty() && (kw_idStr.empty() || numStr.empty()) && comment == false)
	{
			lexemes.push(punctStr);
			punctStr.clear();
	}
	// ================================================================
	// ================================================================
	// This block of code will push any whole number or floating point
	// number into the queue.
	if (isdigit(*number) && kw_idStr.empty())
	{
		if (!numStr.empty() && numStr.back() == '.')
		{
			numStr += *number;
		}
		else
		{
			numStr += *number;
		}
	}
	else if (!numStr.empty() && numStr.back() != '.' && comment == false)
	{
		lexemes.push(numStr);
		numStr.clear();
	}
	// ================================================================
}
}
// ================================================================

// Definition of lexer() function

Record lexer(string lexem, set<string>& keywords, vector<Record>& finalRecords){
Record returnVal;
returnVal.lexem = lexem;

if (isKeyword(lexem, keywords))
	returnVal.token = "KEYWORD\t";
else if (isOperator(lexem))
	returnVal.token = "OPERATOR";
else if (isSeparator(lexem))
	returnVal.token = "SEPERATOR";
else
{
	if (isNumber(lexem))
	returnVal.token = "NUMBER";
	else if (isIdentifier(lexem))
	returnVal.token = "IDENTIFIER";
}
finalRecords.push_back(returnVal);
return returnVal;
}

// Definition of isKeyword() Function
bool isKeyword(string str, set<string>& keywords){

if(keywords.find(str) != keywords.end())
	return true;

	return false;
}

// Will loop through the queue and determine which lexemes belong to
// which tokens. Will call functions Record lexer() and other Record functions
// to be defined. When token is determined will write token and lexeme to the
// text file output.txt.
void writeToFile(queue<string>& lexemes, set<string>& keywords, vector<Record>& finalRecords)
{
//ofstream writeFile;
Record tokenLex;


//writeFile.open("output.txt");
//writeFile << "\tTOKENS\t\t\t\tLEXEMES\n\n";

while (!lexemes.empty())
{
	tokenLex = lexer(lexemes.front(), keywords, finalRecords);
	//writeFile << setw(10) << tokenLex.token << "\t = \t" << tokenLex.lexem << endl;
	lexemes.pop();
}

//writeFile.close();
}

bool isIdentifier(string lexem)
{
if(isalpha(lexem[0]))
{
	return true;
}
else
{
	return false;
}
}

bool isNumber(string lexem)
{
	if(isdigit(lexem[0]))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isOperator(string lexem)
{
string operatorslist= "*+-=/><%";
if (operatorslist.find(lexem)!= string::npos){
	return true;
}
return false;
}

bool isSeparator(string lexem){
string separatorList = "'(){}[],.:;! ";
if (separatorList.find(lexem)!= string::npos){
	return true;
}
return false;
}
