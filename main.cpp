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
	int skippedLines = 1; // to count the lines we are skipping. Like comments and blank lines.
	map< Symbols, map<Symbols, int> > table;
	stack<Symbols> ss;
	queue<Record> lex;
	ofstream writeFile;

	linesToAnalyze(lines, skippedLines);
	setupTable(table);
	lexemeQueue(finalRecords, lex);
	writeFile.open("output.txt");
	bool isThereError = false;
	writeRecord(lex, writeFile);
	for(int i = 0; i < lines.size(); i++)
	{
		removeSpaces(&lines[i]);
		cout << "Analyzing this line: " << lines[i] << endl;
		//writeFile << "Analyzing this line: " << lines[i] << endl;
		if(!syntaxAnalysis(lex, lines[i], ss, table, writeFile))
		{
			cout << "There was a Syntax Error in line " << i+skippedLines << endl;
			writeFile << "There was a Syntax Error in line " << i+skippedLines << endl;
			isThereError = true;
			break;
		}
		cout << endl << endl;
		writeFile << endl;
	}
	if(!isThereError){
		writeFile << "Finish.";
		cout<<"Success. No syntax Errors. Please Check Output File."<<endl;
	}
	writeFile.close();
// =========================================================================
// for loop for testing purposes
/******** COMMENT ALL TESTING CODE **********
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
************ END TEST CODE COMMENT *************/
	return 0;
}
