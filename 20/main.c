#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
typedef struct set{
    struct set* p;
    int32_t employee_id;
    int32_t rank;
    int64_t raise;
    int64_t transfer;
    int32_t transfer_times;
    int32_t parent_transfer_times;
}Set;
typedef struct{
    Set* company;
    int64_t salary;
    int64_t bank;
}employee;
Set* genset(int32_t employee_id){
    Set* s=(Set*)malloc(sizeof(Set));
    s->employee_id=employee_id;
    s->rank=0;
    s->p=s;
    s->transfer_times=0;
    s->parent_transfer_times=0;
    s->raise=0;
    s->transfer=0;
    return s;
}
employee* genemployee(int64_t salary){
    employee* e=(employee*)malloc(sizeof(employee));
    e->bank=0;
    e->company=NULL;
    e->salary=salary;
    return e;
}
Set* Find_set(Set** x,int64_t* parent_transfer,int32_t* transfer_times,int64_t* total_raise){
    if((*x)!=(*x)->p){
        (*x)->p=Find_set(&(*x)->p,parent_transfer,transfer_times,total_raise);
        (*transfer_times)-=(*x)->parent_transfer_times;
        (*total_raise)+=(*x)->raise;
        (*x)->transfer+=((*parent_transfer)+(*x)->raise*(*transfer_times));
        (*x)->raise=(*total_raise);
        (*x)->transfer_times+=(*transfer_times);
        (*x)->parent_transfer_times=(*x)->p->transfer_times;
        (*parent_transfer)=(*x)->transfer; 
    }
    (*transfer_times)=(*x)->transfer_times;
    return (*x)->p;
}
void Link(Set** x,Set** y){
    if((*x)->rank>(*y)->rank){
        (*y)->p=(*x);
        (*y)->parent_transfer_times=(*x)->transfer_times;
        (*y)->raise-=(*x)->raise;
        (*y)->transfer-=(*x)->transfer;
    }else{
        (*x)->p=(*y);
        (*x)->parent_transfer_times=(*y)->transfer_times;
        (*x)->raise-=(*y)->raise;
        (*x)->transfer-=(*y)->transfer;
        if((*x)->rank==(*y)->rank){
            (*y)->rank++;
        }
    }
}
void Union(Set** x,Set** y){
    int32_t transfer_times=0;
    int64_t total_raise=0,parent_transfer=0;
    Set* v,*w;
    v=Find_set(x,&parent_transfer,&transfer_times,&total_raise);
    parent_transfer=0;
    transfer_times=0;
    total_raise=0;
    w=Find_set(y,&parent_transfer,&transfer_times,&total_raise);
    if(v!=w){
        Link(&v,&w);
    }
}
int main() {
    int32_t n,q,cmd,transfer_times=0;
    int64_t salary,total_raise=0,c,parent_transfer=0;
    employee** e;
    scanf("%d%d",&n,&q);
    e=(employee**)malloc((n+1)*sizeof(employee*));
    for(int i=1;i<=n;i++){
        scanf("%lld",&salary);
        e[i]=genemployee(salary);
        e[i]->company=genset(i);
    }
    int32_t x,y,k,r;
    Set* target;
    for(int i=0;i<q;i++){
        parent_transfer=0;
        transfer_times=0;
        total_raise=0;
        scanf("%d",&cmd);
        if(cmd==1){
            scanf("%d%d",&x,&y);
            Union(&e[x]->company,&e[y]->company);
        }else if(cmd==2){
            scanf("%d%d",&k,&r);
            target=Find_set(&e[k]->company,&parent_transfer,&transfer_times,&total_raise);
            target->raise+=r;
        }else if(cmd==3){
            scanf("%d",&k);
            target=Find_set(&e[k]->company,&parent_transfer,&transfer_times,&total_raise);
            target->transfer_times++;
            target->transfer+=target->raise;
        }else{
            scanf("%d%lld",&k,&c);
            target=Find_set(&e[k]->company,&parent_transfer,&transfer_times,&total_raise);
            if(target!=e[k]->company){
                e[k]->bank+=(e[k]->company->transfer+e[k]->company->p->transfer);
                e[k]->bank+=(e[k]->salary*(e[k]->company->transfer_times-e[k]->company->parent_transfer_times+e[k]->company->p->transfer_times));
            }else{
                e[k]->bank+=(e[k]->company->transfer);
                e[k]->bank+=(e[k]->salary*e[k]->company->transfer_times);
            }
            e[k]->company=genset(k);
            e[k]->salary=c;
        }
    }
    for(int i=1;i<=n;i++){
        parent_transfer=0;
        transfer_times=0;
        total_raise=0;
        target=Find_set(&e[i]->company,&parent_transfer,&transfer_times,&total_raise);
        if(target!=e[i]->company){
            e[i]->bank+=(e[i]->company->transfer+e[i]->company->p->transfer);
            e[i]->bank+=(e[i]->salary*(e[i]->company->transfer_times-e[i]->company->parent_transfer_times+e[i]->company->p->transfer_times));
        }else{
            e[i]->bank+=(e[i]->company->transfer);
            e[i]->bank+=(e[i]->salary*e[i]->company->transfer_times);
        }
        printf("%lld ",e[i]->bank);
    }
    return 0;
}