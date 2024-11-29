#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include "Review.hpp"
#include "WordList.hpp"

using namespace std;

int main() {
    string reviewFile = "tripadvisor_hotel_reviews.csv";
    string positiveFile = "positive-words.txt";
    string negativeFile = "negative-words.txt";

    cout << "Analyzing... Don't rush!" << endl << endl;
    Review reviewSystem(reviewFile, positiveFile, negativeFile);
    WordList adjSystem(reviewFile, positiveFile, negativeFile);
    reviewSystem.analyzeReviews();
    system("cls");
    reviewSystem.displayMenu();
    reviewSystem.~Review();
    return 0;
}