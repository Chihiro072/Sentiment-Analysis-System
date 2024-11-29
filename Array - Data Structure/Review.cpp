#include "Review.hpp"

using namespace std;

// Constructor
Review::Review(const string& reviewFileName, const string& positiveFileName, const string& negativeFileName) {
    adj = new WordList("tripadvisor_hotel_reviews.csv", "positive-words.txt", "negative-words.txt");
    adj->adjDataArray = new WordStruct[10000]; 
   
    reviewFile.open(reviewFileName);
    wordCount = 0;

    // Allocate dynamic array
    words = new string[2500];
    reviewDataArray = new ReviewData[22000];
}

// Function to ask user to press any key to continue
void Review::pauseForKeyPress() {
    cout << endl << "Press Enter or any key to continue..." << endl;
    cin.ignore();
    cin.get(); // Wait for the user to press any key
    system("cls");
}

// Function to display the main menu
void Review::displayMenu() {
    string option; // Variable for the user to input
    while (true) {
        system("cls");
        // Display the menu text
        cout << endl << ">>> Main Menu" << endl;
        cout << left << setw(120) << setfill('=') << "" << endl;
        cout << "Welcome to the TripAdvisor Hotel Feedback System" << endl;
        cout << left << setw(120) << setfill('=') << "" << endl << endl;
        cout << "1. Display Review Analysis" << endl;
        cout << "2. Display Review Summary" << endl;
        cout << "3. Add New Review" << endl;
        cout << "4. End" << endl;
        cout << endl << "Which one do you want to choose: ";
        cin >> option; // Get user input
        cout << endl;

        system("cls");
        if (option == "1") {
            displayMiniMenu(); // Display the submenu of the main menu
            pauseForKeyPress();  // Pause program
        }
        else if (option == "2") {
            displaySummary();
            pauseForKeyPress();
        }
        else if (option == "3") {
            addReview();
            pauseForKeyPress();
        }
        else if (option == "4") {
            break; 
        }
        else {
            cout << "Invalid input! Please fill in another option..." << endl;
            pauseForKeyPress(); 
        }
    }
}

// Function to display the sub menu
void Review::displayMiniMenu() {
    string option, ratingOption;
    while (true) {
        // Display the submenu text
        cout << endl << ">>> Main Menu >> Display Review" << endl;
        cout << left << setw(120) << setfill('=') << "" << endl;
        cout << "TripAdvisor Hotel View Review List" << endl;
        cout << left << setw(120) << setfill('=') << "" << endl << endl;
        cout << "1. Display All Review Analysis" << endl;
        cout << "2. Display Consistent Analysis" << endl;
        cout << "3. Display Inconsistent Analysis" << endl;
        cout << "4. Display Selected Rating Analysis" << endl;
        cout << "5. Return Back to the Main Menu" << endl;
        cout << endl << "Which one do you want to choose: ";
        cin >> option;

        if (option == "1") {
            displayAnalysis(); // Display all reviews
            pauseForKeyPress();
        }
        else if (option == "2") {
            displayAnalysis(1); // Display consistent reviews only
            pauseForKeyPress();
        }
        else if (option == "3") {
            displayAnalysis(-1); // Display inconsistent reviews only
            pauseForKeyPress();
        }
        else if (option == "4") {
            // Display rating to select for the filter display
            cout << endl << ">>> Main Menu >> Display Review > By Rating" << endl;
            cout << left << setw(80) << setfill('=') << "" << endl;
            cout << "TripAdvisor Hotel Select Rating List" << endl;
            cout << left << setw(80) << setfill('=') << "" << endl << endl;
            cout << "1. Display All Rating 1 Review" << endl;
            cout << "2. Display All Rating 2 Review" << endl;
            cout << "3. Display All Rating 3 Review" << endl;
            cout << "4. Display All Rating 4 Review" << endl;
            cout << "5. Display All Rating 5 Review" << endl;
            cout << endl << "Which one do you want to choose: ";
            cin >> ratingOption;

            if (ratingOption == "1") displayAnalysis(0, 1); // Only display review with rating of 1
            else if (ratingOption == "2") displayAnalysis(0, 2); // Only display review with rating of 2
            else if (ratingOption == "3") displayAnalysis(0, 3); // Only display review with rating of 3
            else if (ratingOption == "4") displayAnalysis(0, 4); // Only display review with rating of 4
            else if (ratingOption == "5") displayAnalysis(0, 5); // Only display review with rating of 5
            else {
                cout << "Invalid input! Please fill in another option..." << endl;
                pauseForKeyPress();
                break;
            }
            pauseForKeyPress(); // Pause after rating analysis
        }
        else if (option == "5") {
            break; // Exit the submenu and return to the main menu
        }
        else {
            cout << "Invalid input! Please fill in another option..." << endl;
            pauseForKeyPress();
            break;
        }
    }
}

// Function to add new review to the csv
void Review::addReview() {
    ofstream reviewOutFile("tripadvisor_hotel_reviews.csv", ios::app);
    string reviewText;
    int reviewRating;

    cout << "Enter your review: ";
    cin.ignore();
    getline(cin, reviewText);

    cout << "Enter a rating (1 to 5): ";
    cin >> reviewRating;

    // Validate rating input
    while (reviewRating < 1 || reviewRating > 5) {
        cout << "Invalid rating. Please enter a value between 1 and 5: ";
        cin >> reviewRating;
    }

    // Write the review text and rating to the file
    reviewOutFile << "\"" << reviewText << "\"," << reviewRating << endl;
    reviewOutFile.close(); // Close the file

    cout << "Review added successfully!" << endl;
    review = "\"" + reviewText + "\"," + to_string(reviewRating); 

    // Count positive and negative adjectives in the new review
    int positiveCount = 0;
    int negativeCount = 0;
    string positiveArray[500];
    string negativeArray[500];
    splitReviewAndCountAdj(positiveCount, negativeCount, positiveArray, negativeArray);

    // Accumulate total positive and negative counts
    adj->totalPositive += positiveCount;
    adj->totalNegative += negativeCount;
    double sentiment = calcSentiment(positiveCount, negativeCount, reviewRating);

    reviewDataArray[reviewDataCount].reviewText = review;
    reviewDataArray[reviewDataCount].positiveCount = positiveCount;
    reviewDataArray[reviewDataCount].foundPositive = new string[positiveCount];
    reviewDataArray[reviewDataCount].negativeCount = negativeCount;
    reviewDataArray[reviewDataCount].foundNegative = new string[negativeCount];
    reviewDataArray[reviewDataCount].rating = reviewRating;
    reviewDataArray[reviewDataCount].sentiment = sentiment;

    for (int i = 0; i < positiveCount; i++) {
        reviewDataArray[reviewDataCount].foundPositive[i] = positiveArray[i];
    }
    for (int i = 0; i < negativeCount; i++) {
        reviewDataArray[reviewDataCount].foundNegative[i] = negativeArray[i];
    }

    reviewDataCount++;
}

// Function to remove all signs except hyphen 
string Review::removeSign(string& word) {
    string cleanedWord;
    for (char c : word) { // Iterate through each letter within the word
        // Check if letter or number or hypen
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') || c == '-') {
            cleanedWord += tolower(c); // Convert to lowercase 
        }
    }
    return cleanedWord;
}

// Function to extract the rating from the review
int Review::extractRating() {
    size_t lastComma = review.find_last_of(','); // Find the last comma 
    if (lastComma != string::npos) {
        string ratingStr = review.substr(lastComma + 1); // Extract rating
        return stoi(ratingStr); // Convert string to integer
    }
    return 0;
}

// Function to split review into words and check if it is an adjective
int Review::splitReviewAndCountAdj(int& positiveCount, int& negativeCount, string* foundPositive, string* foundNegative) {
    string word = "";
    int count =  0;
    stringstream ss(review);
    while (ss >> word) { // Extract word by space
        word = removeSign(word);
        words[count++] = word;
        // Check for positive word
        //if (binarySearch(adj->positiveWords, adj->positiveWordCount, word)) {
        if (jumpSearch(adj->positiveWords, adj->positiveWordCount, word)) {
        //if (linearSearch(adj->positiveWords, adj->positiveWordCount, word)) {
            foundPositive[positiveCount] = word;
            positiveCount++;
            recordAdjective(word);
        }
        // Check for negative word
        //else if (binarySearch(adj->negativeWords, adj->negativeWordCount, word)) {
        else if (jumpSearch(adj->negativeWords, adj->negativeWordCount, word)) {
        //else if (linearSearch(adj->negativeWords, adj->negativeWordCount, word)) {
            foundNegative[negativeCount] = word;
            negativeCount++;
            recordAdjective(word);
        }
    }
    wordCount = count;
    return count;
}

//Function to perform binary search
bool Review::binarySearch(string words[], int count, string& word) {
    int left = 0, right = count - 1;
    while (left <= right) {
        timeEfficiency++;
        int mid = (left + right) / 2; // Calculate the middle index
        if (words[mid] == word) return true;
        if (words[mid] < word) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}

// Function to perform jump search
bool Review::jumpSearch(string words[], int count, string& word) {
    // Calculate block size to jump
    int blockSize = sqrt(count);
    int start = 0;
    int end = 0;

    // Jump through blocks until the target is within a block range
    while (end < count && words[end] < word) {
        start = end;
        end += blockSize;
        if (end > count - 1) {
            end = count;
        }
        timeEfficiency++;
    }

    // Perform linear search within the identified block
    for (int i = start; i <= end; i++) {
        timeEfficiency++;
        if (words[i] == word) {
            return true;  // Target found
        }
        if (words[i] > word) return false;
    }
    return false;
}

// Function to perform linear search
bool Review::linearSearch(string words[], int count, string& word) {
    for (int i = 0; i < count; i++) {
        timeEfficiency++;
        if (words[i] > word) return false;  
        if (words[i] == word) return true;
    }
    return false;
}

// Function to record the adjective and its frequency 
void Review::recordAdjective(string adjective) {
    // Add adjective count
    for (int i = 0; i < adj->combinedWordCount; i++) {
        if (adj->adjDataArray[i].adjective == adjective) {
            adj->adjDataArray[i].count++;
            return;
        }
    }

    // Add new adjective to the array with a frequency of 1
    adj->adjDataArray[adj->combinedWordCount].adjective = adjective;
    adj->adjDataArray[adj->combinedWordCount].count = 1;
    adj->combinedWordCount++;
}

// Function to calculate the sentiment value
double Review::calcSentiment(int positive, int negative, int rating) {
    int rawSentiment = positive - negative;
    int N = positive + negative;
    double normalizedScore = (double)(rawSentiment - (-N)) / (N - (-N));
    return (1 + (rating * normalizedScore)); // return raw sentiment score
}

// Function to compare sentiment and rating
string Review::compareRating(int sentiment, int rating) {
    if (sentiment == rating) {
        return "User's subjective evaluation matches the sentiment score provided by the analysis. "
            "There is a consistency between the sentiment score generated by the analysis and the "
            "user's evaluation of the sentiment.";
    }
    else {
        return "User's subjective evaluation does not match the sentiment score provided by the analysis. "
            "There is an inconsistency between the sentiment score generated by the analysis and the "
            "user's evaluation of the sentiment.";
    }
}

// Function to analyze review based on condition given
void Review::analyzeReviews() {
    auto start = chrono::high_resolution_clock::now(); // Get start time
    reviewFile.clear();  // Clear flags
    reviewFile.seekg(0);  // Reset file pointer to the beginning
    reviewDataCount = 0;
    while (getline(reviewFile, review)) {
        if (review.find("Review") != string::npos) {
            continue; // Skip header row
        }
        if (review.empty()) {
            break;
        }
        // Count positive and negative adjectives
        int positiveCount = 0;
        int negativeCount = 0;
        string positiveArray[500];
        string negativeArray[500];
        splitReviewAndCountAdj(positiveCount, negativeCount, positiveArray, negativeArray);

        // Accumulate total positive and negative counts
        adj->totalPositive += positiveCount;
        adj->totalNegative += negativeCount;

        int rating = extractRating();
        double sentiment = calcSentiment(positiveCount, negativeCount, rating);

        // Store the review analysis in the array
        reviewDataArray[reviewDataCount].reviewText = review;
        reviewDataArray[reviewDataCount].positiveCount = positiveCount;
        reviewDataArray[reviewDataCount].foundPositive = new string[positiveCount];  // Initialize dynamic array
        reviewDataArray[reviewDataCount].negativeCount = negativeCount;
        reviewDataArray[reviewDataCount].foundNegative = new string[negativeCount]; 
        reviewDataArray[reviewDataCount].rating = rating;
        reviewDataArray[reviewDataCount].sentiment = sentiment;
        
        // Store list of positive words found within the review into array
        for (int i = 0; i < positiveCount; i++) {
            reviewDataArray[reviewDataCount].foundPositive[i] = positiveArray[i];
        }

        // Store list of negative words found within the review into array
        for (int i = 0; i < negativeCount; i++) {
            reviewDataArray[reviewDataCount].foundNegative[i] = negativeArray[i];
        }

        reviewDataCount++; // Iterate to next array index
    }
    //auto stop = chrono::high_resolution_clock::now(); // Get end time
    //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    //cout << "Time Execution: " << duration.count() << " milliseconds" << endl;
    //cout << "Time Efficiency: " << timeEfficiency << " operations" << endl;
}

// Function to display the review analysis
void Review::displayAnalysis(int checkConsistency, int checkRating) {
    for (int i = 0; i < reviewDataCount; i++) {
        string evaluation = " (Neutral)";
        // Set final value of the sentiment
        int roundedSentiment = (reviewDataArray[i].sentiment > 5) ? 5 : round(reviewDataArray[i].sentiment);

        if (roundedSentiment > 3) evaluation = " (Positive)";
        else if (roundedSentiment < 3) evaluation = " (Negative)";

        // Apply display filter conditions
        if (checkConsistency == 1 && (roundedSentiment != reviewDataArray[i].rating)) { // Only display consistent reviews
            continue; 
        }
        if (checkConsistency == -1 && (roundedSentiment == reviewDataArray[i].rating)) { // Only display inconsistent reviews
            continue; 
        }
        if (checkRating != 0 && reviewDataArray[i].rating != checkRating) { // Only display review with a specific rating value
            continue; 
        }

        // Display review analysis
        cout << setfill('=') << setw(120) << "=" << endl;
        cout << "Review: " << reviewDataArray[i].reviewText << endl << endl;
        cout << "Positive Words =  " << reviewDataArray[i].positiveCount << endl;
        for (int j = 0; j < reviewDataArray[i].positiveCount; j++) {
            cout << "- " << reviewDataArray[i].foundPositive[j];
            if (j < reviewDataArray[i].positiveCount - 1) cout << endl;
        }
        cout << endl << endl;
        cout << "Negative Words: " << reviewDataArray[i].negativeCount << endl;
        for (int j = 0; j < reviewDataArray[i].negativeCount; j++) {
            cout << "- " << reviewDataArray[i].foundNegative[j];
            if (j < reviewDataArray[i].negativeCount - 1) cout << endl;
        }
        cout << endl << endl;
        cout << fixed << setprecision(2);
        cout << "Sentiment score (1-5) is " << reviewDataArray[i].sentiment << 
            ", thus the rating should be " << roundedSentiment << evaluation << endl;
        cout << "Rating given by user: " << reviewDataArray[i].rating << endl << endl;
        cout << "Analysis Output:" << endl;
        cout << compareRating(roundedSentiment, reviewDataArray[i].rating) << endl << endl;
    }
}

// Function to display both the least and most used adjectives
void Review::displayMaxMinAdjectives() {
    int minCount = adj->adjDataArray[0].count;
    int maxCount = adj->adjDataArray[adj->combinedWordCount - 1].count;

    // Display maximum adjectives used
    cout << endl << "Maximum used word in the reviews\t: ";
    bool first = true;
    for (int i = 0; i < adj->combinedWordCount; i++) {
        if (adj->adjDataArray[i].count == maxCount) {
            // Check whether need to add comma or not
            if (!first) {
                cout << ", ";
            }
            cout << adj->adjDataArray[i].adjective;
            first = false;
        }
    }

    // Display minimum adjectives used
    cout << endl << "Minimum used word in the reviews\t: ";
    first = true;
    for (int i = 0; i < adj->combinedWordCount; i++) {
        if (adj->adjDataArray[i].count == minCount) {
            if (!first) {
                cout << ", ";
            }
            cout << adj->adjDataArray[i].adjective;
            first = false;
        }
    }
    cout << endl;
}

// Function to display review summary
void Review::displaySummary() {
    auto start = chrono::high_resolution_clock::now();

    // Display summary statistics
    cout << "Total Reviews Processed: " << reviewDataCount << endl;
    cout << "Total Positive Words: " << adj->totalPositive << endl;
    cout << "Total Negative Words: " << adj->totalNegative << endl << endl;

    // Display adjectives used from least to most
    cout << "Adjectives and their counts in ascending order of frequency:" << endl;
    adj->insertionSortAdjectives();
    //adj>bubbleSortAdjectives();
    //adj->mergeSortAdjectives(0, adj->combinedWordCount - 1);

    for (int i = 0; i < adj->combinedWordCount; i++) {
        cout << left << setw(20) << setfill(' ') << adj->adjDataArray[i].adjective << " = " << adj->adjDataArray[i].count << " times" << endl;
    }

    displayMaxMinAdjectives();

    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    //cout << endl << "Time Execution: " << duration.count() << " milliseconds" << endl;
    //cout << "Time Efficiency: " << adj->sortTimeEfficiency << " operations" << endl << endl;
}

// Destructor
Review::~Review() {
    delete[] words;
    delete[] reviewDataArray;
    adj->~WordList();
    delete adj;
}