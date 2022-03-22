#include "ToolHelper.h"
#include <fstream>
#include <io.h>
#include <iostream>
#include <wchar.h>

ToolHelper::ToolHelper(string file, unordered_map<string, string> hashmap):m_hashmap(hashmap),m_file(file)
{

}

void ToolHelper::Replace_all_Ditincte(string& str,const string& strFind, time_t startTime, ofstream& ofs)
{
    string::size_type pos = str.find(strFind, 0);
    if (pos == string::npos) return;
    pos += strFind.length() + 1; //指到要替换的位置上

    string temp = str.substr(pos, str.find(")", pos) - pos); //获得括号里的字符串
    if (!m_hashmap.count(temp)) return;

    string TransStr = m_hashmap.find(temp)->second;
    if (strFind == "nMainCmd" || strFind == "nSubCmd")
    {
        TransStr = TransStr.replace(0, 2, "SC");
    }
    str.replace(pos, str.find(")", pos) - pos, TransStr);

    //{"ts": 100,"tid": "TCP - iMainCmd","name": "CS_LOGIN","ph": "X","dur": 100,"pid": 1,"cname": "yellow"},
    string::size_type findPos = str.find("'");
    time_t currTime = StringToDatetime(str.substr( findPos + 1, str.find("'", findPos + 1) - findPos - 1));
    string ts = to_string((currTime - startTime)*1000000);
    string jsons;
    jsons += " { \"ts\":" + ts;
    jsons += ", \"tid\":\"" + strFind + "\"";
    jsons += ", \"name\":\"" + TransStr + "\"";
    string pid = "1";
    if(strFind == "nMainCmd" || strFind == "nSubCmd")
    {
        pid = "2";
    }
    jsons += ", \"ph\": \"X\",\"dur\": 1000000,\"pid\":"+ pid + " }, ";

    cout << jsons << endl;
    ofs << jsons << endl;
}

void ToolHelper::GetInfo()
{
    string filePath = m_file;

    ifstream logFile(filePath);
    if (logFile.fail()) {
        cout << "input file open fail" << endl;
        return;
    }
    string outfile = "output" + filePath;
    ofstream coutReport(outfile);
    if (coutReport.fail()) {
        cout << "output file create fail" << endl;
        return;
    }
    string jsonFile_name = filePath.replace(filePath.rfind("."), filePath.length() - filePath.rfind("."), ".json");
    cout << jsonFile_name << endl;
    ofstream JsonReport(jsonFile_name);
    if (JsonReport.fail()) {
        cout << "Json file create fail" << endl;
        return;
    }
    JsonReport << "{\"traceEvents\": [" << endl;

    string str;
    getline(logFile, str); coutReport << str;
    string::size_type findPos = str.find("'");
    time_t StartTime = StringToDatetime(str.substr(findPos + 1, str.find("'", findPos + 1) - findPos - 1));
        cout << StartTime << endl;

    //隔行读入数据
    while (getline(logFile, str))
    {
        Replace_all_Ditincte(str, "TCP - iMainCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "iSubCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "nMainCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "nSubCmd", StartTime, JsonReport);

        coutReport << str << endl;
        str.clear(); //重复加载，所以每次需要清空
    }
    JsonReport << "{}" << "]}";

    //关闭文件
    logFile.close();
    coutReport.close();
    JsonReport.close();
}

void ToolHelper::FindFiles()
{
    string strFile = m_file;
    struct _finddata_t fileinfo;
    /***遍历目录系统函数要求先尝试寻找一个文件，看是否存在***/
    intptr_t handle;
    if ((handle = _findfirst(strFile.c_str(), &fileinfo)) == -1L)
    {
        cout << "查询log文件失败" << endl;
        return;
    }
    else
    {
        //strFile = file + fileinfo.name;

        GetInfo();     // 文件处理

        ///***一直遍历，直到所有.log文件得到加载与处理***/
        //while (!(_findnext(handle, &fileinfo)))
        //{
        //    strFile = file + fileinfo.name;
        //    GetInfo(strFile); // 文件处理
        //}
        //coutReport.close(); // 释放文件加载

    }
    _findclose(handle); // 释放遍历目录的句柄
    cout << "文件生成成功" << endl;
}

time_t ToolHelper::StringToDatetime(string str)
 {
    str = "2022-03-17 " + str;
     char* cha = (char*)str.data();             // 将string转换成char*。
     tm tm_;                                    // 定义tm结构体。
     int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
     sscanf_s(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
     tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
     tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
     tm_.tm_mday = day;                         // 日。
     tm_.tm_hour = hour;                        // 时。
     tm_.tm_min = minute;                       // 分。
     tm_.tm_sec = second;                       // 秒。
     tm_.tm_isdst = 0;                          // 非夏令时。
     time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
     return t_;                                 // 返回值。 
 }

ToolHelper::~ToolHelper()
{

}
