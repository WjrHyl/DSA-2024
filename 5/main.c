#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXM 500000
#define MAXN 1000000
#define MAXT 500000
#define int int64_t
typedef struct record{
    int timesidx;
    int64_t increase;
    struct record* prev;
    struct record* next;
    struct record* below; 
}rec;
typedef struct{
    int toplayer;
    rec *nhead[22];
}Head;
typedef struct{
    int idx;
    int attacktimes;
    int null;
    int64_t power;
    Head*head;
}power;
rec* gennode(int timesidx,int64_t increase,rec* prev,rec* next){
    rec *r;
    r=(rec*)malloc(sizeof(rec));
    r->increase=increase;
    r->prev=prev;
    r->next=next;
    r->timesidx=timesidx;
    return r;
}
void headinit(Head**head){
    (*head)=(Head*)malloc(sizeof(Head));
    (*head)->toplayer=0;
    for(int i=0;i<22;i++){
        (*head)->nhead[i]=gennode(-1,-1,NULL,NULL);
        if(i>0){
            (*head)->nhead[i]->below=(*head)->nhead[i-1];
        }else{
            (*head)->nhead[i]->below=NULL;
        }
    }
}
void slowget(Head*head,int M){
    int64_t attack[M];
    int c=0;
    rec*ptr=head->nhead[0];
    for(;ptr->next;){
        if(ptr->next->timesidx==1){
            attack[M-1-c]=ptr->next->increase;
        }else{
            attack[M-1-c]=ptr->next->increase-ptr->next->next->increase;
        }
        c++;
        ptr=ptr->next;
    }
    for(int i=0;i<c;i++){
        printf("%lld ",attack[M-c+i]);
    }
}
rec* fastget(Head* head,int idxkey){
    rec* ptr=head->nhead[head->toplayer];
    rec* p=head->nhead[0];
    int currlayer=head->toplayer;
    for(;ptr->below;){
        for(;ptr->next&&idxkey<=ptr->next->timesidx;){
            ptr=ptr->next;
        }
        ptr=ptr->below;
    }
    
    for(;ptr->next&&idxkey<=ptr->next->timesidx;){
        ptr=ptr->next;
    }
    if(ptr->timesidx==idxkey){
        return ptr;
    }else{
        return NULL;
    }
}
int isodd(int n){
    return n%2;
}
int coinflip(int a[],int idxkey,int i){
    return isodd((idxkey/a[i]));
}
void insert(int a[],int idxkey,Head**head,int64_t increase){
    int idx=0,currlayer;
    for(int i=1;i<22;i++){
        if(coinflip(a,idxkey,i)){
            idx=i;
        }else{
            break;
        }
    }
    if(idx>(*head)->toplayer){
        (*head)->toplayer=idx;
    }
    currlayer=idx;
    rec** ptr=&(*head)->nhead[idx],**uplayer,*null=NULL;
    uplayer=&null;
    for(;(*ptr)->below;){ 
        rec* next=(*ptr)->next;
        (*ptr)->next=gennode(idxkey,increase,*ptr,next);
        if(next){
            (*ptr)->next->increase=increase+next->increase;
            next->prev=(*ptr)->next;
        }
        if(*uplayer){
            (*uplayer)->below=(*ptr)->next;
        }
        uplayer=&(*ptr)->next;    
        ptr=&(*ptr)->below;
    }
    rec* next=(*ptr)->next;
    (*ptr)->next=gennode(idxkey,increase,*ptr,next);
    if(next){
        (*ptr)->next->increase=increase+next->increase;
        next->prev=(*ptr)->next;
    }
    if(*uplayer){
        (*uplayer)->below=(*ptr)->next;   
    }
}
void binsearch(power p[],int addpowertimes[],int idxpowerrank[],int inc2times,int N,int key){
    int m;
    int left=1,right=N;
    while(left<=right){
        m=(left+right)/2;
        if(p[m].power+(N-m)*(inc2times-addpowertimes[p[m].idx])<key){
            right=m-1;   
        }else{
            left=m+1;
        }
    }
    if(p[m].power+(N-m)*(inc2times-addpowertimes[p[m].idx])>=key){
        printf("%lld %lld\n",m,p[m].idx);
    }else{
        printf("%lld %lld\n",m-1,p[m-1].idx);
    }
}
int32_t main(){
    int type,M,N,T;
    int inc2times=0,bin[22],n=1;
    int *idxpowerrank;// should swap
    int *addpowertimes;
    int64_t qi;
    power *p; // should swap
    scanf("%lld%lld%lld",&N,&M,&T);
    p=(power*)malloc((N+1)*sizeof(power));
    idxpowerrank=(int*)malloc((N+1)*sizeof(int));
    addpowertimes=(int*)malloc((N+1)*sizeof(int));
    for(int i=1;i<22;i++){
        bin[i]=n;
        n*=2;
    }
    for(int i=1;i<=N;i++){
        idxpowerrank[i]=0;
        addpowertimes[i]=0;
        p[i].attacktimes=0;
        p[i].null=1;
    }
    for(int i=1;i<=N;i++){
        idxpowerrank[i]=i;
        scanf("%lld",&p[i].power);
        p[i].idx=i;
    }
    for(int i=0;i<M;i++){
        scanf("%lld",&type);
        if(type==1){
            int idx,idxrank=-1,prerankidx=0,prerankidxrank=0;
            scanf("%lld",&idx);
            idxrank=idxpowerrank[idx];
            p[idxrank].power+=(N-idxpowerrank[idx])*(inc2times-addpowertimes[idx]);
            addpowertimes[idx]=inc2times;
            if(idxrank>1){
                prerankidx=p[idxrank-1].idx;
                prerankidxrank=idxrank-1;
                if(p[idxrank].null){
                    headinit(&p[idxrank].head);
                    p[idxrank].null=0;
                }
                p[prerankidxrank].power+=(N-prerankidxrank)*(inc2times-addpowertimes[prerankidx]);
                addpowertimes[prerankidx]=inc2times; // add idx which rank less 1 than target idx power back  
                int64_t increase=p[prerankidxrank].power-p[idxrank].power;
                insert(bin,p[idxrank].attacktimes+1,&p[idxrank].head,increase);// insert powerincrease node
                p[idxrank].power=p[prerankidxrank].power;// target idx power increase
                p[idxrank].attacktimes++; // record the attack log
                power tem;
                tem=p[idxrank];
                p[idxrank]=p[prerankidxrank];
                p[prerankidxrank]=tem; // swap powerarray
                idxpowerrank[idx]--;
                idxpowerrank[prerankidx]++; // swap the rank  
            }
        }else if(type==2){
            inc2times++;
        }else if(type==3){
            scanf("%lld",&qi);
            int idx=p[1].idx;
            if(p[1].power+(N-1)*(inc2times-addpowertimes[idx])<qi){
                fputs("0 0\n",stdout);
            }else if(p[N].power>qi){
                printf("%lld %lld\n",N,p[N].idx);
            }else{
                binsearch(p,addpowertimes,idxpowerrank,inc2times,N,qi);
            }
        }else{
            int bi,mi,idxrank;
            scanf("%lld %lld",&bi,&mi);
            idxrank=idxpowerrank[bi];
            if(p[idxrank].attacktimes<=mi){
                if(p[idxrank].attacktimes==0){
                    fputs("0\n",stdout);
                }else{
                    rec* ptr;
                    ptr=fastget(p[idxrank].head,p[idxrank].attacktimes);
                    printf("%lld\n",ptr->increase);
                }    
            }else{
                rec* ptr1,*ptr2;
                ptr1=fastget(p[idxrank].head,p[idxrank].attacktimes-mi);
                ptr2=fastget(p[idxrank].head,p[idxrank].attacktimes);
                printf("%lld\n",ptr2->increase-ptr1->increase);
            }
        }
    }
    putchar('\n');
    for(int i=1;i<=N;i++){
        int idxrank=idxpowerrank[i];
        printf("%lld ",p[idxrank].attacktimes);
        if(p[idxrank].attacktimes>0){
            slowget(p[idxrank].head,M);
        }
        (i!=N)&&putchar('\n');
    }
    return 0;
}