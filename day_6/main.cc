#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <set>

std::vector<std::string> parseFile(std::string fileName);

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
};

class Grid
{
    const std::vector<Point> directions = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};
    const std::vector<std::string> grid;
    const int lines, columns;

    std::set<Point> obstacles = std::set<Point>();
    Point guardPos = Point(0, 0);
    Point addedObstacle = Point(0, 0);
    int guardDir = 0;
    std::set<Point> visited = std::set<Point>();
    std::set<Point> turnLocations = std::set<Point>();

public:
    Grid(std::vector<std::string>);
    void init();
    std::pair<bool, bool> step();
    bool simulate();
    void part1();
    void part2();
    void print();
};

int main()
{
    std::vector<std::string> input = parseFile("input.txt");
    Grid grid(input);
    grid.part1();
    grid.part2();
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

Grid::Grid(std::vector<std::string> input) : grid(input), lines(input.size()), columns(input[0].size())
{
    init();
}

void Grid::init()
{
    guardDir = 0;
    visited = std::set<Point>();
    turnLocations = std::set<Point>();
    for (int i = 0; i < columns; i++)
    {
        for (int j = 0; j < lines; j++)
        {
            if (grid[i][j] == '#')
                obstacles.insert(Point(j, i));
            if (grid[i][j] == '^')
                guardPos = Point(j, i);
        }
    }
    visited.insert(guardPos);
}

std::pair<bool, bool> Grid::step()
{
    Point next = guardPos + directions[guardDir];
    if (next.x < 0 || next.y < 0 || next.x > columns || next.y > lines)
    {
        return std::pair<bool, bool>(true, false);
    }
    if (obstacles.contains(next))
    { // Turn
        if (turnLocations.contains(guardPos))
            return std::pair<bool, bool>(false, true);
        guardDir = (guardDir + 1) % directions.size();
        if (!obstacles.contains(guardPos + directions[guardDir]))
            turnLocations.insert(guardPos);
    }
    else
    { // Advance
        visited.insert(next);
        guardPos = next;
    }
    return std::pair<bool, bool>(false, false);
}

bool Grid::simulate()
{
    do {
        auto [exited, looped] = step();
        if (exited) return false;
        if (looped) return true;
    } while (true);
}

void Grid::part1()
{
    simulate();
    std::cout << "Done " << visited.size() - 1 << std::endl;
}

void Grid::part2()
{
    int positions = 0;
    int options = 0;
    visited = std::set<Point>();
    turnLocations = std::set<Point>();
    for (int i = 0; i < columns; i++)
    {
        for (int j = 0; j < lines; j++)
        {
            init();
            addedObstacle = Point(j, i);
            if (!obstacles.contains(addedObstacle))
            {
                std::cout << "Adding obstacle to " << addedObstacle << ": ";
                obstacles.insert(addedObstacle);
                bool looped = simulate();
                if (looped)
                {
                    std::cout << "Looped " << visited.size() - 1 << std::endl;
                    options++;
                }
                else
                    std::cout << "Done " << visited.size() - 1 << std::endl;
                obstacles.erase(addedObstacle);
            }
        }
    }
    std::cout << "Options: " << options << std::endl;
}

void Grid::print()
{
    std::cout << "Guard: " << guardPos << std::endl;
    std::cout << "Obstacles: ";
    for (auto o : obstacles)
        std::cout << o << ", ";
    std::cout << std::endl;

    std::vector<char> poses = {'^', '>', 'v', '<'};
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            Point curPoint = Point(j, i);
            if (guardPos == curPoint)
            {
                std::cout << poses[guardDir];
            }
            else if (addedObstacle == curPoint)
                std::cout << 'O';
            else if (obstacles.contains(curPoint))
                std::cout << '#';
            else
                std::cout << '.';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
