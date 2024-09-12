#ifndef __ZX_CONFIG_CONFIG_H_BASICFROMSTRING_H__
#define __ZX_CONFIG_CONFIG_H_BASICFROMSTRING_H__
#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
namespace zx{
    //F->T
    template<class F,class T>
    class LexicalCast{
        public:
        T operator()(const F&v){
            return boost::lexical_cast<T>(v);
        }
    };
    template<class T>
    class LexicalCast<std::string,std::vector<T>>{
        public:
        std::vector<T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
            std::vector<T>vec;
            std::stringstream ss;
            for(size_t i=0;i<node.size();++i){
                ss.str("");
                ss<<node[i];
                vec.push_back(LexicalCast<std::string,T>()(ss.str()));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::vector<T>,std::string>{
        public:
        std::string operator()(const std::vector<T>&v){
            std::stringstream ss;
            for(auto &i:v){
                ss<<LexicalCast<T,std::string>()(i);
            }
            return ss.str();
        }
    };
    template<class T>
    class LexicalCast<std::string,std::list<T>>{
        public:
        std::list<T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
            std::list<T>vec;
            std::stringstream ss;
            for(size_t i=0;i<node.size();++i){
                ss.str("");
                ss<<node[i];
                vec.push_back(LexicalCast<std::string,T>()(ss.str()));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::list<T>,std::string>{
        public:
        std::string operator()(const std::list<T>&v){
            std::stringstream ss;
            for(auto &i:v){
                ss<<LexicalCast<T,std::string>()(i);
            }
            return ss.str();
        }
    };
     template<class T>
    class LexicalCast<std::string,std::set<T>>{
        public:
        std::set<T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
            std::set<T>vec;
            std::stringstream ss;
            for(size_t i=0;i<node.size();++i){
                ss.str("");
                ss<<node[i];
                vec.insert(LexicalCast<std::string,T>()(ss.str()));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::set<T>,std::string>{
        public:
        std::string operator()(const std::set<T>&v){
            std::stringstream ss;
            for(auto &i:v){
                ss<<LexicalCast<T,std::string>()(i);
            }
            return ss.str();
        }
    };
    template<class T>
    class LexicalCast<std::string,std::unordered_set<T>>{
        public:
        std::unordered_set<T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
            std::unordered_set<T>vec;
            std::stringstream ss;
            for(size_t i=0;i<node.size();++i){
                ss.str("");
                ss<<node[i];
                vec.insert(LexicalCast<std::string,T>()(ss.str()));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::unordered_set<T>,std::string>{
        public:
        std::string operator()(const std::unordered_set<T>&v){
            std::stringstream ss;
            for(auto &i:v){
                ss<<LexicalCast<T,std::string>()(i);
            }
            return ss.str();
        }
    };
    template<class T>
    class LexicalCast<std::string,std::map<std::string,T>>{
        public:
        std::map<std::string,T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
           std::map<std::string,T>vec;
            std::stringstream ss;
            for(auto it=node.begin();it!=node.end();++it){
                ss.str("");
                ss<<it->second;
                vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::map<std::string,T>,std::string>{
        public:
        std::string operator()(const std::map<std::string,T>&v){
            YAML::Node node;
            for(auto &i:v){
                node[i.first]=YAML::Load(LexicalCast<T,std::string>()(i.second));
            }
            std::stringstream ss;
            ss<<node;
            return ss.str();
        }
    };
    template<class T>
    class LexicalCast<std::string,std::unordered_map<std::string,T>>{
        public:
        std::unordered_map<std::string,T> operator()(const std::string&v){
            YAML::Node node=YAML::Load(v);
           std::unordered_map<std::string,T>vec;
            std::stringstream ss;
            for(auto it=node.begin();it!=node.end();++it){
                ss.str("");
                ss<<it->second;
                vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
            }
            return vec;
        }
    };
    template<class T>
    class LexicalCast<std::unordered_map<std::string,T>,std::string>{
        public:
        std::string operator()(const std::unordered_map<std::string,T>&v){
            YAML::Node node;
            for(auto &i:v){
                node[i.first]=YAML::Load(LexicalCast<T,std::string>()(i.second));
            }
            std::stringstream ss;
            ss<<node;
            return ss.str();
        }
    };

}
#endif