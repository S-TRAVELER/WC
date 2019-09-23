#ifndef WC_CMD_H
#define WC_CMD_H
#include <string>
#include <sys/stat.h>
#include <list>
#include "Util/util.h"

#if defined(_WIN32)
#define stat _stat
#endif

using namespace std;
using namespace CMDToolkit;

class Counter:public noncopyable{
public:
    static Counter& Instance();
    void count (const string &file,long &lcount,long &wcount, long &bcount,long &noteLines, long &emptyLines);

private:
    void countBytes(const string &file,long &bcount){
        struct stat info;
        if(stat(file.c_str(), &info)<0){
            bcount=-1 ;
        }
        bcount=info.st_size;
    }
    void DirCursor(list<string> fileNames){

    }
private:
    bool nextline=true;
    bool isNote=true;
};


};

#endif // WC_CMD_H
