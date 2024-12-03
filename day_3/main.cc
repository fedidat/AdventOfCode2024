#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

std::string parseFile(std::string fileName);
void part1();
void part2();
void resolve(bool isPart2);
int doMultiply(std::string input);
std::pair<int, int> nextMultiply(std::string input, std::regex exp);

int main()
{
    part1();
    part2();
    return 0;
}

std::string parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

void part1()
{
    resolve(false);
}

void part2()
{
    resolve(true);
}

void resolve(bool isPart2)
{
    std::string input = parseFile("input.txt");
    std::regex exp("mul\\(\\d{1,3},\\d{1,3}\\)");
    int sum = 0;
    while (true)
    {
        std::pair<int, int> nextMul = nextMultiply(input, exp);
        if (nextMul.first == -1)
            break;
        if (isPart2)
        {
            int nextDont = input.find("don't()");
            if (nextDont != -1 && nextMul.first > nextDont)
            {
                input = input.substr(nextDont + input.substr(nextDont).find("do()"));
                continue;
            }
        }
        sum += nextMul.second;
        input = input.substr(nextMul.first + 1);
    }
    std::cout << sum << std::endl;
}

int doMultiply(std::string input)
{
    std::regex exp("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    std::smatch matches;
    std::regex_search(input, matches, exp);
    return std::stoi(matches[1]) * std::stoi(matches[2]);
}

std::pair<int, int> nextMultiply(std::string input, std::regex exp)
{
    std::smatch res;
    std::string::const_iterator searchStart(input.cbegin());
    bool found = std::regex_search(searchStart, input.cend(), res, exp);
    if (found)
        return std::pair<int, int>(res.position(), doMultiply(res[0].str()));
    return std::pair<int, int>(-1, 0);
}
