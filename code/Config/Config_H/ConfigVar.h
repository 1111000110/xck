#ifndef __ZX_CONFIG_CONFIG_H_CONFIGVAR_H__
#define __ZX_CONFIG_CONFIG_H_CONFIGVAR_H__
#include "../../Log/Log_H/LoggerManger.h"
//#include "../../Log/Log_H/LogDefine.h"
#include "ConfigVarBase.h"
#include <boost/lexical_cast.hpp>
#include "LexicalCast.h"
#include <functional>


namespace zx{
//FromStr T operator()(const std::string&)
//ToStr std::string operator()(const &T)
// 定义一个模板类ConfigVar，用于存储特定类型的配置变量
    template<class T,class FromStr=LexicalCast<std::string,T>,class ToStr=LexicalCast<T,std::string>>
    class ConfigVar : public ConfigVarBase {
    public:
        typedef std::shared_ptr<ConfigVar> ptr; // 定义智能指针类型
        typedef std::function<void (const T& old_value,const T&new_value)>on_change_cb;
        // 构造函数，初始化基类和值
        ConfigVar(const std::string& name, const T& default_value, const std::string& description="") : ConfigVarBase(name, description), m_val(default_value) {}
        // 重写toString函数，使用boost::lexical_cast将值转换为字符串
        std::string toString() override {
            try {
                return ToStr()(m_val);
            } catch (std::exception& e) {
                ZX_LOG_ERROR(ZX_LOG_ROOT()) << "configVar::toString exception " << e.what() << " convert: " << typeid(m_val).name() << " to string ";
            }
            return "";
        }
        // 重写fromString函数，使用boost::lexical_cast从字符串解析值
        bool fromString(const std::string& val) override {
            try {
                setValue(FromStr()(val));
            } catch (std::exception &e) {
                ZX_LOG_ERROR(ZX_LOG_ROOT()) << "configVar::fromString exception " << e.what() << " convert: " << typeid(m_val).name() << " to string ";
            }
            return false;
        }
        // 获取配置变量的值
        const T getValue() const { return m_val; }
        // 设置配置变量的值
        void setValue(const T& v) { 
            if(v==m_val){
                return ;
            }
            for(auto&i:m_cbs){
                i.second(m_val,v);
            }
            m_val=v;
        }
        std::string getTypeName()const override{return typeid(T).name();}
        void addListender(uint64_t key,on_change_cb cb){
            m_cbs[key]=cb;
        }
        void delListender(uint64_t key){
            m_cbs.erase(key);
        }
        on_change_cb getListener(uint64_t key){
            auto it=m_cbs.find(key);
            return it==m_cbs.end()?nullptr:it->second;
        }
        void clearListender(){
            m_cbs.clear();
        }
    private:
        T m_val; // 配置变量的值
        //变更回调函数组，
        std::map<uint64_t,on_change_cb>m_cbs;//function没有比较函数，手动使用map给一个键进行比较，比如判断是否写入了两个相同的on_chaneg_cb
    };
}
#endif
