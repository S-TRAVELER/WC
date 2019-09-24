#include <iostream>
#include "Cmd/CMD.h"
#include "Util/util.h"
#include <signal.h>
#include <sys/stat.h>


using namespace std;
using namespace CMDToolkit;

#if defined(_WIN32)
#define stat _stat
#endif


//命令(http)
class CMD_wc: public CMD {
public:
    CMD_wc(){

        _cb=[this](const std::shared_ptr<ostream> &stream, const string &arg){

            /*countBytes(stream,arg);
            FILE *fp = NULL;
            fp = fopen(arg.c_str(), "r");
            if(fp==NULL){
                return false;
            }
            count(fp);
            (*stream)<<" "<<lcount<<" "<<wcount<<endl;
            fclose(fp);*/
            cout<<"file: "<<arg<<endl;
            return true;
        };
        _parser.reset(new OptionParser([this](const std::shared_ptr<ostream> &stream,map<string,variant> &args){
            //所有选项解析完毕后触发该回调，我们可以在这里做一些全局的操作
//            (*stream)<<"wc command line"<<endl;

            long total_bytes=0,total_lcount=0,total_wcount=0;

            if(args.size()==0){
                args.emplace("bytes","");
                args.emplace("lines","");
                args.emplace("words","");
            }else{
                if(hasKey("version")){
                    return true;
                }
            }
            if(_argvs.size()==0){
                (*stream)<<"wc: 请指定统计的文件,输入\"-h\"获取帮助."<<endl;;
                return false;
            }

            for(auto &it:_argvs){
                FILE *fp = NULL;
                fp = fopen(it.c_str(), "r");
                if(fp==NULL){
                    (*stream)<<"无法打开文件："<<it<<endl;
                    continue;
                }
                count(fp);


                if(hasKey("lines")){
                    total_lcount+=lcount;
                    (*stream)<<" "<<lcount<<"\t";

                }
                if(hasKey("words")){
                    total_wcount+=wcount;
                    (*stream)<<" "<<wcount<<"\t";
                }
                if(hasKey("bytes")){
                    long &&tmp=countBytes(it);
                    if(tmp<0){
                        stream->clear();
                        (*stream)<<"无法打开文件："<<it<<endl;
                        continue;
                    }
                    total_bytes+=tmp;
                    (*stream)<<" "<<tmp<<"\t";
                }
                (*stream)<<" "<<it<<endl;
            }
            if(_argvs.size()>1){
                if(hasKey("lines")){
                    (*stream)<<" "<<total_lcount<<"\t";
                }
                if(hasKey("words")){
                    (*stream)<<" "<<total_wcount<<"\t";
                }
                if(hasKey("bytes")){
                    (*stream)<<" "<<total_bytes<<"\t";
                }
                (*stream)<<" 总用量"<<endl;
            }
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


    }

    ~CMD_wc() {}

    const char *description() const override {
        return "WC统计";
    }

private:
    long countBytes(const string &arg){
        struct stat info;
        if(stat(arg.c_str(), &info)<0){
            return -1;
        }
        return info.st_size;
    }
    void count (FILE *fp)
    {
      int c;
      bool tag=false;

      if (feof (fp))
        return ;

      wcount=0;
      lcount=0;

      while ((c = getc (fp)) != EOF){
          if ((c>'A'&&c<'z')){
              tag=true;
          }else if(tag){
              wcount++;
              tag=!tag;
          }
          if (c == '\n'){
              lcount++;
          }
        }
    }
private:
    long wcount=0;
    long lcount=0;
};


int main(int argc,char *argv[]){
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
