#ifndef __ZX_CONFIG_CONFIG_H_CONFIGVARBASE_H__
#define __ZX_CONFIG_CONFIG_H_CONFIGVARBASE_H__
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h> // 包含yaml-cpp库，用于解析YAML文件
namespace zx{
    // 定义一个基类ConfigVarBase，用于所有的配置变量
    class ConfigVarBase{
    public:
        typedef std::shared_ptr<ConfigVarBase> ptr; // 定义智能指针类型
        // 构造函数，初始化名称和描述
        ConfigVarBase(const std::string &name, const std::string& description="") : m_name(name), m_description(description) {
            std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower); // 将名称转换为小写
        }
        virtual ~ConfigVarBase(){} // 虚析构函数
        // 获取配置变量的名称
        const std::string &getName() const { return m_name; }
        // 获取配置变量的描述
        const std::string &getDescription() const { return m_description; }
        // 纯虚函数，用于将配置变量转换为字符串
        virtual std::string toString() = 0;
        // 纯虚函数，用于从字符串解析配置变量
        virtual bool fromString(const std::string& val) = 0;
        virtual std::string getTypeName()const=0;
    protected:
        std::string m_name; // 配置变量的名称
        std::string m_description; // 配置变量的描述
    };
}
#endif