#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

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

void resolve()
{
    std::vector<std::string> input = parseFile("input.txt");

    // Set antenna positions by frequency symbol
    std::map<char, std::vector<Point>> antennas = std::map<char, std::vector<Point>>();
    int lines = input.size(), columns = input[0].size();
    for (int i = 0; i < input.size(); i++)
        for (int j = 0; j < input[i].size(); j++)
            if (input[i][j] != '.')
                antennas[input[i][j]].push_back(Point(i, j));

    // Set antinode positions
    std::set<Point> antinodes1 = std::set<Point>(), antinodes2 = std::set<Point>();
    for (auto &[key, value] : antennas)
        for (int i = 0; i < value.size(); i++)
            for (int j = i + 1; j < value.size(); j++)
            {
                Point dist = value[j] - value[i];

                // Part 1: Add antinode to opposite positions of each antenna
                if ((value[j] + dist).in(lines, columns))
                    antinodes1.insert(value[j] + dist);
                if ((value[i] - dist).in(lines, columns))
                    antinodes1.insert(value[i] - dist);

                // Part 2: Same but start at antenna and keep going while in grid
                for (Point antinodeC = value[j]; antinodeC.in(lines, columns); antinodeC = antinodeC + dist)
                    antinodes2.insert(antinodeC);
                for (Point antinodeC = value[i]; antinodeC.in(lines, columns); antinodeC = antinodeC - dist)
                    antinodes2.insert(antinodeC);
            }
    std::cout << antinodes1.size() << std::endl;
    std::cout << antinodes2.size() << std::endl;
}
