#include<iostream>  // cin、cout
#include<fstream>   // 包含文件读取类与方法
#include<sstream>   // 字符串流操作
#include<string>    // 字符串类
#include<io.h>      // 遍历操作
#include<unordered_map>
#include "ToolHelper.h" //处理数据的类
using namespace std;// 标准库命名空间


unordered_map<string, string> hashmap;
vector<string> Datas{ "CS_LOGIN", "CS_SYSTEM", "CS_ACCOUNT" };



int main(int argc, char* argv[])
{
    string file;
    for (int i = 1; i <= Datas.size(); i++)
    {
        hashmap[to_string(i)] = Datas[i - 1];
    }

    if (argc == 1)
    {
        cout << "请输入文件绝对地址或者cmd命令中打开:" << endl;
        cin >> file;
    }
    else 
    {
        file = argv[1];

    }
    cout << file << endl;
    ToolHelper TH(file, hashmap);
    TH.FindFiles();

    return 0;
}
