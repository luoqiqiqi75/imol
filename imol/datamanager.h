/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file datamanager.h
 * @brief global data storage
 * @author LuoQi
 * @date 2021-04-23
 */

#ifndef IMOL_DATA_MANAGER_H
#define IMOL_DATA_MANAGER_H

#include <json/json.h>

#include "base.h"

namespace imol {

/**
 * @brief TemplateInterface is a template class container for any data type
 *
 * @tparam T
 */
template<typename T>
class TemplateInterface
{
public:
    TemplateInterface() {}
    TemplateInterface(const T& t) : m_data(t) {}
    TemplateInterface(T&& t) : m_data(std::move(t)) {}

    typedef typename basic::Meta<T> MetaT;

    T& ref() { return m_data; }
    T* ptr() { return &m_data; }

    const T& get() const { return m_data; }
    void set(const T& t) { m_data = t; }
    void set(T&& t) { m_data = std::move(t); }

protected:
    T m_data;
};

template<>
class TemplateInterface<void>
{
public:
    TemplateInterface() {}

    typedef typename basic::Meta<void> MetaT;
};

class Controllable
{
public:
    bool isReadable() const;
    bool isWritable() const;

protected:
    void setReadable(bool readable);
    void setWritable(bool writable);

private:
    bool m_readable = true;
    bool m_writable = true;
};

/**
 * @brief JsonRef is a readonly json viewer, avoiding crash
 */
class JsonRef
{
public:
    JsonRef(const Json::Value& value);
    JsonRef(Json::Value&& value);
    virtual ~JsonRef();

    const Json::Value& value() const;

    explicit operator bool() const;

    size_t size() const;
    int arraySize() const;
    int objectSize() const;

    Strings objectKeys() const;

    bool has(int index) const;
    bool has(const std::string& key) const;

    JsonRef at(int index, bool* found = nullptr) const;
    JsonRef at(const std::string& key, bool use_index = false, bool* found = nullptr) const;
    JsonRef get(const std::string& path, bool* found = nullptr) const;

    JsonRef operator[](int index) const;
    JsonRef operator[](const std::string& key) const;

    bool toBool(bool default_bool = false, bool* ok = nullptr) const;
    int toInt(int default_int = 0, bool* ok = nullptr) const;
    double toDouble(double default_double = .0f, bool* ok = nullptr) const;
    std::string toString(const std::string& default_string = "", bool* ok = nullptr) const;

    Values toValues() const;

    template<typename T> T toT(bool* ok = nullptr) const;
    template<typename T> T toT(T& t, bool* ok = nullptr) const { return t = toT<T>(ok); }

    double operator()(int index) const;
    double operator()(const std::string& key) const;

private:
    TemplateInterface<Json::Value>* m_holder;
    const Json::Value* m_ptr;
};

/**
 * \brief JsonInterface
 **/
class JsonInterface
{
public:
    virtual Json::Value serialize() const;
    virtual bool deserialize(const JsonRef& jr);
};

/**
 * \brief DataObject
 **/
class DataObject : public Object, public Controllable
{
public:
    enum Signal { CHANGED = 0x0200 };

public:
    DataObject(const std::string& name = "");
    virtual ~DataObject();

    virtual std::string dataTypeName() const;
};

template<typename T>
class TDataObject : public DataObject, public TemplateInterface<T>
{
public:
    TDataObject(const std::string& name = "") : DataObject(name) {}
    TDataObject(const std::string& name, const T& t) : DataObject(name), TemplateInterface<T>(t) {}
    TDataObject(const std::string& name, T&& t) : DataObject(name), TemplateInterface<T>(std::move(t)) {}

    std::string dataTypeName() const override { return TemplateInterface<T>::MetaT::typeName(); }

    TDataObject<T>* readonly() { DataObject::setWritable(false); return this; }

    void update() { trigger(CHANGED); }

    void update(const T& t)
    {
        this->set(t);
        this->update();
    }

    void update(T&& t)
    {
        this->set(std::move(t));
        this->update();
    }

    void updateIfDifferent(const T& t)
    {
        if (!basic::equals(this->get(), t)) update(t);
    }

    void link(T* t_ptr)
    {
        this->set(*t_ptr);
        connect(CHANGED, this, [=] { *t_ptr = this->get(); });
    }
};

template<>
class TDataObject<void> : public DataObject, public TemplateInterface<void>
{
public:
    TDataObject(const std::string& name = "") : DataObject(name) {}
    std::string dataTypeName() const override { return TemplateInterface<void>::MetaT::typeName(); }
    void update() { trigger(CHANGED); }
};

template<typename T>
class BasicDataObject : public TDataObject<T>, public JsonInterface
{ public: using TDataObject<T>::TDataObject; };

template<typename T>
class CustomDataObject : public BasicDataObject<T>
{
public:
    typedef std::function<Json::Value(const T&)> Serializer;
    typedef std::function<bool(const imol::JsonRef&, T&)> Deserializer;

public:
    CustomDataObject(const std::string& name, const Serializer& s, const Deserializer& d) : BasicDataObject<T>(name), m_s(s), m_d(d) {}

    Serializer serializer() const { return m_s; }
    Deserializer deserializer() const { return m_d; }

    Json::Value serialize() const override { return m_s(this->get()); }
    bool deserialize(const imol::JsonRef& jr) override { return m_d ? m_d(jr, this->ref()) : false; }

private:
    Serializer m_s;
    Deserializer m_d;
};

template<>
class CustomDataObject<void> : public BasicDataObject<void>
{
public:
    typedef std::function<Json::Value()> Serializer;
    typedef std::function<bool(const imol::JsonRef&)> Deserializer;

public:
    CustomDataObject(const std::string& name, const Serializer& s, const Deserializer& d) : BasicDataObject(name), m_s(s), m_d(d) {}

    Serializer serializer() const { return m_s; }
    Deserializer deserializer() const { return m_d; }

    Json::Value serialize() const override { return m_s(); }
    bool deserialize(const imol::JsonRef& jr) override { return m_d ? m_d(jr) : false; }

private:
    Serializer m_s;
    Deserializer m_d;
};

namespace data {
Manager& mgr();
DataObject* get(const std::string& name);

template<typename T> inline TemplateInterface<T>* ti(DataObject* dobj) { return dynamic_cast<TemplateInterface<T>*>(dobj); }
template<typename T> inline TemplateInterface<T>* ti(const std::string& name) { return ti<T>(get(name)); }

inline JsonInterface* ji(DataObject* dobj) { return dynamic_cast<JsonInterface*>(dobj); }
inline JsonInterface* ji(const std::string& name) { return ji(get(name)); }

enum DataType
{
    DATA_NORMAL,
    DATA_BASIC,
    DATA_SERIALIZABLE,
    DATA_DOBJ,
    DATA_DOBJ_PTR
};

enum DataCreateType
{
    DATA_CREATE_NEW,
    DATA_CREATE_COPY,
    DATA_CREATE_MOVE
};

template<typename T, class = void>
struct Checker
{
    typedef TDataObject<T> DobjT;
    enum { Type = DATA_NORMAL, CreateType = DATA_CREATE_NEW };
};

template<typename T>
struct Checker<T, typename std::enable_if<std::is_convertible<T, DataObject*>::value, void>::type>
{
    typedef std::remove_pointer<T> DobjT;
    enum { Type = DATA_DOBJ_PTR, CreateType = DATA_CREATE_MOVE };
};
template<typename T>
struct Checker<T, typename std::enable_if<std::is_base_of<DataObject, T>::value, void>::type>
{
    typedef T DobjT;
    enum { Type = DATA_DOBJ, CreateType = DATA_CREATE_COPY };
};
template<typename T>
struct Checker<T, typename std::enable_if<std::is_base_of<JsonInterface, T>::value
        && !std::is_base_of<DataObject, T>::value, void>::type>
{
    class SerializableDobj : public BasicDataObject<T>
    {
    public:
        using BasicDataObject<T>::BasicDataObject;
        Json::Value serialize() const { return this->get().serialize(); }
        bool deserialize(const JsonRef& jr) { return this->ref().deserialize(jr); }
    };

    typedef SerializableDobj DobjT;
    enum { Type = DATA_SERIALIZABLE, CreateType = DATA_CREATE_NEW };
};
template<typename T>
struct Checker<T, typename std::enable_if<!std::is_pointer<T>::value
        && std::is_convertible<T, Json::Value>::value, void>::type>
{
    class ConvertibleDobj : public BasicDataObject<T>
    {
    public:
        using BasicDataObject<T>::BasicDataObject;
        Json::Value serialize() const { return this->get(); }
        bool deserialize(const JsonRef& jr) { bool ok; this->set(jr.toT<T>(&ok)); return ok; }
    };

    typedef ConvertibleDobj DobjT;
    enum { Type = DATA_BASIC, CreateType = DATA_CREATE_NEW };
};
template<typename T>
struct Checker<T, typename std::enable_if<T::ListLike::value
        && std::is_convertible<typename T::value_type, Json::Value>::value, void>::type>
{
    class ListDobj : public BasicDataObject<T>
    {
    public:
        using BasicDataObject<T>::BasicDataObject;
        Json::Value serialize() const
        {
            Json::Value value;
            value.resize(this->get().size());
            int cnt = 0;
            for (const auto& it : this->get()) {
                value[cnt++] = it;
            }
            return value;
        }
        bool deserialize(const JsonRef& jr)
        {
            int len = jr.arraySize();
            if (len < 0) return false;
            int cnt = 0;
            bool ok = false;
            this->ref().resize(len);
            for (auto& it : this->ref()) {
                jr.at(cnt++).toT(it, &ok);
                if (!ok) return false;
            }
            return true;
        }
    };

    typedef ListDobj DobjT;
    enum { Type = DATA_BASIC, CreateType = DATA_CREATE_NEW };
};
template<typename T>
struct Checker<T, typename std::enable_if<T::DictLike::value
        && std::is_base_of<std::string, typename T::key_type>::value
        && std::is_convertible<typename T::value_type::second_type, Json::Value>::value, void>::type>
{
    class DictDobj : public BasicDataObject<T>
    {
    public:
        using BasicDataObject<T>::BasicDataObject;
        Json::Value serialize() const
        {
            Json::Value value;
            for (const auto& kv : this->get()) {
                value[kv.first] = kv.second;
            }
            return value;
        }
        bool deserialize(const JsonRef& jr)
        {
            bool ok = false;
            for (auto& kv : this->ref()) {
                jr.at(kv.first).toT(kv.second, &ok);
                if (!ok) return false;
            }
            return true;
        }
    };

    typedef DictDobj DobjT;
    enum { Type = DATA_BASIC, CreateType = DATA_CREATE_NEW };
};

template<typename T, DataCreateType DCT>
using DobjPtrT = typename std::enable_if<Checker<T>::CreateType == DCT, typename Checker<T>::DobjT*>::type;

template<typename T> inline DobjPtrT<T, DATA_CREATE_NEW> from(T t, const std::string& name = "") { return new typename Checker<T>::DobjT(name, std::forward<T>(t)); }
template<typename T> inline DobjPtrT<T, DATA_CREATE_COPY> from(const T& t) { return new typename Checker<T>::DobjT(t); }
template<typename T> inline DobjPtrT<T, DATA_CREATE_MOVE> from(T t) { return t; }

template<typename T>
inline typename std::enable_if<std::is_base_of<BasicDataObject<T>, typename Checker<T>::DobjT>::value, BasicDataObject<T>*>::type create(const std::string& name, const T& t = T())
{ return dynamic_cast<BasicDataObject<T>*>(mgr().add(from(t, name), true)); }

template<typename SerializerF, typename T = basic::_t_remove_rc<typename basic::FInfo<SerializerF>::ArgsT::FirstT>>
inline basic::FIfSame<SerializerF, typename CustomDataObject<T>::Serializer, BasicDataObject<T>*> create(const std::string& name, SerializerF s)
{ return dynamic_cast<BasicDataObject<T>*>(mgr().add(new CustomDataObject<T>(name, s, NULL), true)); }

template<typename SerializerF, typename DeserializerF, typename T = basic::_t_remove_rc<typename basic::FInfo<SerializerF>::ArgsT::FirstT>>
inline basic::FIfSame<DeserializerF, typename CustomDataObject<T>::Deserializer, BasicDataObject<T>*> create(const std::string& name, SerializerF s, DeserializerF d)
{ return dynamic_cast<BasicDataObject<T>*>(mgr().add(new CustomDataObject<T>(name, s, d), true)); }

}

DataObject* d(const std::string& name);
template<typename T> TDataObject<T>* d(const std::string& name) { return dynamic_cast<TDataObject<T>*>(d(name)); }

#define P_IMOL_D_QUICK_IMPL(...) auto dobj = d<T>(name); if (dobj) __VA_ARGS__; return dobj
template<typename T> TDataObject<T>* d_get(const std::string& name, T& t) { P_IMOL_D_QUICK_IMPL(t = dobj->get()); }
template<typename T> TDataObject<T>* d_set(const std::string& name, const T& t) { P_IMOL_D_QUICK_IMPL(dobj->set(t)); }
template<typename T> TDataObject<T>* d_update(const std::string& name, const T& t) { P_IMOL_D_QUICK_IMPL(dobj->update(t)); }

}

#endif //IMOL_DATA_MANAGER_H