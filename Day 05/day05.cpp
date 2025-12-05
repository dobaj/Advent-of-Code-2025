#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int part1() {
    string text;
    ifstream File("./day05.txt");
    int total = 0;
    vector<pair<long long, long long>> ranges;

    // Get only the ranges
    while(getline(File, text) && text != "") {
        long long first, second;
        size_t sz;

        first = stoll(text, &sz);
        second = stoll(text.substr(sz+1), &sz);

        ranges.push_back(make_pair(first,second));
    }

    while(getline(File, text)) {
        long long test = stoll(text);
        bool fresh = false;
        for (auto &a : ranges){
            if (test >= a.first && test <= a.second) {
                fresh = true; // No need to check further
                break;
            }
        }
        if (fresh) total++;
    }

    return total;
}

long long part2() {
    string text;
    ifstream File("./day05.txt");
    long long total = 0;
    vector<pair<long long, long long>> ranges;

    // Get only the ranges
    while(getline(File, text) && text != "") {
        long long first, second;
        size_t sz;

        first = stoll(text, &sz);
        second = stoll(text.substr(sz+1), &sz);

        ranges.push_back(make_pair(first,second));
    }
    
    // Turns out it's very important to sort first
    sort(
        begin(ranges), 
        end(ranges), 
        [](pair<long long,long long> a, pair<long long,long long> b) 
            {
                return a.first < b.first;
            }
    );

    for (auto &c : ranges) {
        // Shrink all ranges so that they don't overlap anymore
        long long left = c.first;
        long long right = c.second;

        for (auto &b : ranges) {
            if (&c == &b) break; // Don't look at unshrunk ones
            if (b.first == -1) continue; // One of the duplicate ranges

            if (b.first <= left && right <= b.second) {
                // This is completely inside b ignore this
                left = -1;
                right = -1;
                break;
            }
            else if (left < b.first && b.first <= right && right <= b.second) {
                // Right side dips into b
                right = b.first-1;
            }
            else if (b.first <= left && left <= b.second && b.second < right) {
                // Left side dips into b
                left = b.second+1;
            }
             
        }
        
        c.first = left;
        c.second = right;
        if (left != -1) {
            total += right - left + 1;
        }
    }
    
    return total;
}

int main() {
    cout << part1() << endl;
    cout << part2() << endl;
}