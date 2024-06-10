#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
typedef struct node{
    int priority;
    int job_id;
    int degree;
    struct node* parent;
    struct node* sibling;
    struct node* child;
}Node;
typedef struct{
    Node* node;
    int job_num;
}Data;
Node* CreateNode(int priority,int job_id){
    Node* node=(Node*)malloc(sizeof(Node));
    node->priority=priority;
    node->job_id=job_id;
    node->child=NULL;
    node->sibling=NULL;
    node->parent=NULL;
    node->degree=0;
    return node;
}
void BinomialHeapLink(Node** x,Node** y){
    (*x)->parent=(*y);
    (*x)->sibling=(*y)->child;
    (*y)->child=(*x);
    (*y)->degree++;
}
Node* RevertList(Node* h){
    if(h->sibling){
        Node*head=RevertList(h->sibling);
        h->sibling->sibling=h;
        return head;
    }
    return h;
}
Node* BinomialHeapMerge(Node* h1,Node* h2){
    Node* h=NULL;
    Node* a,*hptr,*h1ptr=h1,*h2ptr=h2;
    while(h1ptr&&h2ptr){
        if(h1ptr->degree<h2ptr->degree){
            a=h1ptr->sibling;
            if(!h){
                h=h1ptr;
                hptr=h;
            }else{
                hptr->sibling=h1ptr;
                hptr=hptr->sibling;
            }
            h1ptr=a;
        }else if(h1ptr->degree==h2ptr->degree){
            a=h1ptr->sibling;
            if(!h){
                h=h1ptr;
                hptr=h;
            }else{
                hptr->sibling=h1ptr;
                hptr=hptr->sibling;
            }
            h1ptr=a;
        }else{
            a=h2ptr->sibling;
            if(!h){
                h=h2ptr;
                hptr=h;
            }else{
                hptr->sibling=h2ptr;
                hptr=hptr->sibling;
            }
            h2ptr=a;
        }
    }
    if(h1ptr){
        if(!h){
            h=h1ptr;
        }else{
            hptr->sibling=h1ptr;
        }
    }else{
        if(!h){
            h=h2ptr;
        }else{
            hptr->sibling=h2ptr;
        }
    }
    return h;
}
Node* BinomialHeapUnion(Node* h1,Node* h2){
    Node* h;
    h=BinomialHeapMerge(h1,h2);
    if(!h){
        return h;
    }
    Node* prev=NULL,*x=h,*next=x->sibling;
    while(next){
        if((x->degree!=next->degree)||(next->sibling&&next->sibling->degree==x->degree)){
            prev=x;
            x=next;
        }else{
            if(x->priority>=next->priority){
                x->sibling=next->sibling;
                BinomialHeapLink(&next,&x);
            }else{
                if(!prev){
                    h=next;
                }else{
                    prev->sibling=next;
                }
                BinomialHeapLink(&x,&next);
                x=next;
            }
        }
        next=x->sibling;
    }
    return h;
}
Node* BinomialHeapInsert(Node* h,Node* x){
    Node*re; 
    re=BinomialHeapUnion(h,x);
    return re;
}
Node* BinomialHeapExtractMax(Data** h){
    int max;
    Node* hr=NULL,*p,*x=(*h)->node,*t=NULL,*re;
    Node* x_=x;
    if(!x){
        return NULL;
    }
    max=(*h)->node->priority;
    p=(*h)->node->sibling;
    while(p){
        if(p->priority>max){
            max=p->priority;
            t=p;
            x_=x;
        }
        x=x->sibling;
        p=p->sibling;
    }
    if(!t){ 
        t=(*h)->node;
        (*h)->node=t->sibling;
    }else{
        x_->sibling=t->sibling;
    }
    printf("%d printed\n",t->job_id);
    if(t->child){
        hr=RevertList(t->child);
        t->child->sibling=NULL;
    }
    re=BinomialHeapUnion((*h)->node,hr);
    return re;
}
int main(){
    int n,m,idx;
    scanf("%d%d",&n,&m);
    int job_id,priority,printer_id,printer_id1,printer_id2;
    Data* printers[1000001];
    Node* node1,*node2;
    for(int i=1;i<1000001;i++){
        printers[i]=(Data*)malloc(sizeof(Data));
        printers[i]->node=NULL;
        printers[i]->job_num=0;
    }
    for(int i=0;i<m;i++){
        scanf("%d",&idx);
        if(idx==1){
            scanf("%d%d%d",&job_id,&priority,&printer_id);
            node1=printers[printer_id]->node;
            node2=CreateNode(priority,job_id);
            printers[printer_id]->node=BinomialHeapInsert(node1,node2);
            printers[printer_id]->job_num++;
            printf("%d jobs waiting on printer %d\n",printers[printer_id]->job_num,printer_id);
        }else if(idx==2){
            scanf("%d",&printer_id);
            if(printers[printer_id]->job_num>0){
                printers[printer_id]->node=BinomialHeapExtractMax(&printers[printer_id]);
                printers[printer_id]->job_num--;
            }else{
                fputs("no documents in queue\n",stdout);
            }
        }else{
            scanf("%d%d",&printer_id1,&printer_id2);
            printers[printer_id2]->node=BinomialHeapUnion(printers[printer_id1]->node,printers[printer_id2]->node);
            printers[printer_id2]->job_num+=printers[printer_id1]->job_num;
            printf("%d jobs waiting on printer %d after moving\n",printers[printer_id2]->job_num,printer_id2);
            printers[printer_id1]->node=NULL;
            printers[printer_id1]->job_num=0;
        }
    }
    return 0;
}