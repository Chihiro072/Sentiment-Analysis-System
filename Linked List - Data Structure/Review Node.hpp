#ifndef REVIEWNODE
#define REVIEWNODE

#include <string>
#include "Word List.hpp"

using namespace std;

struct ReviewNode {
	string review;
	int rating = 0;
	string sentimentRating;
	int positiveWordCount = 0;
	int negativeWordCount = 0;
	double sentimentScore = 0;
	WordList* wordList = new WordList();
	ReviewNode* next = nullptr;
};

#endif