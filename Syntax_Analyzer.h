#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <fstream>
#include <cctype>
#include "record.h"
#include <iomanip>

using namespace std;

void linesToAnalyze(vector<string>& lines);

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
