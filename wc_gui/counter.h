#ifndef WC_CMD_H
#define WC_CMD_H
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include <sys/stat.h>
#include "Util/util.h"

#if defined(_WIN32)
#define stat _stat
#endif

#define RuleList vector<vector<string>>

using namespace std;
using namespace CMDToolkit;

class RulesParser:public noncopyable{
public:
    typedef shared_ptr<RuleList> RuleList_ptr;

    static RulesParser& Instance();

    RuleList_ptr getRule(const string &name){
        if(_rulesMap.find(name)==_rulesMap.end()){
            return nullptr;
        }
        return _rulesMap[name];
    }
    void print(const std::shared_ptr<ostream> &stream){
        for(auto &it:_rulesMap){
            (*stream)<<it.first<<" ";
        }
         (*stream)<<endl;
    }
private:
    RulesParser();
private:
    map<string, RuleList_ptr> _rulesMap;
};

class Counter:public noncopyable{
public:
    static Counter& Instance();
    void count (const string &file,long &lcount,long &wcount, long &bcount,long &noteLines, long &emptyLines);
    void setRule(RulesParser::RuleList_ptr lanVec){
        if(lanVec!=nullptr){
            _lanVec=lanVec;
        }
    }

private:
    void countBytes(const string &file,long &bcount){
        struct stat info;
        if(stat(file.c_str(), &info)<0){
            bcount=-1 ;
        }
        bcount=info.st_size;
    }

private:
    Counter(){
        _lanVec=make_shared<RuleList>();
        _lanVec->push_back({"/*","*/"});
        _lanVec->push_back({"//"});
    }
private:
    bool nextline=true;
    bool isNote=true;
    bool noteLine=true;
    RulesParser::RuleList_ptr _lanVec;
};




#endif // WC_CMD_H
