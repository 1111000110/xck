#include <assert.h>
#include "../code/zx.h"
zx::Logger::ptr g_logger=ZX_LOG_ROOT();
void test_assert(){
    ZX_LOG_INFO(g_logger)<< zx::BacktraceToString(10,2,"   ");
    ZX_ASSERT(false);
    ZX_ASSERT2(0==1,"abcdef XX");
}
int main(){
    test_assert();
    return 0;
}