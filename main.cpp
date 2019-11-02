// Junji Bressan
// Vijay Duggirala
// Hunter Gerace
// CPSC323 Section 3
#include "Lexical_Analyzer.h"

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

// for loop for testing purposes
	for(int i = 0; i < finalRecords.size(); i++)
	{
		cout << finalRecords[i].token << "   " << finalRecords[i].lexem << endl;
	}

	return 0;
}
