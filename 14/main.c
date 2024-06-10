#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
void pre_pro(int pi[],int64_t P_[],int length){
    int l=length;
    pi[0]=-1;
    int k=-1;
    for(int i=1;i<l;i++){
        for(;!(k<0)&&P_[k+1]!=P_[i];){
            k=pi[k];
        }
        if(P_[k+1]==P_[i]){
            k++;
        }
        pi[i]=k;
    }
}
void sp_hit(char** T,char** P,int k,int m,int idx,int* cnt,int*hit){
    for(int i=0;i<k;i++){
        for(int j=0;j<m;j++){
            if(P[i][j]!=T[i][j+idx]){
                hit[(*cnt)++]=idx;
                return;
            }
        }
    }  
}
int main(){
    int k,n,m;
    int64_t base=1,q;
    scanf("%d%d%d%lld",&k,&n,&m,&q);
    int64_t T_[n],P_[m];
    int pi[m],hit[n];
    char **T,**P;
    T=(char**)malloc(k*sizeof(char*));
    P=(char**)malloc(k*sizeof(char*));
    for(int i=0;i<k;i++){
        T[i]=(char*)malloc((n+1)*sizeof(char));
        P[i]=(char*)malloc((m+1)*sizeof(char));
    }
    for(int i=0;i<n;i++){
        hit[i]=-1;
        T_[i]=0;
    }
    for(int i=0;i<m;i++){
        P_[i]=0;
    }
    for(int i=0;i<k;i++){
        scanf("%s",T[i]);
    }
    for(int i=0;i<k;i++){
        scanf("%s",P[i]);
    }
    for(int i=0;i<n;i++){
        base=1;
        for(int j=k-1;j>=0;j--){
            if(islower(T[j][i])){
                T_[i]+=(base*((int)(T[j][i]-'a')+26));
            }else{
                T_[i]+=base*(int)(T[j][i]-'A');
            }
            base*=52;
            base%=q;
        }
        T_[i]%=q;
        printf("%lld ",T_[i]);
    }
    putchar('\n');
    for(int i=0;i<m;i++){
        base=1;
        for(int j=k-1;j>=0;j--){
            if(islower(P[j][i])){
                P_[i]+=(base*((int)(P[j][i]-'a')+26));
            }else{
                P_[i]+=base*(int)(P[j][i]-'A');
            }
            base*=52;
            base%=q;
        }
        P_[i]%=q;
        printf("%lld ",P_[i]);
    }
    putchar('\n');
    pre_pro(pi,P_,m);
    int qq=-1,cnt=0,cnt2=0;
    for(int i=0;i<n;i++){
        for(;!(qq<0)&&P_[qq+1]!=T_[i];){
            qq=pi[qq];
        }
        if(P_[qq+1]==T_[i]){
            qq++;
        }
        if(qq==m-1){
            sp_hit(T,P,k,m,i-m+1,&cnt,hit);
            printf("%d ",i-m+1);
            cnt2++;
            qq=pi[qq];
        }
    }
    if(!cnt2){
        fputs("-1",stdout);
    }
    putchar('\n');
    if(cnt>0){
        for(int i=0;i<cnt;i++){
            printf("%d ",hit[i]);
        }
    }else{
        fputs("-1",stdout);
    }
    return 0;
}