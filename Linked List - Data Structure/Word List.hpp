#ifndef WORDLIST
#define WORDLIST

#include <string>
#include "Word Node.hpp"

using namespace std;

class WordList {
private:
	WordNode* head = nullptr;
	WordNode* tail = nullptr;
	int count = 0;
public:
	WordList();
	WordList(string fileName);
	WordNode* getHead();
	WordNode* getTail();
	int getCount();
	WordNode* createNewNode(string word);
	void insert(string word);
	void mergeLists(WordList* positive, WordList* negative);
	void bubbleSort();
	void insertionSort();
	WordNode* split(WordNode* temp, int step);
	WordNode* merge(WordNode* left, WordNode* right);
	void splitAndMerge(int step);
	void mergeSort(int length);
};

#endif