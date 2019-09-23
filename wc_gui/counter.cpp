#include "counter.h"
#include "Util/util.h"

INSTANCE_IMP(Counter);

void Counter::count (const string &file,long &lcount,long &wcount, long &bcount,long &noteLines, long &emptyLines)
{
  int c1,c0;
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

  countBytes(file,bcount);
  while ((c1 = getc (fp)) != EOF){
      if(c0=='/'&&c1=='*'){
          isNote=true;
      }
      if(c0=='*'&&c1=='/'){
          isNote=false;
      }
      if(!isNote&&c0=='/'&&c1=='/'){
          ++noteLines;
      }

      if ((c1>'A'&&c1<'z')){
          tag=true;
      }else if(tag){
          wcount++;
          tag=!tag;
      }
      if (c1 == '\n'){

          char next='\n';
          #if defined(_WIN32)
            next='\r';
          #endif
          if(c0==next){
              ++emptyLines;
          }else if(isNote){
              ++noteLines;
          }
          lcount++;
      }
      c0=c1;
    }

}


