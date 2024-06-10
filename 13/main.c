#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
void swap(int* a,int* b,int* cnt){
    int temp=(*a);
    (*a)=(*b);
    (*b)=temp;
    (*cnt)++;
}
int partition(int a[],int l,int r,int* cnt,int b){
    int pivot=a[l];
    int i=l+1;
    int j=r;
    for(;i<j;){
        for(;i<=r&&a[i]<=pivot;){
            i++;
        }
        for(;j>=l&&a[j]>pivot;){
            j--;
        }
        if(i>=j){
            break;
        }
        swap(&a[i],&a[j],cnt);
        if((*cnt)>=b){
            return j;
        }
    }
    if(l<j&&a[l]>a[j]){
        swap(&a[l],&a[j],cnt);
    }
    return j;
}
void quicksort(int a[],int l,int r,int* cnt,int b){
    if(l<r){
        int m=partition(a,l,r,cnt,b);
        //fprintf(stderr,"%d\n",m);
        if((*cnt)>=b){
            return;
        }
        quicksort(a,l,m-1,cnt,b);
        if((*cnt>=b)){
            return;
        }
        quicksort(a,m+1,r,cnt,b);
    }
}
int main(){
    int a[2000];
    int n,b;
    int cnt=0;
    scanf("%d%d",&n,&b);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    quicksort(a,0,n-1,&cnt,b);
    fputs("The array is ",stdout);
    for(int i=0;i<n;i++){
        printf("%d ",a[i]);
    }
    fputs("after ",stdout);
    if(cnt<b){
        printf("%d ",cnt);
    }else{
        printf("%d ",b);
    }
    fputs("swaps.",stdout);
    return 0;
}