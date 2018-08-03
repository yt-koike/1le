#include<stdio.h>
#include<string.h>
#define DEBUG 0
#define LEN_SIZE 128
#define LINE_SIZE 256

typedef struct{
char line[LINE_SIZE][LEN_SIZE];
int linesize;
}text;

int in(char* input){
do{
gets(input);
//if(strlen(input)<=0)printf("NULL string!\n");
}while(strlen(input)<=0);
return strlen(input);
}

void showhelp(){
printf("@- del\n@e edit\n@p map\n@q or @x save and exit");
return;
}

void inittext(text *txt){
int i;
for(i=0;i<LINE_SIZE;i++)
txt->line[i][0]='\0';
txt->linesize=0;
return;
}

int loadtext(text *txt,char *filename){
FILE *fpr=fopen(filename,"r");
if(!fpr)return -1;
int i=0;
while(fgets(txt->line[i],LEN_SIZE,fpr)){
if(i>=LINE_SIZE)return -1;
txt->line[i][strlen(txt->line[i])-1]='\0';
i++;
}
txt->linesize=i;
fclose(fpr);
return 0;
}

void savetext(text *txt){
FILE *fp=fopen("result.txt","w");
int i;
for(i=0;i<=txt->linesize;i++)
fprintf(fp,"%s\n",txt->line[i]);
fclose(fp);
return;
}

void showtext(text *txt){
int i;
for(i=0;i<txt->linesize;i++)
printf("%d:%s\n",i,txt->line[i]);
return;
}

int insert(text *txt,char *str,int at){//at=-1:at the end ,otherwise at var"at"
if(DEBUG)printf("insert:%s at %d\n",str,at);
if(at<0){
at=txt->linesize;
txt->linesize++;
}else{
if(at+1>LINE_SIZE)return -1;
int i;
for(i=txt->linesize;i>at;i--)
memcpy(txt->line[i+1],txt->line[i],LEN_SIZE);
}
memcpy(txt->line[at],str,LEN_SIZE);
return 0;
}

void editline(text *txt){
printf("edit");
int d;
scanf("%d",&d);
if(d<0||LINE_SIZE<=d){
printf("out of range");
return;
}
char editinput[LEN_SIZE];
in(editinput);
memcpy(txt->line[d],editinput,LEN_SIZE);
return;
}

void delline(text *txt){
int d;
printf("del");
scanf("%d",&d);
insert(txt,"\0",d);
int i;
for(i=d;i<txt->linesize;i++){
memcpy(txt->line[i],txt->line[i+1],LEN_SIZE);
}
txt->linesize--;
return;
}

int main(int argc,char *argv[]){
text txt;
inittext(&txt);
if(argc==2)loadtext(&txt,argv[1]);
char input[LEN_SIZE];
printf("@h help\n");

while(1){
printf("$");
in(input);
if(DEBUG)printf("START %c\n",input[0]);

if(input[0]!='@'){
insert(&txt,input,-1);
}

if(input[0]=='@'){
 switch(input[1]){
 case'-':
 showtext(&txt);
 delline(&txt);
 break;

 case'e':
 editline(&txt);
 break;

 case'h':
 showhelp();
 break;

 case'p':
 showtext(&txt);
 break;

 case'q':
 case'x':
 savetext(&txt);
 return 0;
 break;

 default:
 printf("Error\n");
 break;
 }
}

}
return 0;
}

