#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <stack>

class Point
{
public:
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    bool operator<(const Point &other) const
    {
        return (x < other.x) || (x == other.x && y < other.y);
    }
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        return os << "(" << p.x << ", " << p.y << ")";
    }
    Point operator+(const Point &p)
    {
        return Point(x + p.x, y + p.y);
    }
    Point operator-(const Point &p)
    {
        return Point(x - p.x, y - p.y);
    }
    bool in(int lines, int columns)
    {
        return x >= 0 && y >= 0 && x < lines && y < columns;
    }
};

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

const std::vector<Point> directions = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};

int getTrailheadScore(std::vector<std::string> grid, Point trailStart) {
    int lines = grid.size(), columns = grid[0].size(), reached = 0;
    std::stack<Point> toVisit({trailStart});
    std::set<Point> visited;
    while (!toVisit.empty()) {
        Point current = toVisit.top();
        toVisit.pop();
        if (grid[current.x][current.y] == '9') {
            reached++;
            continue;
        }
        for (auto dir : directions) {
            Point adj = current + dir;
            if (!visited.contains(adj) && adj.in(lines, columns) && grid[adj.x][adj.y] == grid[current.x][current.y] + 1) {
                visited.insert(adj);
                toVisit.push(adj);
            }
        }
    }
    return reached;
}

int getTrailheadRating(std::vector<std::string> grid, Point trailStart) {
    int lines = grid.size(), columns = grid[0].size(), reached = 0;
    std::set<std::vector<Point>> visitedPaths;
    std::stack<std::vector<Point>> pathsToVisit;
    pathsToVisit.push({Point(trailStart.x, trailStart.y)});
    while (!pathsToVisit.empty()) {
        std::vector<Point> currentPath = pathsToVisit.top();
        pathsToVisit.pop();
        Point current = currentPath.back();
        if (grid[current.x][current.y] == '9') {
            reached++;
            continue;
        }
        for (auto dir : directions) {
            Point adj = current + dir;
            if (adj.in(lines, columns) && grid[adj.x][adj.y] == grid[current.x][current.y] + 1) {
                std::vector<Point> nextPath(currentPath);
                nextPath.push_back(adj);
                if (!visitedPaths.contains(nextPath)) {
                    visitedPaths.insert(nextPath);
                    pathsToVisit.push(nextPath);
                }
            }
        }
    }
    return reached;
}

void resolve() {
    std::vector<std::string> input = parseFile("input.txt");
    int scoreSum = 0, ratingSum = 0;
    for (int i=0; i<input.size(); i++)
        for (int j=0; j<input[i].size(); j++)
            if (input[i][j] == '0') {
                scoreSum += getTrailheadScore(input, Point(i, j));
                ratingSum += getTrailheadRating(input, Point(i, j));
            }
    std::cout << "Part 1: " << scoreSum << std::endl;
    std::cout << "Part 2: " << ratingSum << std::endl;
}
