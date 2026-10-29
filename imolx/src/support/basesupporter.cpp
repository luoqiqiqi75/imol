#include "basesupporter.h"

BaseGrammar::BaseGrammar(QObject *parent) : QObject(parent)
{

}

BaseGrammar::~BaseGrammar()
{

}

QString BaseGrammar::lastError() const { return m_res.reason; }

bool BaseGrammar::compile(ix::Node *node, QString &statement)
{
    m_res.reset(node->c("type")->getString());
    m_res.succ = compileVar(m_res.type, node, statement);
    if (m_res.isUnsupport()) m_res.succ = compileIns(m_res.type, node, statement);
    if (m_res.isUnsupport()) WLOG << "<imolx::grammar> unsupport type: " << m_res.type;
    return m_res.succ;
}

bool BaseGrammar::decompile(const QString &statement, ix::Node *node)
{
    m_res.reset(node->c("type")->getString());
    m_res.succ = decompileVar(m_res.type, statement, node);
    if (m_res.isUnsupport()) m_res.succ = decompileIns(m_res.type, statement, node);
    if (m_res.isUnsupport()) WLOG << "<imolx::grammar> unsupport type: " << m_res.type;
    return m_res.succ;
}

bool BaseGrammar::compileVar(const QString &, ix::Node *, QString &) { return false; }
bool BaseGrammar::decompileVar(const QString &, const QString &, ix::Node *) { return false; }

bool BaseGrammar::compileIns(const QString &, ix::Node *, QString &) { return false; }
bool BaseGrammar::decompileIns(const QString &, const QString &, ix::Node *) { return false; }

BaseSupporter::BaseSupporter(BaseGrammar *interpreter, QObject *parent) : QObject{parent},
    interpreter(interpreter),
    m_tn(m("xsupport.trigger"))
{

}

BaseSupporter::~BaseSupporter()
{

}

void BaseSupporter::init()
{
}

void BaseSupporter::supportXService(ix::Node *ref, const ix::Node *pkg)
{
    QString id = pkg->c("id")->getString();
    foreach (ix::Node *cmd_node, pkg->cmobjs()) {
        if (cmd_node->name() == "id") continue;
        ix::Node *t_cmd_node = m_tn->c(cmd_node->name());
        foreach (ix::Node *key_node, cmd_node->cmobjs()) {
            ix::Node *trigger_key_node = t_cmd_node->r(key_node->name());
            if (trigger_key_node->isEmptyMobj()) continue;
            m_sub_key = id.mid(key_node->name().length() + 1);
            trigger_key_node->set(ref, QVariant::fromValue<QObject *>(key_node));
        }
    }
}
