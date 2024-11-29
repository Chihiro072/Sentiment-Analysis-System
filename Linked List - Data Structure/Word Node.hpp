#ifndef WORDNODE
#define WORDNODE

#include <string>

using namespace std;

struct WordNode {
	string word;
	int count = 0;
	WordNode* next = nullptr;
};

#endif