#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <map>
#include <vector>

using namespace std;
#define RuleList vector<vector<string>>
int main(){
    fstream test_file("rules.wc", ios::in);
    string str;
     if(test_file.is_open()){
         while(!test_file.eof()){
             test_file>>str;
             size_t left=str.find("[")+1;
             size_t right=str.find("]",left);
             int i=0;
             map<string, RuleList> rulesMap;
             RuleList lanVec;
             string lan=str.substr(left,right-1);

             while(str[right+1]!=';'&&str[right+1]!='\n'){
                 left=str.find("(",right+1)+1;
                 right=str.find(")",left);

                 if(left==str.npos||left==0||left==str.npos||left==0){
                     break;
                 }

                 string tmp=str.substr(left,right-left);
                 auto pos=tmp.find(",");
                 if(pos!=tmp.npos){
                     lanVec.push_back({tmp.substr(0,pos),tmp.substr(pos+1)});
                 }else{
                     lanVec.push_back({tmp.substr(0,pos)});
                 }
             }
             rulesMap.emplace(lan,lanVec);

         }
     }
     test_file.close();

    return 0;
}
