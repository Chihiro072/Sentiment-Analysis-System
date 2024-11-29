#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <iostream>
#include <fstream>
#include "WordStruct.hpp"

using namespace std;

class WordList {
public:
    ifstream reviewFile;
    string* positiveWords = nullptr; 
    string* negativeWords = nullptr; 
    int positiveWordCount = 0;
    int negativeWordCount = 0;
    int totalPositive = 0;
    int totalNegative = 0;
    WordStruct* adjDataArray = nullptr;
    string* combinedWords; 
    int combinedWordCount;
    long long sortTimeEfficiency = 0;

    // Constructor 
    WordList(const string& reviewFileName, const string& positiveFileName, const string& negativeFileName);
    // Convert into arrays
    void readAdjective(const string& fileName, string wordArray[], int& wordCount);
    // Sorting algorithms
    void bubbleSortAdjectives();
    void insertionSortAdjectives();
    void mergeSortAdjectives(int left, int right);
    void merge(int left, int mid, int right);
    // Destructor
    ~WordList();
};

#endif
