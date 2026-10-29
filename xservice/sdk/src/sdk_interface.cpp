#include "../sdk_interface.h"

IMOL_REGISTER_VERSION(ix::sdk, 4)

namespace ix {
namespace sdk {

#define cip_ti imol::data::ti<imol::Map<std::string, Cip>>("ix.sdk.cip")

void registerCip(const std::string& cmd, const Cip& cip)
{
    if (auto c = cip_ti) c->ref().insert({cmd, cip});
}

void unregisterCip(const std::string& cmd)
{
    if (auto c = cip_ti) c->ref().erase(cmd);
}

Cip getCip(const std::string& cmd)
{
    if (auto c = cip_ti) return c->ref().value(cmd);
    return NULL;
}

}
}
