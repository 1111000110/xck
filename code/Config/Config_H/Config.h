#ifndef __ZX_CONFIG_CONFIG_H_CONFIG_H__
#define __ZX_CONFIG_CONFIG_H_CONFIG_H__
#include "ConfigVarBase.h"
#include "ConfigVar.h"
#include "../../Log/Log_H/LoggerManger.h"
#include <boost/lexical_cast.hpp>
#include <map>
namespace zx{    
    class Config {
    public:
        typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap; // 定义配置变量映射类型
        // 定义Lookup函数的模板，用于查找或创建配置变量
        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& default_value, const std::string& description="") {
            auto it=GetDatas().find(name);
            if(it!=GetDatas().end()){
                auto tmp=std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
                if(tmp){
                        ZX_LOG_INFO(ZX_LOG_ROOT()) << "Lookup name " << name << " exits";
                        return tmp;
                }else{
                    ZX_LOG_INFO(ZX_LOG_ROOT()) << "Lookup name " << name << " exits but type not "<<typeid(T).name()<<" real_type= "<<it->second->getTypeName()<<" "<<it->second->toString();
                    return nullptr;
                }
            }
            // 检查配置变量名称是否有效
            if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._0123456789") != std::string::npos) {
                ZX_LOG_ERROR(ZX_LOG_ROOT()) << "Lookup name invalid " << name;
                throw std::invalid_argument(name);
            }
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description)); // 创建新的配置变量
            GetDatas()[name] = v; // 将配置变量添加到映射中
            return v;
        }
        // 定义Lookup函数的另一个模板，用于查找特定类型的配置变量
        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
            auto it = GetDatas().find(name); // 在映射中查找配置变量
            if (it == GetDatas().end()) {
                return nullptr;
            }
            return std::dynamic_pointer_cast<ConfigVar<T>>(it->second); // 将基类智能指针转换为派生类智能指针
        }
        //查找并返回指针
        static ConfigVarBase::ptr LookupBase(const std::string&name);
        // 定义LoadFromYaml函数，用于从YAML文件加载配置
        static void LoadFromYaml(const YAML::Node& node);
    private:
        static ConfigVarMap&GetDatas(){
            static ConfigVarMap s_datas; // 定义静态成员，用于存储配置变量的映射
            return s_datas;
        }
       
    };
}
#endif 