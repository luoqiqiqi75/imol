/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file module.h
 * @brief module interface
 * @author LuoQi
 * @date 2021-06-01
 */

#ifndef IMOL_MODULE_H
#define IMOL_MODULE_H

#include "base.h"

namespace imol {

/**
 * @brief 
 * 
 */
class Module : public Object
{
public:
    explicit Module();
    virtual ~Module();

    std::string name() const override;
    std::string name(const std::string& suffix) const;

    // state control
    enum ModuleStateSignal : int {
        GONNA_INIT      = 0x0110,
        INITED          = 0x0100,
        GONNA_IDLE      = 0x0111,
        IDLED           = 0x0101,
        GONNA_DEINIT    = 0x0112,
        DEINITED        = 0x0102
    };

    ModuleStateSignal state() const;

    void startToInit();
    void startToIdle();
    void startToDeinit();

    // object control
    void adoptObj(Object* obj);
    void abandonObj(Object* obj, bool auto_delete = false);

protected:
    virtual void init();
    virtual void idle();
    virtual void deinit();

private:
    std::atomic<ModuleStateSignal> m_state;
};

}

#endif //IMOL_MODULE_H