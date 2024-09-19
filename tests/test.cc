#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
void sv(){
  printf("sv\n");
  return;
}
void *s=(void *)malloc(1024*1024);
int main(){
      ucontext_t t;
    ucontext_t t2;
    getcontext(&t2);
    getcontext(&t);
    printf("hello world\n");
    makecontext(&t2,&sv,0);//参数格式
    t2.uc_link=&t;
    t2.uc_stack.ss_sp=s;
    t2.uc_stack.ss_size=1024*1024;
    setcontext(&t2);
    setcontext(&t);
    return 0;
}