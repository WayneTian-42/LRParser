<div class="cover" style="page-break-after:always;font-family:方正公文仿宋;width:100%;height:100%;border:none;margin: 0 auto;text-align:center;">
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:10%;">
        </br>
       <img src="https://raw.githubusercontent.com/WayneTian-42/imageRepo/master/school.png" alt="校名" style="width:100%;"/>
    </div>
	</br></br></br></br></br></br></br></br>
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:40%;">
        <img src="https://raw.githubusercontent.com/WayneTian-42/imageRepo/master/bupt.png" alt="校徽" style="width:80%;"/>
	</div>
    </br></br></br></br></br></br>
    <span style="font-family:华文黑体Bold;text-align:center;font-size:20pt;margin: 10pt auto;line-height:30pt;">《编译原理与技术》</span>
    <p style="text-align:center;font-size:14pt;margin: 0 auto">实验报告</p>
    </br>
    </br>
    <table style="border:none;text-align:center;width:72%;font-family:仿宋;font-size:14px; margin: 0 auto;">
    <tbody style="font-family:方正公文仿宋;font-size:12pt;">
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">题　　目</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">LR语法分析</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">班　　级</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2019211307</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">姓　　名</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"> 田文阳</td>     </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:right;">学　　号</td>
    		<td style="width:2%">：</td> 
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2019211438 </td>     </tr>
    </tbody>              
    </table>
</div>

<!-- 注释语句：导出PDF时会在这里分页 -->

# LR语法分析实验报告

## 实验内容

​		编写语法分析程序实现自底向上的分析，构造LR分析程序。根据实验要求分发来看，决定实现SLR(0)文法，输入SLR(0)预测分析表，然后输入要分析的串，程序输出分析过程。

## 开发环境

* 操作系统：Linux Ubuntu 20.04.3 LTS
* IDE: Visual Studio Code 1.61.2
* 编译器: gcc 9.3.0 

## 设计思路

​		LR预测分析过程的算法书上已经通过伪代码方式给出，因此只需要将其转换为C++代码即可。

![9D0E1E11-4B7C-494F-A30E-7AA951DE8C23](https://raw.githubusercontent.com/WayneTian-42/imageRepo/master/2021/12/02/20211202-113557.jpeg)

## 代码实现

### 数据结构

```cpp
	std::unordered_map<int, std::string> grammar; // 文法，编号与产生式对应
    std::unordered_map<char, int> non, ter; // 终结符与非终结符，与编号对应
    std::vector<std::vector<std::string>> Action; // Action子表
    std::vector<std::vector<int>> Goto; // Goto子表
```

### 函数实现

```cpp
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
    // 数据结构如上述所示
};
```

### main函数实现

```cpp
int main(int argc, char *argv[])
{
    if (argc != 2)
        std::runtime_error("Error: Need 2 parameters\n");
    LRParser parser;
    parser.readFromFile(argv[1]);
    std::string input;
    std::cout << "Please enter the symbols:" << std::endl;
    std::cin >> input;
    parser.predictAnalysis(input);
    return 0;
}
```

## 测试

### 测试输入说明

​		因为程序只实现了LR预测分析过程，因此要读入预测分析表，首先要靠考虑的就是预测分析表输入方式，本程序所要求的输入格式如下

​		数字m：表示接下来共有m个文法产生式，**注意每行只允许一个产生式**
​		m行产生式
​		_空行_
​		数字n 数字r：表示有n个终结符(**包括`$`**)，和r个非终结符(**不包括拓广文法起始符`S`**)
​		一行内输入n个终结符和r个非终结符，**中间不要空格分割**
​		_空行_
​		数字t：表示有**t+1**行预测分析表
​		t+1行分析表，注意每一个表项结束后以`,`结尾

​		程序运行时，需要在命令行输入程序名及输入文件路径

### 测试样例：书上文法

#### 样例输入

```
9
S->E
E->E+T
E->E-T
E->T
T->T*F
T->T/F
T->F
F->(E)
F->n

8 3
+-*/()n$ETF

15
,,,,S4,,S5,,1,2,3,
S6,S7,,,,,,accept,,,,
R3,R3,S8,S9,,R3,,R3,,,,
R6,R6,R6,R6,,R6,,R6,,,,
,,,,S4,,S5,,10,2,3,
R8,R8,R8,R8,,R8,,R8,,,,
,,,,S4,,S5,,,11,3,
,,,,S4,,S5,,,12,3,
,,,,S4,,S5,,,,13,
,,,,S4,,S5,,,,14,
S6,S7,,,,S15,,,,,,
R1,R1,S8,S9,,R1,,R1,,,,
R2,R2,S8,S9,,R2,,R2,,,,
R4,R4,R4,R4,,R4,,R4,,,,
R5,R5,R5,R5,,R5,,R5,,,,
R7,R7,R7,R7,,R7,,R7,,,,
```

#### 程序执行及输出

![2021-12-02 17-28-07 的屏幕截图](https://raw.githubusercontent.com/WayneTian-42/imageRepo/master/2021/12/02/20211202-172838.png)

### 测试样例：额外文法

#### 样例输入

```
6
E->S
S->aSAB
S->BA
A->aA
A->B
B->b

3 3
ab$SAB

11
S3,S4,,1,,2,
,,accept,,,,
S6,S4,,,5,7,
S3,S4,,8,,2,
R5,R5,R5,,,,
R2,R2,R2,,,,
S6,S4,,,9,7,
R4,R4,R4,,,,
S6,S4,,,10,7
R3,R3,R3,,,,
,S4,,,,11,
R1,R1,R1,,,,
```

#### 程序运行及输出

![2021-12-02 17-47-37 的屏幕截图](https://raw.githubusercontent.com/WayneTian-42/imageRepo/master/2021/12/02/20211202-174754.png)

## 心得体会

​		通过本次实验，加深了对于LR(0)，SLR(1)等文法的理解，掌握了手动构造LR分析程序的算法，并能够实现该算法。手动构造完整的LR分析程序远比想象中的复杂，鉴于时间和个人能力因素本实验仅仅完成了最基本的LR分析程序的构建。希望能够在假期空闲时间完成完整的LR分析程序。
