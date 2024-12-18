#include "common.h"
#include <sstream>

// Function to generate a random number in a given range
ZZ generate_random_number(const ZZ &a, const ZZ &b) {
    if (a > b) {
        throw invalid_argument("Invalid range for generate_random_number: a > b");
    }
    ZZ range = b - a + 1;
    ZZ random_number;
    do {
        random_number = a + RandomBnd(range);
    } while (random_number > b);
    return random_number;
}

// Convert ZZ to string using ZZToString
string serialize_ZZ(const ZZ& num) {
    ostringstream oss;
    oss << num;
    return oss.str();
}

// Convert string to ZZ using ZZFromString
ZZ unserialize_ZZ(string str) {
    return conv<ZZ>(str.c_str());
}