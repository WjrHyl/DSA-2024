#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct{
    int num;
    int idx;
}data;
data getmin(int a[],int idx,int n){
    data re;
    re.num=INT32_MAX;
    for(int i=idx;i<n;i++){
        if(a[i]<re.num){
            re.num=a[i];
            re.idx=i;
        }
    }
    return re;
}
int main(){
    int n,b;
    scanf("%d%d",&n,&b);
    int a[n],c=0;
    data d;
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=0;c<b&&i<n;i++){
        d=getmin(a,i,n);
        if(d.num!=a[i]){
            int temp=a[i];
            a[i]=a[d.idx];
            a[d.idx]=temp;
            c++;
        }
    }
    fputs("The array is ",stdout);
    for(int i=0;i<n;i++){
        printf("%d ",a[i]);
    }
    fputs("after ",stdout);
    printf("%d ",(c<b)?c:b);
    fputs("swaps.",stdout);
    return 0;
}