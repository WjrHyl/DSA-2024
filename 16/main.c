#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    int64_t num;
}data;
void pre_pro(int32_t pi[],char* s,int64_t length){
    int64_t l=length;
    pi[0]=-1;
    int64_t k=-1;
    for(int i=1;i<l;i++){
        for(;!(k<0)&&s[k+1]!=s[i];){
            k=pi[k];
        }
        if(s[k+1]==s[i]){
            k++;
        }
        pi[i]=k;
    }
}
int main(){
    char *s,*c;
    s=(char*)malloc(5000001*sizeof(char));
    c=(char*)malloc(5000001*sizeof(char));
    int64_t slen,clen,n;
    int64_t pre_and_suf=0,ptr,*c_pre_and_suf,appear=0;
    int32_t *c_pi;
    data* d;
    c_pre_and_suf=(int64_t*)malloc(5000001*sizeof(int64_t));
    c_pi=(int32_t*)malloc(5000001*sizeof(int32_t));
    d=(data*)malloc(5000001*sizeof(data));
    for(int i=0;i<5000001;i++){
        c_pi[i]=0;
        c_pre_and_suf[i]=1;
        d[i].num=0;
    }
    scanf("%s",s);
    scanf("%s",c);
    slen=strlen(s);
    clen=strlen(c);
    pre_pro(c_pi,c,clen);
    int qq=-1;
    for(int i=0;i<slen;i++){
        for(;!(qq<0)&&c[qq+1]!=s[i];){
            qq=c_pi[qq];
        }
        if(c[qq+1]==s[i]){
            qq++;
            d[qq].num++;
        }
        if(qq==clen-1){
            qq=c_pi[qq];
        }
    }
    for(int i=clen-1;i>=0;i--){
        if(c_pre_and_suf[i]==1){
            pre_and_suf=0;
            ptr=c_pi[i];
            for(;!(ptr<0);){
                pre_and_suf++;
                ptr=c_pi[ptr];
            }
            ptr=i;
            for(;!(ptr<0)&&c_pre_and_suf[ptr]==1;){
                c_pre_and_suf[ptr]+=pre_and_suf;
                pre_and_suf--;
                ptr=c_pi[ptr];
            }
        }
    }
    for(int i=clen-1;i>=0;i--){
        if(d[i].num>0){
            ptr=c_pi[i];
            n=d[i].num;
            if(!(ptr<0)){
                d[ptr].num+=n;
            }
        }
    }
    for(int i=0;i<clen;i++){
        printf("%lld\n",d[i].num*c_pre_and_suf[i]);
    }
    return 0;
}