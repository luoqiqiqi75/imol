#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include "core_global.h"

#include <QObject>
#include <QHash>
#include <QVariant>

#ifndef IMOL_MODULE_NAME_SEPARATOR
#define IMOL_MODULE_NAME_SEPARATOR "."
#endif

#ifndef IMOL_MODULE_SPECIAL_START
#define IMOL_MODULE_SPECIAL_START "_"
#endif

#ifndef IMOL_MODULE_ID_START
#define IMOL_MODULE_ID_START "#"
#endif

#ifndef IMOL_ROOT_MODULE_NAME
#define IMOL_ROOT_MODULE_NAME "@m"
#endif

#ifndef IMOL_EMPTY_MODULE_NAME
#define IMOL_EMPTY_MODULE_NAME "@e"
#endif

#ifndef IMOL_MODULE_XML_INDENT
#define IMOL_MODULE_XML_INDENT 4
#endif

class QMutex;
class QXmlStreamReader;
class QXmlStreamWriter;
namespace imol {
//!
//! \brief The ModuleObject class saves a variant and controls the signals of change add remove operations. As one tree
//! node in an object tree, a ModuleObject provides a series of methods to build and search its children.
//!
class CORESHARED_EXPORT ModuleObject : public QObject
{
    Q_OBJECT

public:
    enum ActivateType {
        MOBJ_CHANGE,
        MOBJ_INSERT,
        MOBJ_REMOVE
    };

    explicit ModuleObject(const QString &name, ModuleObject *parent_mobj = nullptr);
    virtual ~ModuleObject();

    virtual bool isEmptyMobj() const;

    //! \property name is the first basic property for ModuleObject
    Q_PROPERTY(QString m_name READ name)
    QString name() const {return m_name;}

    //! \a quiet restricts all signals and activation
    Q_PROPERTY(bool m_is_quiet READ isQuiet WRITE quiet)
    bool isQuiet() const {return m_is_quiet;}
    void quiet(bool is_quiet, bool is_recursively = false);

    //! \a watch controls wether parent nodes are able to be activated
    Q_PROPERTY(bool m_is_watching READ isWatching WRITE watch)
    bool isWatching() const {return m_is_watching;}
    void watch(bool is_watching, bool is_recursively = false);

    //!
    //! \brief Pmobj is the Abbreviation of Parent Module Object
    //!
    ModuleObject * pmobj() const;
    ModuleObject * pmobj(int level) const;
    ModuleObject * p(int level = 0) const; //shortcut
    void setPmobj(ModuleObject *parent_mobj);

    static int indexOfRname(const QString &rname);

    //!
    //! \brief Cmobj is the Abbreviation of Child Module Object
    //!
    //! Visit the cmobj by \property name as default, or visit cmobj with \property index
    //!
    ModuleObject * cmobj(const QString &child_name, bool is_strict = false) const;
    ModuleObject * c(const QString &child_name, bool is_strict = false) const; //shortcut
    ModuleObject * cmobj(int index) const;
    ModuleObject * c(int index) const; //shortcut
    bool hasCmobj(const QString &child_name = "", bool is_strict = false) const;
    bool hasCmobj(ModuleObject *child_mobj) const;

    ModuleObject * first() const;
    ModuleObject * last() const;

    int cmobjCount() const;
    int size() const;

    static QString generateId(ModuleObject *mobj);

    int indexInPmobj() const;

    //!
    //! \brief Rmobj is the Abbreviation of Relative Module Object
    //!
    //! Relative path uses \def IMOL_MODULE_NAME_SEPARATOR to seperate cmobj \property name, however you can also visit cmobj with
    //! \property index by useing \def IMOL_MODULE_ID_START at beginning and integer value comes next
    //!
    ModuleObject * rmobj(const QString &relative_name) const;
    ModuleObject * r(const QString &relative_name) const; //shortcut
    bool hasRmobj(const QString &relative_name) const;
    bool isRmobjOf(ModuleObject *ancestral_mobj) const;

    //! rname is the name in relative name
    QString rname() const;

    //! fullName is the opposite progress of rmobj
    QString fullName(ModuleObject *relative_mobj = nullptr) const;

    QStringList cmobjNames(bool is_strict = false) const;
    QList<ModuleObject *> cmobjs(bool is_ordered = false) const;

    void activate(QObject *context, ModuleObject *changed_mobj, ActivateType type);

    virtual QVariant get(const QVariant &default_var = QVariant()) const;

    bool getBool(bool default_bool = false) const;
    int getInt(int default_int = 0) const;
    double getDouble(double default_double = 0) const;
    QString getString(const QString &default_str = "") const;

    QVariantList getList() const;

    virtual ModuleObject * set(QObject *context, const QVariant &var);
    virtual ModuleObject * set(QObject *context, const QString &relative_name, const QVariant &var, bool intelligent = true);

    void setList(QObject *context, const QVariantList &var_list, bool intelligent = true);
    void setList(QObject *context, const QString &relative_name, const QVariantList &var_list, bool intelligent = true);

    virtual bool insert(QObject *context, ModuleObject *cmobj, bool auto_replace = false, int index = -1);
    virtual bool insert(QObject *context, const QString &relative_name, bool auto_replace = false);
    virtual ModuleObject * append(QObject *context, const QString &rname = "");

    virtual bool remove(QObject *context, ModuleObject *cmobj, bool auto_delete = true);
    virtual bool remove(QObject *context, const QString &relative_name, bool auto_delete = true);

    virtual void copyFrom(QObject *context, ModuleObject *other, bool auto_insert = true, bool auto_remove = false);

    void clear(QObject *context, bool auto_delete = true, bool only_var = false);

    //! output all info into log
    void dump(ModuleObject *rmobj = nullptr) const;

    //!
    //! \brief use json struction to construct children nodes
    //!
    //! Json Value (\p value) -> set current mobj with \p value
    //! Json Object (\p key, \p value) -> create cmobj with name is \p key, its structure is defined with \p value
    //! Json Array (\p value) -> Module Object with name started with IMOL_MODULE_ID_START, its structure id defined with \p value
    //!
    virtual void importFromJson(QObject *context, const QJsonValue &json_value, bool auto_insert = true, bool auto_replace = true, bool auto_remove = false);
    virtual QJsonValue exportToJson(bool auto_ignore = true) const;

    //!
    //! \brief use xml struction to construct children nodes, saves current var value only when can convert to QString
    //!
    virtual void importFromXml(QObject *context, QXmlStreamReader &xml_reader, bool auto_insert = true, bool auto_replace = true, bool only_standard = true);
    virtual void exportToXml(QXmlStreamWriter &xml_writer, bool is_root = true, bool auto_ignore = true) const;

    void importFromXmlStr(QObject *context, const QString &xml_str, bool auto_insert = true, bool auto_replace = true, bool only_standard = true);
    QString exportToXmlStr(int indent = IMOL_MODULE_XML_INDENT, bool auto_ignore = true) const;

    //!
    //! \brief use custom binary to construct children nodes, saves current var value with all support types. Custom variant will
    //! be copied directly, unsupported types are banned and covered with empty QVariant
    //!
    //! \caption Unsupport types
    //! \enum UnknownType, Void, Nullptr, VoidStar, QObjectStar
    //!
    //! \warning use invalid type of variant (like pointer) may CRASH
    //!
    virtual void importFromBin(QObject *context, const QByteArray &bytes, bool auto_insert = true, bool auto_replace = true, bool auto_remove = false);
    virtual QByteArray exportToBin(bool auto_ignore = true) const;

signals:
    /**
     * @brief activated signal is emitted when child mobj changed
     * @param changed_mobj is the changed mobj
     * @param changer is the context
     */
    void activated(ModuleObject *changed_mobj, int type, QObject *changer);
    /**
     * @brief changed signal is emitted when value changed
     * @param var is the new value
     * @param changer is the context
     */
    void changed(const QVariant &var, const QVariant &old_var, QObject *changer);
    /**
     * @brief added signal is emitted when this mobj inserts new child
     * @param rname is the child rname
     * @param changer is the context
     */
    void added(const QString &rname, QObject *changer);
    /**
     * @brief removed signal is emmited when this mobj removes its child
     * @param rname is the child rname
     * @param changer is the context
     */
    void removed(const QString &rname, QObject *changer);

    /**
     * @brief gonnaInsert signal is emmited when this mobj will add a new child
     * @param target is the inserting child
     * @param ref is this mobj
     * @param changer is the context
     */
    void gonnaInsert(ModuleObject *target, ModuleObject *ref, QObject *changer);
    /**
     * @brief gonnaRemove signal is emmited when this mobj will remove an exsisting child
     * @param target is the removing child
     * @param ref is this mobj
     * @param changer is the context
     */
    void gonnaRemove(ModuleObject *target, ModuleObject *ref, QObject *changer);

public slots:
    /**
     * @brief trigger slot works as a changed signal generator
     */
    void trigger();

private:
    ModuleObject *m_pmobj;

    QString m_name;
    QVariant m_var;

    QStringList m_cmobj_names;
    QHash<QString, ModuleObject *> m_cmobjs;

    bool m_is_quiet;
    bool m_is_watching;

    QMutex *m_mutex;
};

//!
//! \brief The EmptyModuleObject class is a mobj but does nothing, helps to avoid crash.
//!
class CORESHARED_EXPORT EmptyModuleObject : public ModuleObject
{
    Q_OBJECT

public:
    explicit EmptyModuleObject(const QString &name, ModuleObject *parent_mobj = nullptr);

    bool isEmptyMobj() const override;

    QVariant get(const QVariant &default_var = QVariant()) const override;

    ModuleObject * set(QObject *context, const QVariant &var) override;
    ModuleObject * set(QObject *context, const QString &relative_name, const QVariant &var, bool intelligent = true) override;

    void copyFrom(QObject *context, ModuleObject *other, bool auto_insert = true, bool auto_remove = false) override;

    bool insert(QObject *context, ModuleObject *child_mobj, bool auto_replace = false, int index = -1) override;
    bool insert(QObject *context, const QString &name, bool auto_replace = false) override;
    ModuleObject * append(QObject *context, const QString &rname = "") override;

    bool remove(QObject *context, ModuleObject *child_mobj, bool auto_delete = true) override;
    bool remove(QObject *context, const QString &name, bool auto_delete = true) override;

    void importFromJson(QObject *context, const QJsonValue &json_value, bool auto_insert = true, bool auto_replace = true, bool auto_remove = false) override;
    QJsonValue exportToJson(bool auto_ignore = true) const override;

    void importFromXml(QObject *context, QXmlStreamReader &xml_reader, bool auto_insert = true, bool auto_replace = true, bool only_standard = true) override;
    void exportToXml(QXmlStreamWriter &xml_writer, bool is_root = true, bool auto_ignore = true) const override;

    void importFromBin(QObject *context, const QByteArray &bytes, bool auto_insert = true, bool auto_replace = true, bool auto_remove = false) override;
    QByteArray exportToBin(bool auto_ignore = true) const override;
};

class CORESHARED_EXPORT ModuleManager : public QObject
{
    Q_OBJECT

public:
    explicit ModuleManager(QObject *parent = nullptr);
    virtual ~ModuleManager();

    static ModuleManager & instance();
    static QString separator();

    ModuleObject * rootMobj() const;
    ModuleObject * emptyMobj() const;

    ModuleObject * regist(QObject *context, const QString &full_name);
    bool cancel(QObject *context, const QString &full_name);

    bool importFromJson(QObject *context, const QString &json_path, const QString &prefix = "", bool is_watched = false, bool auto_insert = true); //obsolete
    bool exportToJson(QObject *context, const QString &full_name, const QString &json_path); //obsolete

    static QString convertToUtf8(const QByteArray &content);

    static QJsonValue readFromJson(const QString &json_path);
    static bool writeToJson(const QString &json_path, const QJsonValue &json_value, bool auto_create_path = true);

    static bool readFromXmlFile(QObject *context, ModuleObject *mobj, const QString &xml_path);
    static bool writeToXmlFile(const QString &xml_path, ModuleObject *mobj, bool auto_create_path = true, bool auto_ignore = true);

    static QByteArray readFromBin(const QString &bin_path);
    static bool writeToBin(const QString &bin_path, const QByteArray &bin_bytes, int compress_level = -1, bool auto_create_path = true);

private:
    ModuleObject *m_root_mobj;
    EmptyModuleObject *m_empty_mobj;
};
}

//output function
CORESHARED_EXPORT imol::ModuleManager & m();
CORESHARED_EXPORT imol::ModuleObject * m(const QString &relative_name, imol::ModuleObject *relative_mobj = nullptr);

CORESHARED_EXPORT QString mIndex(int index, const QString &default_name = "");
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1);
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1, const QString &n2);
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3);
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4);
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4, const QString &n5);
CORESHARED_EXPORT QString mName(const QString &n0, const QString &n1, const QString &n2, const QString &n3, const QString &n4, const QString &n5, const QString &n6);

#endif // MODULEMANAGER_H
