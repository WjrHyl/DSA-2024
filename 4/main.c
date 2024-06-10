#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct node{
    int64_t val;
    int layer;
    struct node* prev;
    struct node* next;
    struct node* below;
}Node;
typedef struct{
    struct node* nhead[62];
    int toplayer;
}Head;
void print(Node* head){
    Node*ptr=head;
    while(ptr){
        printf("%lld ",ptr->val);
        ptr=ptr->next;
    }
    putchar('\n');
}
Node* gennode(int64_t key,Node*prev,Node* next,int layer){
    Node* node=(Node*)malloc(sizeof(Node));
    node->below=NULL;
    node->next=next;
    node->prev=prev;
    node->val=key;
    node->layer=layer;
    return node;
}
void headinit(Head**head){
    (*head)=(Head*)malloc(sizeof(Head));
    (*head)->toplayer=0;
    for(int i=0;i<62;i++){
        (*head)->nhead[i]=gennode(INT64_MAX,NULL,NULL,i);
        if(i>0){
            (*head)->nhead[i]->below=(*head)->nhead[i-1];
        }else{
            (*head)->nhead[i]->below=NULL;
        }
    }
}
void slowget(Head*head,int64_t key){
    int c=0;
    Node*ptr=head->nhead[0];
    while(ptr->next&&key<=ptr->next->val){
        c++;
        printf("%lld ",ptr->next->val);
        ptr=ptr->next;
    }
    if(ptr->val!=key&&c<1){
        fputs("-1",stdout);
    }
}
void fastget(Head*head,int64_t key){
    int c=0;
    Node* ptr=head->nhead[head->toplayer];
    int currlayer=head->toplayer;
    for(;ptr->below;){
        for(;ptr->next&&key<=ptr->next->val;){
            c++;
            printf("%lld ",ptr->next->val);
            ptr=ptr->next;
        }
        ptr=ptr->below;
        currlayer--;
        if(ptr&&ptr!=head->nhead[currlayer]){
            c++;
            printf("%lld ",ptr->val);
        }
    }
    for(;ptr->next&&key<=ptr->next->val;){
        c++;
        printf("%lld ",ptr->next->val);
        ptr=ptr->next;
    }
    if(ptr->val!=key&&c<1){
        fputs("-1",stdout);
    }
}
int isodd(int64_t n){
    return n%2;
}
int coinflip(int64_t a[],int64_t key,int i){
    return isodd((key/a[i]));
}
void insert(int64_t a[],int64_t key,Head** head){
    int idx=0,currlayer;
    for(int i=1;i<62;i++){
        if(coinflip(a,key,i)){
            idx=i;
        }else{
            break;
        }
    }
    if(idx>(*head)->toplayer){
        (*head)->toplayer=idx;
    }
    currlayer=(*head)->toplayer;
    Node** ptr=&(*head)->nhead[(*head)->toplayer],**uplayer,*null=NULL;
    uplayer=&null;
    for(;(*ptr)->below;){
        for(;(*ptr)->next&&key<(*ptr)->next->val;){
            ptr=&(*ptr)->next;
        }
        if(idx>=currlayer){
            Node* next=(*ptr)->next;
            (*ptr)->next=gennode(key,*ptr,next,idx);
            if(next){
                next->prev=(*ptr)->next;
            }
            if(*uplayer){
                (*uplayer)->below=(*ptr)->next;
            }
            uplayer=&(*ptr)->next;    
        }
        ptr=&(*ptr)->below;
        currlayer--;
    }
    for(;(*ptr)->next&&key<(*ptr)->next->val;){
        ptr=&(*ptr)->next;
    }
    Node* next=(*ptr)->next;
    (*ptr)->next=gennode(key,*ptr,next,idx);
    if(next){
        next->prev=(*ptr)->next;
    }
    if(*uplayer){
        (*uplayer)->below=(*ptr)->next;   
    }
}
void delete(Head** head,int64_t key,int64_t a[]){
    Node** ptr=&(*head)->nhead[(*head)->toplayer];
    int currlayer=(*head)->toplayer;
    for(;(*ptr)->below;){
        for(;(*ptr)->next&&key<=(*ptr)->next->val;){
            ptr=&(*ptr)->next;
        }
        if((*ptr)->val==key){
            Node* freeptr,*goptr;
            goptr=(*ptr);
            for(;goptr;){
                freeptr=goptr;
                freeptr->prev->next=freeptr->next;
                if(freeptr->next){
                    freeptr->next->prev=freeptr->prev;
                }
                if(!(*head)->nhead[currlayer]->next&&(*head)->toplayer>0){
                    (*head)->toplayer--;
                }           
                goptr=goptr->below;
                free(freeptr);
                currlayer--;
            }       
            return;
        }
        ptr=&(*ptr)->below;
        currlayer--;
    }
    for(;(*ptr)->next&&key<=(*ptr)->next->val;){
        ptr=&(*ptr)->next;
    }
    if((*ptr)->val==key){
        Node* freeptr,*goptr;
        goptr=(*ptr);
        freeptr=goptr;
        freeptr->prev->next=freeptr->next;
        if(freeptr->next){
            freeptr->next->prev=freeptr->prev;
        }
        if(!(*head)->nhead[0]->next){
            (*head)->toplayer=0; 
        }
        free(freeptr);
    }
}
int main(){
    Head *head;
    int64_t bin[62],n=1;
    for(int i=1;i<=61;i++){
        bin[i]=n;
        n*=2;
    }
    int type,m;
    int64_t key;
    head=(Head*)malloc(sizeof(Head));
    headinit(&head);
    scanf("%d",&m);
    for(int i=0;i<m;i++){
        scanf("%d%lld",&type,&key);
        if(type==1){
            slowget(head,key);
            putchar('\n');
        }else if(type==2){
            fastget(head,key);
            putchar('\n');
        }else if(type==3){
            insert(bin,key,&head);
        }else{
            delete(&head,key,bin);
        } 
    }
    return 0;
}