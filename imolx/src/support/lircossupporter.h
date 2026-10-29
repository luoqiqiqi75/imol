#ifndef LIRCOSSUPPORTER_H
#define LIRCOSSUPPORTER_H

#include "xhelper.h"

#include "basesupporter.h"

class LircosSupporter : public BaseSupporter
{
    IX_DECLARE_P

    Q_OBJECT

public:
    typedef std::function<QByteArray(ix::Node *)> CmdInF;
    typedef std::function<bool(ix::Node *, const QByteArray &)> CmdOutF;
    typedef std::function<void(int)> ErrF;
    typedef std::function<int()> ExtF;
    typedef std::function<void(ix::Node *, ix::Node *)> CustomF;

public:
    explicit LircosSupporter(QObject *parent = nullptr);
    ~LircosSupporter();

    void init() override;
};

#endif // LIRCOSSUPPORTER_H
