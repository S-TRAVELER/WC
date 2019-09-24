#include "poller.h"
#include <dirent.h>
#include <iostream>

void Poller::travel(const string &str,const std::shared_ptr<ostream> &stream,bool recursive){
    size_t&& pos=str.rfind("/");
    string path,rawStr;
    if(pos!=str.npos){
        path=str.substr(0,pos);
        rawStr=str.substr(pos+1);
    }else{
        path=".";
        rawStr=str;
    }
    toRegex(rawStr);
    regex matchStr(rawStr);
    recurTravel(path,matchStr,stream,recursive);
}
void Poller::recurTravel(const string &path, const regex &regexName,const std::shared_ptr<ostream> &stream,bool recursive){
    vector<pair<string,int>> dirvec;

    DIR* dir = opendir(path.c_str());//打开指定目录
    dirent* p = NULL;//定义遍历指针
    while((p = readdir(dir)) != NULL)//开始逐个遍历
    {
        dirvec.push_back({p->d_name,static_cast<int>(p->d_type)});
    }
    closedir(dir);//关闭指定目录

    for(auto &it:dirvec)
    {
        //这里需要注意，linux平台下一个目录中有"."和".."隐藏文件，需要过滤掉
        if(it.first[0] != '.')//d_name是一个char数组，存放当前遍历到的文件名
        {
            string name =it.first;
            if(recursive&&it.second==4){
                recurTravel(path+"/"+name,regexName,stream,recursive);
            }else if((it.second)==8&&regex_match(name,regexName)){
                _cb(stream,path+"/"+name);
            }

        }
    }
}
void Poller::toRegex(string &str){ //通配符转正则

    size_t pos=str.find(".");
    while(pos!=str.npos){
        str.insert(pos,"\\");
        pos=str.find(".",pos+3);
    }
    pos=str.find("?");
    while(pos!=str.npos){
        str[pos]='.';
        pos=str.find("?");
    }
    pos=str.find("*");
    while(pos!=str.npos){
        str.insert(pos,".");
        pos=str.find("*",pos+2);
    }

}
