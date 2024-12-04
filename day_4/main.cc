#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void part1();
void part2();

int main()
{
    part1();
    part2();
    return 0;
}

std::vector<std::string> parseFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
        lines.push_back(line);
    file.close();
    return lines;
}

void part1()
{
    std::vector<std::string> input = parseFile("input.txt");
    int count = 0;
    std::string lookup = "XMAS";
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            for (auto dir : directions)
            {
                bool found = true;
                for (int k = 0; k < lookup.length(); k++)
                {
                    if (input[i + k * dir[0]][j + k * dir[1]] != lookup[k])
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    count++;
            }
        }
    }
    std::cout << count << std::endl;
}

void part2()
{
    std::vector<std::string> input = parseFile("input.txt");
    int count = 0;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'A')
            {
                std::string diag1 = {input[i - 1][j - 1], input[i + 1][j + 1]};
                std::string diag2 = {input[i - 1][j + 1], input[i + 1][j - 1]};
                if ((diag1 == "MS" || diag1 == "SM") && (diag2 == "MS" || diag2 == "SM"))
                    count++;
            }
        }
    }
    std::cout << count << std::endl;
}
