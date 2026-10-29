#include "../datamanager.h"

#include "../common.h"

namespace imol {

bool Controllable::isReadable() const { return m_readable; }
bool Controllable::isWritable() const { return m_writable; }
void Controllable::setReadable(bool readable) { m_readable = readable; }
void Controllable::setWritable(bool writable) { m_writable = writable; }

Json::Value g_empty_value;

JsonRef::JsonRef(const Json::Value& value) : m_holder(nullptr), m_ptr(&value) {}
JsonRef::JsonRef(Json::Value&& value) : m_holder(new TemplateInterface<Json::Value>(std::move(value))), m_ptr(&m_holder->ref()) {}
JsonRef::~JsonRef() { delete m_holder; }

const Json::Value& JsonRef::value() const { return *m_ptr; }

JsonRef::operator bool() const { return m_ptr != &g_empty_value; }

size_t JsonRef::size() const { return m_ptr->size(); }
int JsonRef::arraySize() const { return m_ptr->isArray() ? static_cast<int>(m_ptr->size()) : -1; }
int JsonRef::objectSize() const { return m_ptr->isObject() ? static_cast<int>(m_ptr->size()) : -1; }

Strings JsonRef::objectKeys() const { return m_ptr->isObject() ? m_ptr->getMemberNames() : Strings(); }

bool JsonRef::has(int index) const { return index < arraySize() && index >= 0; }
bool JsonRef::has(const std::string &key) const { return m_ptr->isObject() && m_ptr->isMember(key); }

JsonRef JsonRef::at(int index, bool* found) const
{
    if (!has(index)) {
        if (found) *found = false;
        return g_empty_value;
    }
    if (found) *found = true;
    return (*m_ptr)[index];
}

JsonRef JsonRef::at(const std::string &key, bool use_index, bool* found) const
{
    if (use_index && key.length() > 1 && key[0] == '#') {
        return at(atoi(key.c_str() + sizeof(char)), found);
    }
    if (!has(key)) {
        if (found) *found = false;
        return g_empty_value;
    }
    if (found) *found = true;
    return (*m_ptr)[key];
}

JsonRef JsonRef::get(const std::string &path, bool* found) const
{
    if (m_ptr->isNull()) {
        if (found) *found = false;
        return g_empty_value;
    }
    int len = path.find('.');
    if (len < 0) {
        return at(path, true, found);
    } else if (m_ptr->isObject()) {
        return at(path.substr(0, len), true, found).get(path.substr(len + 1, path.size() - len - 1), found);
    }
    if (found) *found = false;
    return g_empty_value;
}

JsonRef JsonRef::operator[](int index) const { return at(index); }
JsonRef JsonRef::operator[](const std::string &key) const { return get(key); }

#define _F_to(JSON_TYPE, JSON_FUNC, TYPE) \
if (!m_ptr->isNull() && m_ptr->isConvertibleTo(Json::JSON_TYPE##Value)) { \
    if (ok) *ok = true; \
    return m_ptr->as##JSON_FUNC(); \
} \
if (ok) *ok = false; \
return default_##TYPE \

bool JsonRef::toBool(bool default_bool, bool *ok) const { _F_to(boolean, Bool, bool); }
int JsonRef::toInt(int default_int, bool *ok) const { _F_to(int, Int, int); }
double JsonRef::toDouble(double default_double, bool *ok) const { _F_to(real, Double, double); }
std::string JsonRef::toString(const std::string &default_string, bool *ok) const { _F_to(string, String, string); }

Values JsonRef::toValues() const
{
    Values values(arraySize());
    for (int i = 0; i < arraySize(); ++i) {
        values[i] = at(i).toDouble();
    }
    return values;
}

template<> bool JsonRef::toT(bool *ok) const { return toBool(false, ok); }
template<> int JsonRef::toT(bool *ok) const { return toInt(0, ok); }
template<> float JsonRef::toT(bool *ok) const { return toDouble(0, ok); }
template<> double JsonRef::toT(bool *ok) const { return toDouble(0, ok); }
template<> std::string JsonRef::toT(bool *ok) const { return toString("", ok); }
template<> Values JsonRef::toT(bool *ok) const { return toValues(); }
template<> Json::Value JsonRef::toT(bool *ok) const { return value(); }

double JsonRef::operator()(int index) const { return at(index).toDouble(); }
double JsonRef::operator()(const std::string &key) const { return get(key).toDouble(); }

Json::Value JsonInterface::serialize() const { return Json::Value(); }
bool JsonInterface::deserialize(const JsonRef&) { return false; }

DataObject::DataObject(const std::string& name) : Object(name) {}
DataObject::~DataObject() {}
std::string DataObject::dataTypeName() const { return IMOL_UNDEFINED_OBJECT_NAME; }

namespace data {

Manager& mgr()
{
    static Manager m("imol_data_mgr");
    return m;
}

DataObject* get(const std::string& name)
{
    return mgr().get<DataObject>(name);
}

}

DataObject* d(const std::string& name)
{
    return data::get(name);
}

}