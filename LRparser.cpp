#include <iostream>
#include <set>
#include <cmath>
#include <stack>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>

class LRParser
{
public:
    void readFromFile(const std::string &fileName); // 从文件中获取预测分析表
    void predictAnalysis(std::string &input); // 进行预测分析

private:
    inline std::string cutStr(const std::string &str, int pos); // 对字符串进行切片, 便于从指定位置开始输出
    inline void output(const std::vector<int> &out, int len); // 从指定位置按格式输出vector<int>
    std::vector<std::string> split(const std::string &str, const char delim); // 根据指定字符切割字符串

private:
    std::unordered_map<int, std::string> grammar; // 文法，编号与产生式对应
    std::unordered_map<char, int> non, ter; // 终结符与非终结符，与编号对应
    std::vector<std::vector<std::string>> Action; // Action子表
    std::vector<std::vector<int>> Goto; // Goto子表
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        std::runtime_error("Error: Need 2 parameters\n");
    LRParser parser;
    parser.readFromFile(argv[1]);
    // parser.readFromFile("test0.in");
    std::string input;
    std::cout << "Please enter the symbols:" << std::endl;
    std::cin >> input;
    parser.predictAnalysis(input);
    return 0;
}

void LRParser::readFromFile(const std::string &fileName)
{
    std::fstream fileIn;
    fileIn.open(fileName);
    int num;
    std::string line;
    fileIn >> num;
    for (int i = 0; i < num; i++)
        fileIn >> grammar[i];
    int terNum, nonNum;
    fileIn >> terNum >> nonNum;
    getline(fileIn, line);
    getline(fileIn, line);
    for (int i = 0; i < terNum; i++)
        ter[line[i]] = i;
    for (int i = terNum; i < (int)line.size(); i++)
        non[line[i]] = i - terNum;
    fileIn >> num;
    Action.resize(num + 1);
    Goto.resize(num + 1);
    std::vector<std::string> tmp;
    getline(fileIn, line);
    for (int i = 0; i <= num; i++)
    {
        Action[i].resize(terNum);
        Goto[i].resize(nonNum, -1);
        getline(fileIn, line);
        tmp = split(line, ',');
        for (int j = 0; j < (int)ter.size(); j++)
            Action[i][j] = tmp[j];
        for (int j = terNum; j < (int)tmp.size(); j++)
            if (tmp[j].size())
                Goto[i][j - terNum] = std::stoi(tmp[j]);
    }
    fileIn.close();
}

void LRParser::predictAnalysis(std::string &input)
{
    std::vector<int> state;
    std::string symbol;
    state.push_back(0);
    symbol.push_back('#');
    int ip = 0;
    char s1[] = "State stack", s2[] = "Symbol Stack", in[] = "input", act[] = "Analysis";
    printf("%-30s%-20s%-20s%-30s\n", s1, s2, in, act);
    input += "$";
    while (1)
    {
        int topS = state.back();
        char now = input[ip];
        if (ter.find(now) == ter.end())
        {
            output(state, 30);
            std::cout << std::setw(20) << std::left << symbol;
            printf("%-20s", cutStr(input, ip).c_str());
            printf("Error: symbol %c is invalid\n", now);
            return;
        }
        if (Action[topS][ter[now]][0] == 'S')
        {
            int ord = std::stoi(cutStr(Action[topS][ter[now]], 1));
            output(state, 30);
            std::cout << std::setw(20) << std::left << symbol;
            printf("%-20s", cutStr(input, ip).c_str());
            state.push_back(ord);
            symbol.push_back(now);
            ip++;
            std::string ana = "Shift ";
            ana += char(ord + '0');
            printf("%-30s\n", ana.c_str());
        }
        else if (Action[topS][ter[now]][0] == 'R')
        {
            int ord = std::stoi(cutStr(Action[topS][ter[now]], 1));
            output(state, 30);
            std::cout << std::setw(20) << std::left << symbol;
            int len = grammar[ord].size() - 3;
            for (int i = 0; i < len; i++)
            {
                state.pop_back();
                symbol.pop_back();
            }
            topS = state.back();
            now = grammar[ord][0];
            state.push_back(Goto[topS][non[now]]);
            symbol.push_back(now);
            std::string ana = "Remove by " + grammar[ord];
            printf("%-20s%-30s\n", cutStr(input, ip).c_str(), ana.c_str());
        }
        else if (Action[topS][ter[now]] == "accept")
        {
            output(state, 30);
            std::cout << std::setw(20) << std::left << symbol;
            printf("%-20s", cutStr(input, ip).c_str());
            printf("Accept\n");
            return;
        }
        else
        {
            output(state, 30);
            std::cout << std::setw(20) << std::left << symbol;
            printf("%-20s", cutStr(input, ip).c_str());
            printf("Error\n");
            return;
        }
    }
}

inline std::string LRParser::cutStr(const std::string &str, int pos)
{
    std::string tmp;
    for (int i = pos; i < str.size(); i++)
        tmp += str[i];
    return tmp;
}

inline void LRParser::output(const std::vector<int> &out, int len)
{
    int i = 0;
    while (i < len)
    {
        if (i < (int)out.size())
        {
            printf("%d ", out[i]);
            len--;
            if (out[i] >= 10)
                len -= int(log10(out[i]));
        }
        else
            printf(" ");
        i++;
    }
}

inline std::vector<std::string> LRParser::split(const std::string &str, const char delim)
{
    std::stringstream input(str);
    std::string tmp;
    std::vector<std::string> res;
    while (getline(input, tmp, delim))
        res.push_back(tmp);
    return res;
}