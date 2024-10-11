#include "../code/zx.h"
zx::Logger::ptr g_logger=ZX_LOG_ROOT();
void run_in_fiber(){
    ZX_LOG_INFO(g_logger)<<"run_in_fiber begin";
    zx::Fiber::GetThis()->YieldToHold();
    ZX_LOG_INFO(g_logger)<<"run_in_fiber end";
     zx::Fiber::GetThis()->YieldToHold();
}
void test_fiber(){
    zx::Fiber::GetThis();//初始化主协程
    ZX_LOG_INFO(g_logger)<<"main begin";
    zx::Fiber::ptr  fiber(new zx::Fiber(run_in_fiber));//创建新协程
    fiber->call();//将新协程执行到前台
    ZX_LOG_INFO(g_logger)<<"main after swapIn";
    fiber->call();
    ZX_LOG_INFO(g_logger)<<"main after end";

}
int main(){
    zx::Thread::SetName("main");
   std::vector<zx::Thread::ptr>thrs;
   //for(int i=0;i<3;++i){
    thrs.push_back(zx::Thread::ptr(new zx::Thread(&test_fiber,"name_"+std::to_string(0))));
   //}
   //for(auto i:thrs){
        thrs[0]->join();
   //}
}