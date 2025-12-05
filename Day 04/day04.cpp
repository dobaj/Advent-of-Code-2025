#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool accessible(vector<vector<int>>& map, int x, int y, int maxX, int maxY) {
    // See if there are less than 4 surrounding paper rolls
    int sum = 0;
    for (int dy = y-1; dy <= y+1; dy++) {
        if (dy < 0 || dy >= maxY) {
            continue;
        }
        for (int dx = x-1; dx <= x+1; dx++) {
            if (dx == x && dy == y) continue;
            if (dx < 0 || dx >= maxX ) continue;
            
            if (map[dy][dx]) {
                sum++;
            }
        } 
    }
    return sum < 4;
}

int answer(bool repeat = true) {
    string text;
    ifstream File("./day04.txt");
    long long total = 0;
    
    int maxY = 0;
    int maxX = 0;

    // Scope out map size
    while(getline(File,text)) {
        if (text.length() > maxX) {
            maxX = text.length();
        }
        maxY++;
    }

    vector<vector<int>> map(maxY, vector<int>(maxX,0));

    // Now fill the map
    
    File.clear();
    File.seekg(0);

    int y = 0;
    while(getline(File,text)) {
        int x = 0;
        for (char& c: text) {
            if (c == '@') {
                map[y][x] = 1;
            }
            x++;
        }
        y++;
    }

    int change = -1;
    while (change != 0) {
        // Sorry for the brute force
        change = 0;
        // See if all paper rolls are accessible
        for (int y = 0; y < maxY; y++) {
            for (int x = 0; x < maxX; x++) {
                if (map[y][x]) {
                    if (accessible(map, x, y, maxX, maxY)) {
                        change++;
                        if (repeat) {
                            map[y][x] = 0;
                        }
                    }
                }
            }
        }
        total+=change;
        if (!repeat) break;
    }
    

    return total;
}

int main() {
    cout << answer(false) << endl;
    cout << answer() << endl;
}