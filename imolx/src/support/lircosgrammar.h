#ifndef LIRCOSGRAMMAR_H
#define LIRCOSGRAMMAR_H

#include "xhelper.h"
#include "basesupporter.h"
#include "lircos/editor_struct.h"

template<int> QString associateVar(ix::Node *n);
template<int> bool resolveVar(const QString &v, ix::Node *n);

class LircosGrammar : public BaseGrammar
{
    Q_OBJECT

    Q_ENUMS(DataType)

public:
    /**
     * @brief 数据类型
     */
    enum DataType {
        DATA_NONE,
        DATA_INT,
        DATA_DOUBLE,
        DATA_BOOL,
        DATA_STRING,
        DATA_TOOL,
        DATA_WOBJ,
        DATA_SPEED,
        DATA_LOAD,//力控8
        DATA_FCBOXVOL,
        DATA_FCSPHEREVOL,
        DATA_FCCYLINDERVOL,
        DATA_FCXYZNUM,
        DATA_FCCARTNUM,
        DATA_POSE,

        DATA_ZONE,
        DATA_USERFRAME,
        DATA_ROBTARGET,
        DATA_JOINTTARGET,
        DATA_TRIGG_DATA,
        DATA_CLOCK,
        DATA_INTNUM,
        DATA_SYNCIDENT,
        DATA_TASKS,
        DATA_BYTE,           //byte
        DATA_TOTAL_NUM
    };

    /**
     * @brief 指令类型
     */
    enum InstructionType {
        INSTRUCTION_NONE,
        INSTRUCTION_MOVEABSJ,
        INSTRUCTION_MOVEJ,
        INSTRUCTION_MOVEL,
        INSTRUCTION_MOVEC,
        INSTRUCTION_MOVET,
        INSTRUCTION_SEARCHL,
        INSTRUCTION_PERCENTSET,
        INSTRUCTION_MOTIONSUP,
        INSTRUCTION_DI,
        INSTRUCTION_DO,
        INSTRUCTION_GI,
        INSTRUCTION_GO,
        INSTRUCTION_AI,
        INSTRUCTION_AO,
        INSTRUCTION_SETDO,
        INSTRUCTION_SETGO,
        INSTRUCTION_SETAO,
        INSTRUCTION_MATH,
        INSTRUCTION_IF,
        INSTRUCTION_WHILE,
        INSTRUCTION_IF_ELSE,
        INSTRUCTION_ELSE,
        INSTRUCTION_WAIT,
        INSTRUCTION_WAITUNTIL,
        INSTRUCTION_CONTINUE,
        INSTRUCTION_PAUSE,
        INSTRUCTION_BREAK,
        INSTRUCTION_EXIT,
        INSTRUCTION_SOCKET_CREATE,
        INSTRUCTION_SOCKET_CLOSE,
        INSTRUCTION_CLEAR_SOCKET_BUFF,
        INSTRUCTION_SOCKET_READ_BYTE,
        INSTRUCTION_SOCKET_READ_BIT,
        INSTRUCTION_SOCKET_SEND_BYTE,
        INSTRUCTION_SOCKET_READ_INT,
        INSTRUCTION_SOCKET_SEND_INT,
        INSTRUCTION_SOCKET_READ_STRING,
        INSTRUCTION_SOCKET_CHKNET,
        INSTRUCTION_SOCKET_SEND_STRING,
        INSTRUCTION_SOCKET_READ_DOUBLE,
        INSTRUCTION_TRIGGIO,
        INSTRUCTION_TRIGGJ,
        INSTRUCTION_TRIGGL,
        INSTRUCTION_TRIGGC,
        INSTRUCTION_CLKSTART,
        INSTRUCTION_CLKSTOP,
        INSTRUCTION_CLKREAD,
        INSTRUCTION_CLKRESET,
        INSTRUCTION_WAITWOBJ,
        INSTRUCTION_DROPWOBJ,
        INSTRUCTION_DEACTUNIT,
        INSTRUCTION_ACTUNIT,
        INSTRUCTION_CONNECT,
        INSTRUCTION_ISIGNALDI,
        INSTRUCTION_RETURN,
        INSTRUCTION_ACCSET,
        INSTRUCTION_TESTANDSET,
        INSTRUCTION_WAITSYNCTASK,
        INSTRUCTION_FC_SUPV_FORCE,   //FCSupvForce
        INSTRUCTION_FC_SUPV_TORQUE,  //FCSupvTorque
        INSTRUCTION_FC_SUPV_POSB,    //FCSupvPosBox
        INSTRUCTION_FC_SUPV_POSS,    //FCSupvPosSphere
        INSTRUCTION_FC_SUPV_POSC,    //FCSupvPosCylinder
        INSTRUCTION_FC_SUPV_ORIENT,  //FCSupvOrient
        INSTRUCTION_FC_SUPV_LINES,   //FCSupvTCPSpeed
        INSTRUCTION_FC_SUPV_ORIENTS, //FCSupvReoriSpeed
        INSTRUCTION_FC_CALIB,        //FCCalib
        INSTRUCTION_FC_ACT,          //FCAct
        INSTRUCTION_FC_DEACT,        //FCDeact

        INSTRUCTION_FC_REFCIRCLE,    //FCRefCircle
        INSTRUCTION_FC_REFLINE,      //FCRefLine
        INSTRUCTION_FC_REFROT,       //FCRefRot
        INSTRUCTION_FC_REFSPIRAL,    //FCRefSpiral
        INSTRUCTION_FC_REFFORCE,     //FCRefForce
        INSTRUCTION_FC_REFTORQUE,    //FCRefTorque
        INSTRUCTION_FC_REFSTART,     //FCRefStart
        INSTRUCTION_FC_REFSTOP,      //FCRefStop
        INSTRUCTION_FC_SETF,         //FCSetSDPara

        INSTRUCTION_FC_CONDFORCE,    //FCCondForce
        INSTRUCTION_FC_CONDPOSB,     //FCCondPosBox
        INSTRUCTION_FC_CONDPOSC,     //FCCondPosCyliner
        INSTRUCTION_FC_CONDPOSS,     //FCCondPosSphere
        INSTRUCTION_FC_CONDORIENTS,  //FCCondReoriSpeed
        INSTRUCTION_FC_CONDORIENT,   //FCCondOrient
        INSTRUCTION_FC_CONDLINES,    //FCCondTCPSpeed
        INSTRUCTION_FC_CONDTORQUE,   //FCCondTorque
        INSTRUCTION_FC_GETCOND,      //FCCondWaitWhile
        INSTRUCTION_FC_LOADID,    //FCLoadID
        INSTRUCTION_BIT_CLEAR,    //BitClear
        INSTRUCTION_BIT_SET,      //BitSet
        INSTRUCTION_PULSE_DO,     //PulseDo

        INSTRUCTION_DEC_TO_HEX,
        INSTRUCTION_HEX_TO_DEC,
        INSTRUCTION_INT_TO_STR,
        INSTRUCTION_INT_TO_BYT,
        INSTRUCTION_DOUBLE_TO_BYT,
        INSTRUCTION_DOUBLE_TO_STR,
        INSTRUCTION_CALC_JOINT_WITH_EULER,
        INSTRUCTION_EULER_TO_QUATERNION,
        INSTRUCTION_QUATERNION_TO_EULER,

        INSTRUCTION_HOME_SET,
        INSTRUCTION_HORDR,
        INSTRUCTION_HOME_DEF,
        INSTRUCTION_HOME_CLR,
        INSTRUCTION_HOME,
        INSTRUCTION_HOME_SET_AT,
        INSTRUCTION_HORDR_AT,

        INSTRUCTION_SEARCHT,

        INSTRUCTION_BOX,
        INSTRUCTION_BOXCLR,
        INSTRUCTION_BOXDEF,
        INSTRUCTION_INSIDEBOX,
        INSTRUCTION_XYLIM,
        INSTRUCTION_XYLIMCLR,
        INSTRUCTION_XYLIMDEF,
        INSTRUCTION_PALLETEND,
        INSTRUCTION_PALLETRESET,
        INSTRUCTION_MPALLETEND,
        INSTRUCTION_MPALLETRESET,
        INSTRUCTION_SERIAL_OPEN,
        INSTRUCTION_SERIAL_CLOSE,
        INSTRUCTION_SERIAL_SEND_STRING,
        INSTRUCTION_SERIAL_READ_STRING,
        INSTRUCTION_SERIAL_SEND_BYTE,
        INSTRUCTION_SERIAL_READ_BYTE,
        INSTRUCTION_SERIAL_READ_BIT,
        INSTRUCTION_CLEAR_SERIAL_BUFF,
        INSTRUCTION_GET_SERIAL_BUFF_SIZE,

        INSTRUCTION_POSEMULT,
        INSTRUCTION_POSEINV,

        INSTRUCTION_TOTAL_NUM
    };

    //作用域类型
    enum ScopeType {
        SCOPE_GLOBAL,
        SCOPE_LOCAL,
        SCOPE_ROUTINE,
        SCOPE_TOTAL_NUM
    };

    //存储类型
    enum StorageType {
        STORAGE_VAR,
        STORAGE_PERS,
        STORAGE_CONST
    };

    //函数类型
    enum RoutineType {
        ROUTINE_PROC,
        ROUTINE_TRAP,
        ROUTINE_TOTAL_NUM
    };


    //FX界面插入标志
    enum FxFlag {
        FX_NONE,     //无
        FX_OFFS,     //插入offs
        FX_RELTOOL   //插入RelTool
    };

public:
    LircosGrammar(QObject *parent = nullptr);

    static QString dataType2String(int dt);
    static int string2DataType(const QString &str);

    bool compileVar(const QString &type, ix::Node *node, QString &statement) override;
    bool decompileVar(const QString &type, const QString &statement, ix::Node *node) override;

private:
    QHash<QString, int> m_var_book;
};

#endif // LIRCOSGRAMMAR_H
