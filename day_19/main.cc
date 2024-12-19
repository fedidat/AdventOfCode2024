#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

std::vector<std::string> getWordsInLine(std::string line){
    std::regex numberRegex(R"(\w+)");
    std::vector<std::string> numbers;
    for (std::sregex_token_iterator it(line.begin(), line.end(), numberRegex), end; it != end; ++it)
        numbers.push_back(it->str());
    return numbers;
}

std::pair<std::vector<std::string>, std::vector<std::string>> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    std::vector<std::string> options = getWordsInLine(line);
    std::getline(file, line);
    std::vector<std::string> words;
    while(std::getline(file, line))
        words.push_back(line);
    file.close();
    return {options, words};
}

uint64_t waysToFormWord(std::string word, std::vector<std::string> options) {
    std::map<std::string, uint64_t> cache = {{"", 1}};
    for (int i = word.size() - 1; i >= 0; i--) {
        std::string subword = word.substr(i);
        for (auto option : options)
            if (subword.substr(0, option.size()) == option)
                cache[subword] += cache[subword.substr(option.size())];
    }
    return cache[word];
}

int main() {
    auto [options, words] = parseFile("input.txt");
    uint64_t formableWords = 0, totalWays = 0;
    for (auto word : words) {
        uint64_t waysForWord = waysToFormWord(word, options);
        totalWays += waysForWord;
        formableWords += waysForWord > 0;
    }
    std::cout << "Part 1: " << formableWords << std::endl;
    std::cout << "Part 2: " << totalWays << std::endl;
    return 0;
}
