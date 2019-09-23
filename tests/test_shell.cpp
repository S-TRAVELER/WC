#include <iostream>
#include "Cmd/CMD.h"
#include "Util/util.h"
#include <signal.h>

using namespace std;
using namespace CMDToolkit;

//命令(http)
class CMD_test: public CMD {
public:
    CMD_test(){
        _parser.reset(new OptionParser([this](const std::shared_ptr<ostream> &stream,mINI &args){
            //所有选项解析完毕后触发该回调，我们可以在这里做一些全局的操作
            for(auto &it:args){
                cout<<it.first<<" "<<it.second<<endl;
            }
            if(hasKey("connect")){
                connect(stream);
                return;
            }
            if(hasKey("commit")){
                commit(stream);
                return;
            }
        }));

        (*_parser) << Option('T', "type", Option::ArgRequired, nullptr, true, "应用程序模式，0：传统模式，1：shell模式", nullptr);

        (*_parser) << Option('s',/*该选项简称，如果是\x00则说明无简称*/
                             "server",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgRequired,/*该选项后面必须跟值*/
                             "www.baidu.com:80",/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "tcp服务器地址，以冒号分隔端口号",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                 cout<<"server operator"<<endl;
                                 if(arg.find(":") == string::npos){
                                     //中断后续选项的解析以及解析完毕回调等操作
                                     throw std::runtime_error("\t地址必须指明端口号.");
                                 }
                                 //如果返回false则忽略后续选项的解析
                                 return true;
                             });

        (*_parser) << Option('d', "disconnect", Option::ArgNone, nullptr ,false, "是否断开连接",
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){
                                 //断开连接操作，所以后续的参数我们都不解析了
                                 disconnect(stream);
                                 return false;
                             });

        (*_parser) << Option('c', "connect", Option::ArgNone, nullptr, false, "发起tcp connect操作", nullptr);
        (*_parser) << Option('t', "time_out", Option::ArgRequired, "3",false, "连接超时间", nullptr);
        (*_parser) << Option('m', "method", Option::ArgRequired, "GET",false, "HTTP方法,譬如GET、POST", nullptr);
        (*_parser) << Option('p', "path", Option::ArgRequired, "/index.html",false, "HTTP url路径", nullptr);
        (*_parser) << Option('C', "commit", Option::ArgNone, nullptr, false, "提交HTTP请求", nullptr);
    }

    ~CMD_test() {}

    const char *description() const override {
        return "http测试客户端";
    }

private:
    void connect(const std::shared_ptr<ostream> &stream){
        (*stream) << "connect操作" << endl;
    }
    void disconnect(const std::shared_ptr<ostream> &stream){
        (*stream) << "disconnect操作" << endl;
    }
    void commit(const std::shared_ptr<ostream> &stream){
        (*stream) << "commit操作" << endl;
    }

};


int main(int argc,char *argv[]){
    REGIST_CMD(test);
    signal(SIGINT,[](int ){
        exit(0);
    });
    try{
        CMD_DO("test",argc,argv);
    }catch (std::exception &ex){
        cout << ex.what() << endl;
        return 0;
    }
    if(GET_CMD("test")["type"] == 0){
        cout << "传统模式，已退出程序，请尝试shell模式" << endl;
        return 0;
    }
    GET_CMD("test").delOption("type");

    cout << "> 欢迎进入命令模式，你可以输入\"help\"命令获取帮助" << endl;
    string cmd_line;
    while(cin.good()){
        try{
            cout << "> ";
            getline(cin,cmd_line);
            CMDRegister::Instance()(cmd_line);
        }catch (ExitException &ex){
            break;
        }catch (std::exception &ex){
            cout << ex.what() << endl;
        }
    }
    return 0;
}
