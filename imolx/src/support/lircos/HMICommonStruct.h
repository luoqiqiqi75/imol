#ifndef HMICOMMONSTRUCT_H
#define HMICOMMONSTRUCT_H

#include <stdint.h>
#include <string>

#include <QMetaType>
#include <QHash>
#include <QTime>

using std::string;

#include "HMICommonDefine.h"

#pragma pack(push)
#pragma pack(4)

class QMessageBox;

struct HMICmd{
    uint16_t frame_number;                      // 帧号
    uint16_t cmd_solicited_type;                // 命令类型
    uint16_t cmd;                               // 命令
    uint16_t cmd_extension;                     // 命令扩展信息
    uint16_t data_length;                       // 数据长度
    char data[ DATA_PACKAGE_SIZE - HMI_DATA_PACKAGE_HEADER_SIZE ];      // 数据信息
};

// HMI设置信息
struct HMISettingInfo {
    unsigned int language;                // 语言
    // 服务端信息
    QString strServerIP;                        // 服务端地址
    unsigned short usServerPort;                // 服务端端口
    // 窗口布局
    unsigned int uiTaskBarWidth;                // 任务栏的宽度
    unsigned int uiSystemBarHeight;             // 系统栏的高度
    unsigned int uiJogBarWidth;                 // JOG栏的宽度
    // 数据库目录
    QString strDbRootPath;                      //　数据库目录
    //project info
    QString strRootPath;
    int     nChannelNum;
    int     nCurrentChannelNum;     //通道编号
    //Editor info
    //字体大小比例
    int     nZoomTo;
    QString strFamily;
    //配置实体按键，４个自定义按键
    int customedKey[4];
    // 以下数据不会保存到配置文件中
    UserLevel eUserLevel;                       // 用户级别
    OperatingMode eOperMode;                    // 操作模式
    ROBOT_State eMotorState;                    // 电机加电状态
    ProgExecState eProgExecState;               // 程序的运行状态
    ProgStatusCause program_status_reason;      // 程序状态的原因

    bool is_jog_state;                          //判断当前是否是jog状态

    ProgRunMode eSentenceExecMode;              // 语句执行模式
    ProgDirMode eProgDirMode;                   // 程序运行方向
    ProgLoopMode eProgramExecMode;              // 程序执行模式

    bool is_hmi_started;                        //标记hmi是否启动，默认是否
    IncrementalMode eIncrementalMode;           // 增量模式
    int iToolId;                                // 工具坐标系id
    int iWobjId;                                // 工件坐标系id
    int iRateJog;                               // Jog执行速率
    int iRateProg;                              // 程序执行速率
    int iDIBroadCnt;                            // DI/IO板个数
    int iDOBroadCnt;                            // DO板个数
    int iCurMechUnitIndex;                      // 当前的机械单元索引
    bool bVrepMode;                             // 是否已启动vrep, true-vrep已连接，false-vrep未连接
    QString strRobotType;                       //机器人类型
    bool is_xs_pcb;                                 //是否是xs类型机器人
    int pcb_aix;                              //是否是pcb4轴机器人
    DynamicParametersType robot_dynamic_parameters_type;      //运动规划时候使用的机器人动力学参数类型
    int brightness;                             //屏幕亮度
    bool bLoadIdent;                            //参数辨识状态
    QString strCoorIconPath;                    //右下角坐标系按钮图标路径
    AxisMode axis_mode;                         //记录当前的坐标系
    MechanicalUnit mechancial_unit;             //记录当前机械单元
    QString HMIVersion;                         //HMI版本号
    QString RCVersion;                          //RC版本号
    QStringList programmable_keys;              //可编程按键列表

    bool bInterfaceStateTrack;                           //导轨界面灰显状态，用于手动/自动切换的记录上一次状态
    bool bInterfaceStateDragTeach;                       //拖动示教界面灰显状态，用于手动/自动切换的记录上一次状态
    bool bInterfaceStateCollisionDetection;              //碰撞检测界面灰显状态，用于手动/自动切换的记录上一次状态
    bool bInterfaceStateConveyor;                        //传送带界面灰显状态，用于手动/自动切换的记录上一次状态
    bool bInterfaceCooperativeMode;                                //协作模式界面灰显状态，用于手动/自动切换的记录上一次状态

    bool bDyanmicIdentFlag;                     //本体参数辨识状态标志
    ROBOT_State eRobotState;                    //机器人状态
    bool robot_lock_state;                      //机器人锁定状态,true锁机，false解锁
    QString strCurrentProject;                  //记录当前加载的工程
    QString publishSocketName;                  //状态发布socket名称
    bool is_vrep_connect;                       //vrep是否是连接的
    bool softLimit;                               //软限位使能开关
    InstallType strInstallType;                   //机器人安装类型   0：地面安装，1：吊装
    double m_max_line_speed;                                 //最大线速度
    double m_max_rotation_speed;                             //最大旋转速度
    int version_type;                      //控制器版本  0：REER安全模块，1：ROKAE自研安全模块
    int control_cycle;                     //控制器周期
    int servo_connect;                      //伺服接通是否使用
    //功能是否授权的指示变量
    bool tabWgtSecurityEnabled;              //1.碰撞检测是否授权
    bool WgtLeadThroughProEnabled;           //2.拖动示教是否授权
    bool wgtProfinetEnabled;                 //3.Profinet是否授权
    bool wgtEthernetIpEnabled;               //4.Ethernet是否授权
    bool wgtStatePublishEnabled;             //5.状态发布是否授权
    bool wgtTrackParaEnabled;                //6.导轨是否授权
    bool wgtConveyorEnabled;                 //7.传送带是否授权
    bool wgtCooperativeModeEnabled;          //8.协作模式是否授权
    bool is_fc_authorized;                   //9.力控模式是否授权
    bool multi_tasking_state;                //11.多任务是否授权,除了授权码使用这一变量，也从RC可以直接获取这个授权功能
    bool is_rokae_connect_authorized;        //12.rokae.connect是否授权，true--授权，false--不授权
    bool license_synergistic_sawing;         //13.在线协同缝制是否授权
    bool license_modbus;                    //14.modbus功能是否授权，true--授权，fales--不授权

    //功能是否真正开启及相关检测功能是否真正开启检测的变量
    bool bStatePathCollisionDetection;       //1.路径碰撞检测状态
    bool bStateJogCollisionDetection;        //1.jog碰撞检测状态
    bool bLeadThroughProgFlag;               //2.拖动示教标志，ture:拖动示教启用   false：拖动示教关闭
    MotorControlMode eCurrentMotorMode;      //2.当前的电机控制模式（拖动示教）
    bool b_track_state;                      //6.导轨的状态
    int iCurAxisCnt;                         //6.当前轴个数
    int iCurExtendedAxisCnt;                 //6.当前扩展轴个数
    bool btnConveyorState;                   //7.传送带的状态
    bool b_cooperative_mode;                 //8.协作模式是否开启的状态
    bool b_cooperative_mode_second;          //8.协作模式第二级的状态
    bool fc_state;                           //9.力控开启的状态
    bool is_rokae_connect_open;              //10.rokae.connect是否开启，true--开启，false--失败

    bool monitor_flag;  //状态监控的标志位
    bool HMI_coder_flag; //项目管理标志位

    ControllerAdaptation  controller_type;    //是用的哪家的控制器

    //开机需要插入的日志号，恢复出厂值设定、恢复
    QStringList log_number_to_insert;

    bool task_list_changed;                  //多任务情况，标志task列表是否改变；单任务的情况，标志是否改变加载工程

    bool is_current_load_project_changed;    //判断当前加载的工程是否改变

    bool is_current_io_simulation_mode;      //当前的io模式是否是仿真模式，true--仿真模式，false--实际模式


    bool is_hmi_using;                      //是否使用hmi


    bool is_simulation_mode;                     //hmi是仿真模式还是实际模式

    EulerAngularSequence angular_sequence;        //欧拉角顺序
    //屏幕是否进入屏幕校准
    bool is_calibrate_screen_state;
    bool analog_input_and_out_enabled;          //模拟输入输出是否使能
};

//用户信息
struct User {
    User()
    {
        memset(this, 0, sizeof(User));
    }
    //用户id
    int id;
    //用户级别
    UserLevel level;
    //用户名
    char name[MAX_USER_NAME_LENGTH];
    //用户密码
    char password[MAX_USER_PWD_LENGTH];
};

// 报警信息
struct AlarmInfo{
    int alarm_code;                             // 报警码
    int alarm_code_extension;                   // 子码
};

struct RunToCursorData {
    int task_id;
    char file_name[MAX_FILE_NAME_LENGTH + 1];
    int  line_num;
};

// IO状态
struct IOState{
    IOType type;                                // IO类型
    AnalogType analog_type;                     // IO类型为AI，AO时有意义
    int io_no;                                  // IO号
    bool digital_value;                         // 数字信号值
    double analog_value;                        // 模拟信号值
};

// DI/DO信号值
struct SingleDIOData{
    int io_no;                  // IO号
    bool digital_value;         // 数字信号值
};

// DI data
struct DIDatas{
    SingleDIOData di_datas[ MAX_DI_BOARD * DI_NUM_PER_BOARD ];
};

// DO data
struct DODatas{
    SingleDIOData do_datas[ MAX_DO_BOARD * DO_NUM_PER_BOARD  ];        // 数字信号值
};

// 程序指针
struct ProgramPointer
{
    ProgramPointer()
    {
        memset(this, 0, sizeof(ProgramPointer));
    }

    // 运动指针所在文件路径
    char moved_line_mod[MAX_FILE_NAME_LENGTH + 1];
    // 前瞻指针所在文件路径
    char advanced_line_mod[MAX_FILE_NAME_LENGTH + 1];
    // 运动指针所在行号
    int move_line_num;
    // 前瞻指针所在行号
    int advanced_line_num;
    // 标识语句执行进度，目前分2档，0表示开始，1表示结束
    int progress;
};

struct RobDimensions  {
    double L12x;        // 1，2轴沿1轴轴坐标系x方向距离
    double L12z;        // 1，2轴沿1轴轴坐标系z方向距离
    double L23z;        // 2，3轴沿2轴轴坐标系z方向距离
    double L34mx;       // 3，4轴沿3轴轴坐标系-x方向距离
    double L34y;        // 3，4轴沿3轴轴坐标系y方向距离
    double L34z;        // 3，4轴沿3轴轴坐标系z方向距离
    double L45z;        // 4，5轴沿3轴轴坐标系z方向距离
    double L56z;        // 5，6轴沿5轴轴坐标系z方向距离
};

//点坐标
struct Pos{
    Pos(): X(0), Y(0), Z(0) {}
    double X;
    double Y;
    double Z;
};
//坐标系方向
struct Orient{
    Orient(): A(0), B(0), C(0), Q1(1), Q2(0), Q3(0), Q4(0) {}
    double A;
    double B;
    double C;
    double Q1;
    double Q2;
    double Q3;
    double Q4;
};
//坐标系
struct Frame{
    Frame(): id(0) {
    }
    int id;
    Pos origin;
    Orient ori;
};

// 浮点型轴数据
struct JointData{
    double joint_1;
    double joint_2;
    double joint_3;
    double joint_4;
    double joint_5;
    double joint_6;
};


// 整型轴数据
struct JointInt{
    int joint_1;
    int joint_2;
    int joint_3;
    int joint_4;
    int joint_5;
    int joint_6;
};

// 轴信息
struct AxisInfo{
    int encoder_offset;         	// 该轴对应的零点标定值，单位为编码器脉冲数
    int encoder_resolution;         // 该轴电机转一圈的编码器脉冲数
    double reducer_retio;           // 减速器减速比
    double axis_coupling_factor;    // 该轴和下一个轴之间的耦合系数
};

// 带外轴的机器人轴格式位置点
struct ERobJoint{
    double robot_joint[6];      //对于变位机来说这项无意义
    double ext_joint[6];
};
Q_DECLARE_METATYPE(ERobJoint)

// 轴配置数据
struct ConfData{
    int cf1;
    int cf4;
    int cf6;
    int cfx;
};

// 速度数据
struct SpeedData{
    double v_percent;       // MoveJ速度百分比
    double v_tcp;           // tcp线速度
    double v_ori;           // tcp旋转角速度
    double v_exj;           // 外轴角速度
    double v_exl;           // 外轴线速度
};

struct DynData{
    double TCP_speed;
    double TCP_acc;
    double TCP_jerk;
    double rotate_speed;
    double axis_speed[12];
    double axis_acc[12];
    double axis_jerk[12];
};

// 机器人笛卡尔格式位置点
struct RobPose{
    Frame tool_frame;           // 这里是工具坐标系，即使使用固定工具
    ConfData axis_config;       // 轴配置
    //double ext_joint[6];        //外部轴角度
};
// 带外轴的机器人笛卡尔格式位置点
struct ERobPose{
    RobPose rob_pose;
    double ext_joint[6];
};

// 平滑数据，转弯区
struct SlipData{
    double pos_distance;        // 开始平滑距离目标点空间距离 distance
    double joint_distance;      // 开始平滑距离目标角度位置距离 percent
};

struct EX_RobJoint
{
    double ext_joint[6];
};

// 机械单元位置
struct Position{
    ERobJoint joint_position;   // 轴空间位置
    RobPose rob_pose;           // 笛卡尔空间位置，对于变位机这项无意义
};

struct MaterialPalletPoint{
    int ID;
    double X;
    double Y;
    double Z;

    MaterialPalletPoint(){
        memset(this, 0, sizeof(MaterialPalletPoint));
    }
};

//手动配置
struct ManuParaCfg{
    int tool_id;	//工具坐标系id
    int wobj_id;	//工件坐标系index
    CoordinateSystemType cart_ref_cs_type;//JOG参考工件坐标系类型
    AxisMode axis_mode;	//轴模式
    int step_ovverride; //步进倍率
    int speed_percent;	//速度百分比
};

// 任务
struct Task{
    char name[ MAX_FILE_NAME_LENGTH + 1 ];     // 任务名字
    int priority;                       // 任务优先级
};

// 负载
struct Load
{
    Load(): id(0), mass(0.0), ix(0.0), iy(0.0), iz(0.0)
    {
        //字符数组清０
        memset(name, 0, MAX_STRING_LENGTH);
    }
    int id;                         // 编号
    char name[ MAX_STRING_LENGTH ]; // 名称
    double mass;                    // 负载质量
    Pos cog;                        // 负载质心
    Orient aom;                     // 转动惯量定义方向
    double ix;                      // 绕aom的X轴转动的转动惯量
    double iy;                      // 绕aom的Y轴转动的转动惯量
    double iz;                      // 绕aom的Z轴转动的转动惯量
};
//力控相关
/**
 * @brief 盒状三维空间限制
 */
struct FcBoxVol{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
};

/**
 * @brief 圆柱体范围限制
 */
struct FcCylinderVol{
    double xcbottom;    ///圆柱地面圆心位置
    double ycbottom;
    double zcbottom;
    double height;      ///圆柱高度
    double radius;      ///圆柱半径
};

/**
 * @brief 球范围限制
 */
struct FcSphereVol{
    double xc;          ///球心位置
    double yc;
    double zc;
    double radius;      ///球半径
};

/**
 * @brief 六维变量
 */
struct FcCartNum {
    FcCartNum() {
        memset(this, 0, sizeof(FcCartNum));
    }

    double num1;
    double num2;
    double num3;
    double num4;
    double num5;
    double num6;
};

/**
 * @brief 位姿
 */
struct Pose {
    Pose() :
        x(0.0), y(0.0), z(0.0),
        q1(1), q2(0), q3(0), q4(0) {
    }

    double x;
    double y;
    double z;
    double q1;
    double q2;
    double q3;
    double q4;
};

/**
 * @brief 负载
 * @note  重量单位kg,距离单位mm
 */
struct LoadData {
    LoadData() :
        mass(0.0), cogx(0.0), cogy(0.0), cogz(0.0),
        q1(1.0), q2(0.0), q3(0.0), q4(0.0),
        ix(0.0), iy(0.0), iz(0.0) {
    }
    double mass;
    double cogx;
    double cogy;
    double cogz;
    double q1;
    double q2;
    double q3;
    double q4;
    double ix;
    double iy;
    double iz;
};
/**
 * 三维变量
 */
struct FcXyzNum{
    double x;
    double y;
    double z;
};


/*工作台*/
struct Workbench {
    Workbench() : id(0) {
        //字符数组清０
        memset(name, 0, MAX_STRING_LENGTH);
    }
    int id;                             // 编号
    char name[MAX_STRING_LENGTH];       // 名称
    Frame frame;                        // 工作台坐标系
};

// 工具
struct Tool{
    Tool(): id(0), rob_hold(true) {
        //字符数组清０
        memset(name, 0, MAX_STRING_LENGTH);
    }
    int id;                         // 编号
    char name[ MAX_STRING_LENGTH ]; // 名称
    bool rob_hold;                  // true:工具固定在机器人法兰  false:工具固定在机器人外部
    Frame frame;                    // 工具坐标系。当工具固定在机器人法兰时，工具坐标系相对法兰坐标系定义。当工具固定在机器人外部时，工具坐标系相对世界坐标系定义。
    Load load;                		// 负载特性
};
//四元数
struct Quaternion {
    Quaternion(): Q1(0), Q2(0), Q3(0), Q4(0){}
    double Q1;
    double Q2;
    double Q3;
    double Q4;
};
//欧拉角
struct Euler {
    Euler(): A(0), B(0), C(0){}
    double A;
    double B;
    double C;
};

// 工件
struct Wobj{
    Wobj() : id(0), rob_hold(false), movable(false) {
        //字符数组清０
        memset(name, 0, MAX_STRING_LENGTH);
        memset(mec_unit, 0, MAX_STRING_LENGTH);
    }
    int id;                             // 编号
    char name[ MAX_STRING_LENGTH ];     // 名称
    bool rob_hold;                      // true:工件固定在机器人法兰,手持  false:工件固定在机器人外部，非手持
    bool movable;                       // true:用户坐标系相对世界坐标系运动 false: 用户坐标系相对世界坐标系静止。
    char mec_unit[MAX_STRING_LENGTH];   // 当uf_movable为true时，这里指定用户坐标系随哪个机械单元运动。这里指定的是机械单元的名字。
    Frame oframe;                       // 工件坐标系，相对用户坐标系定义
    int workbench_id;                   // 工作台坐标系。不动时相对世界坐标系定义；随机器人运动时，相对法兰坐标系定义；随变位机运动时，相对变位机的工装坐标系定义。
};

// 控制器版本
struct RC_Version
{
    char RC_version[ MAX_STRING_LENGTH ];
    char MCCard_version[ MAX_STRING_LENGTH ];
};

// 机器人软件版本，包含控制器版本和HMI版本
struct Version
{
    RC_Version rc_version;
    char HMI_version[ MAX_STRING_LENGTH ];
};

struct LogRecordInfo {
    int32_t log_id;                         //系统日志编号
    int32_t code_id;                        // 代码编号
    int32_t level;                          // 日志级别
    QString title;                          // 日志名称
    QString content;
    QString date;
    QString time;
    //    QString arg;
    bool confirm_flag;
};

struct LogDetailInfo {
    int32_t code_id;                        // 代码编号
    int32_t level;                          // 日志级别
    int32_t module;                         // 日志发生模块
    QString title;                        // 日志中文名称
    QString content;                      // 日志中文内容
    QString reason;                        // 产生的原因中文
    QString repair_method;                 // 对应的措施中文
    QString comment;                      // 中文备注
    QString date;
    QString time;
    QString confirm_flag;
};

//new -　日志记录
struct LogRecord {
    int32_t id;                         //　日志编号，根据日志产生顺序生成
    int32_t code_id;                    // 代码编号，唯一
    QString time;                       //　产生日志时间
    QString arg;                        //　参数
    bool confirm_flag;                  //　待确认标志
    int type;                           //  日志类别
};

//new - 日志详情
struct LogContent {
    int32_t code_id;                        // 代码编号
    int32_t level;                          // 日志级别
    int32_t module;                         // 日志发生模块
    QString title;                          // 日志中文标题
    QString title_en;                       // 日志英文标题
    QString content;                        // 日志中文内容
    QString content_en;                     // 日志英文内容
    QString reason;                         // 产生的原因中文
    QString reason_en;                      // 产生的原因英文
    QString repair_method;                  // 对应的措施中文
    QString repair_method_en;               // 对应的措施英文
    QString comment;                        // 中文备注
};

// 日志级别过滤器
struct LogLevelFilter {
    bool LevelFilter_info;
    bool LevelFilter_warning;
    bool LevelFilter_debug;
    bool LevelFilter_error;
    bool LevelFilter_danger;

    LogLevelFilter () {
        LevelFilter_info = true;
        LevelFilter_warning = true;
        LevelFilter_debug = true;
        LevelFilter_error = true;
        LevelFilter_danger = true;
    }
};

// 日志类型过滤器
struct LogLocationFilter {
    bool LocationFilter_general;
    bool LocationFilter_system;
    bool LocationFilter_hardware;
    bool LocationFilter_srl;
    bool LocationFilter_movement;
    bool LocationFilter_io;

    LogLocationFilter() {
        LocationFilter_general = true;
        LocationFilter_system = true;
        LocationFilter_hardware = true;
        LocationFilter_srl = true;
        LocationFilter_movement = true;
        LocationFilter_io = true;
    }
};

//发送日志类型
struct LogItem {
    int32_t code_id;
    char arg[MAX_LOGSTRING_LENGTH];
};

// 4点、6点标定数据
struct CoordianteCalibrateInfo_Pt{
    CoordinateSystemType eCoordinateType;           // 坐标系类型
    int iCoordinateIndex;                           // 坐标系索引 从0开始
    int iPointIndex;                                // 标定的点索引 从0开始
};

// 手动标定坐标系数据
struct CoordianteCalibrateInfo_Manual{
    CoordinateSystemType eCoordinateType;           // 坐标系类型
    int iCoordinateIndex;                           // 坐标系索引 从0开始
    char acCoordinateName[ MAX_STRING_LENGTH ];     // 坐标系名称

    // 原点偏移
    float fOrgOffsetX;
    float fOrgOffsetY;
    float fOrgOffsetZ;
    // 姿态偏移
    float fAttitudeOffsetQ1;
    float fAttitudeOffsetQ2;
    float fAttitudeOffsetQ3;
    float fAttitudeOffsetQ4;
    // 质量
    float fQuality;
    // 重心
    float fFocusX;
    float fFocusY;
    float fFocusZ;
    // 惯性主轴
    float fInertiaAxisQ1;
    float fInertiaAxisQ2;
    float fInertiaAxisQ3;
    float fInertiaAxisQ4;
    // 惯量
    float fInertiaIX;
    float fInertiaIY;
    float fInertiaIZ;
};

// 薄膜按钮事件
struct ThinFilmButtonEvent
{
    int iKeyBoard;              // 键盘
    int iKey;                   // 按键
    ThinFilmButtonState eState; // 状态
};

//轴监控参数
struct AxleMouitParam
{
    int MonitorParamResolution[6];     //编码器分辨率
    double SpeedReduceRation[6];       //减速比
    double MonitorBottom[6];           //运动范围
    double MonitorTop[6];              //运动范围
};

//轴运动速度参数
struct AxleMovementSpeedParam
{
    double MaxSpeed[6];                //最大速度
    double MaxAcceleration[6];         //最大加速度
    double MaxAccelerationPlus[6];     //最大加加速度
};

//空间监控参数
struct SpaceMouitParam
{
    double SpaceMaxSpeed[2];          //最大允许速度
    double SpaceAcceleration[2];      //最大加速度
    double SpaceAccelerationPlus[2];  //最大加加速度
};

//耦合系数
struct CouplingCoefficient
{
    //CouplingCoefficient():CouplingCoefficientParam1(0),CouplingCoefficientParam2(0),CouplingCoefficientParam3(0){}
    double CouplingCoefficientParam1;    //耦合参数一
    double CouplingCoefficientParam2;    //耦合参数二
    double CouplingCoefficientParam3;    //耦合参数三
};

//刷新变量时向RC请求的数据类型
struct RefreshVariableContent
{
    static const size_t VAR_CONTENT_HEDEAR_SIZE = sizeof(uint16_t) * 6;
    //有效数据长度,需减去boost asio使用的4个额外字节
    static const size_t MAX_DATA_LENGTH = DATA_PACKAGE_SIZE - HMI_DATA_PACKAGE_HEADER_SIZE - VAR_CONTENT_HEDEAR_SIZE - 4;
    //是否是数组
    uint16_t isArray;
    //数组维度
    uint16_t dimensions[3];
    //存储类型
    uint16_t storage;
    //作用域
    uint16_t scope;
    //数据内容
    char data[MAX_DATA_LENGTH];
};

struct VariableDeclareInfo
{
    int  VariableType;                                 // 变量类型，RefreshVarType枚举
    int  VariableScopeType;                            // VarScopeType枚举，作用域GLOBAL,LOCAL,ROUTINE
    char FileName[MAX_FILE_NAME_LENGTH + 1];           // 文件名
    char RoutineName[MAX_USER_NAME_LENGTH + 1];        // 子函数名称, 仅对Routine级别变量生效
    char VariableName[MAX_USER_NAME_LENGTH + 1];       // 变量名称
    bool IsArrayType;                                  // 是否是数组类型的变量
};

// 导轨参数结构体定义
struct TrackConfigInfo
{
    double encoder_resolution; //编码器分辨率
    double reducer_retio;      //减速比
    double max_stroke;         //最大运动范围，单位mm
    double min_stroke;         //最小运动范围，单位mm
    double max_stroke_soft;    //软限位上限
    double min_stroke_soft;    //软限位下限
    double max_speed;          //最大速度
    double max_acc;            //最大加速度
    double max_jerk;           //最大加加速度
};

/**
 * @brief IO板参数
 */
struct IOConfigPara {
    int  DigitalIOBoardNum;   ///数字IO板的个数
    int  DIBoardNum;          ///DI板个数
    int  unitDINum;           ///每个DI板上的DI个数
    int  DOBoardNum;          ///DO板个数
    int  unitDONum;           ///每个DO板上的DO个数
    int  SPLCType;            ///安全PLC类型
    bool SPLCEnable;          ///安全PLC启用状态
};

//视教器使用模式设置失败，返回按键的一致状态
struct TPButtonState {
    bool mode_key_state; //手自动状态，true表示一致，false表示不一致
    bool estop_key_state;//急停按键状态，true表示一致，false表示不一致
    bool safe_plc_support;//安全板是否支持， true支持，false不支持
};

/**
 * @brief 安全PLC类型
 */
enum SafetyPLCType {
    SPLC_TYPE_REER, //!< REER安全模块
    SPLC_TYPE_ROKAE //!< ROKAE自研安全模块
};
//IO板设置信息
struct SysIOSignalContent {
    static const int MAX_SYS_IO_NAME_LEN     = 45;
    static const int MAX_MAPPED_SIG_NAME_LEN = 30;
    char name_zh[MAX_SYS_IO_NAME_LEN + 1];           ///系统输入指令/输出状态的名称，中文
    char name_en[MAX_SYS_IO_NAME_LEN + 1];           ///系统输入指令/输出状态的名称，英文
    char mappedSignal[MAX_MAPPED_SIG_NAME_LEN + 1];  ///该输入/输出映射的信号名称
    bool enable;                                     ///启用状态
};


//ProfinetIO
struct ProfinetIOPara
{
    static const size_t SIGNALS_PER_BYTE = 8;
    static const size_t MAX_DEV_NAME_LENGTH = 79;
    //数据交换周期
    double Period;
    //接受字节数
    int RecvByteNum;
    //发送字节数
    int SendByteNum;
    //波特率
    int Baud;
    //设备名称
    char DevName[MAX_DEV_NAME_LENGTH + 1];
    //Serial Profinet IO是否开启
    bool ProfinetIOEnable;
    //是否为串口ProfinetIO模式
    int WorkMode;
};
//Ethernet/ip IO
struct EthernetIOPara
{
    static const size_t SIGNALS_PER_BYTE = 8;
    static const size_t MAX_DEV_NAME_LENGTH = 79;
    //数据交换周期
    double Period;
    //接受字节数
    int RecvByteNum;
    //发送字节数
    int SendByteNum;
    //设备名称
    char DevName[MAX_DEV_NAME_LENGTH + 1];
    //Serial Profinet IO是否开启
    bool EthernetIpIOEnable;
};

//Modbus
struct ModbusInfo
{
    int  ModbusPort;                    //端口
    int  ModbusSlaveID;                 //从站ID
    int  ModbusCoil;                    //线圈状态个数（0 < n <= 64)
    int  ModbusInputStatus;             //输入状态个数（0 < n <= 64)
    int  ModbusKeepRegister;            //保持寄存器（0 < m <= 8)
    int  ModbusInputRegister;           //输入寄存器（0 < m <= 8)
    char ModbusIP[16];                  //IP
    bool ModbusEnable;                  //模块使能
    bool ModbusMaster;                  //主站
    bool ModbusAutoCon;                 //连接状态

    ModbusInfo() {memset(this, 0, sizeof(ModbusInfo));}
};

//任务管理任务记录
struct TaskRecord
{
    QString TaskName;  //任务名称
    QString PicPath;   //图片路径
};

//BoardAIO
// 0 是电流, 1 是电压
struct BoardAIOPara
{
    int board_one_ai_channel_type[4];
    int board_one_ao_channel_type[4];
    int board_two_ai_channel_type[4];
    int board_two_ao_channel_type[4];
    BoardAIOPara () { memset(this, 0, sizeof(BoardAIOPara)); }
};

//板载IO类型
// -1   :   此口无板
//  0   :   DIO-0
//  1   :   DIO-1
//  2   :   DIO-2
//  3   :   DIO-3
// 10   :   AIO-0
// 11   :   AIO-1
struct OnboardIOType
{
    int type[4];
    OnboardIOType() { type[0] = -1; type[1] = -1; type[2] = -1; type[3] = -1; }
};

//AIO
struct AIOPara
{
    bool AIO_enable;
    int channel_num_ai;
    int channel_num_ao;
    int self_module;
    AIOPara () {
        AIO_enable = false;
        channel_num_ai = 0;
        channel_num_ao = 0;
        self_module = 2;
    }
};

//IO信号内容，如果类型为di或者do,则起始地址和终止地址一致
struct SignalContent
{
    static const size_t MAX_SIGNAL_NAME_LEN = 30;
    int  id;                            //ID
    int  signalType;                    //类型
    int  physicalBoard;                 //IO模块
    int  startMappingAddr;              //起始地址
    int  endMappingAddr;                //终止地址
    char name[MAX_SIGNAL_NAME_LEN + 1]; //名称
};

/*!
 * Enumeration defines the possible bytesizes for the serial port.
 * 数据位，默认为8. 可选5，6，7，8
 */
typedef enum {
    fivebits = 5,
    sixbits = 6,
    sevenbits = 7,
    eightbits = 8
} bytesize_t;

/*!
 * Enumeration defines the possible parity types for the serial port.
 * 校验位，默认为无校验。可选：无校验，奇校验，偶校验，
 */
typedef enum {
    parity_none = 0,  //无校验
    parity_odd = 1,   //奇校验
    parity_even = 2,  //偶校验
    parity_mark = 3,  //校验位为1
    parity_space = 4  //校验位为0
} parity_t;

/*!
 * Enumeration defines the possible stopbit types for the serial port.
 * 停止位，默认为1. 可选 1，2，1.5
 */
typedef enum {
    stopbits_one = 1,
    stopbits_two = 2,
    stopbits_one_point_five   //1.5
} stopbits_t;

/*!
 * Enumeration defines the possible flowcontrol types for the serial port.
 * 流控制，默认为无控制
 */
typedef enum {
    flowcontrol_none = 0,	   //无控制, None
    flowcontrol_software,    //软件流控制，Software
    flowcontrol_hardware   //硬件流控制，Hardware
} flowcontrol_t;

//串口属性参数
struct SerialPara
{
    static const size_t MAX_SERIAL_NAME_LEN = 30;
    static const size_t MAX_SERIAL_PORT_LEN = 30;
    static const size_t MAX_SERIAL_DESC_LEN = 30;
    SerialPara() {
        baud = 9600;
        flow_ctrl = flowcontrol_none; //默认没有流控制
        databits = eightbits; //默认为8
        stopbits = stopbits_one; //默认为1
        parity = parity_none; //默认没有校验
    }

    int id;
    char name[MAX_SERIAL_NAME_LEN + 1]; //配置的串口名字
    char port[MAX_SERIAL_PORT_LEN + 1]; //串口设备的名字
    char port_description[MAX_SERIAL_DESC_LEN + 1]; //串口设备的描述
    int baud; //波特率
    flowcontrol_t flow_ctrl; //流控制，0，1，2，3
    bytesize_t databits; //数据位，可选 5，7，8
    stopbits_t stopbits; //停止位，可选 1，1.5，2
    parity_t parity; //校验位，一般三种校验方式，偶（E）、奇（O）、无（N）
};

struct PortInfo {
    static const size_t MAX_SERIAL_ID_LEN = 30;
    static const size_t MAX_SERIAL_PORT_LEN = 30;
    static const size_t MAX_SERIAL_DESC_LEN = 30;

    char port[MAX_SERIAL_PORT_LEN + 1];   /*! Address of the serial port (this can be passed to the constructor of Serial). */
    char description[MAX_SERIAL_DESC_LEN + 1];  /*! Human readable description of serial device if available. */
    char hardware_id[MAX_SERIAL_ID_LEN + 1]; /*! Hardware ID (e.g. VID:PID of USB serial devices) or "n/a" if not available. */
};

//拖动示教参数
struct LeadThroughProgPara
{
    double velocityLimitInCartesian;      //笛卡尔空间线速度限制
    double velLimitInJointSpace;          //关节空间速度限制系数
    double saturationFactorJointTorque;   //力矩限幅系数
    double correctionFactorFriction[6];   //关节摩擦力补偿校正系数
};

//本体参数辨识返回值
struct DynamicIdentifyData
{
    double dynamic_parameter[54];         //辨识动力学参数
    double friction_error[6];             //每个轴辨识的摩擦力和名义值的相对误差, 共6个
    double friction[12];                  //每个轴辨识的摩擦力系数共12个
};

//已配置的IO信号的ID号和值
struct IOValuePOD
{
    int ID;
    int Value;
};

struct MotionControlPara {
    double default_acc_param[2];     //全局加减速设置 [两个参数，名字分别是acc(取值范围0.6~1.5) 和 acc_ramp(取值范围0.02~0.2)]
    double trq_overload_factor[6];   //电机过载系数 [每个轴一个，取值范围0.1~2.0]
    double trans_overload_factor[6]; //传动过载系数 [每个轴一个，取值范围0.1～2.0]
    double jerk_limit_params[3];     //转弯区加加速度限制 [三个参数，分别对应笛卡尔平动、笛卡尔旋转、轴空间运动， 取值范围是1~10000 ]
};


struct RoutineInfo {
    int task_id;                                       //task的ID
    char file_path[MAX_FILE_NAME_LENGTH + 1];          //所在文件
    char routine_name[MAX_USER_NAME_LENGTH + 1];       //子函数名称
};

//socket的类型
enum SocketType{
    CLIENT_SOCKET =1,//client
    SERVER_SOCKET =2 //server
};

enum SocketConnectStatus {
    SOCKET_DIS_CONNECTED = 0, //断开
    SOCKET_CONNECTED, //连接
    SOCKET_RECONNECTING //重连
};

//状态发布socket信息
struct StatePublishSocketContent
{
    enum{
        MAX_STATE_IP_LEN = 15,
        MAX_STATE_PORT_LEN = 5,
        MAX_STATE_SOCKET_NAME = 30,
        MAX_STATE_SUFFIX_LEN = 10
    };
    char name[MAX_STATE_IP_LEN + 1];
    char ip[MAX_STATE_IP_LEN + 1];
    int port;
    SocketType soket_type;
    SocketConnectStatus connect_state;//server为监听状态,client为连接状态
    bool reconnect_enable;//外部socket开机是否自动重连， true开机重连，false关闭重连
    char suffix[MAX_STATE_SUFFIX_LEN + 1];
};

/**
 * @brief 定义整个socket变量
 */
struct SocketVar {
    QString socket_name;
    QString socket_ip;
    int port;
    bool is_server;
};

//vrep连接的信息
struct VrepContent {
    static const int MAX_IP_LEN = 15;
    char vrep_ip[MAX_IP_LEN + 1];
    int vrep_port_number;
};

//视觉或调试ip修改的信息
struct ConfigurationIPMode {
    static const int MAX_STATE_IP_LEN = 15;
    IPType type;
    char ip[MAX_STATE_IP_LEN + 1];
};



//编辑器关键字行号结构体
struct EditorKeyWordLine {
    int module_line;         //MODULE关键字所在行号
    int end_module_line;     //ENDMODULE关键字所在行号
    int frist_proc_line;     //第一个PROC所在行
    int last_endproc_line;   //最后一个ENDPROC所在行
};

/**
 * @brief 传送带参数
 */
struct ConveyorPara {
    ///< 最大传送带名称，不包括'0'结束标志
    static const int MAX_CONVEYOR_NAME = 30;
    ///< MAX IP Length, IPv6最大39个字符
    static const int MAX_IP_LENGTH     = 39;
    /// 最大指令长度
    static const size_t MAX_CAMERA_DATA_LEN = 10;

    ///--------属性--------
    ///< ID
    int    id;
    ///< 名称
    char   conveyor_name[MAX_CONVEYOR_NAME + 1];
    ///< 自激活设置
    bool   activate_after_start;
    ///< 禁止反激活
    bool   deactivation_forbidden;

    ///--------编码器属性--------
    ///< 编码器方向
    bool   encoder_direction;
    ///< 编码器类型，暂时不使用
    int    encoder_type;
    ///< 编码器分辨率
    int    encoder_resolution;
    ///< 编码器速度滤波器截止频率 Hz
    double encoder_speed_filter_cutoff_frequency;
    ///< 编码器减速比, cnt/meter
    double counts_per_meter;

    ////-------传送带跟踪参数-------
    ///< 最小跟踪距离(m)
    double min_tracking_distance;
    ///< 最大跟踪距离(m)
    double max_tracking_distance;
    ///< 队列跟踪距离(m)
    double queue_tracking_distance;
    ///< 最小间隔距离(m)
    double sync_separation;
    ///< 开始窗口宽度(m)
    double start_win_width;
    ///< 最大支持工件个数
    int    max_object_num;

    ////-------相机属性-------
    ///< 是否使用相机，暂时不使用
    bool camera_using;
    ///< 相机输入端口
    int  camera_input_port;
    ///< 相机输出端口
    int  camera_output_port;
    ///< 相机IP, IPv6最大字符长度
    char camera_IP[MAX_IP_LENGTH + 1];
    ///< 相机拍照指令
    char camera_cap_instruction[MAX_CAMERA_DATA_LEN + 1];
    ///< 相机通讯数据包结束符
    char camera_end_mark[MAX_CAMERA_DATA_LEN + 1];
    ///< 相机拍照的ACK
    char camera_cap_ack[MAX_CAMERA_DATA_LEN + 1];
    ///< 相机返回空数据包时的ACK
    char camera_empty_data_ack[MAX_CAMERA_DATA_LEN + 1];
    ///< 相机视场宽度(m)
    double camera_field_width;
    ///< 相机拍摄延迟, 单位s
    double camera_capture_delay_s;

    ////-------光电开关相关参数-------
    /// 光电开关参数暂时都不使用
    ///< 输入信号的ID
    int input_signal_id;
    ///< 光电开发触发类型
    int trigger_type;

    ////-------坐标系信息-------
    ///< 传送带基坐标系
    Frame conveyor_base_frame;
    ///< 传送带用户坐标系
    Frame conveyor_user_frame;
};


struct NameChanged {
    QString name_origin;
    QString name_new;
};

//传送带监控属性
struct MotionState {
    double velocity;
    double X;
};





//拖动示教再现参数
struct LeadThroughTeachingPara {
    double path_resolution;
    int max_record_pp_num;
};

//协作模式DI设置
struct CooperativeData {
    int DI_id;
    double velocity;
    CooperativeData () {
        DI_id = -1;
        velocity = .0f;
    }
};

//协作模式两个级别工作状态
struct CooperativeWorkState{
    bool cooperative_first_work_state;
    bool cooperative_second_work_state;
};

//拖动示教工程参数
struct LeadThroughProj {
    static const int MAX_NAME_LENGTH = 30;                //名称最大长度，不包括'\0'结束标志
    char output_signal[MAX_NAME_LENGTH + 1];              //输出信号名称
    //    unsigned int interval _time;                           //轨迹点间隔时间
    double rate;                                          //工程最大运行速率
};

//拖动示教生成工程所需参数
struct LeadThroughProjPara {
    static const int MAX_NAME_LENGTH = 30;              //名称最大长度，不包括'\0'结束标志
    int total_time;                                  //轨迹记录总时间，单位分钟
    char detection_signal[MAX_NAME_LENGTH + 1];         //检测信号名称
    char output_signal[MAX_NAME_LENGTH + 1];            //输出信号名称
    char track_record_signal[MAX_NAME_LENGTH + 1];      //记录轨迹信号名称
    char project_name[MAX_NAME_LENGTH + 1];             //生成工程的名称
    double speed_threshold_value;                       //启停速度阈值[0.0002~0.002 rad/s]
};

//拖动示教工程运行参数
struct LeadThroughProjRunPara {
    static const int MAX_NAME_LENGTH = 30;              //名称最大长度，不包括'\0'结束标志
    unsigned int circulation_patterns;                  //循环模式
    double run_rate;                                	//运行速率
    char run_signal[MAX_NAME_LENGTH + 1];               //启动工程信号
    char start_signal[MAX_NAME_LENGTH + 1];				//机器人运动到轨迹起始点时需要等待的DI信号
    char end_signal[MAX_NAME_LENGTH + 1];				//机器人运动到轨迹终点时需要输出的DO信号
};

//程序状态
struct ProgStateInfo {
    int prog_exec_state;     //程序状态       enum ProgExecState
    int status_cause;        //状态产生原因    enum ProgStateusCause
};

//task
struct TaskParam {
    static const int MAX_NAME_LENGTH = 30;              //名称最大长度，不包括'\0'结束标志
    static const int MAX_PATH_LENGTH = 80;              //路径最大长度，不包括'\0'结束标志
    int id;												//task的 id，作为task的唯一标识符
    char task_name[MAX_NAME_LENGTH + 1];                //task的名称
    int task_in_foreground_id;                          //前置task的id
    char task_in_foreground[MAX_NAME_LENGTH + 1];       //前置task的名称
    int task_type;                                      //task的类型
    bool is_motion_task;                                //是否为MotionTask
    int trust_level;                                    //task的信任级别
    char main_entry[MAX_NAME_LENGTH + 1];               //task绑定工程的入口函数，默认是main
    char project_path[MAX_PATH_LENGTH + 1];             //task绑定的工程
};

//设置task是否由薄膜按键控制开始运行和停止运行
struct TaskControlModel {
    int task_id;    //task id
    bool state;     //设置的状态
};

/**
 * @brief 力控传感器配置
 */
struct FCSensorConfiguration {
    //力控传感器模型类型
    int sensor_model;
    // 传感器类型
    int    sensor_type;    //SensorType
    // optoforce传感器采样参数*******
    int    optoforce_sensor_sample_para;
    // ATI传感器采样参数1*******
    int    ati_sensor_sample_para1;
    // ATI传感器采样参数2*******
    int    ati_sensor_sample_para2;
    //  测量范围************
    double capacity[6];
    //  零偏***********
    double bias[6];
    //  分辨率***********
    double resolution[6];
    //  传感器安装位置***********
    double postion_flange2sensor[3];
    //  传感器安装姿态***********
    double rot_flange2sensor[3];
    double filter_sample_freq;       //  传感器采样频率
    double filter_cutoff_freq;       // 传感器低通滤波截止频率
};

/**
 * @brief 力控用户配置
 */
struct FCUserConfiguration {
    // 力控模式**********
    int    fc_ctrl_mode; //FcMode
    // 力跟踪速度反馈积分饱和值**********
    double vel_fb_integ_limit[6];
    // 力跟踪速度反馈Ramp系数**********
    double ft_vel_ramp_ratio;
    // 力跟踪位置反馈Ramp系数**********
    double ft_pos_ramp_ratio;
    // 条件跳转计数最大值**********
    double count_max_cond_trans;
    // 笛卡尔空间导纳控制（拖动示教）参数**********
    double fc_admittance[6];
    // 笛卡尔空间实现导纳控制的方向选择向量**********
    double fc_admittance_selecotr[6];
    // 自由拖动笛卡尔力/力矩死区**********
    double ft_min[6];
    // 自由拖动线速度反馈Ramp系数**********
    double cartvel_ramp_ratio;
    // 自由拖动关节速度反馈Ramp系数**********
    double jntvel_ramp_ratio;
    // 自由拖动关节速度反馈最大值, 单位rad/s**********
    double jnt_vel_limit[6];
};

/**
 * @brief 在状态监控界面显示的FC数据
 */
struct FcMonitorData {
    double X;
    double Y;
    double Z;
    double RX;
    double RY;
    double RZ;
};

/**
 * @brief 精雕机robottarget属性
 */
struct CarvingMachineRobtarget {
    //精雕机变量ID编号
    int  ID;
    //位置名称
    char name_position[MAX_STRING_LENGTH];
    //robtarget变量名称
    char name_robtarget[MAX_STRING_LENGTH];
    //偏移(基于世界坐标系) X方向
    double  offs_x;
    //偏移(基于世界坐标系) Y方向
    double  offs_y;
    //偏移(基于世界坐标系) Z方向
    double  offs_z;
};

/**
 * @brief 精雕机time属性
 */
struct CarvingMachineTime{
    //TIME变量ID编号
    int  ID;
    //时间名称
    char name_time[MAX_STRING_LENGTH];
    //double型变量名称
    char name_double_type[MAX_STRING_LENGTH];
    //robtarget变量名称
    double time_value;
};

/**
 * @brief 堆盘属性
 * @note  长度单位均为mm
 */
struct Pallet {
    // 料盘编号
    int ID;
    // 工具ID
    int toolID;
    // 工件ID
    int wobjID;

    // 列数，代表x轴工件数
    int num_column;
    // 行数，代表y轴工件数
    int num_row;

    // x轴中心距
    double dXp;
    // y轴中心距
    double dYp;
    // 高度
    double height;

    // 接近点x轴距离，单位mm
    double Xa;
    // 接近点y轴距离，单位mm
    double Ya;

    // 层数
    int layer_count;
    // 层高修正，单位mm
    double layer_offset;

    // 物料盘第一点的初始位置
    RobPose start_pos;

    Pallet() {
        memset(this, 0, sizeof(Pallet));
    }
};

struct MaterialPallet {
    // 料盘编号
    int ID;
    // 工具ID
    int toolID;
    // 工件ID
    int wobjID;

    // 列数，代表x轴工件数
    int num_column;
    // 行数，代表y轴工件数
    int num_row;

    // 工件长
    double l;
    // 工件宽
    double w;
    // 工件高
    double h;

    // x轴中心距
    double dXp;
    // y轴中心距
    double dYp;

    // 物料盘第一点的初始位置
    RobPose start_pos;
    //x方向第二点
    RobPose initial_pos_x;
    //y方向第二点
    RobPose initial_pos_y;

    RobPose end_pos;

    MaterialPallet() {
        memset(this, 0, sizeof(MaterialPallet));
    }
};

/**
 * @brief 重力场向量
 */
struct GravityVector {
    GravityVector(): X(0), Y(0), Z(0) {}
    double X;
    double Y;
    double Z;
};

/**
 * @brief 编码脉冲值/标定角度
 */
struct EncoderPulseValue {
    EncoderPulseValue(): axis_1(0), axis_2(0), axis_3(0), axis_4(0), axis_5(0), axis_6(0) {}
    double axis_1;
    double axis_2;
    double axis_3;
    double axis_4;
    double axis_5;
    double axis_6;
};

struct IntPulseValue {
    IntPulseValue(): axis_1(0), axis_2(0), axis_3(0), axis_4(0), axis_5(0), axis_6(0) {}
    int axis_1;
    int axis_2;
    int axis_3;
    int axis_4;
    int axis_5;
    int axis_6;
};

/**
 * @brief 传送带相机标定数据
 */
struct ConveyorCameraCalibData {
    // 传送带ID
    int id;
    // 机器人点位置
    ERobJoint joint[9];
    // 传送带位移,单位:m
    double disp;
};

/**
 * @brief 传送带4点法标定传送带基坐标系、用户坐标系
 */
struct ConveyorFrameCalibData  {
    // 传送带ID
    int id;
    //机器人点位置
    ERobJoint jointtarget[4];
};

/**
 * @brief 传送带坐标系标定结果
 */
struct ConveyorFrameCalibRes {
    Frame conveyor_base_frame;
    Frame conveyor_user_frame;
};

/**
 * @brief socket信息的结构体
 */
struct SocketInformation {
    enum{
        MAX_STATE_IP_LEN = 15,
        MAX_STATE_PORT_LEN = 5,
        MAX_STATE_SOCKET_NAME = 30,
        MAX_STATE_SUFFIX_LEN = 10
    };
    char name[MAX_STATE_IP_LEN + 1];
    char ip[MAX_STATE_IP_LEN + 1];
    int port;
    SocketType type;
    SocketConnectStatus connect_state;//server为监听状态,client为连接状态
    bool reconnect;
    char suffix[MAX_STATE_SUFFIX_LEN + 1];
};

/**
 * @brief HomePoint信息的结构体
 */
struct HomePointPara {
    double HomePointAngle[7];       //起始点轴角度
    int HomePointTurn[7];           //起始点动作次序
    double HomePointOffset[7];      //起始点轴角度偏移

    HomePointPara() {memset(this, 0, sizeof(HomePointPara));}
};

class MsgInfo {
public:
    QList<QMessageBox *> m_msgbox_list;
    QHash<QString, QMessageBox *> m_unique_msgbox_hash;
};

/**
 * @brief The DebugInfo struct Debug信息
 */
struct DebugInfo {
    QTime debug_time;                       //调试日志暂存时间
    bool is_time_sync;                      //当前时间是否同步
    QStringList debug_buff;                 //在时间同步之前先缓存所有日志

    DebugInfo() {
        //只能放构造函数初始化
        is_time_sync = false;
        debug_buff.clear();
    }
};

#pragma pack(pop)

#endif /* HMICOMMONSTRUCT_H */
