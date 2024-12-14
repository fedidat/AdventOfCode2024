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

std::vector<int> getNumbersInLine(std::string line){
    std::regex numberRegex(R"(-?\d+)");
    std::vector<int> numbers;
    for (std::sregex_token_iterator it(line.begin(), line.end(), numberRegex), end; it != end; ++it)
        numbers.push_back(std::stoi(it->str()));
    return numbers;
}

std::vector<std::vector<int>> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<int>> nums;
    while(std::getline(file, line)) {
        std::vector<int> blockNums;
        for (auto num : getNumbersInLine(line))
            blockNums.push_back(num);
        nums.push_back(blockNums);
    }
    file.close();
    return nums;
}

void resolve() {
    std::vector<std::vector<int>> grid = parseFile("input.txt");
    int lines = 0, columns = 0;
    for (auto nums : grid) {
        lines = std::max(lines, nums[1] + 1);
        columns = std::max(columns, nums[0] + 1);
    }
    int minSafety=INT_MAX, minSafetyCycle = 0;
    for (int cycle=0; cycle < 10000; cycle++) {
        for (int idx=0; idx < grid.size(); idx++) {
            grid[idx][0] = (grid[idx][0] + grid[idx][2] + columns) % columns;
            grid[idx][1] = (grid[idx][1] + grid[idx][3] + lines) % lines;
        }
        std::vector<std::vector<int>> quadrants = {
            {0, lines/2, 0, columns/2}, // top left
            {0, lines/2, columns/2 + 1, columns}, // top right
            {lines/2 + 1, lines, 0, columns/2}, // bottom left
            {lines/2 + 1, lines, columns/2 + 1, columns}, // bootom right
        };
        int safetyFactor = 1;
        for (auto quadrant : quadrants) {
            int qSafety = 0;
            for (auto nums : grid)
                if (nums[1] >= quadrant[0] && nums[1] < quadrant[1] && nums[0] >= quadrant[2] && nums[0] < quadrant[3])
                    qSafety++;
            safetyFactor *= qSafety;
        }
        minSafety = std::min(minSafety, safetyFactor);
        if (cycle == 99)
            std::cout << "Part 1: " << safetyFactor << std::endl;
        if (minSafety == safetyFactor)
            minSafetyCycle = cycle + 1;
    }
    std::cout << "Part 2: " << minSafetyCycle << std::endl;
}
