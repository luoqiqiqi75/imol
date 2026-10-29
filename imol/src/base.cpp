#include "../base.h"

#include <boost/core/demangle.hpp>
#include <kdl/utilities/utility.h>

constexpr double eps = 0.000001;

namespace imol {

namespace basic {
std::string _t_demangle(const char *type_name) { return boost::core::demangle(type_name); }
}

Values::Unit Values::unit() const { return m_unit; }
Values& Values::setUnit(Unit unit) { if (m_unit != SAME) m_unit = unit; return *this; }

Values& Values::add(double d)
{
    std::for_each(begin(), end(), [=] (double& it) { it += d; });
    return *this;
}

Values& Values::multiply(double d, Values::Unit new_unit)
{
    std::for_each(begin(), end(), [=] (double& it) { it *= d; });
    return setUnit(new_unit);
}

Values& Values::m2mm() { return multiply(1000.0, MM); }
Values& Values::mm2m() { return multiply(0.001, M); }
Values& Values::degree2rad() { return multiply(KDL::deg2rad, RAD); }
Values& Values::rad2degree() { return multiply(KDL::rad2deg, DEGREE); }

bool Values::smallerThan(const Values& other) const
{
    for (int i = 0; i < std::min(sizeAsInt(), other.sizeAsInt()); i++) {
        if (at(i) >= other.at(i)) return false;
    }
    return true;
}

bool Values::greaterThan(const Values &other) const
{
    for (int i = 0; i < std::min(sizeAsInt(), other.sizeAsInt()); i++) {
        if (at(i) <= other.at(i)) return false;
    }
    return true;
}

bool Values::equals(const Values& other) const
{
    int s = sizeAsInt();
    if (s != other.sizeAsInt()) return false;
    for (int i = 0; i < s; i++) {
        if (fabs(at(i) - other.at(i)) > eps) return false;
    }
    return true;
}

Object::Object(const std::string& name) : m_name(name.empty() ? IMOL_UNDEFINED_OBJECT_NAME : name), m_parent(nullptr)
{
}

Object::~Object()
{
    trigger(DELETE);

    if (auto mgr = dynamic_cast<Manager*>(parent())) mgr->remove(this, false);
}

Object* Object::parent() const { return m_parent; }
void Object::setParent(Object *obj) { m_parent = obj; }

std::string Object::name() const { return m_name; }

bool Object::hasConnection(int signal, Object* observer)
{
    return m_connections.has(signal) && m_connections[signal].has(observer);
}

void Object::connect(int signal, Object* observer, const Task& action)
{
    if (!m_connections.has(signal)) {
        m_connections[signal] = {{observer, {action}}};
    } else if (!m_connections[signal].has(observer)) {
        m_connections[signal][observer] = {action};
    } else {
        m_connections[signal][observer].push_back(action);
    }
    if (observer && observer != this && signal != DELETE) {
        observer->connect(DELETE, this, [=] { disconnect(observer); });
        this->connect(DELETE, observer, [=] { observer->disconnect(this); });
    }
}

void Object::disconnect(int signal, Object* observer)
{
    if (m_connections.has(signal)) m_connections[signal].erase(observer);
}

void Object::disconnect(Object *observer)
{
    for (auto& kv : m_connections) {
        kv.second.erase(observer);
    }
}

void Object::trigger(int signal)
{
    if (!m_connections.has(signal)) return;
    auto hashmap = m_connections[signal]; // copy needed
    for (auto& kv : hashmap) {
        if (auto obj = kv.first) {
            // todo performance control
            (void)obj;
        }
        for (auto& it : kv.second) {
            it();
        }
    }
}

Manager::Manager(const std::string &name) : Object(name)
{
}

Manager::~Manager()
{
    for (auto& kv : *this) {
        Object* obj = kv.second;
        obj->setParent(nullptr);
        delete obj;
    }
}

Object* Manager::add(Object* obj, bool delete_if_failed)
{
    if (!obj) return nullptr;
    if (has(obj->name())) {
        if (delete_if_failed) delete obj;
        return nullptr;
    }
    obj->setParent(this);
    (*this)[obj->name()] = obj;
    return obj;
}

bool Manager::remove(Object *obj, bool auto_delete)
{
    if (!obj) return false;
    bool find = erase(obj->name()) > 0;
    if (!find) {
        for (const auto& kv : *this) {
            if (obj == kv.second) {
                find = erase(kv.first) > 0;
                break;
            }
        }
    }
    if (find) {
        obj->setParent(nullptr);
        if (auto_delete) delete obj;
    }
    return false;
}

bool Manager::remove(const std::string &name, bool auto_delete)
{
    Object* obj = get(name);
    if (!obj || erase(name) == 0) return false;
    if (auto_delete) delete obj;
    return true;
}

Object* Manager::get(const std::string &key) const
{
    auto it = find(key);
    return it == end() ? nullptr : it->second;
}

void Manager::fixObjectLinks()
{
    for (auto& kv : *this) {
        kv.second->m_name = kv.first;
        kv.second->setParent(this);
    }
}

}

using namespace imol;

std::ostream& operator<< (std::ostream& os, const Object& obj)
{
    return os << obj.name();
}

std::ostream& operator<< (std::ostream& os, Object* obj)
{
    return os << (void*)(obj) << "(" << (obj ? obj->name() : IMOL_UNDEFINED_OBJECT_NAME) << ")";
}