// Junji Bressan
// Vijay Duggirala
// Hunter Gerace
// CPSC323 Section 3
#include "Lexical_Analyzer.h"
#include "Syntax_Analyzer.h"

int main()
{
// =========================================================================
// Lexical Analysis
	set<string> keywords;
	vector<char> input;
	queue<string> lexemes;
	vector<Record> finalRecords;

	readKeywords(keywords);
	readInput(input);
	extractLexeme(input, lexemes);
	writeToFile(lexemes, keywords, finalRecords);
// =========================================================================

// =========================================================================
// Syntax Analysis
	vector<string> lines;
	map< Symbols, map<Symbols, int> > table;
	stack<Symbols> ss;
	queue<Record> lex;
	ofstream writeFile;

	linesToAnalyze(lines);
	setupTable(table);
	lexemeQueue(finalRecords, lex);
	writeFile.open("output.txt");
	for(int i = 0; i < lines.size(); i++)
	{
		cout << "Analyzing this line: " << lines[i] << endl;
		syntaxAnalysis(lex, lines[i], ss, table, writeFile);
		cout << endl << endl;
	}
	writeFile.close();
// =========================================================================
// for loop for testing purposes
	for(int i = 0; i < finalRecords.size(); i++)
	{
		cout << finalRecords[i].token << "   " << finalRecords[i].lexem << endl;
	}
	for(int i = 0; i < lines.size(); i++)
	{
		cout << lines[i] << endl;
	}
	// Test logic for string traversal if identifier is more than 1 character
	testTraverse(lines);
	// Test the table
	cout << "table[NTS_S][TS_I]: " << table[NTS_S][TS_I] << endl;
  cout << "table[NTS_A][TS_I]: " << table[NTS_A][TS_I] << endl;
  cout << "table[NTS_E][TS_I]: " << table[NTS_E][TS_I] << endl;
  cout << "table[NTS_E][TS_OPAREN]: " << table[NTS_E][TS_OPAREN] << endl;
  cout << "table[NTS_EP][TS_PLUS]: " << table[NTS_EP][TS_PLUS] << endl;
  cout << "table[NTS_EP][TS_MINUS]: " << table[NTS_EP][TS_MINUS] << endl;
  cout << "table[NTS_EP][TS_CPAREN]: " << table[NTS_EP][TS_CPAREN] << endl;
  cout << "table[NTS_EP][TS_SEMI]: " << table[NTS_EP][TS_SEMI] << endl;
  cout << "table[NTS_T][TS_I]: " << table[NTS_T][TS_I] << endl;
  cout << "table[NTS_T][TS_OPAREN]: " << table[NTS_T][TS_OPAREN] << endl;
  cout << "table[NTS_TP][TS_PLUS]: " << table[NTS_TP][TS_PLUS] << endl;
  cout << "table[NTS_TP][TS_MINUS]: " << table[NTS_TP][TS_MINUS] << endl;
  cout << "table[NTS_TP][TS_MULTI]: " << table[NTS_TP][TS_MULTI] << endl;
  cout << "table[NTS_TP][TS_DIV]: " << table[NTS_TP][TS_DIV] << endl;
  cout << "table[NTS_TP][TS_MOD]: " << table[NTS_TP][TS_MOD] << endl;
  cout << "table[NTS_TP][TS_CPAREN]: " << table[NTS_TP][TS_CPAREN] << endl;
  cout << "table[NTS_TP][TS_SEMI]: " << table[NTS_TP][TS_SEMI] << endl;
  cout << "table[NTS_F][TS_I]: " << table[NTS_F][TS_I] << endl;
  cout << "table[NTS_F][TS_OPAREN]: " << table[NTS_F][TS_OPAREN] << endl;

	return 0;
}
