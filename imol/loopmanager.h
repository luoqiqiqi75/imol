/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file loopmanager.h
 * @brief global loop manager
 * @author LuoQi
 * @date 2021-05-24
 */

#ifndef IMOL_LOOP_MANAGER_H
#define IMOL_LOOP_MANAGER_H

#include "base.h"

#ifndef IMOL_LOOP_TIMEOUT_MS
#define IMOL_LOOP_TIMEOUT_MS 30000.0
#endif

#ifndef IMOL_LOOP_INTERVAL_MS
#define IMOL_LOOP_INTERVAL_MS 0.5
#endif

namespace imol {

class LoopObject : public Object
{
public:
    explicit LoopObject(const std::string& name = "");

    virtual void addTask(const Task& t);

//    virtual void processTasks();
//    virtual bool execUntil(const Condition& cond, double timeout_ms = IMOL_LOOP_TIMEOUT_MS, double interval_ms = IMOL_LOOP_INTERVAL_MS);

    virtual bool isRunning() const;
    virtual bool hasTask() const;
    virtual int taskCount() const;

    virtual bool start();
    virtual bool stop();
};

enum LoopType {
    LOOP_RTT,
    LOOP_EVPP,
    LOOP_POOL,
};

namespace loop {
Manager& mgr();
LoopObject* get(const std::string& name, bool default_pool = true);
LoopObject* create(const std::string& name, LoopType type);
}

}

#endif //IMOL_LOOP_MANAGER_H