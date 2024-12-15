#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

void resolve();

int main() 
{
    resolve();
    return 0;
}

std::pair<std::vector<std::string>, std::string> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> grid;
    std::string directions;
    while(std::getline(file, line)) {
        grid.push_back(line);
        if (line == "")
            break;
    }
    while(std::getline(file, line))
        directions += line;
    file.close();
    return {grid, directions};
}

const std::map<char, std::pair<int, int>> directionsMap = { {'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}} };
const std::map<char, std::string> conversionMap = { {'#', "##"}, {'O', "[]"}, {'.', ".."}, {'@', "@."} };

bool move(std::vector<std::string>& grid, std::pair<int, int> point, std::pair<int, int> offset, int dryrun, bool needsOther = true) {
    if (grid[point.first][point.second] == '#')
        return false;
    else if (grid[point.first][point.second] == '.')
        return true;
    else if (grid[point.first][point.second] == 'O') {
        if (move(grid, {point.first + offset.first, point.second + offset.second}, offset, dryrun) && !dryrun) {
            grid[point.first + offset.first][point.second + offset.second] = grid[point.first][point.second];
            return true;
        }
    }
    else if (grid[point.first][point.second] == '[' || grid[point.first][point.second] == ']') {
        needsOther = needsOther && offset.first != 0;
        int other = grid[point.first][point.second] == '[' ? 1 : -1;
        if (move(grid, {point.first + offset.first, point.second + offset.second}, offset, dryrun)
            && (!needsOther || move(grid, {point.first, point.second + other}, offset, dryrun, false))) {
            if (!dryrun) {
                grid[point.first + offset.first][point.second + offset.second] = grid[point.first][point.second];
                grid[point.first][point.second] = '.';
            }
            return true;
        }
    }
    else if (grid[point.first][point.second] == '@') {
        if (move(grid, {point.first + offset.first, point.second + offset.second}, offset, dryrun)) {
            if (!dryrun) {
                grid[point.first + offset.first][point.second + offset.second] = grid[point.first][point.second];
                grid[point.first][point.second] = '.';
            }
            return true;
        }
    }
    return false;
}

int processGrid(std::vector<std::string> grid, std::string directions) {
    std::pair<int, int> bot;
    for (bot.first=0; bot.first<grid.size(); bot.first++)
        if ((bot.second = grid[bot.first].find('@')) != std::string::npos) {
            break;
        }
    for (int step=0; step<directions.size(); step++) {
        std::pair<int, int> direction = directionsMap.at(directions[step]);
        if (move(grid, bot, direction, true)) {
            move(grid, bot, direction, false);
            bot.first += direction.first;
            bot.second += direction.second;
        }
    }
    int sum = 0;
    for (int i=0; i<grid.size(); i++)
        for (int j=0; j<grid[i].size(); j++)
           if (grid[i][j] == 'O' || grid[i][j] == '[')
                sum += 100 * i + j;
    return sum;
}

void resolve() {
    auto [grid, directions] = parseFile("input.txt");
    std::cout << "Part 1: " << processGrid(grid, directions) << std::endl;
    std::vector<std::string> grid2;
    for (int i=0; i<grid.size(); i++) {
        std::string line;
        for (int j=0; j<grid[i].size(); j++)
            line += conversionMap.at(grid[i][j]);
        grid2.push_back(line);
    }
    std::cout << "Part 2: " << processGrid(grid2, directions) << std::endl;
}
