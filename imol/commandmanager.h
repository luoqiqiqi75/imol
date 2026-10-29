/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file commandmanager.h
 * @brief global command standard
 * @author LuoQi
 * @date 2021-06-25
 */

#ifndef IMOL_COMMAND_MANAGER_H
#define IMOL_COMMAND_MANAGER_H

#define IMOL_COMMAND_INPUT "_input"

#include "datamanager.h"

namespace imol {

class Module;
class LoopObject;

/**
 * @brief Result holds the return value after a command finished, normally you only take care about the returning \a code,
 * however, you are still able to append custom returns such as \a text and \a content
 *
 * @param code result code is unified as below:
 *      SUCCESS     = 0             - system will generate a "0" as the success return flag
 *      FAIL        = -1            - system will reply a common failure flag (not recommended)
 *      ACCEPT      = 1             - system will response no more
 *      PREFIX      = -0x????0000   - the offset for error codes in a group of commands
 *      ERROR_CODE  <= PREFIX       - specific error code must be under \a PREFIX
 *      HANDLE_MARK >= IGNORE       - specific mark for no response commands
 */
class Result : public JsonInterface
{
public:
    enum ResultCode : int {
        SUCCESS     = 0,
        FAIL        = -1,
        ACCEPT      = 1
    };

    Result(int code);
    Result(bool flag, int error_code = FAIL);
    Result(void);

    int code() const;
    DataObject* data() const;

    template<typename T> Result& setData(T t)
    {
        m_data.reset(data::from(t)); // with copy
        return *this;
    }

    bool isSuccess() const;
    bool isError() const;
    bool isAccepted() const;

    explicit operator bool() const;

    Json::Value serialize() const override;
    bool deserialize(const JsonRef& jr) override;

private:
    int m_code;
    std::shared_ptr<DataObject> m_data;
};

class CommandObject : public Object
{
public:
    class Procedure
    {
    public:
        typedef std::function<Result(CommandObject*)> Functor;

    public:
        explicit Procedure(const Functor& f, LoopObject* l = nullptr);
        explicit Procedure(Functor&& f, LoopObject* l = nullptr);

        template<class P = Procedure>
        static Procedure from(const Procedure& p) { return p; }

        template<typename F>
        static basic::FIfSame<F, Functor, Procedure> from(F f, LoopObject* l = nullptr)
        { return Procedure(std::move(f), l); }

        template<typename F> using FuncIn = typename basic::FInfo<F>::ArgsT::FirstT;

        template<typename T> using DataFunctor = typename std::enable_if<!std::is_pointer<T>::value, std::function<Result(T)>>::type;
        template<typename F>
        static basic::FIfConvertible<F, DataFunctor<FuncIn<F>>, Procedure> from(F f, LoopObject* l = nullptr)
        { return Procedure([=] (CommandObject* cobj) -> Result { return f(cobj->template inputData<basic::_t_remove_rc<FuncIn<F>>>()); }, l); }
        template<typename F>
        static basic::FIfConvertible<F, DataFunctor<FuncIn<F>>, Procedure> from(F f, FuncIn<F> t, LoopObject* l = nullptr)
        { return Procedure([=] (CommandObject*) -> Result { return f(t); }, l); }

        template<typename T> using VoidFunctor = std::function<T()>;
        template<typename F>
        static basic::FIfSame<F, VoidFunctor<typename basic::FInfo<F>::RetT>,
                typename std::enable_if<!std::is_same<typename basic::FInfo<F>::RetT, void>::value, Procedure>::type> from(F f, LoopObject* l = nullptr)
        {  return Procedure([=] (CommandObject*) -> Result { return f(); }, l); }

        using SimpleFunctor = std::function<void()>;
        template<typename F>
        static basic::FIfSame<F, SimpleFunctor, Procedure> from(F f, LoopObject* l = nullptr)
        { return Procedure([=] (CommandObject*) { f(); return Result(); }, l); }

        Functor f() const { return m_f; }
        LoopObject* l() const { return m_l; }

        Result run(CommandObject* cobj) const;

    private:
        Functor m_f;
        LoopObject* m_l;
        bool m_block;
    };

    typedef std::function<void(const Result&)> ResultHandler;

public:
    explicit CommandObject(const std::string& name);
    virtual ~CommandObject();

    CommandObject* clone() const;

    Manager& dataMgr();

    template<typename T>
    T data(const std::string& name, bool* ok = nullptr) const
    {
        TDataObject<T>* obj = m_mgr.get<TDataObject<T>>(name);
        if (ok) *ok = (obj != nullptr);
        return obj ? obj->get() : T();
    }
    template<typename T>
    const T& data(const std::string& name, const T& default_t, bool* ok) const
    {
        TDataObject<T>* obj = m_mgr.get<TDataObject<T>>(name);
        if (ok) *ok = (obj != nullptr);
        return obj ? obj->get() : default_t;
    }
    template<typename T>
    bool setData(const std::string& name, const T& t)
    {
        return m_mgr.add(new TDataObject<T>(name, t), true);
    }

    template<typename T>
    T inputData() const { return data<T>(IMOL_COMMAND_INPUT); }
    template<typename T>
    bool setInputData(const T& t) { return setData(IMOL_COMMAND_INPUT, t); }

    void setResultHandler(const ResultHandler& result_handler);

    CommandObject* prependProc(const Procedure& proc);
    CommandObject* appendProc(const Procedure& proc);

    imol::Result start(bool auto_delete = true);

    void report(const Result& result);
    void finish(const Result& result, bool auto_delete = true);

private:
    Manager m_mgr;
    ResultHandler m_result_handler;
    List<Procedure> m_procedures;
};

namespace command
{
Manager& mgr();
CommandObject* copy(const std::string& name);
CommandObject* copy(const std::string& name, const CommandObject::ResultHandler& result_handler);

bool registerCobj(Module* context, const std::string& key, CommandObject* cobj, bool delete_if_failed = false);
}

}

#endif // IMOL_COMMAND_MANAGER_H