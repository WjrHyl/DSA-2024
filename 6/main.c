#include <stdio.h>
#include <stdlib.h>
void insert(int a[],int m,int *c,int time){
    int data=a[m];
    int i=m-1;
    while(i>0&&a[i]>data&&(*c)<time){
        a[i+1]=a[i];
        i--;
        (*c)++;
    }
    a[i+1]=data;
}
void insertsort(){
    int m,time,cnt=0;
    scanf("%d%d",&m,&time);
    int*a;
    a=(int*)malloc((m+1)*sizeof(int));
    for(int i=1;i<=m;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<=m;i++){
        insert(a,i,&cnt,time);
        if(cnt>=time){
            break;
        }
    }
    fputs("The array is ",stdout);
    for(int i=1;i<=m;i++){
        printf("%d ",a[i]);
    }
    printf("after %d ",(cnt<time)?cnt:time);
    fputs("back-moves.",stdout);
    free(a);
}
int main(){
    insertsort();
    return 0;
}