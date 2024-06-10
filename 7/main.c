#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
long long binsearch(long long a[],long long key,long long l,long long r){
    long long m;
    long long left=l,right=r;
    printf("Searching %lld in range [%lld, %lld].\n",key,left,right);
    while(left<=right){ 
        m=(left+right)/2;
        if(a[m]==key){
            return m;
        }else if(a[m]>key){
            right=m-1;
        }else{
            left=m+1;
        }
        printf("Searching %lld in range [%lld, %lld].\n",key,left,right);
    }
    return 0;
}
int main(){
    long long n,key;
    scanf("%lld%lld,",&n,&key);
    long long a[n+1],idx;
    for(long long i=1;i<=n;i++){
        scanf("%lld",&a[i]);
    }
    idx=binsearch(a,key,1,n);
    if(idx>0){
        printf("Found at index %lld.\n",idx);
    }else{
        fputs("Not found.",stdout);
    }
    return 0;
}