#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
enum{white,gray,black};
typedef struct vertex{
    int32_t vertexidx;
    struct vertex* prev;
    struct vertex* next;
}Vertex;
typedef struct graph{
    int32_t vertexidx;
    int32_t color;
    int32_t d;
    int32_t f;
    int32_t sccid;
    int32_t indegree;
    int32_t outdegree;
    struct graph* pi;
    Vertex* head;
    Vertex* tail;
    Vertex* transhead;
    Vertex* transtail;
}Graph;
typedef struct list{
    struct list* prev;
    struct list* next;
    int32_t vertexidx;
}List;
typedef struct{
    List* shead;
    List* stail;
}Stack;
Graph* gengraph(int32_t vertexidx){
    Graph* v=(Graph*)malloc(sizeof(Graph));
    v->d=0;
    v->f=0;
    v->indegree=0;
    v->outdegree=0;
    v->sccid=0;
    v->color=white;
    v->head=NULL;
    v->pi=NULL;
    v->tail=NULL;
    v->transhead=NULL;
    v->transtail=NULL;
    v->vertexidx=vertexidx;
    return v;
}
Vertex* genvertex(int vertexidx,Vertex* prev,Vertex* next){
    Vertex* v=(Vertex*)malloc(sizeof(Vertex));
    v->next=next;
    v->prev=prev;
    v->vertexidx=vertexidx;
    return v;
}
List* genlist(int32_t vertexidx,List* next,List* prev){
    List* s=(List*)malloc(sizeof(List));
    s->next=next;
    s->prev=prev;
    s->vertexidx=vertexidx;
    return s;
}
void DFS_VISIT(Graph* g[],Graph** u,int32_t* time,Stack* s){
    (*time)++;
    (*u)->d=(*time);
    (*u)->color=gray;
    Vertex* ptr=(*u)->head;
    for(;ptr;){
        if(g[ptr->vertexidx]->color==white){
            DFS_VISIT(g,&g[ptr->vertexidx],time,s);
        }
        ptr=ptr->next;
    }
    (*time)++;
    (*u)->f=(*time);
    (*u)->color=black;
    if(!s->shead){
        s->shead=genlist((*u)->vertexidx,NULL,NULL);
        s->stail=s->shead;
    }else{
        s->stail->next=genlist((*u)->vertexidx,NULL,s->stail); 
        s->stail=s->stail->next;
    }
}
void DFS(Graph* g[],int32_t n,int32_t* time,Stack* s){
    for(int i=1;i<=n;i++){
        if(g[i]->color==white){
            DFS_VISIT(g,&g[i],time,s);
        }
    }
}
void init(Graph* g[],int32_t n){
    for(int i=1;i<=n;i++){
        g[i]->color=white;
        g[i]->d=0;
        g[i]->f=0;
        g[i]->pi=NULL;
    }
}
void revDFS_VISIT(Graph* g[],Graph** u,int32_t*time,Stack* s,int32_t sccid,int32_t* is_connected){
    (*time)++;
    (*u)->d=(*time);
    (*u)->color=gray;
    (*u)->sccid=sccid;
    Vertex* ptr=(*u)->transhead;
    if(sccid==1){
        (*is_connected)=1;
    }
    while(ptr){
        if(g[ptr->vertexidx]->color==white){
            revDFS_VISIT(g,&g[ptr->vertexidx],time,s,sccid,is_connected);
        }else{
            if((*u)->sccid-g[ptr->vertexidx]->sccid==1){
                (*is_connected)=1;
            }
        }
        ptr=ptr->next;
    }
    (*time)++;
    (*u)->color=black;
    (*u)->f=(*time);
}
void revDFS(Graph* g[],int32_t n,int32_t* time,Stack* s,int32_t* sccid,int32_t* connected){
    List* ptr=s->stail;
    int32_t is_connected,vertexidx;
    while(ptr){
        vertexidx=ptr->vertexidx;
        ptr=ptr->prev;
        if(g[vertexidx]->color==white){
            is_connected=0;
            (*sccid)++;
            revDFS_VISIT(g,&g[vertexidx],time,s,*sccid,&is_connected); 
            if(!is_connected){
                (*connected)=0;
            }
        }
    }
}
int main(){
    Graph* g[100001];
    int32_t asize,bsize,mode,x,y,time=0,sccid=0,connected=1;
    Stack s;
    s.shead=NULL;
    s.stail=NULL;
    scanf("%d%d%d",&bsize,&asize,&mode);
    for(int i=1;i<=bsize;i++){
        g[i]=gengraph(i);
    }
    for(int i=0;i<asize;i++){
        scanf("%d%d",&x,&y);
        if(!g[x]->head){
            g[x]->head=genvertex(y,NULL,NULL);
            g[x]->tail=g[x]->head;
        }else{
            g[x]->tail->next=genvertex(y,g[x]->tail,NULL);
            g[x]->tail=g[x]->tail->next;
        }
        if(!g[y]->transhead){
            g[y]->transhead=genvertex(x,NULL,NULL);
            g[y]->transtail=g[y]->transhead;
        }else{
            g[y]->transtail->next=genvertex(x,g[y]->transtail,NULL);
            g[y]->transtail=g[y]->transtail->next;
        }
        g[x]->outdegree++;
        g[y]->indegree++;
    } //O(V+E)
    DFS(g,bsize,&time,&s); //O(V+E)
    init(g,bsize); // O(V+E)
    time=0;
    revDFS(g,bsize,&time,&s,&sccid,&connected);
    printf("%d\n",sccid);
    if(mode==2){
        printf("%d",connected);
    }
    return 0;
}