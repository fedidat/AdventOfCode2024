#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<vector<string>> parseFile(string fileName)
{
    ifstream file(fileName);
    string line;
    vector<vector<string>> grids;
    vector<string> grid;
    while(getline(file, line)) {
        if (line == "") {
            grids.push_back(grid);
            getline(file, line);
            grid = vector<string>();
        }
        grid.push_back(line);
    }
    grids.push_back(grid);
    file.close();
    return grids;
}

pair<vector<vector<int>>, vector<vector<int>>> parseGrids(vector<vector<string>> grids) {
    vector<vector<int>> locks, keys;
    for (auto grid : grids) {
        vector<int> numbers(5, -1);
        for (int i=0; i < grid.size(); i++)
            for (int j=0; j < grid[i].size(); j++)
                if (grid[i][j] == '#')
                    numbers[j]++;
        if (grid[0] == "#####")
            locks.push_back(numbers);
        else
            keys.push_back(numbers);
    }
    return {locks, keys};
}

int keyMatches(vector<int> key, vector<int> lock) {
    for (int i=0; i<key.size(); i++)
        if (key[i] + lock[i] >= 6)
            return i;
    return -1;
}

int countMatches(vector<vector<int>> locks, vector<vector<int>> keys) {
    int matches = 0;
    for (vector<int> lock : locks)
        for (int keyIdx = 0; keyIdx < keys.size(); keyIdx++)
            if (keyMatches(lock, keys[keyIdx]) == -1)
                matches++;
    return matches;
}

int main() {
    auto grids = parseFile("input.txt");
    auto [locks, keys] = parseGrids(grids);
    cout << "Part 1: " << countMatches(locks, keys) << endl;
    return 0;
}
