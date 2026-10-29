#include "imolx_global.h"

#include "core/common.h"
#include "bwaf/bmodule.h"
#include "bwaf/bfactory.h"

#include <QDir>
#include <QPixmap>
#include <QIcon>

#include "xhelper.h"
#include "xtheme.h"
#include "xsupport.h"
#include "xservice.h"

#define NAME "imolx"

IMOL_VER_REG(NAME, 1)

#define IMOLX_VER 4
#define IMOLX_VER_N 29

class Imolx : public bwaf::BModule
{
public:
    explicit Imolx(QObject *parent) : bwaf::BModule(parent)
    {
        DLOG << "<imolx> version: " << IMOLX_VER << "." << (ver(NAME, true) - IMOLX_VER_N);
    }
    virtual ~Imolx()
    {

    }

    QString name() const { return NAME; }

private:
    void setup()
    {
        bfactory().keyMobj(NAME)->reorder(this, QStringList() << "xtheme" << "xservice" << "xsupport");

        BFACTORY_CREATE_SUBMODULES(this, NAME);
    }

    void init()
    {
    }
};

IMOL_PLUGIN(NAME, Imolx)
