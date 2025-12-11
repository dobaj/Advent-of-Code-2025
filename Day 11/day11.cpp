#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

long long countPaths (
    unordered_map<string,long long>& memo, 
    unordered_map<string, vector<string>>& graph, 
    string start, string end) {
    if (start == end) {
        return 1;
    }
    if (memo.find(start) != memo.end()) {
        // Avoid having to redo recursion
        return memo[start];
    }

    long long sum = 0;
    for (auto& outs: graph[start]) {
        // The amount of paths is the total of its neighbours' paths to the end
        sum += countPaths(memo, graph, outs, end);
    }

    memo[start] = sum;
    return sum;
}

long long answer(bool part1 = false) {
    string text;
    ifstream File("./day11.txt");
    unordered_map<string, vector<string>> graph;
    unordered_map<string, long long> memo;

    while (getline(File,text)) {
        string device = text.substr(0,text.find(':'));
        text = text.substr(text.find(':')+2);
        
        vector<string> outputs;

        while (!text.empty()) {
            string output = text;
            if (text.find(' ') != string::npos) {
                output = text.substr(0,text.find(' '));
                text = text.substr(text.find(' ')+1);
            } else {
                text = "";
            }
            outputs.push_back(output);
        }

        graph[device] = outputs;
    }
    
    if (part1) {
        return countPaths(memo, graph, "you", "out");
    }

    long long sum = 0;
    // Try both fft visited first and dac first, although 
    // because this is a DAG there will be only one route that works
    vector<vector<string>> routes = 
        {{"svr", "fft", "dac", "out"}, 
        {"svr", "dac", "fft", "out"}};

    for (auto& route: routes) {
        long long prod = 1;
        for (int i = route.size()-2; i >= 0 ; i--) {
            if (prod == 0) break; // No point in continuing
            // Multiply the permutations
            prod *= countPaths(memo, graph, route[i], route[i+1]);
            memo.clear();
        }
        sum+=prod;
    }
    
    return sum;
}

int main() {
    cout << answer(true) << endl;
    cout << answer() << endl;
}