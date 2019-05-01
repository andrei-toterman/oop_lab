#include "utils.h"
#include <sstream>

vector<string> tokenize(string& line, char delimiter) {
    vector<string> tokens;
    stringstream   ss(line);
    string         token;
    while (getline(ss, token, delimiter)) tokens.push_back(token);
    return tokens;
}
