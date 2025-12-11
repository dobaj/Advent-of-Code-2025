#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <z3++.h>

using namespace std;

int makeLight (int light, vector<short>& buttons) {
    // Use BFS to try and match (pair of form buttons pressed, light)
    deque<tuple<int,short>> queue({make_tuple(0, 0)});

    set<short> history;

    while(queue.size()) {
        auto comp = queue[0];
        queue.pop_front();

        if (light == get<1>(comp)) return get<0>(comp);

        for (short but: buttons) {
            if (but == get<1>(comp)) continue;

            short newLight = get<1>(comp) ^ but;

            if (history.find(newLight) != history.end()) {
                // If we already visited this don't waste our time here
                continue;
            }
            history.insert(get<1>(comp));

            queue.push_back({get<0>(comp)+1, newLight});
        }
    }

    return -1;
}

int part1() {
    string text;
    ifstream File("./day10.txt");
    int total = 0;

    while(getline(File,text)) {
        // Read out light
        string lightStr = text.substr(text.find('[')+1, text.find(']')-text.find('[')-1);
        text = text.substr(text.find(']')+1);
        int light = 0;

        int i = 0;
        for(char c: lightStr) {
            if (lightStr[i] == '#') {
                light |= 1 << i;
            }
            i++;
        }
        
        vector<short> buttons;

        while (text.find('(') != string::npos) {
            // Read button
            string buttonStr = text.substr(text.find('(')+1, text.find(')')-text.find('(')-1);
            text = text.substr(text.find(')')+1);

            int but = 0;
            int i = 0;
            size_t sz;
            while (i <= buttonStr.length()) {
                int x = stoi(buttonStr.substr(i), &sz);
                i+=sz+1;

                but |= 1 << x;
            }

            buttons.push_back(but);
        }

        total += makeLight(light, buttons);     
    }

    return total;
}

long long part2() {
    string text;
    ifstream File("./day10.txt");
    long long total = 0;

    while(getline(File,text)) {
        int lightLen = text.find(']')-text.find('[')-1;
        vector<vector<unsigned char>> buttons(lightLen);

        z3::context c;
        z3::optimize s(c);
        
        // Very important that this comes after the solver for dealloc
        vector<z3::expr> A;
        z3::expr zero = c.int_const("zero");
        z3::expr sum = zero;
        s.add(zero == 0);

        int butI = 0;
        while (text.find('(') != string::npos) {
            // Read button
            string buttonStr = text.substr(text.find('(')+1, text.find(')')-text.find('(')-1);
            text = text.substr(text.find(')')+1);
            
            for (int digit = 0; digit < buttons.size(); digit++) {
                // Init vector in matrix
                buttons[digit].push_back(0);
            }

            // Add the vector as a variable in z3
            stringstream name; 
            name << "a" << butI;
            z3::expr xp = c.int_const(name.str().c_str());
            s.add(xp >= 0);
            A.push_back(xp);
            sum = sum + xp;


            int i = 0;
            size_t sz;
            while (i < buttonStr.length()) {
                int x = stoi(buttonStr.substr(i), &sz);
                i+=sz+1;

                // The matrix at this digit and index is 1
                buttons[x][butI] = 1;
            }

            butI++;
        }
        
        // Read out joltage
        string joltStr = text.substr(text.find('{')+1, text.find('}')-text.find('{')-1);
        vector<unsigned short> jolt;

        int i = 0;
        size_t sz;
        while (i <= joltStr.length()) {
            int x = stoi(joltStr.substr(i), &sz);
            i+=sz+1;
            
            jolt.push_back(x);
        }

        // Add assertions for each jolt number
        for (int b = 0; b < jolt.size(); b++) {

            z3::expr rowSum = zero;
            for (int j = 0; j < buttons[b].size(); j++) {
                if (buttons[b][j]) {
                    rowSum = rowSum + A[j];
                }
            }
            s.add(rowSum == (int)jolt[b]);

        }

        // Let z3 do the solving
        z3::optimize::handle h1 = s.minimize(sum);

        s.check();
        z3::model m = s.get_model();
        long long smallest = m.eval(sum).as_int64();        
        total += smallest;
    }

    return total;
}

int main() {
    cout << part1() << endl;
    cout << part2() << endl;
}