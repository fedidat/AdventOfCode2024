#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <queue>

std::vector<int> getNumbersInLine(std::string line){
    std::regex numberRegex(R"(\d+)");
    std::vector<int> numbers;
    for (std::sregex_token_iterator it(line.begin(), line.end(), numberRegex), end; it != end; ++it)
        numbers.push_back(std::stoi(it->str()));
    return numbers;
}

std::vector<std::pair<int, int>> parseFile(std::string fileName)
{
    std::vector<std::pair<int, int>> points;
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> input;
    while(std::getline(file, line)) {
        auto numbers = getNumbersInLine(line);
        points.push_back({numbers[0], numbers[1]});
    }
    file.close();
    return points;
}

const std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int distanceToGoal(std::vector<std::pair<int, int>> points, int rows, int columns, int pixels) {
    std::set<std::pair<int, int>> grid(points.begin(), points.begin() + pixels), visited;
    std::queue<std::pair<std::pair<int, int>, int>> toVisit({{{0, 0}, 0}});
    while (!toVisit.empty()) {
        auto [current, distance] = toVisit.front();
        toVisit.pop();
        if (visited.contains(current))
            continue;
        visited.insert(current);
        if (current == std::pair<int, int>({columns-1, rows - 1}))
           return distance;
        for (auto dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            if (next.first >= 0 && next.first < columns && next.second >= 0 && next.second < rows && !grid.contains(next))
                toVisit.push({next, distance + 1});
        }
    }
    return INT_MAX;
}

std::pair<int, int> getBlockingCoordinates(std::vector<std::pair<int, int>> points, int rows, int columns) {
    int left = 0, right = points.size();
    while (left < right) {
        int mid = (left + right) / 2;
        if (distanceToGoal(points, rows, columns, mid) != INT_MAX)
            left = mid;
        else
            right = mid - 1;
    }
    return points[left];
}

int main() 
{
    std::vector<std::pair<int, int>> points = parseFile("input.txt");
    int rows = 0, columns = 0;
    for (auto pt : points) {
        columns = std::max(columns, pt.first + 1);
        rows = std::max(rows, pt.second + 1);
    }
    std::cout << "Part 1: " << distanceToGoal(points, rows, columns, 1024) << std::endl;
    std::pair<int, int> blockingCoordinate = getBlockingCoordinates(points, rows, columns);
    std::cout << "Part 2: " << blockingCoordinate.first << "," << blockingCoordinate.second << std::endl;
    return 0;
}
