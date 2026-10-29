/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file evpploopobject.h
 * @brief event loop based loop object implementation
 * @author LuoQi
 * @date 2021-06-01
 */

#ifndef IMOL_INTERNAL_EVPP_LOOP_OBJECT_H
#define IMOL_INTERNAL_EVPP_LOOP_OBJECT_H

#include <evpp/event_loop.h>
#include <evpp/event_loop_thread.h>

#include "imol/loopmanager.h"

namespace imol {

class EvppLoopObject : public LoopObject
{
public:
    explicit EvppLoopObject(const std::string& name);
    explicit EvppLoopObject(const std::string& name, evpp::EventLoop* current_loop);
    virtual ~EvppLoopObject();

    void addTask(const Task& t) override;

    bool isRunning() const override;
    bool hasTask() const override;
    int taskCount() const override;
    
    bool start() override;
    bool stop() override;

private:
    friend class EvppClientNetObject;
    friend class EvppServerNetObject;

private:
    evpp::EventLoopThread* m_elt;
    evpp::EventLoop* m_loop;
};

}

#endif //IMOL_INTERNAL_EVPP_LOOP_OBJECT_H