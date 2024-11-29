#ifndef REVIEW_HPP
#define REVIEW_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <sstream>
#include <chrono>
#include "ReviewStruct.hpp"
#include "WordList.hpp"

class Review {
private:
    WordList* adj;
    ifstream reviewFile;
    ofstream reviewOutFile;
    string review;
    string* words = nullptr; // Dynamic array
    int wordCount;
    ReviewData* reviewDataArray; // Dynamic array
    int reviewDataCount = 0;
    int checkConsistency;
    int selectedRating;
    long long timeEfficiency = 0;
public:
    Review(const string& reviewFileName, const string& positiveFileName, const string& negativeFileName);
    void displayMenu();
    void displayMiniMenu();
    void pauseForKeyPress();
    void addReview();
    bool binarySearch(string words[], int count, string& word);
    bool jumpSearch(string words[], int count, string& word);
    bool linearSearch(string words[], int count, string& word);
    int splitReviewAndCountAdj(int& positiveCount, int& negativeCount, string* foundPositive, string* foundNegative);
    string removeSign(string& word);
    int extractRating();
    double calcSentiment(int positive, int negative, int rating);
    string compareRating(int sentiment, int rating);
    void analyzeReviews();
    void displayAnalysis(int checkConsistency = 0, int checkRating = 0);
    void recordAdjective(string adjective);
    void displayMaxMinAdjectives();
    void displaySummary();
    ~Review();
};

#endif