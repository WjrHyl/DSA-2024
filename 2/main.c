#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct node{
    int num;
    struct node*next;
}Node;
Node* gennode(Node* head,int num){
    Node* node;
    node=(Node*)malloc(sizeof(Node));
    node->num=num;
    node->next=head;
    return node;
}
enum{notfound,add,removed,move,merge};
int main(){
    int n,m,type;
    Node* dec[100001],*dectail[100001];
    scanf("%d%d",&n,&m);
    for(int i=0;i<100001;i++){
        dec[i]=NULL;
        dectail[i]=NULL;
    }
    for(int i=0;i<m;i++){
        scanf("%d",&type);
        Node* node,*ptr;
        if(type==add){
            int ii,u;
            scanf("%d%d",&ii,&u);
            node=gennode(dec[ii],u);
            if(!dec[ii]){
                dectail[ii]=node;
                dectail[ii]->next=NULL;
            }
            dec[ii]=node;
        }else if(type==removed){
            int ii;
            scanf("%d",&ii);
            if(dec[ii]){
                dec[ii]=dec[ii]->next;
            }
            if(!dec[ii]){
                dectail[ii]=NULL;
            }
        }else if(type==move){
            int ii,jj;
            scanf("%d%d",&ii,&jj);
            if(dec[ii]){
                dectail[ii]->next=dec[jj];
                if(!dectail[jj]){
                    dectail[jj]=dectail[ii];
                }
                dec[jj]=dec[ii];
                dec[ii]=NULL;
                dectail[ii]=NULL;
            }   
        }else{
            int ii,jj;
            Node* temphead=NULL,*temptail=NULL,*node1,*node2;
            scanf("%d%d",&ii,&jj);
            for(;dec[ii]&&dec[jj];){
                node1=dec[ii];
                dec[ii]=dec[ii]->next;
                node1->next=NULL;
                if(!temphead){   
                    temptail=node1;
                    temphead=temptail;
                }else{
                    temptail->next=node1;
                    temptail=temptail->next;
                }
                if(!dec[ii]){
                    dectail[ii]=NULL;
                }
                node2=dec[jj];
                dec[jj]=dec[jj]->next;
                node2->next=NULL;
                temptail->next=node2;
                temptail=node2;
                if(!dec[jj]){
                    dectail[jj]=NULL;
                }
            }
            if(dec[ii]){
                if(temptail){
                    temptail->next=dec[ii];
                }else{
                    temphead=dec[ii];
                }
                temptail=dectail[ii];
                dec[ii]=NULL;
                dectail[ii]=NULL;
                dec[jj]=temphead;
                dectail[jj]=temptail;
            }else{
                if(dec[jj]){
                    if(temphead){
                        temptail->next=dec[jj];
                        dec[jj]=temphead;
                    }
                }else{
                    if(temphead){
                        dec[jj]=temphead;
                        dectail[jj]=temptail;
                    }
                }  
            }
        }
    }
    for(int i=1;i<=n;i++){
        if(dec[i]){
            Node* ptr=dec[i],*nptr=dec[i];
            int count=0;
            for(;nptr;){
                count++;
                nptr=nptr->next;
            }
            printf("%d ",count);
            for(;ptr;){
                printf("%d ",ptr->num);
                ptr=ptr->next;
            }
            putchar('\n');
        }else{
            fputs("0\n",stdout);
        }
    }
    return 0;
}