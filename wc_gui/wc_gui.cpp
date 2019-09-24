#include <iostream>
#include "Cmd/CMD.h"
#include "Util/util.h"
#include <signal.h>
#include "WinGui.h"
#include <QApplication>
#include "counter.h"
#include "poller.h"



using namespace std;
using namespace CMDToolkit;

char **app;

//命令(http)
class CMD_wc: public CMD {
public:
    CMD_wc(){

        _cb=[this](const std::shared_ptr<ostream> &stream, const string &arg){
            cout<<"file: "<<arg<<endl;
            return true;
        };

        _parser.reset(new OptionParser([this](const std::shared_ptr<ostream> &stream,map<string,variant> &args){
            //所有选项解析完毕后触发该回调，我们可以在这里做一些全局的操作
            total_bcount=0,total_lcount=0,total_wcount=0,total_ncount=0,total_ecount=0;
            _isMulti=false;

            if(!hasKey("bytes")&&!hasKey("lines")&&!hasKey("words")){
                args.emplace("bytes","");
                args.emplace("lines","");
                args.emplace("words","");
            }
            if(hasKey("version")||hasKey("gui")){
                return true;
            }
            if(_argvs.size()==0){
                (*stream)<<"wc: 请指定统计的文件,输入\"-h\"获取帮助."<<endl;;
                return false;
            }
            for(auto &it:_argvs){
                _poller->travel(it,stream,hasKey("match"));
            }
           if(_isMulti){
                if(hasKey("lines")){
                    (*stream)<<" "<<total_lcount<<"\t";
                }
                if(hasKey("words")){
                    (*stream)<<" "<<total_wcount<<"\t";
                }
                if(hasKey("bytes")){
                    (*stream)<<" "<<total_bcount<<"\t";
                }
                if(hasKey("complex")){
                    (*stream)<<" "<<total_ncount<<"\t";
                    (*stream)<<" "<<total_ecount<<"\t";
                }
                (*stream)<<" Total"<<endl;
            }
        }));
        _poller.reset(new Poller([this](const std::shared_ptr<ostream> &stream,const string &arg){
            long lcount,wcount,bcount,ncount,ecount;
             Counter::Instance().count(arg,lcount,wcount,bcount,ncount,ecount);
             if(bcount==-1){
                 (*stream)<<"无法打开文件："<<arg<<endl;
                 return;
             }
             if(!_isMulti&&total_bcount>0){
                 _isMulti=true;
             }
            if(hasKey("lines")){
                total_lcount+=lcount;
                (*stream)<<" "<<lcount<<"\t";
            }
            if(hasKey("words")){
                total_wcount+=wcount;
                (*stream)<<" "<<wcount<<"\t";
            }
            if(hasKey("bytes")){
                total_bcount+=bcount;
                (*stream)<<" "<<bcount<<"\t";
            }
            if(hasKey("complex")){
                total_ncount+=ncount;
                total_ecount+=ecount;
                (*stream)<<" "<<ncount<<"\t";
                (*stream)<<" "<<ecount<<"\t";
            }
            (*stream)<<" "<<arg<<endl;

        }));

        (*_parser) << Option('c',/*该选项简称，如果是\x00则说明无简称*/
                             "bytes",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "显示Bytes数",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                    return true;
                             });

        (*_parser) << Option('l',/*该选项简称，如果是\x00则说明无简称*/
                             "lines",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "显示行数",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                     return true;
                             });

        (*_parser) << Option('w',/*该选项简称，如果是\x00则说明无简称*/
                             "words",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "显示字数",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                     return true;
                             });

        (*_parser) << Option('v',/*该选项简称，如果是\x00则说明无简称*/
                             "version",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "查看命令版本",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                    (*stream)<<" Command line \"wc\" 1.0.0"<<endl
                                             <<" Based on Windows or Linux"<<endl
                                             <<" Author: ZWJ. All rights reserved."<<endl;

                                     return true;
                             });
        (*_parser) << Option('x',/*该选项简称，如果是\x00则说明无简称*/
                             "gui",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "查看命令版本",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                int tmp=1;
                                QApplication a(tmp, app);
                                WinGui w;
                                char *argv[]={app[0],"-h"};

                                w.show();
                                a.exec();
                                return true;

                             });

        (*_parser) << Option('s',/*该选项简称，如果是\x00则说明无简称*/
                             "match",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "查看命令版本",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/

                                    return true;

                             });

        (*_parser) << Option('a',/*该选项简称，如果是\x00则说明无简称*/
                             "complex",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgNone,/*该选项后面必须跟值*/
                             nullptr,/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "查看命令版本",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/
                                    return true;

                             });

        (*_parser) << Option('L',/*该选项简称，如果是\x00则说明无简称*/
                             "Language",/*该选项全称,每个选项必须有全称；不得为null或空字符串*/
                             Option::ArgRequired,/*该选项后面必须跟值*/
                             "C++",/*该选项默认值*/
                             false,/*该选项是否必须赋值，如果没有默认值且为ArgRequired时用户必须提供该参数否则将抛异常*/
                             "查看命令版本",/*该选项说明文字*/
                             [this](const std::shared_ptr<ostream> &stream, const string &arg){/*解析到该选项的回调*/

                                    RulesParser::RuleList_ptr tmp=RulesParser::Instance().getRule(arg);
                                    if(tmp==nullptr){
                                        (*stream)<<"不支持："<<arg<<" ,"<<"设为默认语言 C++"<<endl;
                                        (*stream)<<"支持语言：";
                                        RulesParser::Instance().print(stream);
                                    }else{
                                        Counter::Instance().setRule(tmp);
                                    }
                                    return true;
                             });
    }

    ~CMD_wc() {}

    const char *description() const override {
        return "WC统计";
    }
private:
    shared_ptr<Poller> _poller;
    long total_bcount,total_lcount,total_wcount,total_ncount,total_ecount;
    bool _isMulti;

};


int main(int argc,char *argv[]){

    app=&argv[0];
    REGIST_CMD(wc);
    signal(SIGINT,[](int ){
        exit(0);
    });
    try{
        CMD_DO("wc",argc,argv);
    }catch (std::exception &ex){
        cout << ex.what() << endl;
        return 0;
    }

    return 0;
}
