#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int> > parseFile();
void part1(std::vector<std::vector<int> > allLevels);
void part2(std::vector<std::vector<int> > allLevels);
bool isSafe(std::vector<int> levels);
std::vector<int> splitBySpace(const std::string &str);

int main()
{
    std::vector<std::vector<int> > allLevels = parseFile();
    part1(allLevels);
    part2(allLevels);
    return 0;
}

std::vector<std::vector<int> > parseFile()
{
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<int> > lines;
    while (std::getline(file, line))
        lines.push_back(splitBySpace(line));
    file.close();
    return lines;
}

void part1(std::vector<std::vector<int> > allLevels)
{
    int sum = 0;
    for (auto levels : allLevels)
        if (isSafe(levels))
            sum++;
    std::cout << sum << std::endl;
}

void part2(std::vector<std::vector<int> > allLevels)
{
    int sum = 0;
    for (auto levels : allLevels)
    {
        if (isSafe(levels))
            sum++;
        else
        {
            for (int i = 0; i < levels.size(); i++)
            {
                int toErase = levels[i];
                levels.erase(levels.begin() + i);
                if (isSafe(levels))
                {
                    sum++;
                    break;
                }
                levels.insert(levels.begin() + i, toErase);
            }
        }
    }
    std::cout << sum << std::endl;
}

bool isSafe(std::vector<int> levels)
{
    int direction = 0;
    for (int i = 0; i < levels.size() - 1; i++)
    {
        if (abs(levels[i] - levels[i + 1]) > 3 || levels[i] == levels[i + 1])
            return false;
        if (!direction)
            direction = levels[i + 1] > levels[i] ? 1 : -1;
        else if ((levels[i + 1] < levels[i] && direction == 1) || (levels[i + 1] > levels[i] && direction == -1))
            return false;
    }
    return true;
}

std::vector<int> splitBySpace(const std::string &str)
{
    std::vector<int> words;
    std::stringstream ss(str);
    std::string word;
    while (ss >> word)
    {
        words.push_back(stoi(word));
    }
    return words;
}
