#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int answer() {
    string text;
    ifstream File("./day12.txt");

    int total;
    vector<int> shapes;

    while (getline(File, text) && text.find('x') == string::npos) {
        // We only need to know how many squares the pattern takes up
        int sum = 0;
        
        while (getline(File, text) && !text.empty()) {
            for (char c : text) {
                if (c == '#') {
                    sum+=1;
                }
            }
        }
        shapes.push_back(sum);
    }
    
    while (File) {
        int sum = 0;
        int filledCount = 0; // Amount of tiles the patterns make up

        size_t sz;
        int width = stoi(text,&sz);

        text = text.substr(sz+1);
        int height = stoi(text,&sz);

        int i = 0;
        while (sz+1 < text.length()) {
            text = text.substr(sz+1);
            int num = stoi(text,&sz);

            filledCount+=shapes[i] * num;
            sum+=num;
            i++;
        }
        
        // See if 3x3 boxes fit
        if (width / 3 * height / 3 >= sum) {
            total++;
        }
        else if (filledCount <= width * height) {
            // They might fit then, but it turns out this is never reached. 
            // The presents can either fit as 3x3 or they don't fit at all
            return -1;
        }

        getline(File,text);
    }
    return total;
}

int main() {
    cout << answer() << endl;
}