#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef struct set{
    struct set* p;
    int idx;
    int rank;
}Set;
void Make_set(Set** x,int idx){
    (*x)->idx=idx;
    (*x)->p=(*x);
    (*x)->rank=0;
};
Set* Find_set(Set** x){
    if((*x)!=(*x)->p){
        (*x)->p=Find_set(&(*x)->p);
    }
    return (*x)->p;
};
void Link(Set** x,Set** y){
    if((*x)->rank>(*y)->rank){
        (*y)->p=(*x);
    }else{
        (*x)->p=(*y);
        if((*x)->rank==(*y)->rank){
            (*y)->rank++;
        }
    }
}
void Union(Set** x,Set** y){
    Set* v=Find_set(x),*w=Find_set(y);
    if(v!=w){
        Link(&v,&w);
    }
}
void Print_Path(Set* x){
    Set* k=x;
    while(k->p!=k){
        printf("%d ",k->idx);
        k=k->p;
    }
    printf("%d\n",k->idx);
}
int main(){
    char cmd[30],c,r[3][10],buf[10];
    char* st=buf;
    int n,cmdnum,x,y,t=0;
    Set** s;
    scanf("%d",&n);
    scanf("%d",&cmdnum);
    s=(Set**)malloc((n+1)*sizeof(Set*));
    for(int i=1;i<=n;i++){
        s[i]=(Set*)malloc(sizeof(Set));
        Make_set(&s[i],i);
    }
    scanf("%c",&c);
    for(int i=0;i<cmdnum;i++){
        t=0;
        fgets(cmd,30,stdin);
        st=strtok(cmd," \n");
        for(;st;){
            strcpy(r[t],st);
            t++;
            st=strtok(NULL," \n");
        }
        if(!strcmp(r[0],"F")){
            x=atoi(r[1]);
            s[x]->p=Find_set(&s[x]);
        }else if(!strcmp(r[0],"U")){
            x=atoi(r[1]);
            y=atoi(r[2]);
            Union(&s[x],&s[y]);
        }else{
            x=atoi(r[1]);
            Print_Path(s[x]);
        }
    }
    return 0;
}