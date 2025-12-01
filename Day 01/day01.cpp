#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int part1() {
    string text;
    ifstream File("./day01.txt");

    int pin = 50;
    int count = 0;

    while (getline (File, text)) {
        int change = stoi(&text[1]);
        if (text[0] == 'L') {
            // Any change with a mult of 100 has the same effect 
            pin -= change % 100;
            if (pin < 0) {
                pin = 100 + pin; 
            }
        } else {
            pin += change;
            pin %= 100;
        }

        if (pin == 0) count++;
    }

    return count;
}

int part2() {
    string text;
    ifstream File("./day01.txt");

    int pin = 50;
    int count = 0;

    while (getline (File, text)) {
        int change = stoi(&text[1]);
        if (text[0] == 'L') {
            // now count all mults of 100
            count += change / 100;
            
            // If pin is already 0 then cancel out double count
            if (pin == 0) count--;

            pin -= change % 100;
            if (pin < 0) {
                count++;
                pin = 100 + pin; 
            }
            
            if (pin == 0) count++;
        } else {
            pin += change;
            // This counts landing on 0 as well
            count += pin / 100;
            pin %= 100;
        }
    }

    return count;
}

int main() {
    cout << part1() << endl;
    cout << part2() << endl;
}