/**
 * @file singleton.h
 * @author 张璇
 * @emile xatuzx163.com
 * @iphone 15592075261
 * @date 2024-9-2
 * @brief 单例模式封装类
 */
#ifndef __ZX_SINGLETON_H__
#define __ZX_SINGLETON_H__
#include <memory>

namespace zx{
/**
 * @brief 单例模式
 * @details  T单例类型
 *          V单例标签
 *          N标签第多少个单例
 */
template<class T,class V=void ,int N=0>
class Singleton{
public:
    /**
    *@brief 返回单例裸指针 
    */
    static T* GetInstance(){
        static T v;
        return &v;
    }
};
/**
 * @brief 单例模式
 * @details  T单例类型
 *          V单例标签
 *          N标签第多少个单例
 */
template<class T,class V=void,int N=0>
class SingletonPtr{
public:
    /**
    *@brief 返回单例智能指针
    */
    static std::shared_ptr<T> GetInstance(){
        static std::shared_ptr<T>x(new T);
        return x;
    }
};

}

#endif