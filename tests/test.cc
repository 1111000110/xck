#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "../code/zx.h"
#include "../code/Log/Log_H/LogDefine.h"

//zx::Logger::ptr  g_liogger=ZX_LOG_NAME("system");
int main(){
    YAML::Node root=YAML::LoadFile("log.yml");//加载yam格式的信息
     zx::Config::LoadFromYaml(root);
    zx::Logger::ptr g_liogger=ZX_LOG_NAME("zhangxuan");

  while(1){
      ZX_LOG_INFO(g_liogger)<<"zxghakhdsiuad";
  }

}