#include "../code/zx.h"
static zx::Logger::ptr g_logger = ZX_LOG_ROOT();

void test_fiber() {
    static int s_count = 10;
    if(--s_count >= 0) {
        ZX_LOG_INFO(g_logger)<<"lxqshishabi"<<s_count;
       zx::Scheduler::GetThis()->schedule(&test_fiber);
    }
}
int main(int argc, char** argv) {
    ZX_LOG_INFO(g_logger) << "main";
    zx::Scheduler sc(3, true, "main_test");
    sc.start();
    ZX_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    ZX_LOG_INFO(g_logger) << "over";
    return 0;
}
