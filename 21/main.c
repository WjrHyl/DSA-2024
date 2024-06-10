#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef struct treenode{
    char color;
    int32_t key;
    struct treenode* left;
    struct treenode* right;
    struct treenode* parent;
}Treenode;
Treenode* gentreenode(int32_t key,char color,Treenode* left,Treenode* right,Treenode* parent){
    Treenode* node=(Treenode*)malloc(sizeof(Treenode));
    node->color=color;
    node->key=key;
    node->left=left;
    node->right=right;
    node->parent=parent;
    return node;
}
void insert(Treenode** root,Treenode* parent,int32_t key,char color){
    if(!(*root)){
        (*root)=gentreenode(key,color,NULL,NULL,parent);
    }else{
        if((*root)->key>key){
            insert(&(*root)->left,(*root),key,color);    
        }else{
            insert(&(*root)->right,(*root),key,color);
        }
    }
}
void print(Treenode* root,int32_t key,int32_t depth,int32_t bdepth){
    if(root->key==key){
        if(root->left){
            printf("%d ",root->left->key);
        }else{
            fputs("-1 ",stdout);
        }
        if(root->right){
            printf("%d ",root->right->key);
        }else{
            fputs("-1 ",stdout);
        }
        printf("%d %d\n",bdepth,depth);
        return;
    }else if(root->key>key){
        if(root->color=='R'){
            print(root->left,key,depth+1,bdepth);
        }else{
            print(root->left,key,depth+1,bdepth+1);
        }
    }else{
        if(root->color=='R'){
            print(root->right,key,depth+1,bdepth);
        }else{
            print(root->right,key,depth+1,bdepth+1);
        }
    }
    return;
}
Treenode* find(Treenode* root,int32_t key){
    Treenode* re;
    if(root->key==key){
        return root;
    }else if(root->key>key){
        re=find(root->left,key);
    }else{
        re=find(root->right,key);
    }
    return re;
}
void left_rotate(Treenode** root,Treenode** x){
    Treenode* y=(*x)->right;
    (*x)->right=y->left;
    if(y->left){
        y->left->parent=(*x);
    }
    y->parent=(*x)->parent;
    if(!(*x)->parent){
        (*root)=y;
    }else if((*x)==(*x)->parent->left){
        (*x)->parent->left=y;
    }else{
        (*x)->parent->right=y;
    }
    y->left=(*x);
    (*x)->parent=y;
}
void right_rotate(Treenode** root,Treenode** y){
    Treenode* x=(*y)->left;
    (*y)->left=x->right;
    if(x->right){
        x->right->parent=(*y);
    }
    x->parent=(*y)->parent;
    if(!(*y)->parent){
        (*root)=x;
    }else if((*y)==(*y)->parent->left){
        (*y)->parent->left=x;
    }else{
        (*y)->parent->right=x;
    }
    x->right=(*y);
    (*y)->parent=x;
}
int main(){
    char color,cmd,s1[15],s2[15];
    int32_t key;
    int32_t n,m;
    Treenode* root=NULL,*ptr;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%s%s",s1,s2);
        color=s1[0];
        key=atoi(s2);
        insert(&root,NULL,key,color);
    }
    scanf("%d",&m);
    for(int i=0;i<m;i++){
        scanf("%s%s",s1,s2);
        cmd=s1[0];
        key=atoi(s2);
        if(cmd=='P'){
            print(root,key,0,0);
        }else if(cmd=='L'){
            ptr=find(root,key);
            left_rotate(&root,&ptr);
        }else{
            ptr=find(root,key);
            right_rotate(&root,&ptr);
        }
    }
    return 0;
}