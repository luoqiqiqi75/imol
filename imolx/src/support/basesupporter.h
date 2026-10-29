#ifndef BASESUPPORTER_H
#define BASESUPPORTER_H

#include "xhelper.h"

class BaseGrammar : public QObject
{
    Q_OBJECT

public:
    struct AnalyseResult
    {
        bool succ;
        QString reason;
        QString type;

        void reset(const QString &atype) { succ = false; reason = ""; type = atype; }

        bool succeed() { return (succ = true); }
        bool fail(const QString &areason) { reason = areason; return (succ = false); }

        bool isUnsupport() { return !succ && reason.isEmpty(); }
    };

public:
    explicit BaseGrammar(QObject *parent = nullptr);
    virtual ~BaseGrammar();

    QString lastError() const;
    bool compile(ix::Node *node, QString &statement);
    bool decompile(const QString &statement, ix::Node *node);

    virtual bool compileVar(const QString &type, ix::Node *node, QString &statement);
    virtual bool decompileVar(const QString &type, const QString &statement, ix::Node *node);

    virtual bool compileIns(const QString &type, ix::Node *node, QString &statement);
    virtual bool decompileIns(const QString &type, const QString &statement, ix::Node *node);

protected:
    AnalyseResult m_res;
};

class BaseSupporter : public QObject
{
    Q_OBJECT

public:
    BaseGrammar *interpreter;

public:
    explicit BaseSupporter(BaseGrammar *interpreter, QObject *parent = nullptr);
    virtual ~BaseSupporter();

    virtual void init();

public slots:
    void supportXService(ix::Node *ref, const ix::Node *pkg);

protected:
    QString m_sub_key;

private:
    ix::Node *m_tn;
};

#endif // BASESUPPORTER_H
