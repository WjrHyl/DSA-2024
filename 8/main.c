#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct node{
    int data;
    struct node* next;
}Head;
void floyd_cycle_delete(Head*head){
    Head* hare=head;
    Head* tortoise=head;
    while(hare&&hare->next){
        printf("%d ",hare->data);
        hare=hare->next->next;
        tortoise=tortoise->next;
        if(hare==tortoise){
            break;
        }
    }
    if(hare){
        printf("%d ",hare->data);
    }
}
int32_t main(){
    int N;
    int data,idx;
    Head** node,*head;
    scanf("%d",&N);
    node=(Head**)malloc((N+1)*sizeof(Head*));
    node[0]=NULL;
    for(int i=1;i<=N;i++){
        node[i]=(Head*)malloc(sizeof(Head));
    }
    for(int i=1;i<=N;i++){
        scanf("%d%d",&data,&idx);
        node[i]->data=data;
        node[i]->next=node[idx];
    }
    head=node[1];
    floyd_cycle_delete(head);
    return 0;
}