#include<iostream>  // cin��cout
#include<fstream>   // �����ļ���ȡ���뷽��
#include<sstream>   // �ַ���������
#include<string>    // �ַ�����
#include<io.h>      // ��������
#include<unordered_map>
#include "ToolHelper.h" //�������ݵ���
using namespace std;// ��׼�������ռ�


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
        cout << "�������ļ����Ե�ַ����cmd�����д�:" << endl;
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
