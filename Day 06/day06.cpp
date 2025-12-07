#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long answer(bool part1 = false) {
    string text;
    ifstream File("./day06.txt");
    long long total = 0;

    // Could use pairs but things get ugly
    vector<vector<string>> problems;
    vector<int> opIndices;

    while (getline(File,text)) {
        // Get start locations of the numbers
        if(text.find('+') != string::npos || text.find('*') != string::npos) {
            int i = 0;
            for (auto c : text) {
                if (c == '+' || c == '*') {
                    opIndices.push_back(i);
                    problems.push_back(vector<string>());
                }
                i++;
            }
        }
    }

    // Reset file
    File.clear();
    File.seekg(0);

    while (getline(File,text)) {
        unsigned int vecI = 0, strI = 0;
        if (text.find('+') == string::npos && text.find('*') == string::npos) {
            // Grab numbers
            for (int i = 0; i < opIndices.size(); i++) {
                strI = opIndices[i];
                // Stop at next number or eol
                int end = text.length();
                if (i+1 < opIndices.size()) end = opIndices[i+1];
                
                string num = text.substr(strI,end-strI);

                problems[i].push_back(num);
            }
        } else {
            for (int vecI = 0; vecI < opIndices.size(); vecI++) {
                strI = opIndices[vecI];
                // Convert numbers
                vector<int> numbers;

                if (part1) {
                    // Just convert the strings as is
                    for (auto num : problems[vecI]) {
                        numbers.push_back(stoi(num));
                    }
                } else {
                    // Go through the nums backwards. They should be equal len
                    for(int i = problems[vecI][0].length()-1;
                        i >= 0;
                        i--) {

                        string temp;
                        for (auto num : problems[vecI]) {
                            if(num[i] != ' ') temp += num[i];
                        }
                        if (temp != "") numbers.push_back(stoi(temp));
                    }
                }

                if (text[strI] == '+') {
                    long long sum = 0;
                    for (auto num : numbers) {
                        sum+=num;
                    }
                    total+=sum;
                } else if (text[strI] == '*') {
                    long long product = 1;
                    for (auto num : numbers) {
                        product*=num;
                    }
                    total+=product;
                }
            }
        }
    }
    return total;
}

int main() {
    cout << answer(true) << endl;
    cout << answer() << endl;
}