#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long answer(bool part1 = false) {
    string text;
    ifstream File("./day07.txt");
    int counter = 0;
    long long total = 0;
    // Store how many beams are in each row / col 
    vector<vector<long long>> splits({vector<long long>()});

    // Start initial beam
    getline(File,text);
    for (auto c: text) {
        if (c == 'S') {
            splits[0].push_back(1); 
        } else {
            splits[0].push_back(0);
        }
    }

    int y = 1, x = 0;
    while (getline(File,text)) {
        x = 0;
        // Duplicate row above like a waterfall
        splits.push_back(splits[y-1]);
        for (auto c: text) {
            if (c == '^') {
                long long above = splits[y-1][x];
                if (above > 0) {
                    // We have a beam to split
                    counter++;
                    splits[y][x-1] += above;
                    splits[y][x] = 0; // Stop the beam in this col
                    splits[y][x+1] += above;
                }
            }
            x++;
        }
        y++;
    }

    // For part 1 we don't need to add up the total
    if (part1) return counter;

    // Sum up the last row a la pascal's triangle to get number of paths
    for (auto c: splits[y-1]) {
        total+=c;
    }

    return total;
}

int main() {
    cout << answer(true) << endl;
    cout << answer() << endl;
}