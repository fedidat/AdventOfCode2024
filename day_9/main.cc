#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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
    for (char c : line) {
        digits.push_back(c - '0');
    }
    return digits;
}

void part1() {
    std::vector<int> input = parseFile("input.txt");

    // Put together the intiial file map
    std::vector<uint16_t> fileMap = std::vector<uint16_t>();
    uint16_t fileId = 0;
    for (int i=0; i<input.size(); i+=2) {
        for (int j=0; j<input[i]; j++)
            fileMap.push_back(fileId);
        fileId++;
        if (i+1 < input.size())
            for (int j=0; j<input[i+1]; j++)
                fileMap.push_back(UINT16_MAX);
    }

    // Defragment
    for (int startIndex = 0, endIndex = fileMap.size()-1; ;) {
        while (fileMap[startIndex] != UINT16_MAX && startIndex < endIndex)
            startIndex++;
        while (fileMap[endIndex] == UINT16_MAX)
            endIndex--;
        if (startIndex >= endIndex)
            break;
        fileMap[startIndex] = fileMap[endIndex];
        fileMap[endIndex] = UINT16_MAX;
    }

    // Compute checksum
    long sum = 0;
    for (int i=0; i<fileMap.size() && fileMap[i] != UINT16_MAX; i++)
        sum += i * fileMap[i];
    std::cout << sum << std::endl;
}

void part2() {
    std::vector<int> input = parseFile("input.txt");

    // Put together the intiial file map
    std::vector<std::pair<int, int>> filemap = std::vector<std::pair<int, int>>();
    for (int i=0; i<input.size(); i+=2) {
        filemap.push_back({i/2, input[i]});
        if (i+1 < input.size())
            filemap.push_back({UINT16_MAX, input[i+1]});
    }

    // Defragment
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

    // Compute checksum
    long sum = 0, pos = 0;
    for (int i=0; i<filemap.size(); i++) {
        if (filemap[i].first != UINT16_MAX)
            for (int j=pos; j<pos+filemap[i].second; j++)
                sum += filemap[i].first * j;
        pos += filemap[i].second;
    }
    std::cout << sum << std::endl;
}