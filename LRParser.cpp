#include <iostream>
#include <set>
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
    void generateTable();
    void predictAnalysis(std::string &input);

private:
    inline std::string cutStr(const std::string &str, int pos); // 对字符串进行切片, 便于从指定位置开始输出

private:
    // std::unordered_map<char, std::vector<int>> gramap;
    std::unordered_map<int, std::string> grammar;
    std::vector<std::vector<std::string>> Action;
    std::vector<std::vector<int>> Goto;
};

int main()
{
    std::string str = "10,23,,4,", tmp;
    std::vector<std::string> res;
    std::stringstream ss(str);
    while (std::getline(ss, tmp, ','))
    {
        res.push_back(tmp);
    }
    for (auto &st : res)
        printf("%s ", st.c_str());
    return 0;
}

void LRParser::readFromFile(const std::string &fileName)
{
    std::fstream fileIn;
    fileIn.open(fileName);
    
}

void LRParser::predictAnalysis(std::string &input)
{
    std::vector<int> state;
    std::vector<char> symbol;
    state.push_back(0);
    symbol.push_back('\0');
    int ip = 0;
    char s1[] = "State stack", s2[] = "Symbol Stack", act[] = "Analysis";
    printf("%20s%20s%20s%30s\n", s1, s2, input, act);
    while (1)
    {
        int topS = state[0];
        char now = input[ip];
        int ord = Action[topS][now][1] - '0';
        if (Action[topS][now][0] == 'S')
        {
            state.push_back(ord);
            symbol.push_back(now);
            ip++;
            std::string act = "Shift " + char(ord + '0');
            printf("%20s%20s%20s%30s\n", state, symbol, cutStr(input, now), act);
        }
        else if (Action[topS][now][0] == 'R')
        {
            int len = grammar[ord].size() - 2;
            for (int i = 0; i < len; i++)
            {
                state.pop_back();
                symbol.pop_back();
            }
            topS = state[0];
            now = grammar[ord][0];
            state.push_back(Goto[topS][now]);
            symbol.push_back(now);
            std::string act = "Remove by " + grammar[ord];
            printf("%20s%20s%20s%30s\n", state, symbol, cutStr(input, now), act);
        }
        else if (Action[topS][now] == "accept")
        {
            printf("Accept\n");
        }
        else
            printf("Error\n");
    }
}

inline std::string LRParser::cutStr(const std::string &str, int pos)
{
    std::string tmp;
    for (int i = pos; i < str.size(); i++)
        tmp += str[i];
    return tmp;
}