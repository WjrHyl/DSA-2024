#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define max(a, b) (((a)>(b)?(a):(b)))
typedef struct{
    int64_t dungeonidx;
    int64_t dis_from_start;
}Route;
typedef struct tresure{
    int64_t valuelostdungeon;
    int64_t tresureval;
    struct tresure* next;
    struct tresure* prev;
}Tresure;
typedef struct node{
    int64_t dungeonidx;
    struct node* prev;
    struct node* next;
}Node;
typedef struct maxqueue{
    int64_t maxlength;
    int64_t dungeonidx;
    struct maxqueue* next;
    struct maxqueue* prev;
}Maxqueue;
typedef struct treenode{
    int64_t dungeonidx;
    int64_t dis_to_parent;
    struct treenode* parent;
    int64_t tresurenum;
    Maxqueue* head;
    Maxqueue* tail;
    Maxqueue* maxqnode;
    Node* childhead;
    Node* childtail;
}Treenode;
Treenode* gentreenode(int64_t dungeonidx){
    Treenode* node;
    node=(Treenode*)malloc(sizeof(Treenode));
    node->dungeonidx=dungeonidx;
    node->tresurenum=0;
    node->dis_to_parent=0;
    node->childhead=NULL;
    node->maxqnode=NULL;
    node->childtail=NULL;
    node->parent=NULL;
    node->head=NULL;
    node->tail=NULL;
    return node;
}
Node* gennode(Node* prev,int64_t dungeonidx){
    Node* node=(Node*)malloc(sizeof(Node));
    node->dungeonidx=dungeonidx;
    node->next=NULL;
    node->prev=prev;
    return node;
}
Tresure* gentresurenode(Tresure* prev){
    Tresure* t=(Tresure*)malloc(sizeof(Tresure));
    t->tresureval=-1;
    t->valuelostdungeon=-1;
    t->next=NULL;
    t->prev=prev;
    return t;
}
Maxqueue* genmaxqueue(int64_t maxlength,int64_t dungeonidx,Maxqueue* prev,Maxqueue* next){
    Maxqueue* node=(Maxqueue*)malloc(sizeof(Maxqueue));
    node->dungeonidx=dungeonidx;
    node->prev=prev;
    node->next=NULL;
    node->next=next;
    node->maxlength=maxlength;
    return node;
}
int64_t buildmaxlength(Treenode** root,Treenode* treenode[]){
    Node*ptr=(*root)->childhead;
    int64_t maxi=INT64_MIN,length;
    if(!ptr){
        return 0;
    }
    for(;ptr;){  
        length=buildmaxlength(&treenode[ptr->dungeonidx],treenode)+treenode[ptr->dungeonidx]->dis_to_parent;
        if(!(*root)->head){
            (*root)->head=genmaxqueue(length,ptr->dungeonidx,NULL,NULL);
            (*root)->tail=(*root)->head;    
        }else{
            for(;(*root)->head&&(*root)->head->maxlength<length;){
                (*root)->head=(*root)->head->next;
                if(!(*root)->head){
                    break;
                }
                treenode[(*root)->head->prev->dungeonidx]->maxqnode=NULL;
                (*root)->head->prev=NULL;
            }
            if(!(*root)->head){
                treenode[(*root)->tail->dungeonidx]->maxqnode=NULL;
                (*root)->tail=NULL;
                (*root)->head=genmaxqueue(length,ptr->dungeonidx,NULL,NULL);
                (*root)->tail=(*root)->head;
            }else{
                (*root)->head->prev=genmaxqueue(length,ptr->dungeonidx,NULL,(*root)->head);
                (*root)->head=(*root)->head->prev;
            }
        }
        treenode[ptr->dungeonidx]->maxqnode=(*root)->head;
        maxi=max(maxi,length);
        ptr=ptr->next;
    }
    return maxi;
}
int64_t bisearch(Route* route,int64_t l,int64_t r,int64_t val){
    int64_t m,left=l,right=r;
    int64_t maxlength=route[r].dis_from_start;
    while(left<=right){
        m=(left+right)/2;
        if(maxlength-route[m].dis_from_start>val){
            if(maxlength-route[m+1].dis_from_start<=val){
                return m+1;
            }else{
                left=m+1;
            }
        }else{
            right=m-1;
        }
    }
    return 0;
}
int32_t main(){
    Route* r=(Route*)malloc(1000001*sizeof(Route));
    Treenode* treenode[1000000],*treenodeptr;
    Tresure *tail=NULL,*head=NULL;
    int64_t length,ti,pi;
    int64_t m,n,q,tresurenum=0;
    scanf("%lld%lld%lld",&n,&m,&q);
    int64_t route_run_node=2;
    for(int32_t i=0;i<n;i++){
        treenode[i]=NULL;
        r[i+1].dungeonidx=-1;
        r[i+1].dis_from_start=0;
    }
    treenode[0]=gentreenode(0);
    r[1].dungeonidx=0;
    int64_t ui,vi,li,ridx;
    for(int32_t i=0;i<m;i++){
        scanf("%lld%lld%lld",&ui,&vi,&li);
        if(!treenode[ui]){
            treenode[ui]=gentreenode(ui);
        }
        if(!treenode[vi]){
            treenode[vi]=gentreenode(vi);
        }
        if(!treenode[ui]->childhead){
            treenode[ui]->childhead=gennode(NULL,vi);
            treenode[ui]->childtail=treenode[ui]->childhead;
        }else{
            treenode[ui]->childtail->next=gennode(treenode[ui]->childtail,vi);
            treenode[ui]->childtail=treenode[ui]->childtail->next;    
        }
        treenode[vi]->parent=treenode[ui];
        treenode[vi]->dis_to_parent=li;
    }
    length=buildmaxlength(&treenode[0],treenode)+treenode[0]->dis_to_parent;
    int64_t type;
    treenodeptr=treenode[0];
    for(int32_t i=0;i<q;i++){
        scanf("%lld",&type);
        if(type==1){
            if(treenodeptr->childhead){
                treenodeptr=treenode[treenodeptr->childhead->dungeonidx];
                printf("%lld\n",treenodeptr->dungeonidx);
                r[route_run_node].dis_from_start=r[route_run_node-1].dis_from_start+treenodeptr->dis_to_parent;
                r[route_run_node].dungeonidx=treenodeptr->dungeonidx;
                route_run_node++;
            }else{
                fputs("-1\n",stdout);
            }   
        }else if(type==2){
            if(treenodeptr!=treenode[0]){
                if(treenodeptr->tresurenum>0){
                    if(head!=tail){
                        tail=tail->prev;
                        tail->next=NULL; 
                    }else{
                        head=NULL;
                        tail=NULL;
                    }
                    tresurenum--;
                    treenodeptr->tresurenum--;
                }
                if(treenodeptr->maxqnode){
                    if(treenodeptr->maxqnode==treenodeptr->parent->head){
                        treenodeptr->parent->head=treenodeptr->parent->head->next;
                    }
                    if(treenodeptr->maxqnode==treenodeptr->parent->tail){   
                        treenodeptr->parent->tail=treenodeptr->parent->tail->prev;
                    }
                    if(treenodeptr->maxqnode->next||treenodeptr->maxqnode->prev){
                        if(treenodeptr->maxqnode->next){
                            treenodeptr->maxqnode->next->prev=treenodeptr->maxqnode->prev;
                        }
                        if(treenodeptr->maxqnode->prev){
                            treenodeptr->maxqnode->prev->next=treenodeptr->maxqnode->next;
                        }   
                    }
                }
                treenodeptr->maxqnode=NULL;
                treenodeptr->parent->tresurenum+=treenodeptr->tresurenum;
                treenodeptr=treenodeptr->parent;
                printf("%lld\n",treenodeptr->dungeonidx);
                treenode[treenodeptr->childhead->dungeonidx]->parent=NULL;
                if(treenodeptr->childhead->next){
                    treenodeptr->childhead=treenodeptr->childhead->next;
                    treenodeptr->childhead->prev=NULL;
                }else{
                    treenodeptr->childhead=treenodeptr->childhead->next;
                    treenodeptr->childtail=NULL;
                }
                r[route_run_node-1].dis_from_start=-1;
                r[route_run_node-1].dungeonidx=-1;
                route_run_node--;
            }else{
                fputs("-1\n",stdout);
            }
        }else if(type==3){
            scanf("%lld",&ti);
            if(ti>=r[route_run_node-1].dis_from_start){
                fputs("0\n",stdout);
            }else{
                ridx=bisearch(r,1,route_run_node-1,ti);
                printf("%lld\n",r[ridx].dungeonidx);
            }
        }else if(type==4){
            if(treenodeptr->tail){
                printf("%lld\n",treenodeptr->tail->maxlength);
            }else{
                fputs("0\n",stdout);
            } 
        }else if(type==5){
            scanf("%lld",&pi);
            treenodeptr->tresurenum++;
            tresurenum++;
            if(!head){
                head=gentresurenode(tail);
                tail=head;
            }else{
                tail->next=gentresurenode(tail);
                tail=tail->next;
            }
            if(pi>=r[route_run_node-1].dis_from_start){
                tail->tresureval=pi-r[route_run_node-1].dis_from_start;
            }else{
                ridx=bisearch(r,1,route_run_node-1,pi);
                tail->valuelostdungeon=r[ridx-1].dungeonidx;
            }
            if(tresurenum>=route_run_node-1){
                if(head->tresureval>=0){
                    printf("value remaining is %lld\n",head->tresureval);
                }else{
                    printf("value lost at %lld\n",head->valuelostdungeon);
                }
                if(head==tail){
                    tail=NULL;
                    head=NULL;
                }else{
                    head=head->next;
                    head->prev=NULL;
                }
                tresurenum--;
                treenodeptr->tresurenum--;
            }
        }else{
            scanf("%lld%lld",&vi,&li);
            if(!treenode[vi]){
                treenode[vi]=gentreenode(vi);
            }
            if(!treenodeptr->childhead){
                treenodeptr->childhead=gennode(NULL,vi);
                treenodeptr->childtail=treenodeptr->childhead;
            }else{
                treenodeptr->childtail->next=gennode(treenodeptr->childtail,vi);
                treenodeptr->childtail=treenodeptr->childtail->next;          
            }
            treenode[vi]->parent=treenodeptr;
            treenode[vi]->dis_to_parent=li;
            length=buildmaxlength(&treenode[vi],treenode)+treenode[vi]->dis_to_parent;
            if(!treenodeptr->head){               
                treenodeptr->head=genmaxqueue(length,vi,NULL,NULL);
                treenodeptr->tail=treenodeptr->head;    
            }else{
                for(;treenodeptr->head&&treenodeptr->head->maxlength<length;){
                    treenodeptr->head=treenodeptr->head->next;
                    if(!treenodeptr->head){
                        break;
                    }
                    treenode[treenodeptr->head->prev->dungeonidx]->maxqnode=NULL;
                    treenodeptr->head->prev=NULL;
                }
                if(!treenodeptr->head){
                    treenode[treenodeptr->tail->dungeonidx]->maxqnode=NULL;
                    treenodeptr->tail=NULL;
                    treenodeptr->head=genmaxqueue(length,vi,NULL,NULL);
                    treenodeptr->tail=treenodeptr->head;
                }else{
                    treenodeptr->head->prev=genmaxqueue(length,vi,NULL,treenodeptr->head);
                    treenodeptr->head=treenodeptr->head->prev;
                }
            }
            treenode[vi]->maxqnode=treenodeptr->head;
        }
    }
    return 0;
}