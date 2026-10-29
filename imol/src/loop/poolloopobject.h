/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file poolloopobject.h
 * @brief thread pool based loop object implementation
 * @author LuoQi
 * @date 2021-06-02
 */

#ifndef IMOL_INTERNAL_POOL_LOOP_OBJECT_H
#define IMOL_INTERNAL_POOL_LOOP_OBJECT_H

#include "imol/loopmanager.h"

namespace imol {

class PoolLoopObject : public LoopObject
{
public:
    explicit PoolLoopObject(const std::string& name);

    void addTask(const Task& t) override;

    bool isRunning() const override;
    bool hasTask() const override;
    
    bool start() override;
    bool stop() override;
};

}


#endif //IMOL_INTERNAL_POOL_LOOP_OBJECT_H