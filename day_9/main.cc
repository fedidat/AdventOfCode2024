#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

void part1();
void part2();

int main()
{
    part1();
    part2();
    return 0;
}

std::vector<int> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    file.close();
    std::vector<int> digits = std::vector<int>();
    for (char c : line)
        digits.push_back(c - '0');
    return digits;
}

std::vector<std::pair<int, int>> getFilemap(std::vector<int> input) {
    std::vector<std::pair<int, int>> filemap = std::vector<std::pair<int, int>>();
    for (int i=0; i<input.size(); i+=2) {
        filemap.push_back({i/2, input[i]});
        if (i+1 < input.size())
            filemap.push_back({UINT16_MAX, input[i+1]});
    }
    return filemap;
}

long getChecksum(std::vector<std::pair<int, int>> filemap) {
    long sum = 0, pos = 0;
    for (int i=0; i<filemap.size(); i++) {
        if (filemap[i].first != UINT16_MAX)
            for (int j=pos; j<pos+filemap[i].second; j++)
                sum += filemap[i].first * j;
        pos += filemap[i].second;
    }
    return sum;
}

void part1() {
    std::vector<int> input = parseFile("input.txt");
    std::vector<std::pair<int, int>> filemap = getFilemap(input);
    for(int curBlock = filemap.size()-1, curFree = 1; curFree < curBlock; curBlock++, curFree++) {
        if (filemap[curBlock].second <= 0) {
            filemap.erase(filemap.begin() + curBlock);
            curBlock--;
        }
        while(filemap[curBlock].first == UINT16_MAX)
            curBlock--;
        if (filemap[curFree].second <= 0) {
            filemap.erase(filemap.begin() + curFree);
            curBlock--;
        }
        while(filemap[curFree].first != UINT16_MAX)
            curFree++;
        int toCopy = std::min(filemap[curFree].second, filemap[curBlock].second);
        filemap[curFree].second -= toCopy;
        filemap[curBlock].second -= toCopy;
        filemap.insert(filemap.begin() + curFree, {filemap[curBlock].first, toCopy});
    }
    std::cout << "Part 1: " << getChecksum(filemap) << std::endl;
}

void part2() {
    std::vector<int> input = parseFile("input.txt");
    std::vector<std::pair<int, int>> filemap = getFilemap(input);
    std::set<int> visited = std::set<int>();
    for (int i=filemap.size() - (filemap.size() % 2 == 0 ? 2 : 1); i>=0; i--) {
        if(filemap[i].first == UINT16_MAX || visited.contains(filemap[i].first))
            continue;
        visited.insert(filemap[i].first);
        for (int j=0; j<i; j++) {
            if (filemap[j].first == UINT16_MAX && filemap[j].second >= filemap[i].second) {
                filemap[j].second -= filemap[i].second;
                filemap.insert(filemap.begin() + j, {filemap[i].first, filemap[i].second});
                i++;
                filemap[i].first = UINT16_MAX;
                break;
            }
        }
    }
    std::cout << "Part 2: " << getChecksum(filemap) << std::endl;
}
