/**
 * Copyright(C) 2012 LithoTek. All Rights Reserved.
 * Information in this file is the intellectual property of LithoTek, and may
 * contains trade secrets that must be stored and viewed confidentially.
 *
 * @file editor_struct.h
 * @author: 汤中华
 * @date: 2017-12-14
 * @brief 编辑器相关结构体定义
 */

#ifndef EDITOR_STRUCT_H
#define EDITOR_STRUCT_H

//#include <Qsci/qsciscintilla.h>
//#include <scintilla/Scintilla.h>

#include <QString>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <QDebug>

#include "editor_define.h"
#include "HMICommonDefine.h"

/**
 * @brief 编辑器属性
 */
//struct EditorProperty {
//    /**
//     * @brief 默认构造函数
//     */
//    EditorProperty();

//    //边缘字体
//    QFont margin_font;
//    //背景颜色
//    QColor background_color;
//    //前景颜色
//    QColor foreground_color;
//    //高亮颜色
//    QColor highlight_color;
//    //文本字体
//    QFont text_font;
//    //折叠样式
//    QsciScintilla::FoldStyle fold_style;
//    //比例
//    int zoom;
//};

/**
 * @brief 变量
 */
struct CodeVar {
public:
    /**
     * @brief 构造函数
     */
    CodeVar();

    /**
     * @brief =操作符重载
     * @param info 已有变量
     * @return　构造的变量
     */
    CodeVar& operator=(IN const CodeVar& info);

    /**
     * @brief 清除记录，恢复默认值
     */
    void Clear();

    /**
     * @brief 是否记录为空
     * @return true-空记录, false-非空
     */
    bool IsEmpty();

    /**
     * @brief 操作符==重载，判断变量记录是否相等
     * @param info 已有变量记录
     * @return true-相等,false-不等
     */
    bool operator == (IN const CodeVar& info) const;

    /**
     * @brief 更新变量记录的值
     * @param var 已有变量记录
     * @return true-可更新，false-不能更新，名称或类型不一致
     */
    bool UpdateValue(IN const CodeVar& var);

    /**
     * @brief 根据结构体构造指令
     * @param str 构造得到的指令
     * @return true-构造成功,false-构造失败
     */
    bool CreateCmdByStruct(OUT QString& str);

    /**
     * @brief 初始化变量记录
     * @param str 指令语句
     * @param strMod 模块文件名称
     * @return true-初始化成功,false-初始化失败
     */
    bool InitByString(IN const QString & str, IN const QString & strMod);

    //变量名
    QString name;
    //变量类型
    DataType type;
    //作用域GLOBAL,LOCAL,ROUTINE
    ScopeType scope_type;
    //存储类型var,const,pers
    StorageType storage_type;
    //模块文件
    QString module;
    //数组
    bool is_array;
    //值
    QString value;
};

/**
 * @brief 作用域记录
 */
struct CodeRoutine {
public:
    /**
     * @brief 构造函数，默认local,proc
     */
    CodeRoutine();

    /**
     * @brief 清除记录，恢复默认值
     */
    void Clear();

    /**
     * @brief 更新作用域记录值
     * @param rout 已有作用域记录
     * @return true-更新成功,false-更新失败
     */
    bool UpdateValue(const CodeRoutine& rout);

    /**
     * @brief 根据指令初始化作用域记录
     * @param strcontent 指令
     * @param strMod 模块文件名称
     * @return true-初始化成功,false-初始化失败
     */
    bool InitByString(const QString & strcontent,const QString & strMod);

    //名称
    QString name;
    //作用域GLOBAL,LOCAL,ROUTINE
    ScopeType scope_type;
    //函数类型
    RoutineType type;
    //函数变量列表
    QList<CodeVar> var_list;
    //所属模块
    QString module;
};

/**
 * @brief 根据字符串获取数据类型
 * @param [IN]data_type_str 数据类型字符串
 * @return 数据类型
 */
DataType GetDataTypeByStr(const QString data_type_str);

/**
 * @brief 获取数据类型字符串
 * @param data_type 数据类型
 * @return 数据类型字符串
 */
QString GetDataTypeStr(const DataType data_type);

/**
 * @brief 根据字符串获取命令类型
 * @param [IN] cmd_type 字符串
 * @return 命令类型
 */
InstructionType GetInstructionTypeByStr(const QString cmd_type);

/**
 * @brief 获取命令类型字符串
 * @param [IN] eType 命令类型
 * @return 命令类型字符串
 */
QString GetInstructionTypeStr(const InstructionType eType);

/**
 * @brief 根据字符串获取函数类型
 * @param [IN] type_str 类型字符串
 * @return 作用域类型
 */
RoutineType GetRoutineTypeByStr(const QString type_str);

/**
 * @brief 获取函数类型字符串
 * @param type 作用域类型
 * @return 作用域类型字符串
 */
QString GetRoutineTypeStr(const RoutineType type);

/**
 * @brief 根据作用域类型得到作用域类型字符串
 * @param scope_type 作用域类型
 * @return 作用域类型字符串
 */
QString GetScopeTypeStr(IN const ScopeType scope_type);

/**
 * @brief 根据字符串得到作用域类型
 * @param str 字符串
 * @return 作用域类型
 */
ScopeType GetScopeTypeByStr(const QString str);

/**
 * @brief 根据存储类型得到存储类型字符串
 * @param storage_type 存储类型
 * @return 存储类型字符串
 */
QString GetStorageTypeStr(IN const StorageType storage_type);

/**
 * @brief 根据字符串获取存储类型
 * @param str 字符串
 * @return 存储类型
 */
StorageType GetStorageTypeByStr(const QString str);

//打开文件的历史记录
struct OpenHistoryRecord {
    OpenHistoryRecord() {
        index = 0;
        openModList.clear();
    }
    void clear() {
        index = 0;
        openModList.clear();
    }
    int index;                 //上次打开的页面索引
    QStringList openModList;   //上一次task打开的文件列表
};

#endif // EDITOR_STRUCT_H
