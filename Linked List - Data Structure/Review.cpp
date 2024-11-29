#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Review.hpp"

using namespace std;

// Constructor initializes the head pointer and counts
Review::Review() {
    head = nullptr; // Initialize head of the linked list to nullptr
    tail = nullptr; // Initialize tail of the linked list to nullptr
    totalPositive = 0; // Initialize otal positive words counter
    totalNegative = 0; // Initialize total negative words counter
    size = 0; // Initialize size of the reviews list
    count = 0; // Initialize count of operation
}

// Return operation count
long long Review::getCount() {
    return count;
}

// Creates a new review node with given review and rating
ReviewNode* Review::createNewNode(string review, int rating) {

    ReviewNode* newNode = new ReviewNode;
    newNode->review = review; // Set review text
    newNode->rating = rating; // Set review rating
    newNode->sentimentScore = 0; // Initialize sentiment score to 0
    newNode->next = nullptr; // Initialize next pointer to nullptr

    return newNode; // Return the newly created node
}

// Clean and split words in a review
string Review::cleanWord(string review) {

    string cleanedWord;
    for (char ch : review) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '-')
            cleanedWord += tolower(ch); // Convert to lowercase and ignore punctuation
    }
    return cleanedWord;
}

// Function to perform linear search on the sorted linked list
bool Review::linearSearch(WordNode* temp, string word) {

    while (temp != nullptr) {
        count++; // Increse operation count for each comparison

        if (word == temp->word) {
            temp->count++; // Increase count in the positive list
            return true;
        }
        else if (word < temp->word)
            return false;
        else
            temp = temp->next; // Move to the next node
    }
    return false;
}

// Helper function to find the middle node between 'start' and 'end'
WordNode* Review::getMiddle(WordNode* start, WordNode* end) {

    if (start == nullptr)
        return nullptr;

    WordNode* middle = start;
    WordNode* last = start->next;

    while (last != end) {
        last = last->next;
        if (last != end) {
            middle = middle->next; // Move middle node by 1 step 
            last = last->next; // Move last node by 2 steps
        }
        count++;
    }
    return middle;
}

// Function to perform binary search on the sorted linked list
bool Review::binarySearch(WordNode* start, string word) {

    WordNode* end = nullptr;

    while (start != end) {
        WordNode* middle = getMiddle(start, end); // Find the middle node in the current segment
        count++; // Count the middle search as an operation

        if (middle == nullptr)
            return false;

        if (middle->word == word) {
            middle->count++; // Increase count for the word in the list
            return true;
        }
        else if (word < middle->word)
            end = middle; // Adjust the search range to the left half
        else
            start = middle->next; // Adjust the search range to the right half
    }
    return false;
}

// Function to perform jump search on the sorted linked list
bool Review::jumpSearch(WordNode* head, string word, int listSize) {

    int step = sqrt(listSize); // Define the jump step size (usually √N)
    WordNode* prev = nullptr;
    WordNode* temp = head;

    while (temp != nullptr && word > temp->word) {
        prev = temp; // Track the previous block's start node
        count++; // Increase operation count for each jump

        for (int i = 0; i < step && temp->next != nullptr; i++) {
            temp = temp->next;
            count++;
        }

        if (temp->next == nullptr || word < temp->word)
            break;
    }

    while (prev != nullptr && prev != temp->next) {
        count++; // Increse operation count for each comparison

        if (prev->word == word) {
            prev->count++; // Increase count for the word in the list
            return true; // Word found
        }
        if (prev->word > word)
            return false; // Word not found
        prev = prev->next; // Move to the next node
    }
    return false;
}

// Calculates the sentiment score based on positive and negative word counts
double Review::calculateSentiment(int positiveWordCount, int negativeWordCount) {

    double rawSentimentScore = positiveWordCount - negativeWordCount; // Raw sentiment score
    double totalWordCount = positiveWordCount + negativeWordCount; // Total words count
    double normalizedScore = (rawSentimentScore + totalWordCount) / (2 * totalWordCount); // Normalize the score
    double sentimentScore = 1 + (4 * normalizedScore); // Scale sentiment score to 1-5

    // Ensure sentiment score between 1 and 5
    if (sentimentScore < 1)
        return 1;
    else if (sentimentScore > 5)
        return 5;
    else
        return round(sentimentScore * 100) / 100; // Round and return
}

// Determines sentiment label based on score
string Review::classifySentiment(int score) {

    if (score > 3)
        return to_string(score) + "(Positive.)"; // Positive sentiment
    else if (score < 3)
        return to_string(score) + "(Negative.)"; // Negative sentiment
    else
        return to_string(score) + "(Neutral.)"; // Neutral sentiment
}

// Stores words from the reviews into a word list
void Review::storeWord(ReviewNode* temp) {

    stringstream ss(temp->review); // Create a stringstream for review
    string word;
    while (ss >> word) {
        word = cleanWord(word);
        WordNode* posTemp = positiveList->getHead(); // Get head of the positive words list
        WordNode* negTemp = negativeList->getHead(); // Get head of the negative words list
        //bool posBool = linearSearch(posTemp, word);
        //bool negBool = linearSearch(negTemp, word);
        //bool posBool = binarySearch(posTemp, word);
        //bool negBool = binarySearch(negTemp, word);
        bool posBool = jumpSearch(posTemp, word, positiveList->getCount());
        bool negBool = jumpSearch(negTemp, word, negativeList->getCount());

        if (posBool) {
            temp->positiveWordCount++; // Increase count for positive word of the review
            temp->wordList->insert(word); // Insert the word to the list
            WordNode* wordTemp = temp->wordList->getTail(); // Get the newly inserted word
            wordTemp->count++; // Mark the word as positive
            totalPositive++; // Increase count for total positive word
        }
        else if (negBool) {
            temp->negativeWordCount++; // Increase count for negative word of the review
            temp->wordList->insert(word); // Insert the word to the list
            totalNegative++; // Increase count for total negative word
        }
    }
    temp->sentimentScore = calculateSentiment(temp->positiveWordCount, temp->negativeWordCount);
    temp->sentimentRating = classifySentiment(round(temp->sentimentScore));
}

// Inserts a new review node into the linked list
void Review::insert(string review, int rating) {
    ReviewNode* newNode = createNewNode(review, rating); // Create new node

    // If list is empty, set new node as head and tail
    if (head == nullptr)
        head = tail = newNode;
    else {
        // Link the new node to tail
        tail->next = newNode;
        tail = newNode;
    }
    storeWord(newNode);
    size++; // Increse size of the reviews list
}

// Analyze reviews from a CSV file and store them in the Review object
void Review::analyzeReview() {

    fstream fin;
    fin.open("tripadvisor_hotel_reviews.csv", ios::in);

    // Check if the file opened successfully
    if (!fin.is_open()) {
        cout << "Error: Could not open the file!" << endl;
        return;
    }

    string line, review, rating;
    // Read the file line by line
    while (getline(fin, line)) {
        stringstream ss(line); // Use stringstream to parse the line

        size_t lastComma = line.find_last_of(','); // Find the last comma in the line
        if (lastComma != string::npos) {
            review = line.substr(0, lastComma); // Extract review from the line
            rating = line.substr(lastComma + 1); // Extract rating from the line
        }

        // Skip the header row
        if (review.find("Review") != string::npos)
            continue;
        // Insert the review and rating into the Review object
        insert(review, stoi(rating));
    }
    fin.close(); // Close the file after reading
}

// Display positve and negative words based on n
void Review::displayPositiveAndNegativeWords(ReviewNode* temp, int n) {

    WordNode* wordTemp = temp->wordList->getHead(); // Get head of word list

    // If n == 1, display positve words, if n == 0, display negative words
    while (wordTemp != nullptr) {
        if (wordTemp->count == n)
            cout << wordTemp->word << endl; // Display words
        wordTemp = wordTemp->next; // Move to next word
    }
    cout << endl;
}

// Display a single review and its analysis
void Review::displayReviews(ReviewNode* temp) {

    if (temp == nullptr)
        return; // If no review, exit

    cout << endl;
    cout << temp->review << endl; // Display the review
    cout << endl;

    cout << "Positive Words = " << temp->positiveWordCount << endl; // Display count of positive words
    displayPositiveAndNegativeWords(temp, 1); // Display positve words
    cout << "Negative Words = " << temp->negativeWordCount << endl; // Display count of negative words
    displayPositiveAndNegativeWords(temp, 0); // Display negative words

    cout << "Sentiment score(1 - 5) is " << temp->sentimentScore <<", thus the rating should be equal to " 
        << temp->sentimentRating << endl; // Display sentiment score and rating
    cout << endl;
    cout << "Sentiment Score(1-5) = " << round(temp->sentimentScore) << endl; // Display sentiment score
    cout << "Rating given by user = " << temp->rating << endl; // Display user rating
}

// Display analysis of all reviews
void Review::displayAnalysis() {

    ReviewNode* temp = head; // Start from head

    while (temp != nullptr) {
        displayReviews(temp); // Display current review
        cout << endl;
        cout << "Analysis output:" << endl; // Analysis header
        // Check for consistency between sentiment score and user rating
        if (round(temp->sentimentScore) == temp->rating)
            cout << "User's subjective evaluation matches the sentiment score provided by the analysis. There is a " << 
            "consistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl;
        else
            cout << "User's subjective evaluation does not match the sentiment score provided by the analysis. There " << 
            "is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl;
        cout << endl;

        temp = temp->next; // Move to the next review
    }
}

// Display reviews where sentiment score matches the user rating
void Review::displayConsistentReviews() {

    ReviewNode* temp = head; // Start from head

    while (temp != nullptr) {
        if (round(temp->sentimentScore) == temp->rating) {
            displayReviews(temp); // Display consistent reviews
            cout << endl;
        }
        temp = temp->next; // Move to the next review
    }
}

// Display reviews where sentiment score does not match the user rating
void Review::displayInconsistentReviews() {

    ReviewNode* temp = head; // Start from head

    while (temp != nullptr) {
        if (round(temp->sentimentScore) != temp->rating) {
            displayReviews(temp); // Display inconsistent reviews
            cout << endl;
        }
        temp = temp->next; // Move to the next review
    }
}

// Display reviews based on a specific user rating
void Review::displayReviewsBasedOnRating(int rating) {

    ReviewNode* temp = head; // Start from head

    while (temp != nullptr) {
        if (round(temp->sentimentScore) == rating) {
            displayReviews(temp); // Display reviews with matching rating
            cout << endl;
        }
        temp = temp->next; // Move to the next review
    }
}

// Display frequency of each word that appear more than 1 time
void Review::displayWordFrequency() {

    WordList* mergedList = new WordList();
    mergedList->mergeLists(positiveList, negativeList);

    //mergedList->bubbleSort();
    //mergedList->insertionSort();
    mergedList->mergeSort(positiveList->getCount() + negativeList->getCount());
    count = mergedList->getCount();

    WordNode* temp = mergedList->getHead(); // Get head of the sorted list
    string maxWord = "", minWord = ""; // Variables for max/min word tracking
    int max = 0, min = 0; // Initialize max and min count to 0

    cout << "Frequency of each word used in overall reviews, listed in ascending order based on frequency: " << endl;
    while (temp != nullptr) {
        if (temp->count > 0) {
            cout << temp->word << " = " << temp->count << " times" << endl; // Display word frequency

            if (temp->count != min && min == 0) {
                min = temp->count;
                minWord = temp->word; // Update minword to the new minimum
            }
            else if (temp->count == min)
                minWord += ", " + temp->word; // Append if it matches the current min
            if (temp->count > max) {
                max = temp->count;
                maxWord = temp->word; // Update maxword to the new maximum
            }
            else if (temp->count == max)
                maxWord += ", " + temp->word; // Append if it matches the current max
        }
        temp = temp->next; // Move to the next word
    }
    cout << endl;
    cout << "Maximum used word in the reviews : " << maxWord << endl; // Display maximum used word
    cout << "Minimum used word in the reviews : " << minWord << endl; // Display minimum used word
}

// Display summary of all reviews
void Review::displaySummary() {
    
    cout << "Total Review = " << size << endl; // Display total reviews
    cout << "Total Counts of positive words = " << totalPositive << endl; // Display total positive words
    cout << "Total Counts of negative words = " << totalNegative << endl; // Display total negative words
    cout << endl;
    displayWordFrequency();
}

// Write new review and new rating to the csv file
void Review::writeFile(string review, int rating) {

    ofstream reviewOutFile;  // Declare ofstream inside the function
    reviewOutFile.open("tripadvisor_hotel_reviews.csv", ios::app); // Open the file in append mode

    if (!reviewOutFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;  // Exit the function if the file can't be opened
    }
    review = "\"" + review + "\"";
    // Write the review and rating to the file, appending to the end of the file
    reviewOutFile << review << "," << rating << endl;

    // Close the file
    reviewOutFile.close();

    // Confirm the review was added
    cout << "Review added successfully!" << endl;
}

// Add review to the linked list
void Review::addReview() {

    string newReview;
    int newRating;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any leftover input
    do {
        cout << "Enter New Review: ";
        getline(cin, newReview); // Get the full review

        // Ensure the review is not empty
        if (newReview.empty())
            cout << "Review cannot be empty. Please enter a review." << endl;
    } while (newReview.empty());

    do {
        cout << "Enter New Rating (1-5): ";
        cin >> newRating;

        // Validate user input for rating
        if (cin.fail() || newRating < 1 || newRating > 5) {
            cin.clear(); // Clear input failure flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid rating! Please enter a number between 1 and 5." << endl;
        }
    } while (newRating < 1 || newRating > 5); // Repeat until valid input

    insert(newReview, newRating); // Insert the new review and rating
    writeFile(newReview, newRating); // Write the new review and rating to the file
}
