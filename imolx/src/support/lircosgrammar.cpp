#include "lircosgrammar.h"

#include <QMetaEnum>
#include <qmath.h>

#include "xservice.h"

const constexpr char *reg_int = "(-?\\d+)";
const constexpr char *reg_double = "((?:[-\\+]?\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-\\+]?\\d+)?)";

#define DATA_DEFINE     -0x0002

template<>
QString associateVar<DATA_DEFINE>(ix::Node *n)
{
    return QString("%1 %2 %3 %4").arg(n->c("scope")->getString("GLOBAL").toUpper(),
                                      n->c("storage")->getString("VAR").toUpper(),
                                      n->c("type")->getString("unknown"),
                                      n->c("name")->getString(n->name()));
}

template<>
bool resolveVar<DATA_DEFINE>(const QString &v, ix::Node *n)
{
    QString def_pattern;
    QRegExp rx(def_pattern, Qt::CaseInsensitive);
    rx.setMinimal(true);
    if (rx.exactMatch(v)) {
        for (int i = 0; i < rx.captureCount(); i++) {
//            qDebug() << "cap: " << rx.cap(i + 1);
        }
    }

    return true;
}

QVariantList str2nums(const QString &str)
{
    QVariantList nums;
    QRegExp rx(reg_double);
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        nums << rx.cap(1);
        pos += rx.matchedLength();
    }
    return nums;
}

template <typename T = double>
int nums2n(const QVariantList &nums, ix::Node *node, int from, int size, T scale = 1)
{
    for (int i = 0; i < size; i++) {
        node->set(node, mIndex(i), nums.value(from + i).value<T>() * scale);
    }
    return size;
}

template <typename T = double>
QString n2str(ix::Node *node, int size, T scale = 1, const QString &fill = "0")
{
    QStringList strings;
    strings.reserve(size);
    foreach (ix::Node *n, node->cmobjs()) {
        strings.append(ix::format::toString(n->get().value<T>() * scale));
    }
    for (int i = 0; i < size - node->cmobjCount(); i++) {
        strings.append(fill);
    }
    return strings.join(",");
}

template<>
QString associateVar<DATA_JOINTTARGET>(ix::Node *n)
{
    return QString("j:{%1}{EJ %2}")
        .arg(n2str(n->c("near"), 6, 180.0 / M_PI), n2str(n->c("external"), 6, 180.0 / M_PI));
}

template<>
bool resolveVar<DATA_JOINTTARGET>(const QString &v, ix::Node *n)
{
    auto nums = str2nums(v);
    int offset = nums2n(nums, ix::createNodeAt(n, "near"), 0, 6, M_PI / 180.0);
    nums2n(nums, ix::createNodeAt(n, "external"), offset, 6, M_PI / 180.0);
    return true;
}

template<>
QString associateVar<DATA_ROBTARGET>(ix::Node *n)
{
    ix::Node q("q");
    q.copyFrom(&q, n->c("rot"));
    if (q.cmobjCount() != 4) ix::calc::rot2Quaternion(&q);
    return QString("p:{{%1},{%2}}{cfg %3}{EJ %4}")
        .arg(n2str(n->c("trans"), 3, 1000.0), n2str<double>(&q, 4), n2str<int>(n->c("conf"), 4), n2str(n->c("external"), 6, 180.0 / M_PI));
}

template<>
bool resolveVar<DATA_ROBTARGET>(const QString &v, ix::Node *n)
{
    ix::Node q("q");
    auto nums = str2nums(v);
    int offset = nums2n(nums, ix::createNodeAt(n, "trans"), 0, 3, 0.001);
    offset += nums2n<double>(nums, &q, offset, 4);
    offset += nums2n<int>(nums, ix::createNodeAt(n, "conf"), offset, 4);
    nums2n(nums, ix::createNodeAt(n, "external"), offset, 6, M_PI / 180.0);
    ix::calc::quaternion2Rot(&q, ix::createNodeAt(n, "rot"));
    return true;
}

LircosGrammar::LircosGrammar(QObject *parent) : BaseGrammar(parent)
{
    QMetaEnum var_me = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("DataType"));
    for (int i = DATA_NONE + 1; i < DATA_TOTAL_NUM; i++) {
        QString s = var_me.key(i);
        s.replace(0, 5, "");
        m_var_book.insert(s.toLower(), i);
    }
    m_var_book.insert("cart", DATA_ROBTARGET);
    m_var_book.insert("joint", DATA_JOINTTARGET);
}

QString LircosGrammar::dataType2String(int dt)
{
    QMetaEnum me = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("DataType"));
    QString s = me.valueToKey(dt);
    s.replace(0, 5, "");
    return s.toLower();
}

int LircosGrammar::string2DataType(const QString &str)
{
    const QMetaEnum me = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("DataType"));
    return me.keyToValue(QString("DATA_" + str.toUpper()).toStdString().c_str());
}

bool LircosGrammar::compileVar(const QString &type, ix::Node *node, QString &statement)
{
    DataType dt = static_cast<DataType>(m_var_book.value(type, DATA_NONE));
    if (dt == DATA_NONE) return false;

    // compile define
    bool has_define = node->hasCmobj("define");
    if (has_define) {
        ix::Node def(node->name());
        def.copyFrom(this, node->c("define"));
        def.set(this, "type", dataType2String(dt));
        statement = associateVar<DATA_DEFINE>(&def);
    } else {
        statement = "";
    }

    // compile value
    QString value;
    switch (dt) {
    case DATA_INT: value = ix::format::toString(node->c("value")->getInt()); break;
    case DATA_DOUBLE: value = ix::format::toString(node->c("value")->getDouble()); break;
    case DATA_BOOL: value = ix::format::toString(node->c("value")->getBool()); break;
    case DATA_STRING: value = QString("\"%1\"").arg(node->c("value")->getString()); break;
    case DATA_ROBTARGET: value = associateVar<DATA_ROBTARGET>(node); break;
    case DATA_JOINTTARGET: value = associateVar<DATA_JOINTTARGET>(node); break;
    default: value = node->c("value")->getString(); break;
    }
    if (!value.isEmpty()) statement += has_define ? QString(" = %1").arg(value) : value;

    // compile comment
    QString tag = node->c("tag")->getString();
    if (!tag.isEmpty()) statement += QString(" //%1").arg(tag);

    if (has_define) statement += "\n";
    return true;
}

bool LircosGrammar::decompileVar(const QString &type, const QString &statement, ix::Node *node)
{
    // decompile comment
    int i_end = statement.indexOf("//");
    QString str = i_end < 0 ? statement : statement.left(i_end);
    if (i_end >= 0) node->set(this, "tag", statement.mid(i_end + 2));

    // decompile define
    DataType dt = static_cast<DataType>(m_var_book.value(type, DATA_NONE));
    if (dt == DATA_NONE) {
        ix::Node def(node->name());
        if (resolveVar<DATA_DEFINE>(str, &def)) {
            // todo
        }
    }

    // decompile value
    QString value = str;
    int i_value = str.indexOf('=');
    if (i_value >= 0) {
        value = str.mid(i_value);
        value = value.contains("\"") ? value.trimmed() : value.simplified();
    }

    switch (dt) {
    case DATA_ROBTARGET: resolveVar<DATA_ROBTARGET>(value, node); break;
    case DATA_JOINTTARGET: resolveVar<DATA_JOINTTARGET>(value, node); break;
    default: node->set(this, "value", value); break;
    }
    return false;
}
