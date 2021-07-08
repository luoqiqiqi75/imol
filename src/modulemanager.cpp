#include "modulemanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QUuid>
#include <QDataStream>

#include "logmanager.h"
#include "commandmanager.h"

using namespace imol;

//!
//! ModuleObject implementation
//!
ModuleObject::ModuleObject(const QString &name, ModuleObject *parent_mobj) : QObject(nullptr),
    m_pmobj(parent_mobj),
    m_name(name),
    m_is_quiet(false),
    m_is_watching(false),
    m_mutex(new QMutex)
{
}

ModuleObject::~ModuleObject()
{
//    for (imol::ModuleObject *child_mobj : cmobjs()) {
//        remove(this, child_mobj);
//    }
    qDeleteAll(m_cmobjs);
    delete m_mutex;
}

bool ModuleObject::isEmptyMobj() const
{
    return false;
}

void ModuleObject::quiet(bool is_quiet, bool is_recursively)
{
    m_is_quiet = is_quiet;
    if (is_recursively) foreach (ModuleObject *mobj, cmobjs()) mobj->quiet(is_quiet, true);
}

void ModuleObject::watch(bool is_watching, bool is_recursively)
{
    m_is_watching = is_watching;
    if (is_recursively) foreach (ModuleObject *mobj, cmobjs()) mobj->watch(is_watching, true);
}

ModuleObject * ModuleObject::pmobj() const
{
    return m_pmobj ? m_pmobj : m().emptyMobj();
}

ModuleObject * ModuleObject::pmobj(int level) const
{
    return m_pmobj ? (level > 0 ? m_pmobj->pmobj(level - 1) : m_pmobj) : m().emptyMobj();
}

ModuleObject * ModuleObject::p(int level) const
{
    return level == 0 ? pmobj() : pmobj(level);
}

void ModuleObject::setPmobj(ModuleObject *parent_mobj)
{
    m_mutex->lock();
    m_pmobj = parent_mobj;
    m_mutex->unlock();
}

int ModuleObject::indexOfRname(const QString &rname)
{
    if (!rname.startsWith(IMOL_MODULE_ID_START)) return -1;

    bool is_int = false;
    int index = rname.midRef(1).toInt(&is_int);
    return is_int ? index : -1;
}

ModuleObject * ModuleObject::cmobj(const QString &child_name, bool is_strict) const
{
    int index = indexOfRname(child_name);
    return (!is_strict && index >= 0) ? cmobj(index) : m_cmobjs.value(child_name, m().emptyMobj());
}

ModuleObject * ModuleObject::c(const QString &child_name, bool is_strict) const
{
    return cmobj(child_name, is_strict);
}

ModuleObject * ModuleObject::cmobj(int index) const
{
    QString cmobj_name = m_cmobj_names.value(index, "");
    return cmobj_name.isEmpty() ? m().emptyMobj() : cmobj(cmobj_name);
}

ModuleObject * ModuleObject::c(int index) const
{
    return cmobj(index);
}

bool ModuleObject::hasCmobj(const QString &child_name, bool is_strict) const
{
    return child_name.isEmpty() ? !m_cmobjs.isEmpty() : !cmobj(child_name, is_strict)->isEmptyMobj();
}

bool ModuleObject::hasCmobj(ModuleObject *child_mobj) const
{
    return m_cmobjs.values().contains(child_mobj);
}

ModuleObject * ModuleObject::first() const
{
    return cmobj(0);
}

ModuleObject * ModuleObject::last() const
{
    return cmobj(cmobjCount() - 1);
}

int ModuleObject::cmobjCount() const
{
    return m_cmobjs.size();
}

int ModuleObject::size() const
{
    return cmobjCount();
}

QString ModuleObject::generateId(ModuleObject *mobj)
{
    QString id = IMOL_MODULE_ID_START + QUuid::createUuid().toString();
    if (!mobj->hasCmobj(id, true)) return id;

    int n = 0;
    while (mobj->hasCmobj(QString("%1_%2").arg(id).arg(++n)));

    return QString("%1_%2").arg(id).arg(n);
}

int ModuleObject::indexInPmobj() const
{
    return pmobj()->isEmptyMobj() ? -1 : pmobj()->cmobjs(true).indexOf(const_cast<ModuleObject *>(this));
}

ModuleObject * ModuleObject::rmobj(const QString &relative_name) const
{
    ModuleObject *target_mobj = const_cast<ModuleObject *>(this);
    QStringList mobj_names = relative_name.split(IMOL_MODULE_NAME_SEPARATOR);
    foreach (QString mobj_name, mobj_names) {
        if ((target_mobj = target_mobj->cmobj(mobj_name))->isEmptyMobj()) return m().emptyMobj();
    }
    return target_mobj;
}

ModuleObject * ModuleObject::r(const QString &relative_name) const
{
    return rmobj(relative_name);
}

bool ModuleObject::hasRmobj(const QString &relative_name) const
{
    return !rmobj(relative_name)->isEmptyMobj();
}

bool ModuleObject::isRmobjOf(ModuleObject *ancestral_mobj) const
{
    return pmobj()->isEmptyMobj() ? false : (pmobj() == ancestral_mobj ? true : pmobj()->isRmobjOf(ancestral_mobj));
}

QString ModuleObject::rname() const
{
    return name().startsWith(IMOL_MODULE_ID_START) ? mIndex(indexInPmobj(), name()) : name();
}

QString ModuleObject::fullName(ModuleObject *relative_mobj) const
{
    ModuleObject *mobj = pmobj();
    ModuleObject *rmobj = relative_mobj ? relative_mobj : m().rootMobj();
    return (mobj->isEmptyMobj() || mobj == rmobj) ? rname() : mName(mobj->fullName(relative_mobj), rname());
}

QStringList ModuleObject::cmobjNames(bool is_strict) const
{
    if (is_strict) return m_cmobj_names;

    //replace with mIndex
    QStringList names;
    m_mutex->lock();
    for (int i = 0; i < m_cmobj_names.size(); ++i) {
        QString cmobj_name = m_cmobj_names.value(i, "");
        names.append(cmobj_name.startsWith(IMOL_MODULE_ID_START) ? mIndex(i) : cmobj_name);
    }
    m_mutex->unlock();
    return names;
}

QList<ModuleObject *> ModuleObject::cmobjs(bool is_ordered) const
{
    if (!is_ordered) return m_cmobjs.values();

    QList<ModuleObject *> mobjs;
    foreach (QString mobj_name, cmobjNames(true)) {
        mobjs.append(cmobj(mobj_name, true));
    }
    return mobjs;
}

void ModuleObject::activate(QObject *context, ModuleObject *changed_mobj, ActivateType type)
{
    if (m_is_quiet) return;

    emit activated(changed_mobj, type, context);

    ModuleObject *mobj = pmobj();
    if (!mobj->isEmptyMobj() && mobj->isWatching()) mobj->activate(context, changed_mobj, type);
}

QVariant ModuleObject::get(const QVariant &default_var) const
{
    return m_var.isNull() ? default_var : m_var;
}

bool ModuleObject::getBool(bool default_bool) const
{
    QVariant var = get();
    return var.isNull() || !var.canConvert(QVariant::Bool) ? default_bool : var.toBool();
}

int ModuleObject::getInt(int default_int) const
{
    QVariant var = get();
    return var.isNull() || !var.canConvert(QVariant::Int) ? default_int : var.toInt();
}

double ModuleObject::getDouble(double default_double) const
{
    QVariant var = get();
    return var.isNull() || !var.canConvert(QVariant::Double) ? default_double : var.toDouble();
}

QString ModuleObject::getString(const QString &default_str) const
{
    QVariant var = get();
    return var.isNull() || !var.canConvert(QVariant::String) ? default_str : var.toString();
}

QVariantList ModuleObject::getList() const
{
    QVariantList list;
    foreach (ModuleObject *child_mobj, cmobjs(true)) {
        list.append(child_mobj->get());
    }
    return list;
}

void ModuleObject::setList(QObject *context, const QVariantList &var_list, bool intelligent)
{
    for (int i = 0; i < var_list.size(); ++i) {
        set(context, mIndex(i), var_list.at(i), intelligent);
    }
}

void ModuleObject::setList(QObject *context, const QString &relative_name, const QVariantList &var_list, bool intelligent)
{
    for (int i = 0; i < var_list.size(); ++i) {
        set(context, mName(relative_name, mIndex(i)), var_list.at(i), intelligent);
    }
}

ModuleObject * ModuleObject::set(QObject *context, const QVariant &var)
{
    QVariant tmp_var = var;

    m_mutex->lock();
    m_var.swap(tmp_var);
    m_mutex->unlock();

    //emit signal
    if (!isQuiet()) emit changed(m_var, tmp_var, context);
    if (isWatching()) activate(context, this, MOBJ_CHANGE);

    tmp_var.clear();

    return this;
}

ModuleObject * ModuleObject::set(QObject *context, const QString &relative_name, const QVariant &var, bool intelligent)
{
    ModuleObject *target_mobj = rmobj(relative_name);

    //need insert
    if (target_mobj->isEmptyMobj() && intelligent) {
        insert(context, relative_name);
        target_mobj = rmobj(relative_name);
    }

    //normal set
    if (target_mobj->isEmptyMobj()) WLOG << "<m><" << context << "> set unreachable module: \"" << relative_name << "\" with: " << var;
    target_mobj->set(context, var);

    return this;
}

bool ModuleObject::insert(QObject *context, ModuleObject *cmobj, bool auto_replace, int index)
{
    if (!cmobj || cmobj->isEmptyMobj()) {
        ELOG << "<mobj>" << "attempt to insert a null module obj";
        return false;
    } else if (hasCmobj(cmobj->name())) {
        if (!auto_replace) {
            ELOG << "<mobj>" << "attempt to insert an existed module obj: " << cmobj->name();
            return false;
        }
        if (!remove(context, cmobj->name())) return false;
    }
    cmobj->setPmobj(this);
    cmobj->watch(isWatching());

    if (!isQuiet()) emit gonnaInsert(cmobj, this, context);

    m_mutex->lock();
    if (index < 0 || index >= m_cmobj_names.size()) {
        m_cmobj_names.append(cmobj->name());
    } else {
        m_cmobj_names.insert(index, cmobj->name());
    }
    m_cmobjs.insert(cmobj->name(), cmobj);
    m_mutex->unlock();

    if (!isQuiet()) emit added(cmobj->rname(), context);
    if (isWatching()) activate(context, cmobj, MOBJ_INSERT);

    return true;
}

bool ModuleObject::insert(QObject *context, const QString &relative_name, bool auto_replace)
{
    ModuleObject *target_mobj = this;
    foreach (QString mobj_name, relative_name.split(IMOL_MODULE_NAME_SEPARATOR)) {
        if (target_mobj->isEmptyMobj()) return false;

        if (!target_mobj->hasCmobj(mobj_name)) {
            ModuleObject *mobj = new ModuleObject(mobj_name.startsWith(IMOL_MODULE_ID_START) ? generateId(this) : mobj_name, target_mobj);
            if (!target_mobj->insert(context, mobj, auto_replace)) {
                delete mobj;
                return false;
            }
        }

        target_mobj = target_mobj->cmobj(mobj_name);
    }

    return true;
}

ModuleObject * ModuleObject::append(QObject *context, const QString &rname)
{
    ModuleObject *mobj = new ModuleObject(rname.isEmpty() ? generateId(this) : rname, this);
    if (!this->insert(context, mobj)) {
        delete mobj;
        return m().emptyMobj();
    }
    return mobj;
}

bool ModuleObject::remove(QObject *context, ModuleObject *cmobj, bool auto_delete)
{
    if (!cmobj || cmobj->isEmptyMobj()) {
        ELOG << "<mobj>" << "attempt to remove a null module obj";
        return false;
    } else if (!hasCmobj(cmobj->name(), true)) {
        ELOG << "<mobj>" << "attempt to remove an unexisted module obj: " << cmobj->name();
        return false;
    }
    QString mobj_name = cmobj->name();
    QString mobj_rname = cmobj->rname();

    if (!isQuiet()) emit gonnaRemove(cmobj, this, context);

    m_mutex->lock();
    m_cmobj_names.removeOne(mobj_name);
    m_cmobjs.remove(mobj_name);
    m_mutex->unlock();

    if (!isQuiet()) emit removed(mobj_rname, context);
    if (isWatching()) activate(context, cmobj, MOBJ_REMOVE);

    if (auto_delete) delete cmobj;

    return true;
}

bool ModuleObject::remove(QObject *context, const QString &relative_name, bool auto_delete)
{
    ModuleObject *target_mobj = rmobj(relative_name);
    return target_mobj->pmobj()->remove(context, target_mobj, auto_delete);
}

void ModuleObject::copyFrom(QObject *context, ModuleObject *other, bool auto_insert, bool auto_remove)
{
    if (!other) return;

    //auto remove
    if (auto_remove) {
        int cnt = cmobjCount();
        for (int i = 0; i < cnt; ++i) {
            ModuleObject *child_mobj = cmobj(cnt - i - 1);
            if (!other->hasCmobj(child_mobj->rname())) remove(context, child_mobj);
        }
    }

    //auto insert
    foreach (QString cmobj_name, other->cmobjNames()) {
        if (!hasCmobj(cmobj_name) && auto_insert && !insert(context, cmobj_name)) continue;
        cmobj(cmobj_name)->copyFrom(context, other->cmobj(cmobj_name), auto_insert, auto_remove);
    }

    set(context, other->get());
}

void ModuleObject::clear(QObject *context, bool auto_delete, bool only_var)
{
    if (only_var) {
        //only clear var
        foreach (ModuleObject *child_mobj, cmobjs()) {
            child_mobj->clear(context, auto_delete, only_var);
        }
        set(context, QVariant());
    } else {
        //clear all cmobjs
        foreach (ModuleObject *child_mobj, cmobjs()) {
            remove(context, child_mobj, auto_delete);
        }
        set(context, QVariant());
    }
}

void ModuleObject::dump(ModuleObject *rmobj) const
{
    SLOG << "<mobj>" << fullName(rmobj) << ": " << get() << ", children: " << cmobjNames();
    foreach (ModuleObject *mobj, cmobjs(true)) {
        if (mobj) mobj->dump();
    }
}

void ModuleObject::importFromJson(QObject *context, const QJsonValue &json_value, bool auto_insert, bool auto_replace, bool auto_remove)
{
    //set var from json
    if (!json_value.isObject() && !json_value.isArray()) {
        if (auto_replace) {
            if (auto_remove) {
                foreach (ModuleObject *child_mobj, cmobjs()) {
                    remove(context, child_mobj);
                }
            }
            set(context, json_value.toVariant());
        }
        return;
    }

    //build children
    QJsonArray json_array = json_value.toArray();
    QJsonObject json_obj = json_value.toObject();

    QStringList json_keys;
    if (json_value.isArray()) {
        for (int i = 0; i < json_array.size(); ++i) {
            json_keys.append(mIndex(i));
        }
    } else {
        json_keys = json_obj.keys();
    }

    //auto remove
    if (auto_remove) {
        int cnt = cmobjCount();
        for (int i = 0; i < cnt; ++i) {
            ModuleObject *child_mobj = cmobj(cnt - i - 1);
            if (!json_keys.contains(child_mobj->rname())) remove(context, child_mobj);
        }
    }

    for (int i = 0; i < json_keys.size(); i++) {
        QString json_key = json_keys.at(i);
        if (json_key.contains(IMOL_MODULE_NAME_SEPARATOR)) WLOG << "<m><" << context << "> importing json key contains separator: \"" << json_key << "\"";

        //auto insert
        if (!hasCmobj(json_key)) {
            if (!auto_insert) {
                WLOG << "<m><" << context << "> importing json key not exist: \"" << json_key << "\"";
                continue;
            }

            if (!insert(context, json_key, auto_replace)) continue;
        }

        //replace
        cmobj(json_key)->importFromJson(context, json_value.isArray() ? json_array.at(i) : json_obj.value(json_key), auto_insert, auto_replace, auto_remove);
    }

    //emit signal
    if (!isQuiet()) emit changed(m_var, m_var, context);
    if (isWatching()) activate(context, this, MOBJ_CHANGE);
}

QJsonValue ModuleObject::exportToJson(bool auto_ignore) const
{
    //set json from var
    if (!hasCmobj()) return QJsonValue::fromVariant(get());

    if (cmobj(0)->name().startsWith(IMOL_MODULE_ID_START)) {
        //build json array
        QJsonArray json_array;
        foreach (ModuleObject *mobj, cmobjs(true)) {
            json_array.append(mobj->exportToJson());
        }
        return json_array;
    } else {
        //build json object
        QJsonObject json_obj;
        foreach (QString child_name, cmobjNames()) {
            if (auto_ignore && child_name.startsWith(IMOL_MODULE_SPECIAL_START)) continue;
            json_obj.insert(child_name, cmobj(child_name)->exportToJson());
        }
        return json_obj;
    }
}

#include <QSizeF>
#include <QPointF>

bool qvariant2String(const QVariant &var, QString &str)
{
    if (var.canConvert(QVariant::String)) {
        str = var.toString();
        return true;
    }

    if (QString(var.typeName()).isEmpty()) return false;

    bool success = true;
    switch (var.type()) {
    case QVariant::Size: {
        QSize s = var.toSize();
        str = QString("%1,%2").arg(s.width()).arg(s.height());
    }
        break;
    case QVariant::SizeF: {
        QSizeF s = var.toSizeF();
        str = QString("%1,%2").arg(s.width()).arg(s.height());
    }
        break;
    case QVariant::Point: {
        QPoint p = var.toPoint();
        str = QString("%1,%2").arg(p.x()).arg(p.y());
    }
        break;
    case QVariant::PointF: {
        QPointF p = var.toPointF();
        str = QString("%1,%2").arg(p.x()).arg(p.y());
    }
        break;
    default:
        success = false;
        break;
    }

    return success;
}

bool string2QVariant(int type, const QString &str, QVariant &var)
{
    switch (type) {
    case QVariant::Size: {
        QStringList xy = str.split(",");
        var = QSize(xy.value(0).toInt(), xy.value(1).toInt());
    }
        break;
    case QVariant::SizeF: {
        QStringList xy = str.split(",");
        var = QSizeF(xy.value(0).toDouble(), xy.value(1).toDouble());
    }
        break;
    case QVariant::Point: {
        QStringList xy = str.split(",");
        var = QPoint(xy.value(0).toInt(), xy.value(1).toInt());
    }
        break;
    case QVariant::PointF: {
        QStringList xy = str.split(",");
        var = QPointF(xy.value(0).toDouble(), xy.value(1).toDouble());
    }
        break;
    default:
        var = QVariant(str);
        return var.convert(type);
    }

    return true;
}

void ModuleObject::importFromXml(QObject *context, QXmlStreamReader &xml_reader, bool auto_insert, bool auto_replace, bool only_standard)
{
    QHash<ModuleObject *, int> index_hash; //save index
    QHash<ModuleObject *, QVariant> value_hash; //save value
    for (ModuleObject *target_mobj = this; !xml_reader.atEnd() && target_mobj != pmobj(); xml_reader.readNext()) {
        if (xml_reader.isStartElement()) {
            QXmlStreamAttributes attrs = xml_reader.attributes();

            //value for current node
            QVariant value_var;
            if (attrs.hasAttribute("value")) {
                if (attrs.hasAttribute("type")) {
                    if (!string2QVariant(attrs.value("type").toInt(), attrs.value("value").toString(), value_var)) {
                        WLOG << "<m><" << context << "> cannot convert in xml: ("<< xml_reader.lineNumber() << ", " << xml_reader.columnNumber() << ")";
                    }
                } else {
                    value_var = attrs.value("value").toString();
                }
            }

            //set current value quietly
            if (xml_reader.name() == "m") {
                if (auto_replace) {
                    quiet(true);
                    set(context, value_var);
                    quiet(false);
                }
                continue;
            }

            //set cmobj name
            QString cmobj_name;

            if (xml_reader.name() == "c") {
                QStringRef name_ref = attrs.value("name");
                if (name_ref.isNull() || name_ref.isEmpty()) {
                    WLOG << "<m><" << context << "> no name specified in xml: ("<< xml_reader.lineNumber() << ", " << xml_reader.columnNumber() << ")";
                    xml_reader.skipCurrentElement();
                    continue;
                }
                cmobj_name = QString().append(name_ref);
            } else if (xml_reader.name() == "l") {
                cmobj_name = mIndex(index_hash.value(target_mobj, 0));
            } else {
                cmobj_name = QString().append(xml_reader.name());

                if (only_standard) {
                    WLOG << "<m><" << context << "> non-standard xml element: \"" << cmobj_name << "\"";
                    xml_reader.skipCurrentElement();
                    continue;
                }
            }

            //create cmobj
            if (!target_mobj->hasCmobj(cmobj_name)) {
                if (!auto_insert) {
                    WLOG << "<m><" << context << "> importing xml element not exist: \"" << cmobj_name << "\"";
                    xml_reader.skipCurrentElement();
                    continue;
                } else if (!target_mobj->insert(context, cmobj_name, auto_replace)) {
                    xml_reader.skipCurrentElement();
                    continue;
                }
            }

            //record index
            index_hash[target_mobj] = index_hash.value(target_mobj, 0) + 1;

            //insert child and set value
            target_mobj = target_mobj->cmobj(cmobj_name);
            if (auto_replace) value_hash[target_mobj] = value_var;
        } else if (xml_reader.isEndElement()) {
            //finish cur item
            if (auto_replace) target_mobj->set(context, value_hash.value(target_mobj, QVariant()));

            index_hash.remove(target_mobj);
            value_hash.remove(target_mobj);

            target_mobj = target_mobj->pmobj();
        }
    }

    //emit signal
    if (!isQuiet()) emit changed(m_var, m_var, context);
    if (isWatching()) activate(context, this, MOBJ_CHANGE);
}

void ModuleObject::exportToXml(QXmlStreamWriter &xml_writer, bool is_root, bool auto_ignore) const
{
    xml_writer.writeStartElement(is_root ? "m" : (name().startsWith(IMOL_MODULE_ID_START) ? "l" : "c"));
    if (!is_root && !name().startsWith(IMOL_MODULE_ID_START)) xml_writer.writeAttribute("name", name());

    //write value
    QVariant var = get();
    if (!var.isNull()) {
        xml_writer.writeAttribute("type", QString::number(var.type()));
        QString value_str;
        if (qvariant2String(var, value_str)) xml_writer.writeAttribute("value", value_str);
    }

    //write cmobj
    foreach (imol::ModuleObject *cmobj, cmobjs(true)) {
        if (auto_ignore && cmobj->name().startsWith(IMOL_MODULE_SPECIAL_START)) continue;
        cmobj->exportToXml(xml_writer, false, auto_ignore);
    }

    xml_writer.writeEndElement();
}

void ModuleObject::importFromXmlStr(QObject *context, const QString &xml_str, bool auto_insert, bool auto_replace, bool only_standard)
{
    QXmlStreamReader xml_reader(xml_str);
    importFromXml(context, xml_reader, auto_insert, auto_replace, only_standard);
}

QString ModuleObject::exportToXmlStr(int indent, bool auto_ignore) const
{
    QString xml_str;
    QXmlStreamWriter xml_writer(&xml_str);
    if (indent > 0) {
        xml_writer.setAutoFormatting(true);
        xml_writer.setAutoFormattingIndent(indent);
    }

    xml_writer.writeStartDocument();
    exportToXml(xml_writer, true, auto_ignore);
    xml_writer.writeEndDocument();

    return xml_str;
}

enum BinaryVariantCondition
{
    BIN_NULL,
    BIN_QVARIANT,
    BIN_CUSTOM
};

void ModuleObject::importFromBin(QObject *context, const QByteArray &bytes, bool auto_insert, bool auto_replace, bool auto_remove)
{
    QDataStream in_stream(bytes);

    //variant
    QVariant var;
    uchar cond;
    in_stream >> cond;
    if (cond == BIN_QVARIANT) {
        in_stream >> var;
    } else if (cond == BIN_CUSTOM) {
        QString type_name;
        int type_id;
        QByteArray var_data;
        in_stream >> type_name >> type_id >> var_data;

        if (type_id != QMetaType::type(type_name.toStdString().c_str())) {
            WLOG << "<m> import invalid variant: " << fullName() << " with type \"" << type_name << "\" and id = " << type_id;
        } else {
            var = QVariant(type_id, QMetaType::create(type_id, var_data.data()));
        }

    }

    //cmobjs
    QStringList cmobj_names;
    in_stream >> cmobj_names;

    //start to remove
    if (auto_remove) {
        foreach (ModuleObject *cmobj, cmobjs()) {
            if (!cmobj_names.contains(cmobj->rname())) remove(context, cmobj);
        }
    }

    //start to insert
    foreach (QString cmobj_name, cmobj_names) {
        QByteArray cmobj_bytes;
        in_stream >> cmobj_bytes;

        if (!hasCmobj(cmobj_name)) {
            if (!auto_insert) continue;
            if (!insert(context, cmobj_name)) continue;
        }

        cmobj(cmobj_name)->importFromBin(context, cmobj_bytes, auto_insert, auto_replace, auto_remove);
    }

    //start to replace
    if (auto_replace) {
        set(context, var);
    } else {
        //emit signal
        if (!isQuiet()) emit changed(m_var, m_var, context);
        if (isWatching()) activate(context, this, MOBJ_CHANGE);
    }
}

const QList<int> unexportable_type_ids = {QMetaType::UnknownType, QMetaType::QObjectStar, QMetaType::Void, QMetaType::Nullptr, QMetaType::VoidStar};

QByteArray ModuleObject::exportToBin(bool auto_ignore) const
{
    QByteArray bytes;
    QDataStream out_stream(&bytes, QIODevice::WriteOnly);

    //variant
    uchar cond = BIN_NULL;
    QVariant var = get();
    if (var.isNull() || unexportable_type_ids.contains(var.type())) {
        out_stream << cond;
    } else if (var.type() >= QMetaType::User) {
        cond = BIN_CUSTOM;
        QString type_name = var.typeName();
        int type_id = QMetaType::type(var.typeName());
        out_stream << cond << type_name << type_id << QByteArray((char *)var.data(), QMetaType::sizeOf(type_id));
    } else {
        cond = BIN_QVARIANT;
        out_stream << cond << var;
    }

    //cmobjs
    QStringList cmobj_names;
    foreach (imol::ModuleObject *cmobj, cmobjs(true)) {
        if (auto_ignore && cmobj->name().startsWith(IMOL_MODULE_SPECIAL_START)) continue;
        cmobj_names << cmobj->rname();
    }
    out_stream << cmobj_names;

    foreach (imol::ModuleObject *cmobj, cmobjs(true)) {
        if (auto_ignore && cmobj->name().startsWith(IMOL_MODULE_SPECIAL_START)) continue;
        out_stream << cmobj->exportToBin(auto_ignore);
    }
    return bytes;
}

void ModuleObject::trigger()
{
    set(sender(), get());
}

//!
//! EmptyModuleObject implementation
//!
EmptyModuleObject::EmptyModuleObject(const QString &name, ModuleObject *parent_mobj) : ModuleObject(name, parent_mobj) {}

bool EmptyModuleObject::isEmptyMobj() const {return true;}

QVariant EmptyModuleObject::get(const QVariant &) const {return QVariant();}

ModuleObject * EmptyModuleObject::set(QObject *, const QVariant &) {return this;}
ModuleObject * EmptyModuleObject::set(QObject *, const QString &, const QVariant &, bool) {return this;}

void EmptyModuleObject::copyFrom(QObject *, ModuleObject *, bool, bool) {}

bool EmptyModuleObject::insert(QObject *, ModuleObject *, bool, int) {return false;}
bool EmptyModuleObject::insert(QObject *, const QString &, bool) {return false;}
ModuleObject * EmptyModuleObject::append(QObject *, const QString &) {return this;}

bool EmptyModuleObject::remove(QObject *, ModuleObject *, bool) {return false;}
bool EmptyModuleObject::remove(QObject *, const QString &, bool) {return false;}

void EmptyModuleObject::importFromJson(QObject *, const QJsonValue &, bool, bool, bool) {}
QJsonValue EmptyModuleObject::exportToJson(bool) const {return QJsonValue();}

void EmptyModuleObject::importFromXml(QObject *, QXmlStreamReader &, bool, bool, bool){}
void EmptyModuleObject::exportToXml(QXmlStreamWriter &, bool, bool) const {}

void EmptyModuleObject::importFromBin(QObject *, const QByteArray &, bool, bool, bool) {}
QByteArray EmptyModuleObject::exportToBin(bool) const {return QByteArray();}

class ConvertCommand : public BaseCommand
{
public:
    explicit ConvertCommand(QObject *parent = nullptr) : BaseCommand("convert", false, parent) {}
    QString usage() const override {return tr("convert JSON|XML|BIN JSON|XML|BIN -data=<data>|-path=<path>");}
    QString instruction() const override {return tr("Convert 1st type of data to 2nd type");}

protected:
    void run(imol::ModuleObject *mobj, const QString &param) override
    {
        Q_UNUSED(mobj)

        QString type0 = param.section(" ", 0, 0).trimmed();
        QString type1 = param.section(" ", 1, 1).trimmed();
        if (type0 == type1) {
            emit error(tr("origin type is same with target type"));
            return;
        }

        imol::ModuleObject temp("");

        //read
        if (param.contains("-path")) {
            QString path = param.section("-path=", -1);
            if (type0.startsWith("JSON", Qt::CaseInsensitive)) {
                temp.importFromJson(this, m().readFromJson(path));
            } else if (type0.startsWith("XML", Qt::CaseInsensitive)) {
                m().readFromXmlFile(this, &temp, path);
            } else if (type0.startsWith("BIN", Qt::CaseInsensitive)) {
                temp.importFromBin(this, qUncompress(m().readFromBin(path)));
            } else {
                emit error(tr("unknown origin type: ") + type0);
                return;
            }
        } else if (param.contains("-data")) {
            QString data = param.section("-data=", -1);
            if (type0.startsWith("JSON", Qt::CaseInsensitive)) {
                QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
                if (json_doc.object().isEmpty()) {
                    temp.importFromJson(this, json_doc.array());
                } else {
                    temp.importFromJson(this, json_doc.object());
                }
            } else if (type0.startsWith("XML", Qt::CaseInsensitive)) {
                temp.importFromXmlStr(this, data);
            } else if (type0.startsWith("BIN", Qt::CaseInsensitive)) {
                data.replace(" ", "");
                temp.importFromBin(this, qUncompress(QByteArray::fromHex(data.toUtf8())));
            }
        }

        //write
        if (type1.startsWith("JSON", Qt::CaseInsensitive)) {
            bool is_compact = false;
            QJsonValue json_value = temp.exportToJson(false);
            if (json_value.isArray()) {
                QJsonDocument json_doc(json_value.toArray());
                emit output(QString::fromUtf8(json_doc.toJson(is_compact ? QJsonDocument::Compact : QJsonDocument::Indented)));
            } else if (json_value.isObject()) {
                QJsonDocument json_doc(json_value.toObject());
                emit output(QString::fromUtf8(json_doc.toJson(is_compact ? QJsonDocument::Compact : QJsonDocument::Indented)));
            } else {
                emit output(json_value.toString());
            }
        } else if (type1.startsWith("XML", Qt::CaseInsensitive)) {
            emit output(temp.exportToXmlStr(4, false));
        } else if (type1.startsWith("BIN", Qt::CaseInsensitive)) {
            emit output(QString::fromUtf8(qCompress(temp.exportToBin(false)).toHex(' ')));
        }
    }
};

//!
//! ModuleManager implementation
//!
ModuleManager::ModuleManager(QObject *parent) : QObject(parent),
    m_root_mobj(new ModuleObject(IMOL_ROOT_MODULE_NAME)),
    m_empty_mobj(new EmptyModuleObject(IMOL_EMPTY_MODULE_NAME))
{
    command().regist(new ConvertCommand(this));
}

ModuleManager::~ModuleManager()
{
    delete m_root_mobj;
}

ModuleManager & ModuleManager::instance()
{
    static ModuleManager manager;
    return manager;
}

QString ModuleManager::separator()
{
    return IMOL_MODULE_NAME_SEPARATOR;
}

ModuleObject * ModuleManager::rootMobj() const
{
    return m_root_mobj;
}

ModuleObject * ModuleManager::emptyMobj() const
{
    return m_empty_mobj;
}

ModuleObject * ModuleManager::regist(QObject *context, const QString &full_name)
{
    return m_root_mobj->insert(context, full_name) ? m_root_mobj->rmobj(full_name) : m_empty_mobj;
}

bool ModuleManager::cancel(QObject *context, const QString &full_name)
{
    return m_root_mobj->remove(context, full_name);
}

bool ModuleManager::importFromJson(QObject *context, const QString &json_path, const QString &prefix, bool is_watched, bool auto_insert)
{
    ModuleObject *target_mobj = m(prefix);
    if (target_mobj->isEmptyMobj()) {
        if (!regist(context, prefix)) return false;
        if (!(target_mobj = m(prefix))) {
            ELOG << "<m><" << context << "> import json to null module object: \"" << prefix << "\"";
            return false;
        }
    }

    QJsonValue json_value = readFromJson(json_path);
    if (json_value.isNull()) WLOG << "<m><" << context << "> import empty json from: \"" << json_path << "\"";
    target_mobj->watch(is_watched, true);
    target_mobj->importFromJson(context, json_value, auto_insert);
    return true;
}

bool ModuleManager::exportToJson(QObject *context, const QString &full_name, const QString &json_path)
{
    ModuleObject *target_mobj = m(full_name);
    if (target_mobj->isEmptyMobj()) {
        ELOG << "<m><" << context << "> export non-exist module: \"" << full_name << "\"";
        return false;
    }
    return writeToJson(json_path, target_mobj->exportToJson());
}

QString ModuleManager::convertToUtf8(const QByteArray &content)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode(content.constData(), content.size(), &state);
    if (state.invalidChars > 0) {
        text = QTextCodec::codecForName("GBK")->toUnicode(content);
    } else {
        text = content;
    }

    return text;
}

QJsonValue ModuleManager::readFromJson(const QString &json_path)
{
    QFile json_file(json_path);
    if (!json_file.open(QIODevice::ReadOnly)) {
        ELOG << "<m><static> cannot read json file: \"" << json_path << "\"";
        return QJsonValue();
    }
    QByteArray json_raw = json_file.readAll();
    json_file.close();

    QJsonParseError json_err;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_raw, &json_err);
    //try to fix codec problem
    if (json_err.error == QJsonParseError::IllegalUTF8String) {
        json_doc = QJsonDocument::fromJson(convertToUtf8(json_raw).toUtf8(), &json_err);
    }
    if (json_err.error != QJsonParseError::NoError) {
        ELOG << "<m><static> read invalid json from: \"" << json_path << "\", parse error: \"" << json_err.errorString() << "\"";
    }
    return json_doc.isObject() ? QJsonValue(json_doc.object()) : QJsonValue(json_doc.array());
}

bool ModuleManager::writeToJson(const QString &json_path, const QJsonValue &json_value, bool auto_create_path)
{
    QJsonDocument json_doc;
    if (json_value.isObject()) {
        json_doc.setObject(json_value.toObject());
    } else if (json_value.isArray()) {
        json_doc.setArray(json_value.toArray());
    } else {
        ELOG << "<m><static> writing json is neither array nor object: \"" << json_path << "\"";
        return false;
    }

    if (auto_create_path && !QFileInfo(json_path).dir().exists() && !QDir::current().mkpath(QFileInfo(json_path).dir().absolutePath())) {
        ELOG << "<m><static> cannot create foler \"" << json_path << "\" to write json";
        return false;
    }

    QFile json_file(json_path);
    if (!json_file.open(QIODevice::WriteOnly)) {
        ELOG << "<m><static> cannot write json file: \"" << json_path << "\"";
        return false;
    }
    json_file.write(json_doc.toJson());
    json_file.close();

    return true;
}

bool ModuleManager::readFromXmlFile(QObject *context, ModuleObject *mobj, const QString &xml_path)
{
    if (!mobj || mobj->isEmptyMobj()) {
        ELOG << "<m><static> try to read xml for an empty mobj";
        return false;
    }

    QFile xml_file(xml_path);
    if (!xml_file.open(QIODevice::ReadOnly)) {
        ELOG << "<m><static> cannot read xml file: \"" << xml_path << "\"";
        return false;
    }

    QXmlStreamReader xml_reader(&xml_file);
    mobj->importFromXml(context, xml_reader);

    xml_file.close();
    return true;
}

bool ModuleManager::writeToXmlFile(const QString &xml_path, ModuleObject *mobj, bool auto_create_path, bool auto_ignore)
{
    if (!mobj || mobj->isEmptyMobj()) {
        ELOG << "<m><static> try to write xml from an empty mobj";
        return false;
    }

    if (auto_create_path && !QFileInfo(xml_path).dir().exists() && !QDir::current().mkpath(QFileInfo(xml_path).dir().absolutePath())) {
        ELOG << "<m><static> cannot create path \"" << xml_path << "\" to write xml";
        return false;
    }

    QFile xml_file(xml_path);
    if (!xml_file.open(QIODevice::WriteOnly)) {
        ELOG << "<m><static> cannot write xml file: \"" << xml_path << "\"";
        return false;
    }

    QXmlStreamWriter xml_writer(&xml_file);
    xml_writer.setAutoFormatting(true);
    xml_writer.setAutoFormattingIndent(4);

    xml_writer.writeStartDocument();
    mobj->exportToXml(xml_writer, true, auto_ignore);
    xml_writer.writeEndDocument();

    xml_file.close();
    return true;
}

QByteArray ModuleManager::readFromBin(const QString &bin_path)
{
    QFile bin_file(bin_path);
    if (!bin_file.open(QIODevice::ReadOnly)) {
        ELOG << "<m><static> cannot read binary file: \"" << bin_path << "\"";
        return QByteArray();
    }
    QByteArray bin_bytes = bin_file.readAll();
    bin_file.close();
    return qUncompress(bin_bytes);
}

bool ModuleManager::writeToBin(const QString &bin_path, const QByteArray &bin_bytes, int compress_level, bool auto_create_path)
{
    if (auto_create_path && !QFileInfo(bin_path).dir().exists() && !QDir::current().mkpath(QFileInfo(bin_path).dir().absolutePath())) {
        ELOG << "<m><static> cannot create path \"" << bin_path << "\" to write binary file";
        return false;
    }

    QFile bin_file(bin_path);
    if (!bin_file.open(QIODevice::WriteOnly)) {
        ELOG << "<m><static> cannot write binary file: \"" << bin_path << "\"";
        return false;
    }

    bin_file.write(qCompress(bin_bytes, compress_level));
    bin_file.close();
    return true;
}

//output function
imol::ModuleManager & m()
{
    return ModuleManager::instance();
}

imol::ModuleObject * m(const QString &relative_name, ModuleObject *relative_mobj)
{
    return relative_mobj ? relative_mobj->rmobj(relative_name) : m().rootMobj()->rmobj(relative_name);
}

QString mIndex(int index, const QString &default_name)
{
    return index < 0 ? default_name : QString("%1%2").arg(IMOL_MODULE_ID_START).arg(index);
}

QString mName(const QString &n0, const QString &n1)
{
    return n0 + IMOL_MODULE_NAME_SEPARATOR + n1;
}

QString mName(const QString &n0, const QString &n1, const QString &n2)
{
    return mName(n0, n1) + IMOL_MODULE_NAME_SEPARATOR + n2;
}

QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3)
{
    return mName(n0, n1, n2) + IMOL_MODULE_NAME_SEPARATOR + n3;
}

QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4)
{
    return mName(n0, n1, n2, n3) + IMOL_MODULE_NAME_SEPARATOR + n4;
}

QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4, const QString &n5)
{
    return mName(n0, n1, n2, n3, n4) + IMOL_MODULE_NAME_SEPARATOR + n5;
}

QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4, const QString &n5, const QString &n6)
{
    return mName(n0, n1, n2, n3, n4, n5) + IMOL_MODULE_NAME_SEPARATOR + n6;
}
