/**
 * Copyright(C) 2012 LithoTek. All Rights Reserved.
 * Information in this file is the intellectual property of LithoTek, and may
 * contains trade secrets that must be stored and viewed confidentially.
 *
 * @file editor_define.h
 * @author: 汤中华
 * @date: 2017-12-14
 * @brief 编辑器宏定义
 */

#ifndef EDTIOR_DEFINE_H
#define EDTIOR_DEFINE_H

#include <QString>
#include <QStringList>
#include <QObject>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define     SYSTEM_SCOPE            ( QString("$") )
#define     KEY_GLOBAL              ( QString("GLOBAL") )
#define     KEY_LOCAL               ( QString("LOCAL") )
#define     KEY_ROUTINE             ( QString("ROUTINE") )
#define     KEY_ALL                 ( QString("ALL") )

#define     KEY_VAR                 ( QString("VAR") )
#define     KEY_PERS                ( QString("PERS") )
#define     KEY_CONST               ( QString("CONST") )

#define     KEY_PROC                ( QString("PROC"))
#define     KEY_TRAP                ( QString("TRAP"))


#define     CODE_LINE_SPERATOR      ( QString("\n") )
#define     CODE_SPACE              ( QString(" ") )
#define     CODE_MINUS              ( QString("-") )
#define     CODE_EQUAL              ( QString("=") )
#define     CODE_COMMA              ( QString(",") )
#define     CODE_PAUSE              ( QString(".") )
#define     CODE_VAR                ( QString("VAR") )
#define     CODE_INSTR              ( QString("$") )
#define     CODE_COLON              ( QString(":") )
#define     CODE_BLANKSPACE         ( QString(" ") )
#define     CODE_TAB                ( QString("\t"))

#define     KEY_MOVEABSJ            ( QString("MoveAbsJ") )
#define     KEY_MOVEJ               ( QString("MoveJ") )
#define     KEY_MOVEC               ( QString("MoveC") )
#define     KEY_MOVEL               ( QString("MoveL") )
#define     KEY_MOVET               ( QString("MoveT") )
#define     KEY_SEARCHL             ( QString("SearchL"))
#define     KEY_PERCENTSET          ( QString("PercentSet"))
#define     KEY_MOTIONSUP           ( QString("MotionSup"))
#define     KEY_START_ROKAE_CONNECT ( QString("StartRokaeConnect"))
#define     KEY_ACCSET              ( QString("AccSet"))
#define     KEY_TESTANDSET          ( QString("TestAndSet"))
#define     KEY_WAITSYNCTASK        ( QString("WaitSyncTask"))
#define     KEY_TRIGGIO             ( QString("TriggIO"))
#define     KEY_TRIGGJ              ( QString("TriggJ"))
#define     KEY_TRIGGL              ( QString("TriggL"))
#define     KEY_TRIGGC              ( QString("TriggC"))
#define     KEY_CALCDISTANCE        ( QString("CalcDistance"))
//力控
#define     KEY_FCSUPVFORCE         ( QString("FCSupvForce"))
#define     KEY_FCSUPVTORQUE        ( QString("FCSupvTorque"))
#define     KEY_FCSUPVPOSBOX        ( QString("FCSupvPosBox"))
#define     KEY_FCSUPVPOSSPHERE     ( QString("FCSupvPosSphere"))
#define     KEY_FCSUPVPOSCYLINDER   ( QString("FCSupvPosCylinder"))
#define     KEY_FCSUPVTCPSPEED      ( QString("FCSupvTCPSpeed"))
#define     KEY_FCSUPVREORISPEED    ( QString("FCSupvReoriSpeed"))
#define     KEY_FCSUPVORIENT        ( QString("FCSupvOrient"))
#define     KEY_FCCALIB             ( QString("FCCalib"))
#define     KEY_FCACT               ( QString("FCAct"))
#define     KEY_FCDEACT             ( QString("FCDeact"))
#define     KEY_FCRESETBIAS           ( QString("FCResetBias"))
#define     KEY_FCSETREFMOVERAMP        ( QString("FCSetRefMoveRamp"))

#define     KEY_FCREFCIRCLE         ( QString("FCRefCircle"))
#define     KEY_FCREFLINE           ( QString("FCRefLine"))
#define     KEY_FCREFROT            ( QString("FCRefRot"))
#define     KEY_FCREFSPIRAL         ( QString("FCRefSpiral"))
#define     KEY_FCREFFORCE          ( QString("FCRefForce"))
#define     KEY_FCREFTORQUE         ( QString("FCRefTorque"))
#define     KEY_FCREFSTART          ( QString("FCRefStart"))
#define     KEY_FCREFSTOP           ( QString("FCRefStop"))
#define     KEY_FCSETSDPARA         ( QString("FCSetSDPara"))

#define     KEY_FCCONDFORCE         ( QString("FCCondForce"))
#define     KEY_FCCONDPOSBOX        ( QString("FCCondPosBox"))
#define     KEY_FCCONDPOSCYLINDER   ( QString("FCCondPosCylinder"))
#define     KEY_FCCONDPOSSPHERE     ( QString("FCCondPosSphere"))
#define     KEY_FCCONDREORISPEED    ( QString("FCCondReoriSpeed"))
#define     KEY_FCCONDORIENT        ( QString("FCCondOrient"))
#define     KEY_FCCONDTCPSPEED      ( QString("FCCondTCPSpeed"))
#define     KEY_FCCONDTORQUE        ( QString("FCCondTorque"))
#define     KEY_FCCONDWAITWHILE     ( QString("FCCondWaitWhile"))

#define     KEY_FCLOADID            ( QString("FCLoadID"))

#define     KEY_BIT_CLEAR           ( QString("BitClear"))
#define     KEY_BIT_SET             ( QString("BitSet"))
#define     KEY_BIT_AND             ( QString("BitAnd"))
#define     KEY_BIT_CHECK           ( QString("BitCheck"))
#define     KEY_BIT_LSH             ( QString("BitLSh"))
#define     KEY_BIT_NEG             ( QString("BitNeg"))
#define     KEY_BIT_OR              ( QString("BitOr"))
#define     KEY_BIT_RSH             ( QString("BitRSh"))
#define     KEY_BIT_XOR             ( QString("BitXOr"))
#define     KEY_BYTE_TO_STR         ( QString("ByteToStr"))

#define     KEY_STR_FIND            ( QString("StrFind"))
#define     KEY_STR_LEN             ( QString("StrLen"))
#define     KEY_STR_MAP             ( QString("StrMap"))
#define     KEY_STR_MATCH           ( QString("StrMatch"))
#define     KEY_STR_MEMB            ( QString("StrMemb"))
#define     KEY_STR_ORDER           ( QString("StrOrder"))
#define     KEY_STR_PART            ( QString("StrPart"))
#define     KEY_STR_TO_BYTE         ( QString("StrToByte"))
#define     KEY_STR_EQUAL           ( QString("StrEqual"))
#define     KEY_STR_COMBINE         ( QString("StrCombine"))
#define     KEY_STR_CLEARSPACE      ( QString("ClearSpace"))

#define     KEY_CLKSTART            ( QString("ClkStart"))
#define     KEY_CLKSTOP             ( QString("ClkStop"))
#define     KEY_CLKRESET            ( QString("ClkReset"))
#define     KEY_CLKREAD             ( QString("ClkRead"))
#define     KEY_ACTUNIT             ( QString("ActUnit"))
#define     KEY_DEACTUNIT           ( QString("DeactUnit"))
#define     KEY_WAITWOBJ            ( QString("WaitWObj"))
#define     KEY_DROPWOBJ            ( QString("DropWObj"))

#define     KEY_CONFL               ( QString("ConfL"))
#define     KEY_WOBJANGLE           ( QString("WobjAngle"))
#define     KEY_WOBJIDENTIFIER      ( QString("WobjIdentifier"))

#define     KEY_POSEMULT            ( QString("PoseMult"))
#define     KEY_POSEINV             ( QString("PoseInv"))

#define     KEY_MAIN                ( QString("main") )
#define     KEY_MATH                ( QString("math") )
#define     KEY_OPERATOR            ( QString("operator") )
#define     KEY_PROCESS             ( QString("process") )
#define     KEY_MODULE              ( QString("MODULE"))
#define     KEY_ENDMODULE           ( QString("ENDMODULE"))
#define     KEY_PROC                ( QString("PROC"))
#define     KEY_ENDPROC             ( QString("ENDPROC"))
#define     KEY_FUNC                ( QString("FUNC"))
#define     KEY_ENDFUNC             ( QString("ENDFUNC"))
#define     KEY_TRAP                ( QString("TRAP"))
#define     KEY_ENDTRAP             ( QString("ENDTRAP"))
#define     KEY_INSTR_CONNECT       ( QString("CONNECT"))
#define     KEY_ISIGNALDI           ( QString("ISignalDI"))
#define     KEY_RETURN              ( QString("RETURN"))

#define     KEY_INT                 (QString("int"))
#define     KEY_BYTE                (QString("byte"))
#define     KEY_DOUBLE              (QString("double"))
#define     KEY_BOOL                (QString("bool"))
#define     KEY_STRING              (QString("string"))
#define     KEY_SPEED               ( QString("speed") )
//力控相关
#define     KEY_FCBOXVOL            ( QString("fcboxvol"))
#define     KEY_FCSPHEREVOL         ( QString("fcspherevol"))
#define     KEY_FCCYLINDERVOL       ( QString("fccylindervol"))
#define     KEY_FCXYZNUM            ( QString("fcxyznum"))
#define     KEY_FCCARTNUM           ( QString("fccartnum"))
#define     KEY_POSE                ( QString("pose"))

#define     KEY_ZONE                ( QString("zone") )
#define     KEY_TOOL                ( QString("tool") )
#define     KEY_WOBJ                ( QString("wobj") )
#define     KEY_USERFRAME           ( QString("userframe"))
#define     KEY_JOINTTARGET         ( QString("jointtarget") )
#define     KEY_ROBTARGET           ( QString("robtarget") )
#define     KEY_TRIGGDATA           ( QString("triggdata"))
#define     KEY_CLOCK               ( QString("clock"))
#define     KEY_INTNUM              ( QString("intnum"))
#define     KEY_LOAD                (QString("load"))
#define     KEY_SYNCIDENT           (QString("syncident"))
#define     KEY_TASKS               (QString("tasks"))

#define     KEY_IF                  (QString("IF"))
#define     KEY_IF_ELSE             (QString("IF/ELSE"))
#define     KEY_ELSE                (QString("ELSE"))
#define     KEY_WHILE               (QString("WHILE"))
#define     KEY_WAIT                (QString("Wait"))
#define     KEY_WAITUNTIL           (QString("WaitUntil"))
#define     KEY_CONTINUE            (QString("Continue"))
#define     KEY_INSTR_PAUSE               (QString("Pause"))
#define     KEY_INSTR_BREAK               (QString("Break"))
#define     KEY_INSTR_EXIT                (QString("EXIT"))

#define     KEY_TRUE                (QString("true"))
#define     KEY_FALSE               (QString("false"))
#define     KEY_DI                  (QString("$DI"))
#define     KEY_DO                  (QString("$DO"))
#define     KEY_AI                  (QString("$AI"))
#define     KEY_AO                  (QString("$AO"))
#define     KEY_SETDO               (QString("SetDO"))
#define     KEY_SETGO               (QString("SetGO"))
#define     KEY_SETAO               (QString("SetAO"))
#define     KEY_PULSE_DO            (QString("PulseDO"))

#define     SHOW   QString("+")
#define     HIDE   QString("-")

#define     KEY_SOCKET_CREATE         (QString("SocketCreate"))
#define     KEY_SOCKET_CLOSE          (QString("SocketClose"))
#define     KEY_SOCKET_READ_BYTE       (QString("SocketReadByte"))
#define     KEY_SOCKET_READ_BIT       (QString("SocketReadBit"))
#define     KEY_SOCKET_SEND_BYTE      (QString("SocketSendByte"))
#define     KEY_SOCKET_READ_INT       (QString("SocketReadInt"))
#define     KEY_SOCKET_SEND_INT       (QString("SocketSendInt"))
#define     KEY_SOCKET_READ_STRING    (QString("SocketReadString"))
#define     KEY_SOCKET_SEND_STRING    (QString("SocketSendString"))
#define     KEY_SOCKET_SEND_STRING_LINE    (QString("SocketSendStringLine"))
#define     KEY_SOCKET_READ_DOUBLE    (QString("SocketReadDouble"))
#define     KEY_SOCKET_CHKNET         (QString("ChkNet"))
#define     KEY_CLEAR_SOCKET_BUFF     (QString("ClearSocketBuff"))

#define     KEY_LEFT_SLASH            (QString("/"))

#define     KEY_QUATERNION_TO_EULER    (QString("QuaternionToEuler"))
#define     KEY_EULER_TO_QUATERNION    (QString("EulerToQuaternion"))
#define     KEY_DEC_TO_HEX             (QString("DecToHex"))
#define     KEY_HEX_TO_DEC             (QString("HexToDec"))
#define     KEY_INT_TO_STR             (QString("IntToStr"))
#define     KEY_INT_TO_BYT             (QString("IntToByte"))
#define     KEY_DOUBLE_TO_BYT          (QString("DoubleToByte"))
#define     KEY_DOUBLE_TO_STR          (QString("DoubleToStr"))
#define     KEY_CALCJOINT_WITH_EULER   (QString("CalcJoinTWithEuler"))
//Home
#define     KEY_HOME_SET               (QString("HomeSet"))
#define     KEY_HORDR                  (QString("Hordr"))
#define     KEY_HOME_DEF               (QString("HomeDef"))
#define     KEY_HOME_CLR               (QString("HomeClr"))
#define     KEY_HOME                   (QString("Home"))
#define     KEY_HOME_SET_AT            (QString("HomeSetAt"))
#define     KEY_HORDR_AT               (QString("HordrAt"))
//检测区域
#define     KEY_BOX                    (QString("Box"))
#define     KEY_BOXCLR                 (QString("BoxClr"))
#define     KEY_BOXDEF                 (QString("BoxDef"))
#define     KEY_INSIDEBOX              (QString("InsideBox"))
//安全区域
#define     KEY_XYLIM                  (QString("XYLim"))
#define     KEY_XYLIMCLR               (QString("XYLimClr"))
#define     KEY_XYLIMDEF               (QString("XYLimDef"))
//码垛
#define     KEY_PALLETEND              (QString("PalletEnd"))
#define     KEY_PALLETRESET            (QString("PalletReset"))
#define     KEY_MPALLETEND              (QString("MaterialPalletEnd"))
#define     KEY_MPALLETRESET            (QString("MaterialPalletReset"))

//串口
#define     KEY_SERIAL_OPEN              (QString("SerialOpen"))
#define     KEY_SERIAL_CLOSE              (QString("SerialClose"))
#define     KEY_SERIAL_SEND_STRING              (QString("SerialSendString"))
#define     KEY_SERIAL_READ_STRING              (QString("SerialReadString"))
#define     KEY_SERIAL_SEND_BYTE              (QString("SerialSendByte"))
#define     KEY_SERIAL_READ_BYTE              (QString("SerialReadByte"))
#define     KEY_SERIAL_READ_BIT              (QString("SerialReadBit"))
#define     KEY_CLEAR_SERIAL_BUFF              (QString("ClearSerialBuff"))
#define     KEY_GET_SERIAL_BUFF_SIZE              (QString("GetSerialBuffSize"))

//#define     KEY_RIGHT_SLASH           (QString("\"))
#define     KEY_LEFT_PARENTHESES      (QString("("))
#define     KEY_LEFT_RBACKETS         (QString("["))
#define     KEY_RIGHT_PARENTHESES     (QString(")"))
#define     KEY_RIGHT_RBACKETS        (QString("]"))
#define     KEY_LEFT_QUOTES           (QString("\""))
#define     KEY_RIGHT_QUOTES          (QString("\""))
#define     KEY_DOUBLE_BACKSLASH             (QString("\\"))

#define     KEY_OFFS                  (QString("Offs"))
#define     KEY_RELTOOL               (QString("RelTool"))

#define     KEYWORD_SEPARATOR          (QString(","))

#define SCE_C_WORD          (5)
#define SCE_C_WORD2         (16)
#define SCE_C_STRING        (6)
#define SCE_C_CHARACTER     (7)
#define SCE_C_PREPROCESSOR  (9)
#define SCE_C_COMMENT       (1)
#define SCE_C_COMMENTLINE   (2)
#define SCE_C_COMMENTDOC    (3)
#define MARGIN_FOLD_INDEX   (2)

// Module
static const QString MODULE_PATTERN = QString("(^\\s*\\n*MODULE(\\s+([A-Za-z_][A-Za-z_0-9]*){1})\\s*.*ENDMODULE\\s*\\n*$)");
// Proc
static const QString PROC_PATTERN = QString("\\s*((GLOBAL|LOCAL)\\s+)?PROC\\s+([A-Za-z_][A-Za-z_0-9]*){1}\\(\\)\\s*.*ENDPROC\\s*");
//指令
static const QString INSTRUCTION_PATTERN = QString("\\s*((GLOBAL|LOCAL)\\s+)?(PROC|TRAP|FUNC)\\s+([A-Za-z_][A-Za-z_0-9]*){1}(\\(.*\\))?\\s*.*ENDMODULE\\s*\\n*$");

//作用域
static const QString ROUTINE_PATTERN = QString("\\s*((GLOBAL|LOCAL)\\s+)?(PROC|TRAP|FUNC)\\s+([A-Za-z][A-Za-z_0-9]*){1}\\s*.*$");
// Trap
static const QString TRAP_PATTERN = QString("\\s*((GLOBAL|LOCAL)\\s+)?TRAP\\s+([A-Za-z_][A-Za-z_0-9]*){1}\\s*.*ENDTRAP(\\s)*");
//label
static const QString LABEL_PATTERN = QString("\\s*([A-Za-z_][A-Za-z_0-9]*[ ]{0,1}[:]){1}\\s*");

//声明区
static const QString STATEMENT_AREA = QString("\\s*MODULE(\\s+([A-Za-z_][A-Za-z_0-9]*){1})(\\s)*.*((GLOBAL|LOCAL)\\s+)?(PROC|TRAP|FUNC)\\s+([A-Za-z_][A-Za-z_0-9]*){1}(\\(.*\\))?\\s*");

// While
static const QString WHILE_PATTERN = QString("(^\\s*While\\s*\\(.*\\)\\s*$)");
//endwhile
static const QString ENDWHILE_PATTERN = QString("(^\\s*EndWhile\\s*$)");

/// If
#define IfPattern         (QString("(^\\s*If\\s*\\(.*\\)\\s*$)"))
#define ElsePattern       (QString("(^\\s*Else\\s*$)"))
#define EndIfPattern      (QString("(^\\s*EndIf\\s*$)"))

/// Loop control
#define LoopCtrlPattern   (QString("(^\\s*Break|Continue\\s*$)"))

/// Blank line
#define BlankLinePattern  (QString("(^\\s*$)"))
//变量类型
static const QString VAR_TYPE = QString("int|bool|string|double|robtarget|jointtarget|tool|wobj|speed|zone|userframe|clock|triggdata|intnum|syncident|tasks|load|fcboxvol|fcspherevol|fccylindervol|fcxyznum|fccartnum|pose|byte");
static const QString VAR_DECL_PATTERN = QString("(^\\s*((GLOBAL|LOCAL)\\s+)?((VAR|CONST|PERS)\\s+)?(%1)\\s+)(([A-Za-z_][A-Za-z_0-9]*){1}\\s*.*$)").arg(VAR_TYPE);
//子函数变量正则表达式
#define RoutineVarPattern (QString("(^\\s*((VAR|CONST|PERS)\\s+)?[%1]\\s+)(([A-Za-z_][A-Za-z_0-9]*){1}\\s*.*$)").arg(VAR_TYPE))

//double类型的正则表达式
static const QString DoublePattern = QString("-?([1-9]\\d*.\\d*|0.\\d*[1-9]\\d*|0?.0+|0)");
//robottarget变量声明格式
static const QString PartTargetPattern = QString("\\s*([-+]?[0-9]*\\.?[0-9]*){1}\\s*");
static const QString RobtargetPattern = QString("p:\\{\\s*\\{(%1\\,){2}(%1){1}\\}\\s*\\,\\s*\\{(%1\\,){3}(%1){1}\\}\\s*\\}\\s*\\{\\s*cfg(%1\\,){3}(%1){1}\\}\\s*\\{\\s*EJ(%1\\,){5}(%1){1}\\}").arg(PartTargetPattern);
//jpinttarget变量声明格式
static const QString JointtargetPattern = QString("j:\\{(%1\\,){5}(%1){1}\\}\\s*\\{\\s*EJ(%1\\,){5}(%1){1}\\}").arg(PartTargetPattern);
//运动指令中的offs偏移量的正则表达式
static const QString OffsPattern = QString("Offs\\(\\s*([A-Za-z_][A-Za-z_0-9]*){1}\\s*,(\\s*(([-+]?[0-9]*\\.?[0-9]*)|([A-Za-z_0-9]*))\\s*,){2}\\s*(([-+]?[0-9]*\\.?[0-9]*)|([A-Za-z_0-9]*))\\s*\\)");
//offs语句(其中位置点为声明格式)
static const QString OffsPattern_1 = QString("Offs\\(\\s*(%1){1}\\s*,(\\s*([-+]?[0-9]*\\.?[0-9]*){1}\\s*,){2}\\s*([-+]?[0-9]*\\.?[0-9]*){1}\\s*\\)").arg(RobtargetPattern);
//运动指令中的reltool偏移量的正则表达式
static const QString ReltoolPattern = QString("RelTool\\(\\s*([A-Za-z_][A-Za-z_0-9]*){1}\\s*,(\\s*(([-+]?[0-9]*\\.?[0-9]*)|([A-Za-z_0-9]*))\\s*,){5}\\s*(([-+]?[0-9]*\\.?[0-9]*)|([A-Za-z_0-9]*))\\s*\\)");
//reltool语句(其中位置点为声明格式)
static const QString ReltoolPattern_1 = QString("RelTool\\(\\s*(%1){1}\\s*,(\\s*([-+]?[0-9]*\\.?[0-9]*){1}\\s*,){5}\\s*([-+]?[0-9]*\\.?[0-9]*){1}\\s*\\)").arg(RobtargetPattern);

//批量修改中语句的匹配规则，不可用于其他地方      **重要**
//robottarget变量声明格式
static const QString PartTargetPattern_batchModify = QString("\\s*(\\S*){1}\\s*");
static const QString RobtargetPattern_batchModify = QString("p:\\{\\s*\\{(%1\\,){2}(%1){1}\\}\\s*\\,\\s*\\{(%1\\,){3}(%1){1}\\}\\s*\\}\\s*\\{\\s*cfg(%1\\,){3}(%1){1}\\}\\s*\\{\\s*EJ(%1\\,){5}(%1){1}\\}").arg(PartTargetPattern_batchModify);
//jointtarget变量声明格式
static const QString JointtargetPattern_batchModify = QString("j:\\{(%1\\,){5}(%1){1}\\}\\s*\\{\\s*EJ(%1\\,){5}(%1){1}\\}")
        .arg(PartTargetPattern_batchModify);
//运动指令中的offs偏移量的正则表达式
static const QString OffsPattern_batchModify = QString("Offs\\(\\s*([A-Za-z_][A-Za-z_0-9]*){1}\\s*,(\\s*(%1){1}\\s*,){2}\\s*(%1){1}\\s*\\)")
        .arg(PartTargetPattern_batchModify);
//offs语句(其中位置点为声明格式)
static const QString OffsPattern_1_batchModify = QString("Offs\\(\\s*(%1){1}\\s*,(\\s*(%2){1}\\s*,){2}\\s*(%2){1}\\s*\\)")
        .arg(RobtargetPattern_batchModify)
        .arg(PartTargetPattern_batchModify);
//运动指令中的reltool偏移量的正则表达式
static const QString ReltoolPattern_batchModify = QString("RelTool\\(\\s*([A-Za-z_][A-Za-z_0-9]*){1}\\s*,(\\s*(%1){1}\\s*,){5}\\s*(%1){1}\\s*\\)")
        .arg(PartTargetPattern_batchModify);
//reltool语句(其中位置点为声明格式)
static const QString ReltoolPattern_1_batchModify = QString("RelTool\\(\\s*(%1){1}\\s*,(\\s*(%2){1}\\s*,){5}\\s*(%2){1}\\s*\\)")
        .arg(RobtargetPattern_batchModify);

//变量命令规则
static const QString VarNameParttern = QString("[A-Za-z_][A-Za-z_0-9]*");
//常规的MoveAbsj语句
static const QString MoveAbsjPattern = QString("^\\s*MoveAbsj\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的MoveJ语句
static const QString MoveJPattern = QString("^\\s*MoveJ\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的MoveJ语句
static const QString MoveLPattern = QString("^\\s*MoveL\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的MoveC语句
static const QString MoveCPattern = QString("^\\s*MoveC\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//辅助点包含声明式的robtarget值的,MoveC,TriggC,MoveT语句前半段
static const QString MoveCPattern_1 = QString("^\\s*(MoveC|TriggC|MoveT)\\s+((%2)|(%3)|(%4))\\s*\\,\\s*(%1)\\s*\\,")
        .arg(VarNameParttern)
        .arg(RobtargetPattern)
        .arg(OffsPattern_1)
        .arg(ReltoolPattern_1);
//目标点包含声明式的robtarget值的,MoveC,TriggC,MoveT语句前半段
static const QString MoveCPattern_2 = QString("^\\s*(MoveC|TriggC|MoveT)\\s+(%1)\\s*\\,\\s*((%2)|(%3)|(%4))\\s*")
        .arg(VarNameParttern)
        .arg(RobtargetPattern)
        .arg(OffsPattern_1)
        .arg(ReltoolPattern_1);
//常规的MoveT语句
static const QString MoveTPattern = QString("^\\s*MoveT\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*([0-9]+(?:\\.[0-9]+)?)\\s*\\,\\s*([0-9]+(?:\\.[0-9]+)?)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的TriggJ
static const QString TriggJPattern = QString("^\\s*TriggJ\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的TriggL
static const QString TriggLPattern = QString("^\\s*TriggL\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规的TriggC
static const QString TriggCPattern = QString("^\\s*TriggC\\s+(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
//常规SearchL
static const QString SearchLPattern = QString("^\\s*SearchL\\s+(\\\\(%1))?\\s*(%1)\\s*(\\\\(%1))?\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)\\s*\\,\\s*(%1)(\\,\\s*(%1))?\\s*$").arg(VarNameParttern);
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


#endif // EDTIORDEFINE_H
