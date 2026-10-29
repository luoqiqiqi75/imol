/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file base.h
 * @brief imol base interface
 * @author LuoQi
 * @date 2021-05-24
 */

#ifndef IMOL_BASE_H
#define IMOL_BASE_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <functional>
#include <future>

#define IMOL_UNDEFINED_OBJECT_NAME "@undefined"

namespace imol {

// foundation
namespace basic {

template<class ...> using _void_t = void;
template<typename L, typename R, class = void> struct is_comparable : std::false_type {};
template<typename L, typename R> using _t_comparability = decltype(std::declval<L>() == std::declval<R>());
template<typename L, typename R> struct is_comparable<L,R,_void_t<_t_comparability<L,R>>> : std::true_type {};

template<typename T>
inline static typename std::enable_if<is_comparable<T,T>::value, bool>::type equals(const T& t1, const T& t2) { return t1 == t2; }
template<typename T>
inline static typename std::enable_if<!is_comparable<T,T>::value, bool>::type equals(const T& t1, const T& t2) { return false; }

template<typename...> struct TList { typedef void FirstT; };
template<typename T0, typename... T> struct TList<T0, T...> { typedef T0 FirstT; typedef TList<T...> RestT; };
template<typename T0, typename T1, typename... T> struct TList<T0, T1, T...> { typedef T0 FirstT; typedef T1 SecondT; typedef TList<T...> RestT; };

template<typename T> static decltype(&T::operator()) _t_functional(int);
template<typename T> static void _t_functional(short);

template<typename F> struct FInfo : public FInfo<decltype(_t_functional<F>(0))> {};
template<> struct FInfo<void> {};

template<typename Ret, typename... Args> struct FInfo<Ret (*) (Args...)>
{
    typedef Ret RetT;
    typedef TList<Args...> ArgsT;
    typedef Ret (*FptrT) (Args...);
    typedef Ret (*FunctionT) (Args...);
    enum { IsMember = false, ArgCnt = sizeof...(Args) };
};
template<typename Ret, typename Class, typename... Args> struct FInfo<Ret (Class::*) (Args...)>
{
    typedef Ret RetT;
    typedef Class ClassT;
    typedef TList<Args...> ArgsT;
    typedef Ret (*FptrT) (Args...);
    typedef Ret (Class::*FunctionT) (Args...);
    enum { IsMember = true, ArgCnt = sizeof...(Args) };
};
template<typename Ret, typename Class, typename... Args> struct FInfo<Ret (Class::*) (Args...) const>
{
    typedef Ret RetT;
    typedef Class ClassT;
    typedef TList<Args...> ArgsT;
    typedef Ret (*FptrT) (Args...);
    typedef Ret (Class::*FunctionT) (Args...) const;
    enum { IsMember = true, ArgCnt = sizeof...(Args) };
};

template<typename T> using _t_remove_rc = typename std::remove_const<typename std::remove_reference<T>::type>::type;

std::string _t_demangle(const char* type_name);
template<typename T> struct Meta
{
    typedef T TypeT;
    static const char* typeInfoName() { return typeid(T).name(); }
    static std::string typeName() { return _t_demangle(typeid(T).name()); }
};

template<typename Func1, typename Func2, typename Type>
using FIfSame = typename std::enable_if<std::is_same<typename FInfo<Func1>::FptrT, typename FInfo<Func2>::FptrT>::value, Type>::type;
template<typename Func1, typename Func2, typename Type>
using FIfConvertible = typename std::enable_if<std::is_convertible<Func1, Func2>::value
    && std::is_convertible<typename FInfo<Func1>::RetT, typename FInfo<Func2>::RetT>::value, Type>::type;

}

// type definitions
typedef std::function<void()> Task;
typedef std::function<bool()> Condition;

// container utils
#define IMOL_INHERIT_CONSTRUCTOR_IMPL(CONSTRUCTOR, CLASS, ...) \
using __VA_ARGS__::CONSTRUCTOR; \
CLASS() : __VA_ARGS__() {} \
CLASS(const __VA_ARGS__& other) noexcept : __VA_ARGS__(other) {} \
CLASS(__VA_ARGS__&& other) noexcept : __VA_ARGS__(std::move(other)) {}

#define P_IMOL_CONTAINER_T_INTERFACES_IMPL \
using ListLike = std::true_type; \
int sizeAsInt() const { return static_cast<int>(this->size()); } \
bool has(int index) const { return index >= 0 && index < sizeAsInt(); } \
T value(int index) const { return this->has(index) ? (*this)[index] : T(); } \
T value(int index, const T& default_value) const { return this->has(index) ? (*this)[index] : default_value; } \
std::string toString(const std::string& sep = ",") const \
{ \
    int s = this->size() - 1; \
    if (s < 0) return ""; \
    std::stringstream ss; \
    int i = 0; \
    for (const auto& it : *this) { \
        (i++ < s) ? (ss << it << sep) : (ss << it); \
    } \
    return ss.str(); \
}

template<typename T>
class Vector : public std::vector<T>
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(vector, Vector, std::vector<T>)
    P_IMOL_CONTAINER_T_INTERFACES_IMPL
};

template<typename T> class List : public std::list<T>
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(list, List, std::list<T>)
    P_IMOL_CONTAINER_T_INTERFACES_IMPL
};

#define P_IMOL_CONTAINER_KV_INTERFACES_IMPL \
using DictLike = std::true_type; \
int sizeAsInt() const { return static_cast<int>(this->size()); } \
bool has(const K& key) const { return this->find(key) != this->end(); } \
Vector<K> keys() const \
{ \
    Vector<K> vec; \
    vec.reserve(this->size()); \
    for (const auto& kv : *this) { \
        vec.push_back(kv.first); \
    } \
    return vec; \
} \
Vector<V> values() const \
{ \
    Vector<V> vec; \
    vec.reserve(this->size()); \
    for (const auto& kv : *this) { \
        vec.push_back(kv.second); \
    } \
    return vec; \
}

template<typename K, typename V>
class Map : public std::map<K, V>
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(map, Map, std::map<K, V>)
    P_IMOL_CONTAINER_KV_INTERFACES_IMPL

    Map(const Vector<K>& keys, const Vector<V>& values)
    {
        for (size_t i = 0; i < std::min(keys.size(), values.size()); ++i) {
            (*this)[keys[i]] = values[i];
        }
    }

    V value(const K& key) const
    {
        const auto it = this->find(key);
        return it == this->end() ? V() : it->second;
    }
    V value(const K& key, const V& default_value) const
    {
        const auto it = this->find(key);
        return it == this->end() ? default_value : it->second;
    }
};

template<typename K, typename V>
class HashMap : public std::unordered_map<K, V>
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(unordered_map, HashMap, std::unordered_map<K, V>)
    P_IMOL_CONTAINER_KV_INTERFACES_IMPL

    HashMap(const Vector<K>& keys, const Vector<V>& values)
    {
        for (size_t i = 0; i < std::min(keys.size(), values.size()); ++i) {
            (*this)[keys[i]] = values[i];
        }
    }

    V value(const K& key) const
    {
        const auto it = this->find(key);
        return it == this->end() ? V() : it->second;
    }
    V value(const K& key, const V& default_value) const
    {
        const auto it = this->find(key);
        return it == this->end() ? default_value : it->second;
    }
};

using Ints = Vector<int>;
using Doubles = Vector<double>;
using Strings = Vector<std::string>;

class Values : public Doubles
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(Doubles, Values, Doubles)

    enum Unit : int {
        NONE    = 0x0000,
        M       = 0x0100,
        MM      = 0x0101,
        DEGREE  = 0x0200,
        RAD     = 0x0201,
        SAME    = 0xffff
    };

    Unit unit() const;
    Values& setUnit(Unit unit);

    Values& add(double d);
    Values& multiply(double d, Unit new_unit = SAME);

    Values& mm2m();
    Values& m2mm();
    Values& degree2rad();
    Values& rad2degree();

    bool smallerThan(const Values& other) const;
    bool greaterThan(const Values& other) const;
    bool equals(const Values& other) const;

    bool operator<(const Values& other) const { return smallerThan(other); }
    bool operator>(const Values& other) const { return greaterThan(other); }
    bool operator==(const Values& other) const { return equals(other); }

private:
    Unit m_unit = NONE;
};

/**
 * @brief Object Class describes a minimum set of properties in order to keep controllable
 */
class Object
{
public:
    explicit Object(const std::string& name = "");
    virtual ~Object();

    Object* parent() const;
    void setParent(Object* obj);

    virtual std::string name() const;

    enum Signal : int { DELETE = 0x00ff };

    bool hasConnection(int signal, Object* observer);
    void connect(int signal, Object* observer, const Task& action);
    void disconnect(int signal, Object* observer);
    void disconnect(Object* observer);

    void trigger(int signal);

private:
    friend class Manager;

private:
    std::string m_name;
    Object* m_parent;
    HashMap<int, HashMap<Object*, List<Task>>> m_connections;
};

/**
 * @brief Manager Class is a convenient object container
 */
class Manager : public Object, public HashMap<std::string, Object*>
{
public:
    explicit Manager(const std::string& name);
    virtual ~Manager();

    Object* add(Object* obj, bool delete_if_failed = false);
    bool remove(Object* obj, bool auto_delete = true);
    bool remove(const std::string& name, bool auto_delete = true);

    Object* get(const std::string& key) const;
    template<class SubObj>
    SubObj* get(const std::string& key) const { return dynamic_cast<SubObj*>(get(key)); }

    void fixObjectLinks();
};

template<class Signature>
class Creator : public Object, public HashMap<std::string, std::function<Signature>>
{
public:
    explicit Creator(const std::string& name) : Object(name) {}
    virtual ~Creator() {}

    typedef basic::FInfo<std::function<Signature>> FInfoT;
    typedef typename FInfoT::RetT RetT;

    template<typename... Params>
    RetT exec(const std::string& key, Params... params) { return this->has(key) ? (*this)[key](params...) : (RetT)(NULL); }
    RetT exec(const std::string& key) { return this->has(key) ? (*this)[key]() : (RetT)(NULL); }
};

}

// macros
#define P_IMOL_AUTO_RUN_NAME(_PREFIX, _SUFFIX) P_IMOL_AUTO_RUN_NAME_I(_PREFIX, _SUFFIX)
#define P_IMOL_AUTO_RUN_NAME_I(_PREFIX, _SUFFIX) _PREFIX##_SUFFIX
#define P_IMOL_AUTO_RUN_FUNC P_IMOL_AUTO_RUN_NAME(_imol_auto_run_func_, __LINE__)
#define P_IMOL_AUTO_RUN_VAR P_IMOL_AUTO_RUN_NAME(_imol_auto_run_var_, __LINE__)
#define IMOL_AUTO_RUN(_CONTENT) namespace {int P_IMOL_AUTO_RUN_FUNC() { _CONTENT; return 0; } \
    const int __attribute__ ((unused)) P_IMOL_AUTO_RUN_VAR = P_IMOL_AUTO_RUN_FUNC(); /* NOLINT */}

// logging utils
std::ostream& operator<< (std::ostream& os, const imol::Object& obj);
std::ostream& operator<< (std::ostream& os, imol::Object* obj);

#define P_IMOL_LOG_CONTAINER_T_DECL(_CONTAINER) \
template<typename T> inline std::ostream& operator<< (std::ostream& os, const _CONTAINER<T>& container)
#define P_IMOL_LOG_CONTAINER_KV_DECL(_CONTAINER) \
template<typename K, typename V> inline std::ostream& operator<< (std::ostream& os, const _CONTAINER<K, V>& container)

#define P_IMOL_LOG_CONTAINER_IMPL(_LOG_BEGIN, _LOG_END, ...) \
os << _LOG_BEGIN; \
size_t cnt = 0; \
for (auto& it : container) { \
    os << __VA_ARGS__ ; \
    if (++cnt != container.size()) os << ", "; \
} \
os << _LOG_END; \
return os

P_IMOL_LOG_CONTAINER_T_DECL(std::vector) { P_IMOL_LOG_CONTAINER_IMPL("[", "]", it); }
P_IMOL_LOG_CONTAINER_T_DECL(std::list) { P_IMOL_LOG_CONTAINER_IMPL("(", ")", it); }
P_IMOL_LOG_CONTAINER_T_DECL(std::set) { P_IMOL_LOG_CONTAINER_IMPL("{", "}", it); }
P_IMOL_LOG_CONTAINER_KV_DECL(std::map)
    { P_IMOL_LOG_CONTAINER_IMPL("[", "]", it.first << " : " << it.second); }
P_IMOL_LOG_CONTAINER_KV_DECL(std::unordered_map)
    { P_IMOL_LOG_CONTAINER_IMPL("{", "}", it.first << " : " << it.second); }

template<typename T1, typename T2>
inline std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& pair)
{
    os << "<" << pair.first << ", " << pair.second << ">";
    return os;
}

#endif // IMOL_BASE_H