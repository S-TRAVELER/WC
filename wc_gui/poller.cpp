#include "poller.h"
#if defined(__WIN32)
#include <io.h>
#else
#include <dirent.h>
#endif
#include <iostream>


void Poller::travel(const string &str,const std::shared_ptr<ostream> &stream,bool recursive,bool match){
    if(recursive||match){
        size_t&& pos=str.rfind("/");
        string path,rawStr;
        if(pos!=str.npos){
            path=str.substr(0,pos);
            rawStr=str.substr(pos+1);
        }else{
//            char buff[1000];
//            _getcwd(buff, 1000);
//            path=buff;
            path=".";
            rawStr=str;
        }

        toRegex(rawStr);
        cout<<rawStr<<endl;
        regex matchStr(rawStr);

        recurTravel(path,matchStr,stream,recursive);
    }else{
        _cb(stream,str);
    }

}
void Poller::recurTravel(const string &path, const regex &regexName,const std::shared_ptr<ostream> &stream,bool recursive){
    vector<pair<string,int>> dirvec;
#if defined(__WIN32)
    {
        long hFile = 0;
        struct _finddata_t fileInfo;
        string pathName;

        if ((hFile = _findfirst(pathName.assign(path).
                                append("\\*").c_str(), &fileInfo)) == -1) {
            (*stream)<<"无法打开： "<<path<<endl;
            return;
        }
        do {
            if (fileInfo.attrib&_A_SUBDIR) {
                dirvec.push_back({fileInfo.name,4});

            } else {
                dirvec.push_back({fileInfo.name,8});
            }
        } while (_findnext(hFile, &fileInfo) == 0);
        _findclose(hFile);
    }
#else
    {
        DIR* dir = opendir(path.c_str());//打开指定目录
        dirent* p = NULL;//定义遍历指针
        while((p = readdir(dir)) != NULL)//开始逐个遍历
        {
            dirvec.push_back({p->d_name,static_cast<int>(p->d_type)});
        }
        closedir(dir);//关闭指定目录
    }
#endif

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
    int left=0,right=str.size();
    for(auto &it:str){
        if(it!='\'')break;
        ++left;
    }
    for(auto it=str.end()-1;it!=str.begin();--it){
        if(*it!='\'')break;
        --right;
    }
    if(right==0){
        return;
    }
    str=str.substr(left,right-left);
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
