#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
enum{white,gray,black};
typedef struct col{
    char c;
    struct col* prev;
    struct col* next;
}color;
typedef struct ver{
    int vertexidx;
    struct ver* next;
    struct ver* prev;
}vertex;
typedef struct gra{
    int idx;
    int out_degree;
    int d;
    int f;
    int color;
    struct gra* pi;
    vertex* head;
    vertex* tail;
    color* chead;
    color* ctail;
}graph;
color* gencolor(color* next,color* prev,char c){
    color* node=(color*)malloc(sizeof(color));
    node->c=c;
    node->next=next;
    node->prev=prev;
    return node;
}
graph* gengraph(int idx,int out_degree){
    graph* node=(graph*)malloc(sizeof(graph));
    node->head=NULL;
    node->out_degree=out_degree;
    node->pi=NULL;
    node->tail=NULL;
    node->chead=NULL;
    node->ctail=NULL;
    return node;
}
vertex* genvertex(vertex* next,vertex* prev,int vertexidx){
    vertex* node=(vertex*)malloc(sizeof(vertex));
    node->next=next;
    node->prev=prev;
    node->vertexidx=vertexidx;
    return node;
}
void DFS_VISIT(graph* g[],graph** u,int* time){
    (*time)++;
    (*u)->d=(*time);
    (*u)->color=gray;
    vertex* ptr=(*u)->head;
    while(ptr){
        if(g[ptr->vertexidx]->color==white){
            if(!(*u)->chead) {
                (*u)->chead=gencolor(NULL,NULL,'T');
                (*u)->ctail=(*u)->chead;
            }else{
                (*u)->ctail->next=gencolor(NULL,(*u)->ctail,'T');
                (*u)->ctail=(*u)->ctail->next;
            } 
            g[ptr->vertexidx]->pi=(*u);
            DFS_VISIT(g,&g[ptr->vertexidx],time);
        }else if(g[ptr->vertexidx]->color==gray){
            if(!(*u)->chead) {
                (*u)->chead=gencolor(NULL,NULL,'B');
                (*u)->ctail=(*u)->chead;
            }else{
                (*u)->ctail->next=gencolor(NULL,(*u)->ctail,'B');
                (*u)->ctail=(*u)->ctail->next;
            } 
        }else{
            if((*u)->d<g[ptr->vertexidx]->d){
                if(!(*u)->chead) {
                    (*u)->chead=gencolor(NULL,NULL,'F');
                    (*u)->ctail=(*u)->chead;
                }else{
                    (*u)->ctail->next=gencolor(NULL,(*u)->ctail,'F');
                    (*u)->ctail=(*u)->ctail->next;
                } 
            }else{
                if(!(*u)->chead) {
                    (*u)->chead=gencolor(NULL,NULL,'C');
                    (*u)->ctail=(*u)->chead;
                }else{
                    (*u)->ctail->next=gencolor(NULL,(*u)->ctail,'C');
                    (*u)->ctail=(*u)->ctail->next;
                } 
            }
        }    
        ptr=ptr->next;
    }
    (*u)->color=black;
    (*time)++;
    (*u)->f=(*time);
}
void DFS(graph* g[],int* time,int v){
    for(int i=1;i<=v;i++){
        g[i]->color=white;
        g[i]->pi=NULL;
    }
    for(int i=1;i<=v;i++){
        if(g[i]->color==white){
            DFS_VISIT(g,&g[i],time);
        }
    }
}
int main(){
    graph** g;
    color*ptr;
    int v,num,idx,time=0;
    scanf("%d",&v);
    g=(graph**)malloc((v+1)*sizeof(graph*));
    for(int i=1;i<=v;i++){
        scanf("%d",&num);
        g[i]=gengraph(i,num);
        for(int j=0;j<num;j++){
            scanf("%d",&idx);
            if(!g[i]->head){
                g[i]->head=genvertex(NULL,NULL,idx);
                g[i]->tail=g[i]->head;
            }else{
                g[i]->tail->next=genvertex(NULL,g[i]->tail,idx);
                g[i]->tail=g[i]->tail->next;
            }
        }
    }
    DFS(g,&time,v);
    for(int i=1;i<=v;i++){
        ptr=g[i]->chead;
        printf("%d ",i);
        for(;ptr;){
            printf("%c",ptr->c);
            ptr=ptr->next;
        }
        putchar('\n');
    }
    return 0;
}