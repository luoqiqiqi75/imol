/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file sdk_interface.h
 * @brief sdk usage interface
 * @author LuoQi
 * @date 2021-06-01
 */

#ifndef XSERVICE_SDK_INTERFACE_H
#define XSERVICE_SDK_INTERFACE_H

#include "imol/common.h"

/**
 * @brief SDK: Software Development Kit
 */
namespace ix {
namespace sdk {

enum ErrorCode : int
{
    SUCCESS             = 0,
    PREFIX              = -0x0100,
    ERR_JSON_FORMAT     = -0x0101,
    ERR_NO_DATA_KEY     = -0x0102,
    ERR_NO_CMD_KEY      = -0x0103,
    ERR_INPUT_INVALID   = -0x0104,

    ERR_G_NO_KEY        = -0x0110,
    ERR_G_NON_READABLE  = -0x0111,
    ERR_S_NON_WRITABLE  = -0x0120,
    ERR_S_MERGE         = -0x0121,
    ERR_S_FAILED        = -0x0122,
    ERR_W_CONNECTION    = -0x0130,
    ERR_W_DUPLICATE     = -0x0131,
    ERR_C_NOT_FINISHED  = -0x0140
};

/**
 * Cip: Command Input Preprocessor
 */
typedef std::function<imol::Result(imol::CommandObject*, const Json::Value&)> Cip;
void registerCip(const std::string& cmd, const Cip& cip);
void unregisterCip(const std::string& cmd);
Cip getCip(const std::string& cmd);

namespace reg {

/**
 * @brief Custom Cip: only take care of deserialization and with the willing to return a result
 * @param IN const Json::Value& value: serialized data
 * @param OUT T& t: target data
 * @return imol::Result: result of deserialization
 */
template<typename T> using CustomCip = std::function<imol::Result(const Json::Value&, T& t)>;
template<typename F> inline imol::basic::FIfSame<F, CustomCip<typename imol::basic::FInfo<F>::ArgsT::SecondT>, void> cip(const std::string& cmd, F f)
{
    typedef typename std::remove_reference<typename imol::basic::FInfo<F>::ArgsT::SecondT>::type T;
    registerCip(cmd, [=] (imol::CommandObject* cobj, const Json::Value& value) -> imol::Result {
        T t;
        imol::Result res = f(value, t);
        if (!res.isError()) cobj->setInputData(t);
        return res;
    });
}

/**
 * @brief Simple Cip: just convert json to binary with a SUCCESS result
 * @param IN const Json::Value& value: serialized data
 * @return T: target data
 */
template<typename T> using SimpleCip = std::function<T(const Json::Value&)>;
template<typename F> inline imol::basic::FIfSame<F, SimpleCip<typename imol::basic::FInfo<F>::RetT>, void> cip(const std::string& cmd, F f)
{
    typedef typename imol::basic::FInfo<F>::RetT T;
    registerCip(cmd, [=] (imol::CommandObject* cobj, const Json::Value& value) -> imol::Result {
        cobj->setInputData<T>(f(value));
        return imol::Result::SUCCESS;
    });
}

/**
 * @brief Ref Cip: just convert json reference to binary with a SUCCESS result
 * @param IN const imol::JsonRef& value: serialized data reference
 * @return T: target data
 */
template<typename T> using RefCip = std::function<T(const imol::JsonRef&)>;
template<typename F> inline imol::basic::FIfSame<F, RefCip<typename imol::basic::FInfo<F>::RetT>, void> cip(const std::string& cmd, F f)
{
    typedef typename imol::basic::FInfo<F>::RetT T;
    registerCip(cmd, [=] (imol::CommandObject* cobj, const Json::Value& value) -> imol::Result {
        cobj->setInputData<T>(f(imol::JsonRef(value)));
        return imol::Result::SUCCESS;
    });
}

/**
 * @brief Deserializable Cip: build a Custom Cip by means of JsonInterface from given template class
 */
template<class C> inline typename std::enable_if<std::is_convertible<C, imol::JsonInterface>::value, void>::type cip(const std::string& cmd)
{
    cip(cmd, [] (const Json::Value& value, C& c) -> imol::Result { return c.deserialize(value); });
}

/**
 * @brief Convertable Cip: build a Custom Cip with types which are directly convertable from JSON
 */
template<typename T> inline typename std::enable_if<std::is_convertible<T, Json::Value>::value, void>::type cip(const std::string& cmd)
{
    cip(cmd, [] (const Json::Value& value, T& t) -> imol::Result {
        bool ok = false;
        t = imol::JsonRef(value).toT<T>(&ok);
        return ok;
    });
}

/**
 * @brief Void Cip: do nothing
 */
template<typename T> inline typename std::enable_if<std::is_same<T, void>::value, void>::type cip(const std::string& cmd) { return; }

inline void cip(const std::string& cmd, const Cip& f) { registerCip(cmd, f); }

}

}
}

#endif //XSERVICE_SDK_INTERFACE_H