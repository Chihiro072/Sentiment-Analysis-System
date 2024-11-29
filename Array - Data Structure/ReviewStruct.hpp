#ifndef REVIEWSTRUCT_HPP
#define REVIEWSTRUCT_HPP

#include <iostream>
#include <string>
using namespace std;

struct ReviewData {
    string reviewText;
    int positiveCount;
    string* foundPositive;
    int negativeCount;
    string* foundNegative;
    int rating;
    double sentiment;
};

#endif