/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file rtloopobject.h
 * @brief rt context based loop object implementation
 * @author LuoQi
 * @date 2021-06-01
 */

#ifndef IMOL_INTERNAL_RT_LOOP_OBJECT_H
#define IMOL_INTERNAL_RT_LOOP_OBJECT_H

#include <rtt/TaskContext.hpp>

#include "imol/loopmanager.h"

#include "foundation/container/concurrentqueue.h"

namespace imol {

class RtLoopObject : public RTT::TaskContext, public LoopObject
{
public:
    explicit RtLoopObject(const std::string& name);

    void addTask(const Task& t) override;

    bool isRunning() const override;
    bool hasTask() const override;
    
    bool start() override;
    bool stop() override;

protected:
    void updateHook() override;

private:
    moodycamel::ConcurrentQueue<Task> m_tasks;
};

}

#endif //IMOL_INTERNAL_RT_LOOP_OBJECT_H