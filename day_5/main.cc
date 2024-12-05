#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <map>
#include <set>

void resolve();

int main()
{
    resolve();
    return 0;
}

std::vector<std::vector<int>> split(std::string input, char delim) {
    return input
        | std::views::split('\n')
        | std::views::filter([](auto&& line) { return !line.empty(); })
        | std::views::transform([delim](auto&& line) {
            return std::string_view(&*line.begin(), std::ranges::distance(line))
                | std::views::split(delim)
                | std::views::transform([](auto&& word) {
                    return std::stoi(std::string(&*word.begin(), std::ranges::distance(word)));
                })
                | std::ranges::to<std::vector>();
        })
        | std::ranges::to<std::vector>();
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string input = buffer.str();
    std::string delim = "\n\n";
    int delimIndex = input.find(delim);
    return std::pair(split(input.substr(0, delimIndex), '|'), split(input.substr(delimIndex), ','));
}

std::pair<bool, int> handlePages(std::map<int, std::set<int>> rules, std::vector<int> pages) {
    // Keep swapping violated rules until all pages are compliant and mark if a swap was made.
    bool compliant, swapped = false;
    do {
        compliant = true;
        for (int i=0; i<pages.size(); i++) {
            for (int j=i+1; j<pages.size(); j++) {
                if (rules[pages[i]].contains(pages[j])) {
                    compliant = false;
                    swapped = true;
                    std::swap(pages[j], pages[i]);
                    break;
                }
            }
        }
    } while (!compliant);
    return std::pair<bool, int>(swapped, pages[pages.size()/2]);
}

void resolve()
{
    auto input = parseFile("input.txt");
    std::map<int, std::set<int>> rules;
    for (auto rule : input.first)
        rules[rule[1]].insert(rule[0]);

    int compliantSum = 0, fixedSum = 0;
    for (auto pages : input.second) {
        auto result = handlePages(rules, pages);
        if (result.first)
            fixedSum += result.second;
        else
            compliantSum += result.second;
    }
    std::cout << "Part 1: " << compliantSum << std::endl;
    std::cout << "Part 2: " << fixedSum << std::endl;
}
