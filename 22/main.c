#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
typedef struct list{
    struct list* next;
    struct list* prev;
    char* s;
}List;
typedef struct data{
    List* head;
    List* tail;
    int32_t num;
}Data;
List* genlist(char* s,List* prev,List* next){
    List* l=(List*)malloc(sizeof(List));
    l->s=s;
    l->next=next;
    l->prev=prev;
    return l;
}
Data* gendata(){
    Data* d=(Data*)malloc(sizeof(Data));
    d->head=NULL;
    d->num=0;
    d->tail=NULL;
    return d;
}
uint64_t tao(uint64_t bin[],char* s){
    int32_t len=strlen(s);
    uint64_t re=0;
    for(int i=0;i<len;i++){
        re+=bin[i+64-len]*((uint32_t)s[i]-'a');
    }
    return re;
}
uint64_t lsb(uint64_t val,uint32_t n){
    return val%(1<<n);
}
int main() {
    uint64_t r=2,q=0,hash,already_find=0,t;
    uint64_t bin[64];
    uint32_t n;
    Data** d;
    List* ptr;
    char** s;
    bin[63]=1;
    for(int i=62;i>=0;i--){
        bin[i]=bin[i+1]*26;
    }
    scanf("%u",&n);
    s=(char**)malloc(n*sizeof(char*));
    d=(Data**)malloc((1<<20)*sizeof(Data*));
    for(int i=0;i<(1<<20);i++){
        d[i]=NULL;
    }
    for(int i=0;i<4;i++){
        d[i]=gendata();
    }
    for(int i=0;i<n;i++){
        s[i]=(char*)malloc(65*sizeof(char));
        already_find=0;
        scanf("%s",s[i]);
        t=tao(bin,s[i]);
        hash=lsb(t,r);
        if(hash<q){
            hash=lsb(t,r+1);
        }
        if(d[hash]->num>0){
            d[hash]->tail->next=genlist(s[i],d[hash]->tail,NULL);
            d[hash]->tail=d[hash]->tail->next;
        }else{
            d[hash]->head=genlist(s[i],NULL,NULL);
            d[hash]->tail=d[hash]->head;
        }
        d[hash]->num++;
        if(d[hash]->num>2){
            q++;
            if(!d[(1<<r)+q-1]){
                d[(1<<r)+q-1]=gendata();
            }
            ptr=d[q-1]->head;
            while(ptr){
                if(lsb(tao(bin,ptr->s),r+1)==(1<<r)+q-1){
                    d[q-1]->num--;
                    if(ptr->prev){
                        ptr->prev->next=ptr->next;
                    }
                    if(ptr->next){
                        ptr->next->prev=ptr->prev;
                    }
                    if(d[(1<<r)+q-1]->num==0){
                        d[(1<<r)+q-1]->head=genlist(ptr->s,NULL,NULL);
                        d[(1<<r)+q-1]->tail=d[(1<<r)+q-1]->head;
                    }else{
                        d[(1<<r)+q-1]->tail->next=genlist(ptr->s,d[(1<<r)+q-1]->tail,NULL);
                        d[(1<<r)+q-1]->tail=d[(1<<r)+q-1]->tail->next;
                    }
                    d[(1<<r)+q-1]->num++;
                }else{
                    if(!already_find){
                        d[q-1]->head=ptr;
                        already_find=1;
                    }
                }
                ptr=ptr->next;
            }
            if(d[q-1]->num>0){
                ptr=d[q-1]->head;
                for(;ptr;){
                    if(!ptr->next){
                        d[q-1]->tail=ptr;
                    }
                    ptr=ptr->next;
                }
            }else{
                d[q-1]->head=NULL;
                d[q-1]->tail=NULL;
            }  
        }
        if(q>=(1<<r)){
            q=0;
            r++;
        } // if q = 2^r after the increment, increase the value of r by 1 and set q to 0.
    }
    for(int i=0;i<(1<<r)+q;i++){
        if(d[i]){
            ptr=d[i]->head;
            if(d[i]->num>0){
                for(;ptr;){
                    printf("%s ",ptr->s);
                    ptr=ptr->next;
                }
                putchar('\n');
            }else{
                fputs("-1\n",stdout);
            }
        }else{
            fputs("-1\n",stdout);
        }
    }
    return 0;
}