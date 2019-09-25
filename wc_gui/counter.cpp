#include <fstream>
#include <fstream>
#include <regex>
#include "counter.h"
#include "Util/util.h"

#if defined(__WIN32)
#define CHINESEMEM "1,2"
#else
#define CHINESEMEM "3"
#endif


#define LINE_MAX_LENGTH 100

INSTANCE_IMP(RulesParser)
INSTANCE_IMP(Counter)

void Counter::count (const string &file,long &lcount,long &wcount, long &bcount,long &noteLines, long &emptyLines)
{
  /*char ch[3];
  ch[0]='\n';
  ch[2]='\0';
  bool tag=false;
  FILE *fp = NULL;
  fp = fopen(file.c_str(), "r");

  if(fp==NULL){
      bcount=-1;
      wcount=-1;
      lcount=-1;
      return;
  }

  if (feof (fp))
    return ;

  wcount=0;
  lcount=0;
  noteLines=0;
  emptyLines=0;
  isNote=false;
  noteLine=false;

  countBytes(file,bcount);
  while ((ch[1] = getc (fp)) != EOF){

      if(strcmp(ch,_lanVec->front().front().c_str())==0){
          isNote=true;
          noteLine=true;
      }else if(strcmp(ch,_lanVec->front().back().c_str())==0){
          isNote=false;
      }else if(!isNote){
          if(_lanVec->back().front().size()==1&&strcmp(ch+1,_lanVec->back().front().c_str())==0){
              noteLine=true;
          }else if(strcmp(ch,_lanVec->back().front().c_str())==0){
              noteLine=true;
          }
      }

      if ((ch[1]>'A'&&ch[1]<'z')){
          tag=true;
      }else if(tag){
          wcount++;
          tag=!tag;
      }
      if (ch[0] == '\n'){

          char next='\n';
          #if defined(_WIN32)
            next='\r';
          #endif
          if(ch[1]==next){
              ++emptyLines;
          }else if(noteLine){
              ++noteLines;
              if(!isNote){
                  noteLine=false;
              }
          }
          lcount++;
      }
      ch[0]=ch[1];
    }*/

   isNote=false;
   isnoteLine=false;

   fstream inFile(file, ios::in);


   bcount=-1;
   wcount=-1;
   lcount=-1;


   if(inFile.is_open()){

       wcount=0;
       lcount=0;
       noteLines=0;
       emptyLines=0;

       char strbuf[LINE_MAX_LENGTH+1];
       string lineStr;

       smatch result;
       regex spaceMatch("\\s*");
//       regex wordMatch("\\w+|^[\u4e00-\u9fa5]");
        regex wordMatch(string("\\w+|[\u4e00-\u9fa5]{")+CHINESEMEM+"}");
       string::const_iterator iterStart,iterEnd;

       while(!inFile.eof()){
           inFile.getline(strbuf,LINE_MAX_LENGTH);
           ++lcount;
           lineStr.assign(strbuf);

           iterStart=lineStr.begin();
           iterEnd=lineStr.end();
           if(regex_match(lineStr,spaceMatch)){
               ++emptyLines;
           }else{
               if(!isNote&&lineStr.npos!=lineStr.find(_lanVec->front().front())){
                   isNote=true;
                   isnoteLine=true;
               }else if(isNote&&lineStr.npos!=lineStr.find(_lanVec->front().back())){
                   isNote=false;
               }else if(!isNote&&lineStr.npos!=lineStr.find(_lanVec->back().front())){
                   isnoteLine=true;
               }
               while(regex_search(iterStart,iterEnd,result,wordMatch)){
                   iterStart=result[0].second;
                   ++wcount;
               }


           }
           if(isnoteLine){
               ++noteLines;
               if(!isNote){
                   isnoteLine=false;
               }
           }
       }


       inFile.close();
       countBytes(file,bcount);
   }

}

RulesParser::RulesParser(){

    fstream rfile("rules.wc", ios::in);
    string str;
     if(rfile.is_open()){
         while(!rfile.eof()){
             rfile>>str;
             size_t left=str.find("[")+1;
             size_t right=str.find("]",left);

             shared_ptr<RuleList> lanVec=make_shared<RuleList>();
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
                     lanVec->push_back({tmp.substr(0,pos),tmp.substr(pos+1)});
                 }else{
                     lanVec->push_back({tmp.substr(0,pos)});
                 }
             }
             _rulesMap.emplace(lan,lanVec);

         }
     }
     rfile.close();
}






