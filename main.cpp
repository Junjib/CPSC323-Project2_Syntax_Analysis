// Junji Bressan
// Vijay Duggirala
// Hunter Gerace
// CPSC323 Section 3
#include "Lexical_Analyzer.h"
#include "Syntax_Analyzer.h"

int main()
{
	set<string> keywords;
	vector<char> input;
	queue<string> lexemes;
	vector<Record> finalRecords;

	readKeywords(keywords);
	readInput(input);
	extractLexeme(input, lexemes);
	writeToFile(lexemes, keywords, finalRecords);

	vector<string> lines;

	linesToAnalyze(lines);

// for loop for testing purposes
	for(int i = 0; i < finalRecords.size(); i++)
	{
		cout << finalRecords[i].token << "   " << finalRecords[i].lexem << endl;
	}
	for(int i = 0; i < lines.size(); i++)
	{
		cout << lines[i] << endl;
	}

	return 0;
}
