#include "WordList.hpp"
#include "Review.hpp"

using namespace std;

// Constructor
WordList::WordList(const string& reviewFileName, const string& positiveFileName, const string& negativeFileName) {
    reviewFile.open(reviewFileName);

    positiveWordCount = 0;
    negativeWordCount = 0;
    combinedWordCount = 0;

    // Allocate dynamic memory
    positiveWords = new string[3000];
    negativeWords = new string[5000];
    adjDataArray = new WordStruct[10000];
    combinedWords = new string[10000];

    // Convert list of positive and negative words into array
    readAdjective(positiveFileName, positiveWords, positiveWordCount);
    readAdjective(negativeFileName, negativeWords, negativeWordCount);
}

// Function to convert the list of adjectives into array
void WordList::readAdjective(const string& fileName, string wordArray[], int& wordCount) {
    ifstream file(fileName); // Assign file as input stream
    if (!file.is_open()) {
        return;
    }
    string word;
    while (file >> word) {
        wordArray[wordCount++] = word;
    }
    file.close();
}

// Function to perform bubble sort on adjectives
void WordList::bubbleSortAdjectives() {
    for (int i = 0; i < combinedWordCount - 1; i++) {
        for (int j = i + 1; j < combinedWordCount; j++) {
            if (adjDataArray[j].count > adjDataArray[j + 1].count ||
                (adjDataArray[j].count == adjDataArray[j + 1].count && 
                    adjDataArray[j].adjective > adjDataArray[j + 1].adjective)) {

                // Swap counts and words using struct
                WordStruct temp = adjDataArray[i];
                adjDataArray[i] = adjDataArray[j];
                adjDataArray[j] = temp;
            }
            sortTimeEfficiency++;
        }
    }
}

// Function to perform insertion sort on adjectives
void WordList::insertionSortAdjectives() {
    for (int i = 1; i < combinedWordCount; i++) {
        // Store value in temporary variables
        int keyCount = adjDataArray[i].count;
        string keyAdjective = adjDataArray[i].adjective;
        int j = i - 1;

        // Move elements that are greater to the right
        while (j >= 0 && (adjDataArray[j].count > keyCount ||
            (adjDataArray[j].count == keyCount && adjDataArray[j].adjective > keyAdjective))) {
            adjDataArray[j + 1].count = adjDataArray[j].count;
            adjDataArray[j + 1].adjective = adjDataArray[j].adjective;
            j = j - 1;
            sortTimeEfficiency++;
        }
        adjDataArray[j + 1].count = keyCount;
        adjDataArray[j + 1].adjective = keyAdjective;
    }
}

// Function to merge two halves
void WordList::merge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Initialize temporary arrays
    int* leftCounts = new int[n1];
    string* leftAdjectives = new string[n1];
    int* rightCounts = new int[n2];
    string* rightAdjectives = new string[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftCounts[i] = adjDataArray[left + i].count;
        leftAdjectives[i] = adjDataArray[left + i].adjective;
    }
    for (int j = 0; j < n2; j++) {
        rightCounts[j] = adjDataArray[mid + 1 + j].count;
        rightAdjectives[j] = adjDataArray[mid + 1 + j].adjective;
    }

    int i = 0;
    int j = 0;
    int k = left;

    // Merge temporary arrays back to the original arrays
    while (i < n1 && j < n2) {
        sortTimeEfficiency++;
        if (leftCounts[i] < rightCounts[j] ||
            (leftCounts[i] == rightCounts[j] && leftAdjectives[i] < rightAdjectives[j])) {
            adjDataArray[k].count = leftCounts[i];
            adjDataArray[k].adjective = leftAdjectives[i];
            i++;
        }
        else {
            adjDataArray[k].count = rightCounts[j];
            adjDataArray[k].adjective = rightAdjectives[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements 
    while (i < n1) {
        adjDataArray[k].count = leftCounts[i];
        adjDataArray[k].adjective = leftAdjectives[i];
        i++;
        k++;
    }

    // Copy remaining elements
    while (j < n2) {
        adjDataArray[k].count = rightCounts[j];
        adjDataArray[k].adjective = rightAdjectives[j];
        j++;
        k++;
    }
}

// Function to perform merge sort
void WordList::mergeSortAdjectives(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves recursively
        mergeSortAdjectives(left, mid);
        mergeSortAdjectives(mid + 1, right);

        // Merge sorted halves
        merge(left, mid, right);
    }
}

// Destructor
WordList::~WordList() {
    delete[] positiveWords;
    delete[] negativeWords;
    delete[] adjDataArray;
    delete[] combinedWords;
}