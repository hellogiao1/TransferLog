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
    pos += strFind.length() + 1; //ָ��Ҫ�滻��λ����

    string temp = str.substr(pos, str.find(")", pos) - pos); //�����������ַ���
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

    //���ж�������
    while (getline(logFile, str))
    {
        Replace_all_Ditincte(str, "TCP - iMainCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "iSubCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "nMainCmd", StartTime, JsonReport);
        Replace_all_Ditincte(str, "nSubCmd", StartTime, JsonReport);

        coutReport << str << endl;
        str.clear(); //�ظ����أ�����ÿ����Ҫ���
    }
    JsonReport << "{}" << "]}";

    //�ر��ļ�
    logFile.close();
    coutReport.close();
    JsonReport.close();
}

void ToolHelper::FindFiles()
{
    string strFile = m_file;
    struct _finddata_t fileinfo;
    /***����Ŀ¼ϵͳ����Ҫ���ȳ���Ѱ��һ���ļ������Ƿ����***/
    intptr_t handle;
    if ((handle = _findfirst(strFile.c_str(), &fileinfo)) == -1L)
    {
        cout << "��ѯlog�ļ�ʧ��" << endl;
        return;
    }
    else
    {
        //strFile = file + fileinfo.name;

        GetInfo();     // �ļ�����

        ///***һֱ������ֱ������.log�ļ��õ������봦��***/
        //while (!(_findnext(handle, &fileinfo)))
        //{
        //    strFile = file + fileinfo.name;
        //    GetInfo(strFile); // �ļ�����
        //}
        //coutReport.close(); // �ͷ��ļ�����

    }
    _findclose(handle); // �ͷű���Ŀ¼�ľ��
    cout << "�ļ����ɳɹ�" << endl;
}

time_t ToolHelper::StringToDatetime(string str)
 {
    str = "2022-03-17 " + str;
     char* cha = (char*)str.data();             // ��stringת����char*��
     tm tm_;                                    // ����tm�ṹ�塣
     int year, month, day, hour, minute, second;// ����ʱ��ĸ���int��ʱ������
     sscanf_s(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������
     tm_.tm_year = year - 1900;                 // �꣬����tm�ṹ��洢���Ǵ�1900�꿪ʼ��ʱ�䣬����tm_yearΪint��ʱ������ȥ1900��
     tm_.tm_mon = month - 1;                    // �£�����tm�ṹ����·ݴ洢��ΧΪ0-11������tm_monΪint��ʱ������ȥ1��
     tm_.tm_mday = day;                         // �ա�
     tm_.tm_hour = hour;                        // ʱ��
     tm_.tm_min = minute;                       // �֡�
     tm_.tm_sec = second;                       // �롣
     tm_.tm_isdst = 0;                          // ������ʱ��
     time_t t_ = mktime(&tm_);                  // ��tm�ṹ��ת����time_t��ʽ��
     return t_;                                 // ����ֵ�� 
 }

ToolHelper::~ToolHelper()
{

}
