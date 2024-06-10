#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
typedef struct treap{
    uint32_t pri;
    int32_t val;
    int32_t size;
    int64_t sum;
    struct treap* left;
    struct treap* right;
}Treap;
int32_t size(Treap* t){
    return (t)?t->size:0;
}
int64_t sum(Treap* t){
    return (t)?t->sum:0;
}
int32_t min(int32_t a,int32_t b){
    return (a<b)?a:b;
}
int32_t max(int32_t a,int32_t b){
    return (a>b)?a:b;
}
int32_t position(Treap* target,int32_t pos){
    return pos+1+size(target->left);
}
Treap* gentreap(Treap* left,Treap* right,uint32_t pri,int32_t val){
    Treap* t=(Treap*)malloc(sizeof(Treap));
    t->left=left;
    t->right=right;;
    t->pri=pri;
    t->val=val;
    t->size=1;
    t->sum=val;
    return t;
}
Treap* merge(Treap** left,Treap** right){
    if(!(*left)){
        return *right;
    }
    if(!(*right)){
        return *left;
    }
    if((*left)->pri>(*right)->pri){
        (*left)->right=merge(&(*left)->right,right);
        (*left)->size=1+size((*left)->left)+size((*left)->right);
        (*left)->sum=(*left)->val+sum((*left)->left)+sum((*left)->right);
        return *left;
    }else{
        (*right)->left=merge(left,&(*right)->left);
        (*right)->size=1+size((*right)->left)+size((*right)->right);
        (*right)->sum=(*right)->val+sum((*right)->left)+sum((*right)->right);
        return *right;
    }
}
void inorder(Treap* root){
    if(root->left){
        inorder(root->left);
    }
    printf("%d ",root->val);
    if(root->right){
        inorder(root->right);
    }
}
void split(Treap* root,Treap** left,Treap** right,int32_t k){
    if(!root){
        *left=NULL;
        *right=NULL;
        return;
    }
    if(1+size(root->left)<=k){
        *left=root;
        split(root->right,&(*left)->right,right,k-1-size(root->left));
    }else{
        *right=root;
        split(root->left,left,&(*right)->left,k);
    }
    if((*left)==root){
        (*left)->size=1+size((*left)->left)+size((*left)->right);
        (*left)->sum=(*left)->val+sum((*left)->left)+sum((*left)->right);
    }else{
        (*right)->size=1+size((*right)->left)+size((*right)->right);
        (*right)->sum=(*right)->val+sum((*right)->left)+sum((*right)->right);
    }
}
Treap* delete_node(Treap** root,int32_t k){
    Treap* newroot,**ptr;
    if(!(*root)){
        return NULL;
    }
    if(1+size((*root)->left)==k){
        newroot=merge(&(*root)->left,&(*root)->right);
        (*root)->left=NULL;
        (*root)->right=NULL;
        free(*root);
        return newroot;
    }else if(1+size((*root)->left)>k){
        (*root)->left=delete_node(&(*root)->left,k);
    }else{
        (*root)->right=delete_node(&(*root)->right,k-1-size((*root)->left));
    }
    (*root)->size=1+size((*root)->left)+size((*root)->right);
    (*root)->sum=(*root)->val+sum((*root)->left)+sum((*root)->right);
    return (*root);
}
int64_t calculate_sum(Treap* root,int32_t l,int32_t r,int32_t pos){
    if(!root){
        return 0;
    }
    if(1+pos==l&&pos+size(root->left)+size(root->right)+1==r){
        return root->sum;
    }
    int32_t k=position(root,pos);
    int64_t sum=0;
    if(k>=l&&k<=r){
        sum+=root->val;
    }
    if(l<k){
        sum+=calculate_sum(root->left,l,min(k-1,r),pos);
    }
    if(r>k){
        sum+=calculate_sum(root->right,max(l,k+1),r,pos+1+size(root->left));
    }
    return sum;
}
int main() {
    Treap* root=NULL,*node,*left,*right,*delete,*deleteparent;
    int32_t n,t,val,cmd,l,r,k;
    int64_t sum=0;
    scanf("%d%d",&n,&t);
    for(int i=0;i<n;i++){
        scanf("%d",&val);
        if(!root){
            root=gentreap(NULL,NULL,rand(),val);
        }else{
            node=gentreap(NULL,NULL,rand(),val);
            root=merge(&root,&node);
        }
    }
    for(int i=0;i<t;i++){
        scanf("%d",&cmd);
        if(cmd==1){
            scanf("%d",&k);
            root=delete_node(&root,k);
        }else if (cmd==2){
            scanf("%d%d",&k,&val);
            node=gentreap(NULL,NULL,rand(),val);
            split(root,&left,&right,k);
            left=merge(&left,&node);
            root=merge(&left,&right);
        }else{
            scanf("%d%d",&l,&r);
            sum=calculate_sum(root,l,r,0);
            printf("%lld\n",sum);
        }
    }
    printf("%d\n",size(root));
    if(root){
        inorder(root);
    }
    return 0;
}