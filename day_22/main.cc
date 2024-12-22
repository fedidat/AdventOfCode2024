#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

vector<uint64_t> parseFile(string fileName)
{
    ifstream file(fileName);
    string line;
    vector<uint64_t> numbers;
    while(getline(file, line))
        numbers.push_back(stoll(line));
    file.close();
    return numbers;
}

pair<uint64_t, map<int64_t, uint64_t>> runCycles(uint64_t secret, int cycles) {
    vector<int64_t> sequence;
    map<int64_t, uint64_t> bananasBySequence;
    for (int cycle=0; cycle<cycles; cycle++) {
        secret = secret ^ (secret * 64) % 16777216;
        secret = secret ^ (secret / 32) % 16777216;
        secret = secret ^ (secret * 2048) % 16777216;
        sequence.push_back(secret);
        if (cycle > 3) {
            int64_t key = 0; // Shorten last 4 elements to int64_t to use as sequence map key
            for (int64_t i=sequence.size() - 4, multiplier=1; i<sequence.size(); i++, multiplier *= 100)
                key += multiplier * (sequence[i] % 10 - sequence[i - 1] % 10);
            if (!bananasBySequence.contains(key))
                bananasBySequence[key] = secret % 10;
        }
    }
    return {secret, bananasBySequence};
}

uint64_t getMaxMapValue(map<int64_t, uint64_t> map) {
    uint64_t maxValue = 0;
    for (auto [sequence, bananas] : map)
        maxValue = max(maxValue, bananas);
    return maxValue;
}

pair<uint64_t, uint64_t> cycleSum(vector<uint64_t> secrets, int cycles) {
    map<int64_t, uint64_t> totalBananasBySequence;
    uint64_t sum = 0;
    for (int i=0; i<secrets.size(); i++) {
        auto [secret, bananasBySequence] = runCycles(secrets[i], cycles);
        sum += secret;
        for (auto [sequence, bananas] : bananasBySequence)
            totalBananasBySequence[sequence] += bananas;
    }
    return {sum, getMaxMapValue(totalBananasBySequence)};
}

int main() {
    // vector<uint64_t> secrets = {123};
    // vector<uint64_t> secrets = {1, 10, 100, 2024};
    // vector<uint64_t> secrets = {1, 2, 3, 2024};
    auto secrets = parseFile("input.txt");
    auto [sum, maxBananas] = cycleSum(secrets, 2000);
    cout << "Part 1: " << sum << endl;
    cout << "Part 2: " << maxBananas << endl;
    return 0;
}
