#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

void resolve();

int main() 
{
    resolve();
    return 0;
}

std::vector<std::string> split(std::string input, char delim) {
    std::string word;
    std::vector<std::string> words;
    std::stringstream ss(input);
    while(std::getline(ss, word, delim))
        words.push_back(word);
    return words;
}

std::vector<std::string> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    file.close();
    return split(line, ' ');
}

uint64_t getStoneCount(std::map<uint64_t, uint64_t> stones) {
    uint64_t stoneCount = 0;
    for (auto &[stone, quantity] : stones)
        stoneCount += quantity;
    return stoneCount;
}

std::map<uint64_t, uint64_t> step(std::map<uint64_t, uint64_t> stones) {
    std::map<uint64_t, uint64_t> nextStones;
    for (auto &[stone, quantity] : stones) {
        std::string str = std::to_string(stone);
        if (stone == 0)
            nextStones[1] += quantity;
        else if (str.size() % 2 == 0) {
            nextStones[std::stoll(str.substr(0, str.size() / 2))] += quantity;
            nextStones[std::stoll(str.substr(str.size() / 2))] += quantity;
        }
        else
            nextStones[stone * 2024] += quantity;
    }
    return nextStones;
}

void resolve() {
    std::vector<std::string> input = parseFile("input.txt");
    std::map<uint64_t, uint64_t> stones;
    for (int word=0; word<input.size(); word++)
        stones[std::stoll(input[word])]++;
    for (int cycle=1; cycle <= 75; cycle++) {
        stones = step(stones);
        if (cycle == 25)
            std::cout << "Part 1: " << getStoneCount(stones) << std::endl;
    }
    std::cout << "Part 2: " << getStoneCount(stones) << std::endl;
}
