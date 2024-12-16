#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <map>
#include <tuple>

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
    std::vector<std::string> input;
    while(std::getline(file, line))
        input.push_back(line);
    file.close();
    return input;
}

const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

std::pair<int, std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>> getScore(std::vector<std::string> grid, std::pair<int, int> reindeer, std::pair<int, int> facing) {
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, int> distances;
    distances[{reindeer, facing}] = 0;
    std::priority_queue<std::tuple<int, std::pair<int, int>, std::pair<int, int>>, std::vector<std::tuple<int, std::pair<int, int>, std::pair<int, int>>>, std::greater<std::tuple<int, std::pair<int, int>, std::pair<int, int>>>> queue;
    queue.push({0, reindeer, facing});
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> predecessors;
    while (!queue.empty())
    {
        auto [dist, pos, dir] = queue.top();
        queue.pop();
        if (grid[pos.first][pos.second] == 'E')
            return {dist, predecessors};
        for (auto neighborDir : directions)
        {
            if (grid[pos.first + neighborDir.first][pos.second + neighborDir.second] == '#')
                continue;
            std::pair<std::pair<int, int>, std::pair<int, int>> neighbor = {{pos.first + neighborDir.first, pos.second + neighborDir.second}, neighborDir};
            int neighborDist = dist + 1 + (neighborDir == dir ? 0 : 1000);
            if (!distances.contains(neighbor) || neighborDist < distances[neighbor]) {
                distances[neighbor] = neighborDist;
                queue.push(std::make_tuple(neighborDist, std::pair<int, int>{pos.first + neighborDir.first, pos.second + neighborDir.second}, neighborDir));
                predecessors[neighbor].clear();
                predecessors[neighbor].push_back({pos, dir});
            }
            else if (neighborDist == distances[neighbor])
                predecessors[neighbor].push_back({pos, dir});
        }
    }
    return {INT_MAX, {}};
}

int getTiles(std::vector<std::string> grid, std::pair<int, int> end, std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>> predecessors) {
    std::set<std::pair<int, int>> tiles;
    std::queue<std::pair<std::pair<int, int>, std::pair<int, int>>> queue;
    for (auto direction : directions)
        queue.push({end, direction});
    while (!queue.empty())
    {
        auto [pos, dir] = queue.front();
        queue.pop();
        tiles.insert(pos);
        for (auto predecessor : predecessors[{pos, dir}])
            queue.push(predecessor);
    }
    return tiles.size();
}

void resolve() {
    std::vector<std::string> grid = parseFile("input.txt");
    std::pair<int, int> reindeer, end;
    for (int i=0; i< grid.size(); i++)
        for (int j=0; j< grid[i].size(); j++) {
            if (grid[i][j] == 'S')
                reindeer = {i, j};
            if (grid[i][j] == 'E')
                end = {i, j};
        }
    auto [score, predecessors] = getScore(grid, reindeer, {0, 1});
    std::cout << "Part 1: " << score << std::endl;
    std::cout << "Part 2: " << getTiles(grid, end, predecessors) << std::endl;
}
