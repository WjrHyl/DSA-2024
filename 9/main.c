#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
enum{integer,operator,bracket};
typedef struct{
    int type;
    char ope;
    char br;
    long long integer;
}st;
typedef struct{
    st buf[4050];
    int topidx;
}Stack;
void stackinit(Stack* s){
    for(int i=0;i<4050;i++){
        s->topidx=0;
        s->buf[i].type=-1;
        s->buf[i].br='\0';
        s->buf[i].ope='\0';
        s->buf[i].integer=0;
    }
} 
int precedence(char c) { 
    if(c == '*' || c == '/' || c == '%') 
    return 2; 
    else if(c == '+' || c == '-') 
    return 1; 
    else
    return -1; 
}
void push(Stack* s,char token,long long inte,int type){
    if(type==bracket){
        s->buf[s->topidx].br=token;
        s->buf[s->topidx].type=bracket;
    }else if(type==operator){
        s->buf[s->topidx].ope=token;
        s->buf[s->topidx].type=operator;
    }else{
        s->buf[s->topidx].integer=inte;
        s->buf[s->topidx].type=integer;
    }
    s->topidx++;
}
char peep(Stack* s){
    if(s->buf[s->topidx-1].type==bracket){
        return s->buf[s->topidx-1].br;
    }else{
        return s->buf[s->topidx-1].ope;
    }
}
void pop(Stack* s){
    if(s->buf[s->topidx-1].type==bracket){
        s->buf[s->topidx-1].br='\0';
        s->buf[s->topidx-1].type=-1;
    }else if(s->buf[s->topidx-1].type==operator){
        s->buf[s->topidx-1].ope='\0';
        s->buf[s->topidx-1].type=-1;
    }else{
        s->buf[s->topidx-1].integer=0;
        s->buf[s->topidx-1].type=-1;
    }
    s->topidx--;
}
void infix_to_posfix(Stack* infixst,Stack* posfixst){
    Stack s;
    stackinit(&s);
    for(int i=0;i<infixst->topidx;i++){
        if(infixst->buf[i].type==integer){
            posfixst->buf[posfixst->topidx].type=integer;
            posfixst->buf[posfixst->topidx].integer=infixst->buf[i].integer;
            posfixst->topidx++;
            printf("%lld",infixst->buf[i].integer);
        }else if(infixst->buf[i].type==bracket){
            if(infixst->buf[i].br=='('){
                push(&s,infixst->buf[i].br,0,bracket);
            }else{
                for(;s.topidx>0&&peep(&s)!='(';){
                    posfixst->buf[posfixst->topidx].type=operator;
                    posfixst->buf[posfixst->topidx].ope=peep(&s);
                    posfixst->topidx++;
                    printf("%c",peep(&s));
                    pop(&s);
                }
                pop(&s);
            }
        }else{
            for(;s.topidx>0&&(precedence(peep(&s))>=precedence(infixst->buf[i].ope));){
                posfixst->buf[posfixst->topidx].type=operator;
                posfixst->buf[posfixst->topidx].ope=peep(&s);
                posfixst->topidx++;
                printf("%c",peep(&s));
                pop(&s);
;            }
            push(&s,infixst->buf[i].ope,0,operator);
        }
    }
    while(s.topidx>0){
        if(s.buf[s.topidx-1].type==integer){
            posfixst->buf[posfixst->topidx].type=integer;
            posfixst->buf[posfixst->topidx].integer=s.buf[s.topidx-1].integer;
            posfixst->topidx++;
            printf("%lld",s.buf[s.topidx-1].integer);
        }else{
            posfixst->buf[posfixst->topidx].type=operator;
            posfixst->buf[posfixst->topidx].ope=s.buf[s.topidx-1].ope;
            posfixst->topidx++;
            printf("%c",s.buf[s.topidx-1].ope);
        }
        pop(&s);
    }
}
long long cal(long long n1,char ope,long long n2){
    if(ope=='+'){
        return n1+n2;
    }else if(ope=='-'){
        return n1-n2;
    }else if(ope=='*'){
        return n1*n2;
    }else if(ope=='/'){
        return n1/n2;
    }else{
        return n1%n2;
    }
}
long long posfix_eval(Stack* posfixst){
    Stack s;
    stackinit(&s);
    for(int i=0;i<posfixst->topidx;i++){
        if(posfixst->buf[i].type==integer){
            push(&s,'\0',posfixst->buf[i].integer,integer);
        }else{
            long long n1,n2;
            char ope=posfixst->buf[i].ope;
            n2=s.buf[s.topidx-1].integer;
            pop(&s);
            n1=s.buf[s.topidx-1].integer;
            pop(&s);
            push(&s,'\0',cal(n1,ope,n2),integer);
        }
    }
    return s.buf[s.topidx-1].integer;
}
int main(){
    char infix[4100],str[4100];
    Stack infixst,posfixst;
    infixst.topidx=0;
    posfixst.topidx=0;
    long long val;
    for(int n=0;n<3;n++){
        stackinit(&infixst);
        stackinit(&posfixst);
        scanf("%s",infix);
        int c=0;
        for(int i=0;i<strlen(infix);i++){
            if(infix[i]>='0'&&infix[i]<='9'){
                str[c++]=infix[i];
            }else if(infix[i]=='('||infix[i]==')'){
                if(c!=0){
                    c=0;
                    infixst.buf[infixst.topidx].integer=atoll(str);
                    infixst.buf[infixst.topidx].type=integer;
                    infixst.topidx++;
                    int len=strlen(str);
                    memset(str,0,len);
                }
                infixst.buf[infixst.topidx].br=infix[i];
                infixst.buf[infixst.topidx].type=bracket;
                infixst.topidx++;
            }else{
                if(c!=0){
                    c=0;
                    infixst.buf[infixst.topidx].integer=atoll(str);
                    infixst.buf[infixst.topidx].type=integer;
                    infixst.topidx++;
                    int len=strlen(str);
                    memset(str,0,len);
                }
                infixst.buf[infixst.topidx].ope=infix[i];
                infixst.buf[infixst.topidx].type=operator;
                infixst.topidx++;
            }
        }
        if(c!=0){
            c=0;
            infixst.buf[infixst.topidx].integer=atoll(str);
            infixst.buf[infixst.topidx].type=integer;
            infixst.topidx++;
            int len=strlen(str);
            memset(str,0,len);
        }
        infix_to_posfix(&infixst,&posfixst);
        val=posfix_eval(&posfixst);
        printf("=%lld\n",val);
    }
    return 0;
}