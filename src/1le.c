#include<stdio.h>
#include<string.h>
#define DEBUG 0
#define LEN_SIZE 128
#define LINE_SIZE 256
#define FILENAME_SIZE 30
#define COLOR 0

typedef struct{
char line[LINE_SIZE][LEN_SIZE];
int linesize;
}text;
/*
int strlen(char* str){
int i=0;
while(str[i]!='\0')i++;
return 0;
}
void memcpy(char* dst,char* ori,int size){
if(size<=0)return;
while(size-->0){
dst[size]=ori[size];
}
return;
}
*/
int in(char* input){
do{
fgets(input,LEN_SIZE,stdin);
//if(strlen(input)<=0)printf("NULL string!\n");
}while(strlen(input)<=0);
input[strlen(input)-1]='\0';
return strlen(input);
}

void showhelp(){
printf("@- del\n@e edit\n@p map\n@q or @x save and exit\n@i insert\n");
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
txt->linesize=i-1;
fclose(fpr);
return 0;
}

int savetext(text *txt,char* filename){
FILE *fpw=fopen(filename,"w");
if(!fpw)return -1;
int i;
for(i=0;i<=txt->linesize;i++)
fprintf(fpw,"%s\n",txt->line[i]);
fclose(fpw);
return 0;
}

void intro(){
 printf("--  |   ---\n");
 printf(" |  |   |  \n");
 printf(" |  |   ---\n");
 printf(" |  |   |  \n");
 printf("--- --- ---\n"); 
 return;
}

void showtext(text *txt){
int i;
for(i=0;i<txt->linesize;i++)
printf("%d:%s\n",i,txt->line[i]);
return;
}

void showaround(text *txt,int at,int n){
int i;
for(i=at-n;i<=at+n;i++){
	if(i>0&&i<txt->linesize){
		if(i==at){
		if(COLOR)printf("\x1b[47m");
		printf("%d:%s\n",i,txt->line[i]);
		if(COLOR)printf("\x1b[49m");
		}else{
		printf("%d:%s\n",i,txt->line[i]);
		}
	}
}
return;
}


int insert(text *txt,char *str,int at){//at=-1:at the end ,otherwise at var"at"
if(DEBUG)printf("insert:%s at %d\n",str,at);
txt->linesize++;
if(at<0){
at=txt->linesize-1;
}else{
if(at+1>LINE_SIZE)return -1;
int i;
for(i=txt->linesize;i>=at;i--)
memcpy(txt->line[i+1],txt->line[i],LEN_SIZE);
}
memcpy(txt->line[at],str,LEN_SIZE);
return 0;
}

void editline(text *txt,int at){
if(at<0||LINE_SIZE<=at){
printf("out of range");
return;
}
char editinput[LEN_SIZE];
while(in(editinput)==0);
memcpy(txt->line[at],editinput,LEN_SIZE);

return;
}

void delline(text *txt){
int d;
printf("del");
scanf("%d",&d);
//insert(txt,"\0",d);
int i;
for(i=d;i<txt->linesize;i++){
memcpy(txt->line[i],txt->line[i+1],LEN_SIZE);
}
txt->linesize--;
return;
}

int search(text *txt,char * str){

}

//*UIs*//
void insertUI(text *txt){
showtext(txt);
printf("insert at:");
int at;
scanf("%d",&at);
printf("How many lines:");
int i;
scanf("%d",&i);
while(i-->0){
insert(txt,"\0",at);
editline(txt,at);
at++;}
return;
}

void editUI(text *txt){
printf("edit at:");
int at;
scanf("%d",&at);
showaround(txt,at,2);
editline(txt,at);
return;
}

int saveConfirm(){
printf("Do you save?(Y/n)");
char saveconfirm;
scanf("%c",&saveconfirm);
if(saveconfirm=='Y'||saveconfirm=='y')return 1;
return 0;
}

int main(int argc,char *argv[]){
intro();//introduce
text txt;
inittext(&txt);
char filename[FILENAME_SIZE]="result.txt";
if(argc==2){
memcpy(filename,argv[1],FILENAME_SIZE);
}
printf("%s on edit\n",filename);
loadtext(&txt,argv[1]);
char input[LEN_SIZE];
printf("@h help\n");

while(1){
printf("1le$");
in(input);
if(DEBUG)printf("START %c\n",input[0]);

if(input[0]=='@'){
 switch(input[1]){
 case'-':
 showtext(&txt);
 delline(&txt);
 break;

 case'e':
 editUI(&txt);
 break;

 case'h':
 showhelp();
 break;

 case'i':
 insertUI(&txt);
 break;
 
 case'p':
 showtext(&txt);
 break;

 case'q':
 case'x':
 if(saveConfirm())
  savetext(&txt,filename);
 return 0;
 break;

 case's':
 savetext(&txt,filename);
 printf("\nSaved\n");
 break;

 default:
 printf("Error\n");
 break;
 }
}else{
insert(&txt,input,-1);
}
}
return 0;
}


