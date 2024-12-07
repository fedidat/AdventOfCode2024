#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <ranges>

std::vector<std::vector<int64_t>> parseFile(std::string fileName);
void resolve();

int main()
{
    resolve();
    return 0;
}

std::vector<std::vector<int64_t>> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<int64_t>> input;
    while (std::getline(file, line))
    {
        std::vector<int64_t> numbers;
        std::istringstream stream(line);
        for (std::string number : std::ranges::istream_view<std::string>(stream))
            numbers.push_back(std::stol(number));
        input.push_back(numbers);
    }
    file.close();
    return input;
}

bool seek(int64_t toSeek, int64_t soFar, std::vector<int64_t> numbers, int index, std::vector<int64_t (*)(int64_t, int64_t)> ops)
{
    if (index == numbers.size())
        return soFar == toSeek;
    for (auto func : ops)
        if (seek(toSeek, func(soFar, numbers[index]), numbers, index + 1, ops))
            return true;
    return false;
}

int64_t add(int64_t x, int64_t y) { return x + y; }
int64_t multiply(int64_t x, int64_t y) { return x * y; }
int64_t concat(int64_t a, int64_t b) { return std::pow(10, std::to_string(b).size()) * a + b; }

void resolve()
{
    std::vector<std::vector<int64_t>> input = parseFile("input.txt");
    int64_t part1sum = 0, part2sum = 0;
    for (auto equation : input)
    {
        long seeked = equation[0];
        if (seek(seeked, equation[1], equation, 2, {add, multiply}))
            part1sum += seeked;
        if (seek(seeked, equation[1], equation, 2, {add, multiply, concat}))
            part2sum += seeked;
    }
    std::cout << "Part 1 sum: " << part1sum << std::endl;
    std::cout << "Part 2 sum: " << part2sum << std::endl;
}
