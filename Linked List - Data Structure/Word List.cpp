#include <iostream>
#include <fstream>
#include <sstream>
#include "Word List.hpp"
#include "Word Node.hpp"

using namespace std;

// Default constructor initializes the pointers to nullptr and count to 0
WordList::WordList() {
    head = tail = nullptr;
    count = 0;
}

// Constructor that initializes the list and reads words from a file
WordList::WordList(string fileName) {

    head = tail = nullptr; // Initialize the pointers to hull

    fstream fin; // Create a file stream object for reading the file
    fin.open(fileName, ios::in); // Open the file in read mode
    if (!fin.is_open()) { // Check if the file could not be opened
        cout << "Error: Could not open the file!" << endl; // Print error message
        return; // Exit the constructor if the file could not be opened
    }

    string line;

    // Read each line from the file
    while (getline(fin, line)) {
        stringstream ss(line); // Create a string stream to process the line
        insert(line); // Insert the line (as a word) into the linked list
        count++; // Increase length of word list
    }
    fin.close(); // Close the file after reading all lines
}

// Return the head of the linked list
WordNode* WordList::getHead() {
    return head;
}

// Return the last node in the linked list
WordNode* WordList::getTail() {
    return tail;
}

// Return operation count
int WordList::getCount() {
    return count;
}

// Insert a new word into the linked list
void WordList::insert(string word) {

    WordNode* newNode = createNewNode(word); // Create new node

    // If list is empty, set new node as head
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode; // Link the new node
        tail = newNode;
    }
}

// Create a new WordNode with given word and initializes counts
WordNode* WordList::createNewNode(string word) {

    WordNode* newNode = new WordNode;
    newNode->word = word; // Set the word
    newNode->count = 0; // Initialize count
    newNode->next = nullptr; // Initialize next pointer to nullptr

    return newNode; // Return the new node
}

// Merge the two given linked list
void WordList::mergeLists(WordList* positiveList, WordList* negativeList) {

    WordNode* currentPos = positiveList->getHead(); // Get the head of the positive list
    WordNode* currentNeg = negativeList->getHead(); // Get the head of the negative list

    head = createNewNode(currentPos->word); // Initialize the merged list with the first node from the positive list
    WordNode* currentMerged = head; // Pointer to build the new merged list
    currentPos = currentPos->next;

    // Copy the rest of the positive list
    
    while (currentPos != nullptr || currentNeg != nullptr) {
        if (currentPos != nullptr) {
            currentMerged->next = createNewNode(currentPos->word); // Copy the positive list
            currentMerged = currentMerged->next;
            currentMerged->count = currentPos->count;
            currentPos = currentPos->next;
        }
        
        if (currentNeg != nullptr) {
            currentMerged->next = createNewNode(currentNeg->word); // Copy the negative list
            currentMerged = currentMerged->next;
            currentMerged->count = currentNeg->count;
            currentNeg = currentNeg->next;
        }
    }
    tail = currentMerged;
    tail ->next = nullptr; // Ensure the merged list ends
}

// Sort the linked list of words using bubble sort
void WordList::bubbleSort() {

    if (head == nullptr || head->next == nullptr)
        return;

    bool swapped; // Flag to check if a swap occurred
    WordNode* current; // Current node for iteration
    WordNode* lastSorted = nullptr; // Pointer to the last sorted node

    do {
        swapped = false; // Reset swap flag
        current = head; // Start from head

        // Iterate through the list
        while (current->next != lastSorted) {
            // Compare current node's count with the next node's count
            if (current->count > current->next->count || 
                (current->count == current->next->count && current->word > current->next->word)) {
                // Swap the counts and words if in the wrong order
                int tempData = current->count;
                string tempWord = current->word;
                current->count = current->next->count;
                current->word = current->next->word;
                current->next->count = tempData;
                current->next->word = tempWord;

                swapped = true; // Set swap flag to true
            }
            current = current->next; // Move to the next node
            count++; //Increase operation count by 1
        }
        lastSorted = current; // Update last sorted node
    } while (swapped); // Repeat if swaps occurred
}

// Sort the linked list of words using insertion sort
void WordList::insertionSort() {

    if (head == nullptr || head->next == nullptr)
        return;

    WordNode* sorted = nullptr;  // Start with an empty sorted list
    WordNode* current = head;    // Current node to be sorted

    while (current != nullptr) {
        WordNode* next = current->next; // Save the next node

        // Insert current node into the sorted list
        if (sorted == nullptr || (sorted->count > current->count) ||
            (sorted->count == current->count && sorted->word > current->word)) {
            // Insert at the beginning
            current->next = sorted;
            sorted = current;
            count++;
        }
        else {
            // Find the correct insertion point
            WordNode* temp = sorted;
            while (temp->next != nullptr && (temp->next->count < current->count ||
                (temp->next->count == current->count && temp->next->word < current->word))) {
                temp = temp->next;
                count++;
            }
            // Insert in the correct position
            current->next = temp->next;
            temp->next = current;
        }
        current = next; // Move to the next node in the original list
    }
    head = sorted; // Update head to point to the sorted list
}

// Split the list into two parts
WordNode* WordList::split(WordNode* temp, int step) {

    if (temp == nullptr)
        return temp;

    for (int i = 1; temp->next != nullptr && i < step; i++) {
        temp = temp->next;
    }

    WordNode* next = temp->next;
    temp->next = nullptr;  // Break the list
    return next;
}

// Merge the sorted right and left lists into one sorted list.
WordNode* WordList::merge(WordNode* left, WordNode* right) {

    WordNode* temp = new WordNode();
    WordNode* last = temp;

    // Iterate while there are elements in both left and right lists
    while (left != nullptr && right != nullptr) {
        if (left->count < right->count ||
            (left->count == right->count && left->word < right->word)) {
            last->next = left;
            left = left->next;
        } 
        else {
            last->next = right;
            right = right->next;
        }
        last = last->next;  // Move the tail pointer forward
        count++;
    }

    // Attach any remaining elements in the left or right list
    if (left != nullptr)
        last->next = left;
    else
        last->next = right;

    WordNode* mergedHead = temp->next; // Store the head of the merged list

    delete temp; // Free the temporary node

    return mergedHead;
}

// Split and merge the list
void WordList::splitAndMerge(int step) {

    WordNode* tempHead = new WordNode(); // Temporary node to help with merging
    tempHead->next = head;
    WordNode* prev = tempHead;

    while (head != nullptr) {

        // Split the list into two parts
        WordNode* left = head;
        WordNode* right = split(left, step);
        head = split(right, step);

        // Merge the two parts and attach to the previous node
        WordNode* merged = merge(left, right);
        prev->next = merged;

        // Move previous node to the end of the merged sublist
        while (prev->next != nullptr)
            prev = prev->next;
    }

    head = tempHead->next;
    delete tempHead;  // Free the temporary node
}

// Sort the linked list of words using merge sort
void WordList::mergeSort(int length) {

    if (head == nullptr || head->next == nullptr) 
        return;

    for (int step = 1; step < length; step *= 2)
        splitAndMerge(step);
}

