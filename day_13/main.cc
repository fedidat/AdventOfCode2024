#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

void resolve();

int main() 
{
    resolve();
    return 0;
}

std::vector<int> getNumbersInLine(std::string line){
    std::regex number_regex("\\d+");
    std::sregex_iterator begin(line.begin(), line.end(), number_regex), end;
    std::vector<int> numbers;
    for (auto it = begin; it != end; ++it)
        numbers.push_back(std::stoi(it->str()));
    return numbers;
}

std::vector<std::vector<int>> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<int>> nums;
    int i=0;
    std::vector<int> blockNums;
    while(std::getline(file, line)) {
        for (auto num : getNumbersInLine(line))
            blockNums.push_back(num);
        if (++i % 4 == 0) {
            nums.push_back(blockNums);
            blockNums = std::vector<int>();
        }
    }
    nums.push_back(blockNums);
    file.close();
    return nums;
}

int getPricePart1(std::vector<int> nums) {
    int minPrice = INT_MAX;
    for (int i=0; i<100; i++)
        for (int j=0; j<100; j++)
            if (nums[0] * i + nums[2] * j == nums[4] && nums[1] * i + nums[3] * j == nums[5])
                minPrice = std::min(3 * i + j, minPrice);
    return minPrice == INT_MAX ? 0 : minPrice;
}

uint64_t getPricePart2(std::vector<int> nums) {
    int64_t a = nums[0], c = nums[1], b = nums[2], d = nums[3], e = nums[4] + 10000000000000, f = nums[5] + 10000000000000;
    // Solve {ax+by=c, dx+ey=f} using Cramer's rule, see https://www.1728.org/cramer.htm
    int64_t dn = a * d - c * b, x_det = e * d - f * b, y_det = a * f - c * e, x = x_det / dn, y = y_det / dn;
    if (x_det % dn != 0 || y_det % dn != 0 || x < 0 || y < 0)
        return 0;
    return x * 3 + y;
}

void resolve() {
    uint64_t sum1 = 0, sum2 = 0;
    std::vector<std::vector<int>> input = parseFile("input.txt");
    for (auto nums : input) {
        sum1 += getPricePart1(nums);
        sum2 += getPricePart2(nums);
    }
    std::cout << "Part 1: " << sum1 << std::endl;
    std::cout << "Part 1: " << sum2 << std::endl;
}
