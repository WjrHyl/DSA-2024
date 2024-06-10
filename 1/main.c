#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct{
    int64_t num;
    int can;
}data;
int cmp(const void*a,const void*b){
    data c=*(data*)a,d=*(data*)b;
    if(c.num>d.num){
        return 1;
    }else{
        return -1;
    }
}
int solve(data a[],int64_t rec[200000][20],int64_t re[],int n,int *c,int idx){
    int find=0;
    int64_t last=INT64_MIN;
    if(idx>=n){
        for(int i=0;i<n;i++){
            rec[*c][i]=re[i];
        }
        (*c)++;
        return 1;
    }
    for(int i=0;i<n;i++){
        if(a[i].can&&a[i].num!=last){
            last=a[i].num;
            a[i].can=0;
            re[idx]=a[i].num;
            if(idx>1){
                if((re[idx]-re[idx-1])*(re[idx-1]-re[idx-2])>=0){
                    find=find||0;
                }else{
                    find=solve(a,rec,re,n,c,idx+1)||find;
                }
            }else{
                find=solve(a,rec,re,n,c,idx+1)||find;
            }
            re[idx]=0;
            a[i].can=1;
        }
    }
    return find;
}
int main(){
    int n,m=0;
    scanf("%d",&n);
    data a[n];
    int64_t re[n],rec[200000][20];
    for(int i=0;i<n;i++){
        scanf("%lld",&a[i].num);
        a[i].can=1;
        re[i]=0;
    }
    qsort(a,n,sizeof(data),cmp);
    int find=solve(a,rec,re,n,&m,0);
    if(n>2){
        printf("%d\n",m);
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                printf("%lld ",rec[i][j]);
            }
            (i!=m-1)&&putchar('\n');
        }
    }else{
        if(n==2){
            if(a[0].num!=a[1].num){
                fputs("2\n",stdout);
                printf("%lld %lld\n%lld %lld",a[0].num,a[1].num,a[1].num,a[0].num);
            }else{
                fputs("1\n",stdout);
                printf("%lld %lld",a[0].num,a[1].num);
            }   
        }else{
            fputs("1\n",stdout);
            printf("%lld",a[0].num);
        }
    }
    return 0;
}
