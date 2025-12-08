#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

long long distance(vector<int>& a, vector<int>& b) {
    // Get euclidean distance
    long long sum = 0;
    for (int i = 0; i < 3; i++) {
        long long temp = a[i] - b[i];
        sum+=temp*temp;
    }
    return sum;
}

struct CompDist {
    bool operator()(const pair<long long,int>& lhs, const pair<long long,int>& rhs) {
        return lhs.first > rhs.first;
    }
};

long long answer(bool part1 = false) {
    string text;
    ifstream File("./day08.txt");
    long long total = 1;
    vector<vector<int>> nodes;
    // Each node has a pq with neighbours / distance to them
    vector<priority_queue<pair<long long,int>, vector<pair<long long,int>>, CompDist>> nodeDistMap;

    while(getline(File,text)) {
        // Read in nodes
        size_t sz;
        int strI = 0;
        vector<int> node(3);

        node[0] = stoi(text.substr(strI), &sz);
        strI+=sz+1;

        node[1] = stoi(text.substr(strI), &sz);
        strI+=sz+1;

        node[2] = stoi(text.substr(strI), &sz);

        nodes.push_back(node);
    }

    vector<int> nodeCircuit(nodes.size());
    vector<int> circuitCounts(nodes.size());

    for (int i = 0; i < nodes.size(); i++) {
        // For each node map out the distance to every other node
        auto node = nodes[i];
        nodeDistMap.push_back(
            priority_queue<pair<long long,int>, vector<pair<long long,int>>, CompDist>()
        );

        for (int j = 0; j < nodes.size(); j++) {
            if (i == j) continue;
            auto other = nodes[j];

            long long dist = distance(node,other);

            pair<long long,int> neighbour({dist, j});
            nodeDistMap[i].push(neighbour);
        }
        nodeCircuit[i]=i;
    }

    int counter = 0;
    int circuits = nodeCircuit.size();
     
    while (counter < 1000 && circuits > 1) {
        int first = -1;
        long long minDist = -1;
        for (int i = 0; i < nodeDistMap.size(); i++) {
            if (!nodeDistMap[i].empty() && (minDist == -1 || nodeDistMap[i].top().first < minDist)) {
                first = i;
                minDist = nodeDistMap[i].top().first;
            }
        }
        int second = nodeDistMap[first].top().second;

        // Match circuit numbers
        int thisCirc = nodeCircuit[first];
        int replaceCirc = nodeCircuit[second];
        
        if (thisCirc != replaceCirc) {
            // We are bridging circuits now
            for (auto & val: nodeCircuit) {
                if (val == replaceCirc) {
                    val = thisCirc;
                }
            }
            if (circuits == 2) {
                // Avoid slow mult repeatedly
                total = (long long) nodes[first][0] * nodes[second][0];
            }
            circuits--;
        }
        if (part1) counter++;
        
        
        nodeDistMap[first].pop();
        nodeDistMap[second].pop();
    }

    if (part1) {
        total = 1;
        // Add up size of circuits
        for (int i = 0; i < nodeCircuit.size(); i++) {
            circuitCounts[nodeCircuit[i]]++;
        }

        sort(circuitCounts.begin(), circuitCounts.end(), greater<int>());

        for (int i = 0; i < 3; i++) {
            total*=circuitCounts[i];
        }
    }

    return total;
}

int main() {
    cout << answer(true) << endl;
    cout << answer() << endl;
}