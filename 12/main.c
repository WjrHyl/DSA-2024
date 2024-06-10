#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef struct{
    int a[1000001];
    int heapsize;
}Heap;
int parent(int i){
    return i/2;
}
void MAX_INSERT_KEY(int a[],int i,int key){
    a[i]=key;
    int temp;
    while(i>1&&a[parent(i)]<a[i]){
        temp=a[i];
        a[i]=a[parent(i)];
        a[parent(i)]=temp;
        i=parent(i);
    }
}
void MAX_HEAP_INSERT(Heap* h,int key){
   h->heapsize++;
   h->a[h->heapsize]=INT32_MIN;
   MAX_INSERT_KEY(h->a,h->heapsize,key);
}
int main(){
    Heap h;
    h.heapsize=0;
    int n,key;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&key);
        MAX_HEAP_INSERT(&h,key);
    }
    for(int i=1;i<=n;i++){
        printf("%d ",h.a[i]);
    }
    return 0;
}