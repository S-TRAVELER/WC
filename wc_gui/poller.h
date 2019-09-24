#ifndef POLLER_H
#define POLLER_H

#include <functional>
#include <memory>
#include <regex>
#include "Util/util.h"

using namespace std;
using namespace CMDToolkit;

class Poller:public noncopyable
{
public:
    typedef function<void(const std::shared_ptr<ostream> &stream,const string &arg)> Task;
    Poller(const Task& cb):_cb(cb){}
    void travel(const string &str,const std::shared_ptr<ostream> &stream,bool recursive=false);

private:
    void recurTravel(const string &path, const regex &regexName,const std::shared_ptr<ostream> &stream,bool recursive=false);
    void toRegex(string &str);
private:
    Task _cb;
};

#endif // POLLER_H
