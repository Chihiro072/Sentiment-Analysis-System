#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono> 
#include "Review.hpp"

using namespace std;

void pauseForKeyPress() {
    cout << "Press Enter or any key to continue..." << endl;
    cin.ignore();
    cin.get();     // Wait for the user to press any key
}

// Function to display the main menu and handle user input
int displayMenu() {

    int choice = 0;
    do {
        cout << endl;
        cout << left << setw(80) << setfill('=') << "" << endl;
        cout << "Welcome to the TripAdvisor Hotel Feedback System" << endl;
        cout << left << setw(80) << setfill('=') << "" << endl << endl;
        cout << "1. Display Review Analysis" << endl;
        cout << "2. Display Review Summary" << endl;
        cout << "3. Add New Review" << endl;
        cout << "4. End" << endl;
        cout << endl << "Which one do you want to choose: ";
        cin >> choice;

        // Validate user input
        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input! Please select a number between 1 and 4." << endl;
        }
    } while (choice < 1 || choice > 4); // Repeat until valid input

    system("cls"); // Clear the console screen again

    return choice;
}

void displaySubMenu(int choice, Review& reviews) {

    // Handle sub-menu for choice 1 (View Review with Sentiment Analysis)
    if (choice == 1) {
        int subChoice = 0;

        do {
            cout << "1. Display All Reviews." << endl;
            cout << "2. Display Reviews with Consistent Rating." << endl;
            cout << "3. Display Reviews with Inconsistent Rating." << endl;
            cout << "4. Display Reviews by Rating." << endl;
            cout << "5. Back." << endl;
            cout << "Select Your Choice: ";
            cin >> subChoice;

            // Validate user input
            if (cin.fail() || subChoice < 1 || subChoice > 5) {
                cin.clear(); // Clear error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please select a number between 1 and 5." << endl;
            }
        } while (subChoice < 1 || subChoice > 5); // Repeat until valid input

        system("cls"); // Clear the console screen again

        // Call the appropriate display function based on user choice
        if (subChoice == 1)
            reviews.displayAnalysis();
        else if (subChoice == 2)
            reviews.displayConsistentReviews();
        else if (subChoice == 3)
            reviews.displayInconsistentReviews();
        else if (subChoice == 4) {
            int rating = 0;
            do {
                cout << "Please Insert Rating(1 - 5): ";
                cin >> rating;

                // Validate user input
                if (cin.fail() || rating < 1 || rating > 5) {
                    cin.clear(); // Clear error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid input! Please select a number between 1 and 5." << endl;
                }
            } while (rating < 1 || rating > 5); // Repeat until valid input

            reviews.displayReviewsBasedOnRating(rating); // Display reviews based on the rating
        }
        else
            return;
    }
    else if (choice == 2) {
        auto start = chrono::high_resolution_clock::now(); // Get the starting timepoint

        reviews.displaySummary(); // Display summary

        auto end = chrono::high_resolution_clock::now(); // Get the ending timepoint
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        //cout << "Time Efficiency: " << reviews.getCount() << " operations" << endl;
        //cout << "Time Execution: " << duration.count() << " milliseconds" << endl; // Output the time taken
    }
    else if (choice == 3) {
        reviews.addReview(); // Add review to the csv file
    }
    pauseForKeyPress(); // Wait for user action after display result
    return;
}

int main() {

    cout << "Analyzing Review, Don't Rush...";

    auto start = chrono::high_resolution_clock::now(); // Get the starting timepoint

    Review review;
    review.analyzeReview(); // Analyze existing reviews from the CSV file

    auto end = chrono::high_resolution_clock::now(); // Get the ending timepoint

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start); // Calculate the duration in milliseconds

    system("cls");
    //cout << "Time Efficiency: " << review.getCount() << " operations" << endl;
    //cout << "Time Execution: " << duration.count() << " milliseconds" << endl; // Output the time taken
    //cout << endl;
    int n = 0;
    do {
        n = displayMenu(); // Display the menu and get user choice
        if (n != 4) {
            displaySubMenu(n, review); // Display the sub-menu and get user choice
            system("cls"); // Clear the console screen
        }
            
    } while (n != 4); // Continue until the user chooses to exit
}
