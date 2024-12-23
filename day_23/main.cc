#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

vector<pair<string, string>> parseFile(string fileName)
{
    ifstream file(fileName);
    string line;
    vector<pair<string, string>> input;
    while(getline(file, line))
        input.push_back({line.substr(0, line.find("-")), line.substr(line.find("-") + 1)});
    file.close();
    return input;
}

int getTComputers(map<string, set<string>> neighbors)
{
    set<set<string>> tComputers;
    for (auto [a, _] : neighbors)
        for (auto b : neighbors[a])
            for (auto c : neighbors[b])
                if (c != a && neighbors[a].contains(c) && (a[0] == 't' || b[0] == 't' || c[0] == 't'))
                        tComputers.insert({a, b, c});
    return tComputers.size();
}

string getMaxClique(map<string, set<string>> neighbors)
{
    set<set<string>> cliques;
    set<string> seenGlobal;
    for (auto [a, _] : neighbors) {
        if (seenGlobal.contains(a))
            continue;
        set<string> clique, seenInClique;
        queue<string> toVisit;
        toVisit.push(a);
        while (!toVisit.empty()) {
            auto b = toVisit.front();
            toVisit.pop();
            if (seenInClique.contains(b))
                continue;
            seenInClique.insert(b);
            bool connected = true;
            for (auto c : clique)
                if (b != c && !neighbors[b].contains(c))
                    connected = false;
            if (connected) {
                clique.insert(b);
                for (auto c : neighbors[b])
                    toVisit.push(c);
            }
        }
        for (auto a : clique)
            seenGlobal.insert(a);
        cliques.insert(clique);
    }
    set<string> maxClique;
    for (auto clique : cliques)
        if (clique.size() > maxClique.size())
            maxClique = clique;
    string maxCliqueStr = "";
    for (auto node : maxClique)
        maxCliqueStr += "," + node;
    return maxCliqueStr.substr(1);
}

int main() {
    auto input = parseFile("input.txt");
    map<string, set<string>> neighbors;
    for (auto [a, b] : input)
    {
        neighbors[a].insert(b);
        neighbors[b].insert(a);
    }
    cout << "Part 1: " << getTComputers(neighbors) << endl;
    cout << "Part 2: " << getMaxClique(neighbors) << endl;
    return 0;
}
