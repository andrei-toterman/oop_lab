#pragma once
#include <string>
#include <vector>

using namespace std;

/*
 * takes a string and splits it by a given character
 * input: line: the string to split
 *        delimiter: the character to split by
 * output: a vector of strings delimited by the given character
 */
vector<string> tokenize(string& line, char delimiter);
