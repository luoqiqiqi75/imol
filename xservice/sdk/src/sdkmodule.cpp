#include "sdkmodule.h"

#include "imol/netmanager.h"

IMOL_MODULE(ix.sdk, SdkModule, 9)

#define DEFAULT_PORT 6666

SdkModule::SdkModule() : ix::Module(),
    m_cip_dobj(bindData("cip", imol::Map<std::string, ix::sdk::Cip>()))
{
    adoptObj(m_server = imol::net::createServer("sdk", imol::NET_BACKSLASH_R));
    bindData("port", DEFAULT_PORT, true, true)->readonly();
}

void SdkModule::idle()
{
    DLOG(INFO) << "<ix::sdk>registered command input preprocessors: " << m_cip_dobj->ptr()->keys();
    DLOG(INFO) << "<ix::sdk>registered command: " << imol::command::mgr().keys();
    DLOG(INFO) << "<ix::sdk>registered data: " << imol::data::mgr().keys();

    m_server->setHandler(IC_MEMBER_2(&SdkModule::handleMsg));
    int port = data<int>("port")->get();
    bool success = m_server->startListening(port);
    DLOG(INFO) << "<ix::sdk>start listening on: " << port << (success ? " successfully" : " failed");
}

void SdkModule::notify(const Json::Value& session, const std::string& format, const Json::Value& value) const
{
    imol::JsonRef jr(session);
    Json::Value notify_value;
    notify_value[jr.at("notify").toString()] = value;

    Json::Value formatted_value;
    formatted_value[format] = notify_value;
    if (auto id = jr.at("id")) formatted_value["id"] = id.value();
    
    m_server->send(jr.at("addr").toString(), ic::jsonSerialize(formatted_value));
}

#define SYNC_CMD(_T) \
if (auto _T = jr.at(#_T)) { \
    for (const auto& key : _T.objectKeys()) { \
        _T##Cmd(key, value[#_T][key]); \
    } \
}

#define ASYNC_CMD(_T) \
if (auto _T = jr.at(#_T)) { \
    Json::Value session; \
    session["addr"] = addr; \
    if (auto id = jr.at("id")) session["id"] = id.value(); \
    for (const auto& key : _T.objectKeys()) { \
        session["notify"] = key; \
        if (!_T##Cmd(session, key, value[#_T][key])) { \
            value[#_T].removeMember(key); \
        } \
    } \
    if (_T.objectSize() <= 0) value.removeMember(#_T); \
}

int SdkModule::handleMsg(const std::string& addr, const std::string& content) const
{
    if (content.size() < 2) return imol::Result::SUCCESS; // ignore empty content

    std::string json_err_str;
    Json::Value value = ic::jsonDeserialize(content, &json_err_str);

    imol::JsonRef jr(value);
    if (json_err_str.empty() && jr && jr.objectSize() > 0) {
        // main command handle
        SYNC_CMD(g)
        SYNC_CMD(s)
        ASYNC_CMD(w)
        ASYNC_CMD(c)
        // imol command handle
        if (auto i = jr.at("imol")) imolCmd(value["imol"]);
        // other remains
    } else {
        bool found = false;
        Json::Value id = jr.at("id", false, &found).value();
        value = imol::Result(ix::sdk::ERR_JSON_FORMAT).setData(jr.objectSize() > 0 ? json_err_str : "json object required").serialize();
        if (found) value["id"] = id;
    }

    if (imol::JsonRef(value).objectSize() > 0) m_server->send(addr, ic::jsonSerialize(value));
    return imol::Result::SUCCESS;
}

bool keyInvalid(const std::string& key, const std::string& filter)
{
    return key.size() == 0 || key[0] == '_' || (!filter.empty() && key.find(filter) != 0);
}

#define RETURN_ERR(Code) cmd_value = imol::Result(Code).serialize(); return

void SdkModule::gCmd(const std::string& cmd_key, Json::Value& cmd_value) const
{
    imol::DataObject* dobj = imol::d(cmd_key);
    imol::JsonInterface* ji = imol::data::ji(dobj);
    if (!ji) { RETURN_ERR(ix::sdk::ERR_NO_DATA_KEY); }
    if (!dobj->isReadable()) { RETURN_ERR(ix::sdk::ERR_G_NON_READABLE); }

    imol::JsonRef jr(ji->serialize());
    if (cmd_value.isObject()) {
        bool found = false;
        for (const auto& key : cmd_value.getMemberNames()) {
            cmd_value[key] = jr.get(key, &found).value();
            if (!found) { RETURN_ERR(ix::sdk::ERR_G_NO_KEY); }
        }
    } else {
        cmd_value = jr.value();
    }
}

void SdkModule::sCmd(const std::string& cmd_key, Json::Value& cmd_value) const
{
    imol::DataObject* dobj = imol::d(cmd_key);
    imol::JsonInterface* ji = imol::data::ji(dobj);
    if (!ji) { RETURN_ERR(ix::sdk::ERR_NO_DATA_KEY); }
    if (!dobj->isWritable()) { RETURN_ERR(ix::sdk::ERR_S_NON_WRITABLE); }

    Json::Value value = ji->serialize();
    if (!ic::mergeJson(value, cmd_value)) { RETURN_ERR(ix::sdk::ERR_S_MERGE); }
    if (!ji->deserialize(value)) { RETURN_ERR(ix::sdk::ERR_S_FAILED); }

    if (imol::DataObject* dobj = imol::d(cmd_key)) dobj->trigger(imol::DataObject::CHANGED);

    cmd_value = imol::Result(imol::Result::SUCCESS).serialize();
}

bool SdkModule::wCmd(const Json::Value& session, const std::string &cmd_key, Json::Value &cmd_value) const
{
    imol::DataObject* dobj = imol::d(cmd_key);
    imol::JsonInterface* ji = imol::data::ji(dobj);
    if (!ji) { RETURN_ERR(ix::sdk::ERR_NO_DATA_KEY) true; }
    if (!dobj->isReadable()) { RETURN_ERR(ix::sdk::ERR_G_NON_READABLE) true; }

    imol::LoopObject* lobj = m_server->loopOfClient(imol::JsonRef(session).at("addr").toString());
    if (!lobj) { RETURN_ERR(ix::sdk::ERR_W_CONNECTION) true; }

    bool watch = imol::JsonRef(cmd_value).toBool();
    if (watch) {
        if (dobj->hasConnection(imol::DataObject::CHANGED, lobj)) {
            RETURN_ERR(ix::sdk::ERR_W_DUPLICATE) true;
        } else {
            dobj->connect(imol::DataObject::CHANGED, lobj, [=] {
                notify(session, "n", ji->serialize());
            });
        }
    } else {
        dobj->disconnect(imol::DataObject::CHANGED, lobj);
    }

    return false;
}

bool SdkModule::cCmd(const Json::Value& session, const std::string& cmd_key, Json::Value &cmd_value) const
{
    // step 1: prepare command
    imol::CommandObject* cobj = imol::command::copy(cmd_key);
    if (!cobj) { RETURN_ERR(ix::sdk::ERR_NO_CMD_KEY) true; }

    // step 2: save sdk info
    cobj->setData("_session", session);
    cobj->setResultHandler([=] (const imol::Result& result) {
        notify(cobj->data<Json::Value>("_session"), "c", result.serialize());
    });

    // step 3: input handle
    if (auto cip = m_cip_dobj->ptr()->value(cmd_key)) {
        cobj->prependProc(ic::Proc(std::bind(cip, std::placeholders::_1, cmd_value)));
    } else {
        cobj->setInputData(cmd_value);
    }

    // step 4: execute command
    imol::Result cmd_res = cobj->start();
    if (!cmd_res.isAccepted()) {
        cmd_value = cmd_res.serialize();
        return true;
    }

    return false;
}

void SdkModule::imolCmd(Json::Value& value) const
{
    imol::JsonRef jr(value);
    if (auto d = jr.at("data")) {
        imol::List<std::string> keys;
        for (auto& kv : imol::data::mgr()) {
            std::string key = kv.first;
            if (keyInvalid(key, d.toString())) continue;
            if (!dynamic_cast<imol::JsonInterface*>(kv.second)) continue; // non serializable
            keys.push_back(std::move(key));
        }
        value["data"] = ic::array2Json(keys);
    }
    if (auto c = jr.at("command")) {
        imol::List<std::string> keys;
        for (auto& kv : imol::command::mgr()) {
            if (!keyInvalid(kv.first, c.toString())) keys.push_back(kv.first);
        }
        value["command"] = ic::array2Json(keys);
    }
    if (auto v = jr.at("version")) {
        imol::HashMap<std::string, int> versions;
        for (auto& kv : imol::ver()) {
            if (!keyInvalid(kv.first, v.toString())) versions[kv.first] = kv.second();
        }
        value["version"] = ic::map2Json(versions);
    }
}