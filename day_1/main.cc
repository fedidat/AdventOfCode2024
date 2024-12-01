#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

int strtoint(std::string str);
std::vector<std::string> splitBySpace(const std::string& str);
std::map<int, int> countOccurrences(const std::vector<int>& numbers);
void part1(std::vector<int> first, std::vector<int> second);
void part2(std::vector<int> first, std::vector<int> second);

int main() {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<int> first, second;

    while (std::getline(file, line)) {
        auto firstAndLast = splitBySpace(line);
        first.push_back(stoi(firstAndLast[0]));
        second.push_back(stoi(firstAndLast[1]));
    }
    file.close();

    part1(first, second);
    part2(first, second);
    return 0;
}

void part1(std::vector<int> first, std::vector<int> second) {
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    int sum = 0;
    for (int i = 0; i < first.size(); i++) {
        sum += std::abs(first[i] - second[i]);
    }
    std::cout << sum << std::endl;
}

void part2(std::vector<int> first, std::vector<int> second) {
    std::map<int, int> freq;
    for (int num : second) {
        freq[num]++;
    }
    int sum = 0;
    for (int left : first) {
        sum += left * freq[left];
    }
    std::cout << sum << std::endl;
}

std::vector<std::string> splitBySpace(const std::string& str) {
    std::vector<std::string> words;
    std::stringstream ss(str);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}
