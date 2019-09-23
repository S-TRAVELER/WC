#ifndef UTIL_ONCETOKEN_H
#define UTIL_ONCETOKEN_H

#include <functional>
#include <type_traits>

namespace CMDToolkit {

using namespace std;

class onceToken
{
public:
    typedef function<void(void)> Task;
    onceToken(const Task &onConstructed, const Task &onDestructed=nullptr):_onDestructed(onDestructed){
        if(onConstructed){
            onConstructed();
        }
    }

    onceToken(const Task &onConstructed, const Task &&onDestructed):_onDestructed(move(onDestructed)){
        if(onConstructed){
            onConstructed();
        }
    }

    ~onceToken(){
        if(_onDestructed){
            _onDestructed();
        }
    }

    onceToken()=delete ;
    onceToken(const onceToken &)=delete ;
    onceToken &operator=(const onceToken&)=delete ;
    onceToken &operator=(onceToken&&)=delete ;

private:
    Task _onDestructed;

};

}   //namespace CMDToolkit
#endif // UTIL_ONCETOKEN_H
