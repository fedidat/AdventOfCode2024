#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <queue>

void resolve();

int main() 
{
    resolve();
    return 0;
}

std::vector<std::string> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
        lines.push_back(line);
    file.close();
    return lines;
}


void resolve() {
    const std::vector<std::pair<int, int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    std::vector<std::string> input = parseFile("input.txt");
    int sum = 0, sumUniquePerimeter = 0;
    std::set<std::pair<int, int>> visited;
    for (int line=0; line < input.size(); line++) {
        for (int column=0; column<input[line].size(); column++) {
            int area = 0, perimeter = 0, uniquePerimeter=0;
            std::queue<std::pair<int, int>> toVisit({{line, column}});
            std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> perimetered;
            while (!toVisit.empty()) {
                // Ignore previously visited nodes
                auto [i, j] = toVisit.front();
                toVisit.pop();
                if (visited.contains({i, j}))
                    continue;
                visited.insert({i, j});

                // For area, simply count each node towards total for garden type
                area++;
                for (auto dir : directions) {
                    // Facing same garden type, don't add perimeter but visit next
                    if (input[i+dir.first][j+dir.second] == input[i][j]
                        && !(i < 0 || i >= input.size() || j < 0 || j >= input[i].size()))
                        toVisit.push({i+dir.first, j+dir.second});
                    else {
                        // For simple perimeter, simply increment
                        perimeter++;

                        // For bulk pricing, check if perimeter of any adjacent node facing
                        // the same directions was already priced
                        std::vector<std::pair<int, int>> adjDirections;
                        if (dir.first != 0)
                            adjDirections = {{0, -1}, {0, 1}};
                        else
                            adjDirections = {{-1, 0}, {1, 0}};
                        bool neighborCounted = false;
                        for (auto adj : adjDirections)
                            if (input[i+adj.first][j+adj.second] == input[i][j]
                                && perimetered.contains({{i+adj.first, j+adj.second}, dir}))
                                neighborCounted = true;
                        if (!neighborCounted)
                            uniquePerimeter++;
                        perimetered.insert({{i, j}, dir});
                    }
                }
            }
            sum += area * perimeter;
            sumUniquePerimeter += area * uniquePerimeter;
        }
    }
    std::cout << "Part 1: " << sum << std::endl;
    std::cout << "Part 2: " << sumUniquePerimeter << std::endl;
}
