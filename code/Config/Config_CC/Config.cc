// 包含Config类的头文件
#include "../Config_H/Config.h"

namespace zx {
    // Config::LookupBase函数用于在配置数据集中查找给定名称的配置项。
    // 参数name是要查找的配置项的名称。
    // 返回值是一个指向ConfigVarBase类型（可能是配置变量的基类）的智能指针。
    ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
        // 使用名称作为键在配置数据集中查找配置项。
        auto it = GetDatas().find(name);
        // 如果找到了，返回对应的智能指针，否则返回nullptr。
        return it == GetDatas().end() ? nullptr : it->second;
    }

    // ListAllMember函数递归地列出YAML节点中的所有成员，并将它们存储在输出列表中。
    // 参数prefix是当前节点的前缀，用于构建完整的配置键名。
    // 参数node是当前处理的YAML节点。
    // 参数output是存储结果的列表，其中每个元素是一个键值对，键是配置项的名称，值是对应的YAML节点。
    static void ListAllMember(const std::string& prefix, const YAML::Node& node, std::list<std::pair<std::string, const YAML::Node>>& output) {
        // 检查前缀是否只包含有效的配置键名字符。
        if (prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos) {
            // 如果前缀无效，记录错误并返回。
            ZX_LOG_ERROR(ZX_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
            return;
        }
        // 将当前节点添加到输出列表中。
        output.push_back(std::make_pair(prefix, node));
        // 如果当前节点是一个映射（类似于字典或对象），则递归地处理它的每个成员。
        if (node.IsMap()) {
            for (auto it = node.begin(); it != node.end(); it++) {
                // 构建子成员的键名，如果当前前缀为空，则使用成员的键名，否则在成员键名前加上前缀和点号。
                ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
            }
        }
    }

    // Config::LoadFromYaml函数用于从YAML节点加载配置。
    // 参数root是YAML文档的根节点。
    void Config::LoadFromYaml(const YAML::Node& root) {
        // 创建一个列表来存储所有节点的键值对。
        std::list<std::pair<std::string, const YAML::Node>> all_nodes;
        // 调用ListAllMember函数填充all_nodes列表。
        ListAllMember("", root, all_nodes);
        // 遍历所有节点。
        for (auto i : all_nodes) {
            // 获取当前节点的键名。
            std::string key = i.first;
            // 如果键名为空，则跳过当前循环。
            if (key.empty()) {
                continue;
            }
            // 将键名转换为小写。
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            // 使用小写的键名查找对应的配置变量。
            ConfigVarBase::ptr var = LookupBase(key);
            // 如果找到了对应的配置变量，并且当前节点是一个标量（单个值）。
            if (var) {
                if (i.second.IsScalar()) {
                    // 将YAML节点的值转换为字符串，并使用配置变量的fromString方法进行处理。
                    var->fromString(i.second.Scalar());
                } else {
                    // 如果当前节点不是一个标量，将其转换为字符串流，然后使用配置变量的fromString方法进行处理。
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
    }
}