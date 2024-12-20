#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> grid;
    while(std::getline(file, line))
        grid.push_back(line);
    file.close();
    return grid;
}

std::map<std::pair<int, int>, int> getDistances(std::vector<std::string> grid, std::pair<int, int> start, std::pair<int, int> end) {
    std::map<std::pair<int, int>, int> distances = {{start, 0}};
    std::pair<int, int> prev = {-1 ,-1}, current = start;
    while (current != end) {
        for (auto dir : std::vector<std::pair<int, int>>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            if (prev != next && grid[next.first][next.second] != '#') {
                distances[next] = distances[current] + 1;
                prev = current;
                current = next;
            }
        }
    }
    return distances;
}

int countValidShortcuts(std::vector<std::string> grid, int maxBypass) {
    int rows = grid.size(), columns = grid[0].size();
    std::map<char, std::pair<int, int>> lastSeenNodes;
    for (int row=0; row < grid.size(); row++)
        for (int column = 0; column < grid[row].size(); column++)
            lastSeenNodes[grid[row][column]] = {row, column};
    std::pair<int, int> start = lastSeenNodes['S'], end = lastSeenNodes['E'];
    std::map<std::pair<int, int>, int> distancesFromStart = getDistances(grid, start, end), distancesFromEnd = getDistances(grid, end, start);
    int shortcuts = 0, threshold=100, maxLength = distancesFromEnd[start];
    for (auto [startCheat, startDist] : distancesFromStart)
        for (auto [endCheat, endDist] : distancesFromEnd) {
            int manhattan = abs(startCheat.first - endCheat.first) + abs(startCheat.second - endCheat.second), bypassedLength = startDist + endDist + manhattan;
            if (manhattan >= 2 && manhattan <= maxBypass && maxLength - bypassedLength >= threshold)
                shortcuts++;
        }
    return shortcuts;
}

int main() {
    auto grid = parseFile("input.txt");
    std::cout << "Part 1: " << countValidShortcuts(grid, 2) << std::endl;
    std::cout << "Part 2: " << countValidShortcuts(grid, 20) << std::endl;
    return 0;
}
