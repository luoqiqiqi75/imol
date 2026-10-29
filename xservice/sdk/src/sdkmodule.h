/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file sdkmodule.h
 * @brief sdk module implementation
 * @author LuoQi
 * @date 2021-06-07
 */

#pragma once

#include "imol/xmodule.h"

namespace imol {
class ServerNetObject;
}

class SdkModule : public ix::Module
{
public:
    explicit SdkModule();

protected:
    void idle() override;

    void notify(const Json::Value& session, const std::string& format, const Json::Value& value) const;

private:
    int handleMsg(const std::string& addr, const std::string& content) const;

    void gCmd(const std::string& cmd_key, Json::Value& cmd_value) const;
    void sCmd(const std::string& cmd_key, Json::Value& cmd_value) const;
    bool wCmd(const Json::Value& session, const std::string& cmd_key, Json::Value& cmd_value) const;
    bool cCmd(const Json::Value& session, const std::string& cmd_key, Json::Value& cmd_value) const;

    void imolCmd(Json::Value& value) const;

private:
    imol::ServerNetObject* m_server;
    ix::Data<imol::Map<std::string, ix::sdk::Cip>>* m_cip_dobj;
};
