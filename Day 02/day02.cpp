#include <iostream>
#include <fstream>
#include <string>

using namespace std;

long long part1() {
    string text;
    ifstream File("./day02.txt");
    long long total = 0;

    while (getline (File, text)) {
        int i = 0;
        
        while(i < text.length()) { 
            size_t sz;
            long long first, second;

            // get first num
            first = stoll(&text[i], &sz); // crazy this exists

            // get second num
            i += sz+1;
            second = stoll(&text[i], &sz);

            string potentialID;
            for (long long j = first; j <= second; j++) {
                // just use string comparisons hopefully this isn't too slow
                potentialID = to_string(j);
                int len = potentialID.length();

                if (potentialID.substr(0, len/2) == potentialID.substr(len/2)) {
                    total+=j;
                }
            }

            i += sz+1;
        }
    }

    return total;
}

long long part2() {
    string text;
    ifstream File("./day02.txt");
    long long total = 0;

    while (getline (File, text)) {
        int i = 0;
        
        while(i < text.length()) { 
            size_t sz;
            long long first, second;

            // get first num
            first = stoll(&text[i], &sz);

            // get second num
            i += sz+1;
            second = stoll(&text[i], &sz);

            string potentialID;
            for (long long j = first; j <= second; j++) {
                // use string comparisons again (they were faster than regex)

                potentialID = to_string(j);
                int len = potentialID.length();

                // Loop through all possible repetition lengths
                int divides = 2;
                while (divides <= len) {
                    string portion = potentialID.substr(0, len/divides);

                    // Build string back
                    string testString;
                    testString.reserve(len);
                    for (int i = 0; i < divides; i++) {
                        testString+=portion;
                    }

                    if (testString == potentialID) {
                        total+=j;
                        break;
                    }
                    divides++;
                }
            }

            i += sz+1;
        }
    }

    return total;
}

int main() {
    cout << part1() << endl;
    cout << part2() << endl;
}