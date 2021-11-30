#include <iostream>
#include <set>
#include <cmath>
#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

class LRParser
{
public:
    void readFromFile(const std::string &fileName);
    // void generateTable();
    void predictAnalysis(std::string &input);

private:
    inline std::string cutStr(const std::string &str, int pos); // 对字符串进行切片, 便于从指定位置开始输出
    inline void output(const std::vector<int> &out, int len);
    inline void output(const std::vector<char> &out, int len);
    inline std::vector<std::string> split(const std::string &str, const char delim);

private:
    // std::unordered_map<char, std::vector<int>> gramap;
    std::unordered_map<int, std::string> grammar;
    std::unordered_map<char, int> non, ter;
    std::vector<std::vector<std::string>> Action;
    std::vector<std::vector<int>> Goto;
};

int main(/*int argc, char *argv[]*/)
{
    // if (argc != 2)
    //     std::runtime_error("Error: Need 2 parameters\n");
    LRParser parser;
    // parser.readFromFile(argv[1]);
    parser.readFromFile("test1.in");
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
    fileIn >> num;
    for (int i = 1; i <= num; i++)
    {
        fileIn >> grammar[i];
    }
    int terNum;
    fileIn >> terNum;
    for (int i = 0; i < num; i++)
    {
        char tmp;
        fileIn >> tmp;
        ter[tmp] = i;
    }
    fileIn >> num;
    Action.resize(num + 1);
    std::string line;
    std::vector<std::string> tmp;
    getline(fileIn, line);
    for (int i = 0; i <= num; i++)
    {
        Action[i].resize(terNum);
        getline(fileIn, line);
        tmp = split(line, ',');
        for (int j = 0; j < (int)ter.size(); j++)
        {
            Action[i][j] = tmp[j];
        }
    }
    int nonNum;
    fileIn >> nonNum;
    for (int i = 0; i < nonNum; i++)
    {
        char tmp;
        fileIn >> tmp;
        non[tmp] = i;
    }
    getline(fileIn, line);
    getline(fileIn, line);
    Goto.resize(num + 1);
    for (int i = 0; i <= num; i++)
    {
        Goto[i].resize(nonNum);
        getline(fileIn, line);
        tmp = split(line, ',');
        for (int j = 0; j < (int)nonNum; j++)
        {
            if (!tmp[j].empty())
                Goto[i][j] = std::stoi(tmp[j]);
        }
    }
}

void LRParser::predictAnalysis(std::string &input)
{
    std::vector<int> state;
    std::vector<char> symbol;
    state.push_back(0);
    symbol.push_back('#');
    int ip = 0;
    char s1[] = "State stack", s2[] = "Symbol Stack", in[] = "input", act[] = "Analysis";
    printf("%-20s%-20s%-20s%-30s\n", s1, s2, in, act);
    input += "$";
    while (1)
    {
        int topS = state.back();
        char now = input[ip];
        int ord = Action[topS][ter[now]][1] - '0';
        if (Action[topS][ter[now]][0] == 'S')
        {
            output(state, 20);
            output(symbol, 20);
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
            output(state, 20);
            output(symbol, 20);
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
            output(state, 20);
            output(symbol, 20);
            printf("%-20s", cutStr(input, ip).c_str());
            printf("Accept\n");
            return;
        }
        else
        {
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
            printf("%d", out[i]);
            if (out[i] >= 10)
                len -= int(log10(out[i]));
        }
        else
            printf(" ");
        i++;
    }
}

inline void LRParser::output(const std::vector<char> &out, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i < (int)out.size())
            printf("%c", out[i]);
        else
            printf(" ");
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