#ifndef REVIEW
#define REVIEW

#include <string>
#include "Review Node.hpp"

using namespace std;

class Review {
private:
	ReviewNode* head = nullptr;
	ReviewNode* tail = nullptr;
	WordList* positiveList = new WordList("positive-words.txt");
	WordList* negativeList = new WordList("negative-words.txt");
	int totalPositive = 0;
	int totalNegative = 0;
	int size = 0;
	long long count = 0;
public:
	Review();
	long long getCount();
	void analyzeReview();
	ReviewNode* createNewNode(string review, int rating);
	string cleanWord(string);
	bool linearSearch(WordNode* temp, string word);
	WordNode* getMiddle(WordNode* start, WordNode* end);
	bool binarySearch(WordNode* start, string word);
	bool jumpSearch(WordNode* temp, string word, int listSize);
	double calculateSentiment(int positiveWordCount, int negativeWordCount);
	string classifySentiment(int score);
	void storeWord(ReviewNode* temp);
	void insert(string review, int rating);

	//
	void displayPositiveAndNegativeWords(ReviewNode* temp, int n);
	void displayReviews(ReviewNode* temp);
	void displayAnalysis();
	void displayConsistentReviews();
	void displayInconsistentReviews();
	void displayReviewsBasedOnRating(int rating);
	void displayWordFrequency();
	void displaySummary();
	void writeFile(string review, int rating);
	void addReview();
};

#endif