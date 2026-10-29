//
// Created by LuoQi on 2021/12/22.
//

#ifndef XCORE_XMODULE_H
#define XCORE_XMODULE_H

#include "module.h"
#include "xrobot.h"

#include "xservice/sdk/sdk_interface.h"

namespace ix {

/// 数据类
template<typename T> using Data = imol::TDataObject<T>;

/// 指令类
using Cmd = imol::CommandObject;

/**
 * @brief 基础整合x系列支持的模块类
 */
class Module : public imol::Module
{
public:
    explicit Module();
    virtual ~Module();

    /**
     * @brief 获取全局xModule实例
     * @param module_name 模块名称
     * @return 模块指针
     */
    static Module* instanceOf(const std::string& module_name);

    /**
     * @brief 获取类相关数据类指针
     * @tparam T 数据类型
     * @param sub_key 数据名称
     * @return 数据类指针
     */
    template<typename T> Data<T>* data(const std::string& sub_key) { return dynamic_cast<Data<T>*>(imol::d<T>(name(sub_key))); }

    /**
     * @brief 连接数据类，接管数据类指针，自动加载表示有Json接口的数据类开机自动反序列化，自动保存表示主动触发数据类CHANGED信号时进行保存
     * @note 不可重复连接
     * @tparam T 数据类型
     * @param tdobj 数据类指针
     * @param auto_save 是否自动保存
     * @param auto_load 是否自动加载
     * @return 数据类指针
     */
    template<typename T> Data<typename T::MetaT::TypeT>* bindData(T* tdobj, bool auto_save = false, bool auto_load = false)
    {
        adoptObj(tdobj);
        if (auto ji = imol::data::ji(tdobj)) {
            std::string dobj_path = ic::pathOf(this, tdobj, "json");
            if (auto_load) {
                Json::Value cache = ic::readJson(dobj_path);
                if (cache.empty()) {
                    // generate param
                } else {
                    // cache must be correct
                    ji->deserialize(cache);
                }
            }
            if (auto_save) {
                tdobj->connect(imol::DataObject::CHANGED, this, [=] {
                    ic::writeJson(dobj_path, ji->serialize());
                });
            }
        }
        return tdobj;
    }

    /**
     * @brief 创建并连接数据类，使用模块内数据名，以及初始值，其它形式数据类自行创建使用另一种连接方式
     * @tparam T 数据类型
     * @param sub_key 模块内数据名
     * @param init_value 初始值，类型与T相同
     * @param auto_save 是否自动保存
     * @param auto_load 是否自动上电
     * @return 数据类指针
     */
    template<typename T> Data<T>* bindData(const std::string& sub_key, const T& init_value, bool auto_save = false, bool auto_load = false)
    {
        auto tdobj = imol::data::from(init_value, name(sub_key));
        imol::data::mgr().add(tdobj, false); // force add
        return bindData(tdobj, auto_save, auto_load);
    }

    /**
     * @brief 注册指令，具体用法参考imol
     * @tparam F 指令参数类型
     * @param sub_key 模块内指令名
     * @param f 指令参数
     * @return 指令类指针
     */
    template<typename ...F> Cmd* registerCmd(const std::string& sub_key, F... f) { return ic::reg::cmd(this, name(sub_key), f...); }

    /**
     * @brief 注册指令相应预处理，具体用法参考xservice之sdk
     * @tparam F 预处理参数类型
     * @param sub_key 模块内\b指令名
     * @param f 预处理参数
     */
    template<typename ...F> void registerCip(const std::string& sub_key, F... f) { return sdk::reg::cip(name(sub_key), f...); }

    /**
     * @brief 注册函数形式指令，自动分析预处理函数，属于常用指令形式
     * @tparam F 指令函数类型
     * @param sub_key 模块内指令名
     * @param f_cmd 指令函数
     * @return 指令类指针
     */
    template<typename F> Cmd* registerFuncCmd(const std::string& sub_key, F f_cmd)
    {
        sdk::reg::cip<imol::basic::_t_remove_rc<ic::Proc::FuncIn<F>>>(name(sub_key));
        return ic::reg::cmd(this, name(sub_key), f_cmd);
    }

    /**
     * @brief 注册函数形式指令，手动指定预处理函数，属于常用指令形式
     * @tparam F1 预处理函数类型
     * @tparam F2 指令函数类型
     * @param sub_key 模块内\b指令名
     * @param f_cip 预处理函数
     * @param f_cmd 指令函数
     * @return 指令类指针
     */
    template<typename F1, typename F2> Cmd* registerFuncCmd(const std::string& sub_key, F1 f_cip, F2 f_cmd)
    {
        sdk::reg::cip(name(sub_key), f_cip);
        return ic::reg::cmd(this, name(sub_key), f_cmd);
    }
};


}

#endif //XCORE_XMODULE_H
