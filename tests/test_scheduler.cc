#include "../code/zx.h"
static zx::Logger::ptr g_logger = ZX_LOG_ROOT();

void test_fiber() {
    static int s_count = 2;
    ZX_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    sleep(1);
    if(--s_count >= 0) {
        zx::Scheduler::GetThis()->schedule(&test_fiber, zx::GetThreadid());
    }
}

int main(int argc, char** argv) {
    ZX_LOG_INFO(g_logger) << "main";
    zx::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    ZX_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    ZX_LOG_INFO(g_logger) << "over";
    return 0;
}
