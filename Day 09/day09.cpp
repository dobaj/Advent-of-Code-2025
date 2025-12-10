#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

long long calcArea (pair<int,int> a, pair<int,int> b) {
    return (long long) (abs(a.first-b.first)+1) * (abs(a.second-b.second)+1);
}

struct CompArea {
    bool operator()(const tuple<long long, int, int>& lhs, const tuple<long long, int, int>& rhs) {
        return get<0>(lhs) < get<0>(rhs);
    }
};

void fillCarpet (vector<vector<char>>& carpet, pair<int,int> start) {
    // Use BFS to fill
    deque<pair<int,int>> queue({start});

    while(queue.size()) {
        auto loc = queue[0];
        queue.pop_front();

        if (carpet[loc.first][loc.second] == 2) continue; // already visited

        carpet[loc.first][loc.second] = 2;

        for (int x = loc.first-1; x <= loc.first+1; x++) {
            for (int y = loc.second-1; y <= loc.second+1; y++) {
                // Avoid diagonals
                if (x >= carpet.size() || y >= carpet.size()) continue;

                if (carpet[x][y] == 0) {
                    queue.push_back(make_pair(x,y));
                }
            }
        }
    }
}

long long answer(bool part1 = false) {
    string text;
    ifstream File("./day09.txt");
    vector<pair<int,int>> redTiles;
    priority_queue <tuple<long long, int, int>, vector<tuple<long long, int, int>>, CompArea> areas;

    int minX = -1, maxX = 0, minY = -1, maxY = 0;
    while (getline(File,text)) {
        size_t sz;
        int x = stoi(text, &sz);
        int y = stoi(text.substr(sz+1), &sz);

        if (x > maxX) maxX = x;
        if (minX == -1 || x < minX) minX = x;
        if (y > maxY) maxY= y;
        if (minY == -1 || y < minY) minY = y;

        redTiles.push_back(make_pair(x,y));
    }

    for (int i = 0; i < redTiles.size(); i++) {
        for (int j = i+1; j < redTiles.size(); j++) {
            long long area = calcArea(redTiles[i], redTiles[j]);
            areas.push(make_tuple(area, i, j));
        }
    }

    if (part1) {
        return get<0>(areas.top());
    }

    // Now shrink all corners so that this carpet isn't ridiculously big

    // Make a map of form x: [y,y]
    map<int, vector<int>> tilesByX;
    for (auto tile: redTiles) {
        if (tilesByX.find(tile.first) == tilesByX.end()) {
            tilesByX.insert({tile.first, vector<int>()});
        }
        tilesByX.at(tile.first).push_back(tile.second);
    }

    // Shrink all gaps to be only 1 long
    int shrunkOffset = minX;
    int x = 0;
    while (x + shrunkOffset <= maxX) {
        while (tilesByX.find(x+shrunkOffset) == tilesByX.end()) {
            // Erase gap
            shrunkOffset++;
        }
        while (tilesByX.find(x+shrunkOffset) != tilesByX.end()) {
            // Now we have a tile
            for (auto tileY: tilesByX.at(x+shrunkOffset)) {
                for (auto & tile: redTiles) {
                    // Modify in place in the vector to keep order
                    if (tile == make_pair(x+shrunkOffset,tileY)) {
                        tile.first = x+1;
                        break;
                    }
                }
            }
            // We need to make sure only adjacent tiles stay adjacent
            x++;
        }
        x++; // Keep a gap space
    }

    // Now make a map of form y: [x,x]
    map<int, vector<int>> tilesByY;
    for (auto tile: redTiles) {
        if (tilesByY.find(tile.second) == tilesByY.end()) {
            tilesByY.insert({tile.second, vector<int>()});
        }
        tilesByY.at(tile.second).push_back(tile.first);
    }

    // Same thing as with x - shrink all gaps to 1
    shrunkOffset = minY;
    int y = 0;
    while (y + shrunkOffset <= maxY) {
        while (tilesByY.find(y+shrunkOffset) == tilesByY.end()) {
            // Erase gaps
            shrunkOffset++;
        }
        while (tilesByY.find(y+shrunkOffset) != tilesByY.end()) {
            // Now we have a tile
            for (auto tileX: tilesByY.at(y+shrunkOffset)) {
                for (auto & tile: redTiles) {
                    // Modify in place in the vector to keep order
                    if (tile == make_pair(tileX,y+shrunkOffset)) {
                        tile.second = y+1;
                        break;
                    }
                }
            }
            // We need to make sure only adjacent tiles stay adjacent
            y++;
        }
        y++; // Keep a gap space
    }

    maxX = x+1;
    maxY = y+1;

    // Make a 2d vector of the carpet
    vector<vector<char>> carpet;
    carpet.reserve(maxX);
    for (int i = 0; i < maxX; i++) {
        carpet.push_back(vector<char>(maxY));
    }

    for (int i = 0; i < redTiles.size(); i++) {
        // Lets build the borders
        pair<int,int> tile = redTiles[i];
        pair<int,int> next = redTiles[(i+1)%redTiles.size()];

        int minX = tile.first, maxX = next.first;
        if (tile.first > next.first) {
            minX = next.first, maxX = tile.first;
        }

        int minY = tile.second, maxY = next.second;
        if (tile.second > next.second) {
            minY = next.second, maxY = tile.second;
        }

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                carpet[x][y] = 1;
            }
        }
    }

    // Fill the outside. Luckily the shape doesn't have holes
    fillCarpet(carpet,make_pair(0,0));
    
    while (areas.size()) {
        // Find the largest area that is entirely red and green
        auto test = areas.top();
        areas.pop();

        auto corner1 = redTiles[get<1>(test)];
        auto corner2 = redTiles[get<2>(test)];

        int minX = corner1.first, maxX = corner2.first;
        if (corner1.first > corner2.first) {
            minX = corner2.first, maxX = corner1.first;
        }

        int minY = corner1.second, maxY = corner2.second;
        if (corner1.second > corner2.second) {
            minY = corner2.second, maxY = corner1.second;
        }

        bool valid = true;

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                // A value of 2 marks the outside we filled
                if (carpet[x][y] == 2) { 
                    valid = false;
                    break;
                }
            }
            if (!valid) break;
        }

        if (valid) {
            return get<0>(test);
        }
    }
    
    return 0;
}

int main() {
    cout << answer(true) << endl;
    cout << answer() << endl;
}