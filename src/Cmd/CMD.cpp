#include "CMD.h"
#include "Util/util.h"

#if defined(_WIN32)
#include "win32/getopt.h"
#else
#include <getopt.h>
#endif // defined(_WIN32)

namespace CMDToolkit{

mutex OptionParser::s_mtx_opt;
//默认注册exit/quit/help/clear命令
static onceToken s_token([](){
    REGIST_CMD(exit)
    REGIST_CMD(quit)
    REGIST_CMD(help)
    REGIST_CMD(clear)
}, nullptr);


INSTANCE_IMP(CMDRegister)

void OptionParser::operator ()(CMD &allArg, int argc, char *argv[],const std::shared_ptr<ostream> &stream) {
    vector<struct option> vec_longOpt;
    string str_shortOpt("-");
    do{
        struct option tmp;
        for (auto &pr : _map_options) {
            auto &opt = pr.second;
            //long opt
            tmp.name = (char *) opt._longOpt.data();
            tmp.has_arg = opt._argType;
            tmp.flag = NULL;
            tmp.val = pr.first;
            vec_longOpt.emplace_back(tmp);
            //short opt
            if (!opt._shortOpt) {
                continue;
            }
            str_shortOpt.push_back(opt._shortOpt);
            switch (opt._argType) {
                case Option::ArgRequired:
                    str_shortOpt.append(":");
                    break;
                case Option::ArgOptional:
                    str_shortOpt.append("::");
                    break;
                default:
                    break;
            }
        }
        tmp.flag = 0;
        tmp.name = 0;
        tmp.has_arg = 0;
        tmp.val = 0;
        vec_longOpt.emplace_back(tmp);
    }while(0);

    lock_guard<mutex> lck(s_mtx_opt);
    int index;
    optind = 0;
    opterr = 0;
    auto it = _map_options.end();
    while ((index = getopt_long(argc, argv, &str_shortOpt[0], &vec_longOpt[0],NULL)) != -1) {
        stringstream ss;
        if(index < 0xFF){
            //短参数
            if(index==1){
                allArg.push_argv(optarg ? optarg : "");
                continue;
            }
            if(static_cast<char>(index)==':'){
                ss  << "  选项缺少参数,输入\"-h\"获取帮助.";
                throw std::invalid_argument(ss.str());
            }
            auto it = _map_charIndex.find(index);
            if(it == _map_charIndex.end()){
                ss  << "  未识别的选项,输入\"-h\"获取帮助.";
                throw std::invalid_argument(ss.str());
            }
            index = it->second;
        }
        ss  << "  未识别的选项,输入\"-h\"获取帮助.";

        it=_map_options.find(index);
        if(it == _map_options.end()){
            throw std::invalid_argument(ss.str());
        }

        auto &opt = it->second;
        auto pr = allArg.emplace(opt._longOpt, optarg ? optarg : "");
        if (!opt(stream, pr.first->second)) {   //回调option cb
            return;
        }
        optarg = NULL;
    }


    for (auto &pr : _map_options) {
        if(pr.second._defaultValue && allArg.find(pr.second._longOpt) == allArg.end()){
            //有默认值,赋值默认值
            allArg.emplace(pr.second._longOpt,*pr.second._defaultValue);
        }
    }
    for (auto &pr : _map_options) {
        if(pr.second._mustExist){
            if(allArg.find(pr.second._longOpt) == allArg.end() ){
                stringstream ss;
                ss << "  参数\"" << pr.second._longOpt << "\"必须提供,输入\"-h\"选项获取帮助";
                throw std::invalid_argument(ss.str());
            }
        }
    }
    if(allArg.empty() && _map_options.size() > 1 && !_enableEmptyArgs){
        _helper(stream,"");
        return;
    }
    if (_onCompleted) {
        _onCompleted(stream, allArg);
    }
}

}//namespace CMDToolkit
