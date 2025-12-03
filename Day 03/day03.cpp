#include <iostream>
#include <fstream>
#include <string>

using namespace std;

long long answer(int depth = 2) {
    string text;
    ifstream File("./day03.txt");
    long long total = 0;
    
    while(getline(File,text)) {
        string maxNum = "";
        maxNum.reserve(depth);

        int start = 0;
        for (int digit = 1; digit <= depth; digit++) {
            // Find each digit in the depth sized number 
            int maxIndex = start;
            // If the first thing we check is the max possible then great
            if (text[maxIndex] != '9') {
                // Use depth-digit as we need space for the rest of the digits
                for (int i = start+1; i < text.length()-(depth-digit); i++) {
                    if (text[i] > text[maxIndex]) {
                        maxIndex = i;
                        if (text[maxIndex] == '9') {
                            // Already found the maximum possible
                            break;
                        }
                    }
                }
            }
            maxNum += text.substr(maxIndex,1);
            start = maxIndex+1;

        }
        total += stoll(maxNum);
    }

    return total;
}

int main() {
    cout << answer(2) << endl;
    cout << answer(12) << endl;
}