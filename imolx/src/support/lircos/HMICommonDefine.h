#ifndef HMICOMMONDEFINE_H
#define HMICOMMONDEFINE_H

#include <QPoint>
#include <QString>
#include <QDir>
// 一些常用方法(宏)定义
#define HMI_UNUSED(x)                                       (void)x;

#define HMI_CONFIGURATION_FILE                  ( "config.ini" )

#define HMI_DATA_PACKAGE_HEADER_SIZE            ( 10 )      // 数据包头大小
#define DATA_PACKAGE_SIZE                       ( 4096 )    // 数据包最大长度
#define MAX_CHANNEL_NUM                         ( 4 )
#define MAX_AXIS_EACH_CHANNEL                   ( 12 )
#define MAX_SLAVE_SERVO_NUM                     ( 48 )
#define MAX_HMI_NUMBER                          ( 256 )     // 系统支持最大HMI数量
#define MAX_USER_NAME_LENGTH                    ( 40 )      // 用户名字符串长度
#define MAX_USER_PWD_LENGTH                     ( 40 )      // 密码字符串长度
#define MAX_STRING_LENGTH                       ( 80 )      // 系统支持字符串最大长度
#define MAX_LOGSTRING_LENGTH                    ( 1024 )    // 系统支持日志字符串最大长度
#define MAX_FILE_NAME_LENGTH                    ( 120 )     // 系统支持文件路径最大长度
#define MAX_TOOL_CORD_NUMBER                    ( 4 )       // 系统支持最大工具坐标系
#define MAX_WOBJ_CORD_NUMBER                    ( 4 )       // 系统支持最大工件坐标系

#define VAR_MAX_LENGTH                          ( 20 )      // 变量名、函数名、task名称最大长度

#define MAX_DO_BOARD                            ( 3 ) // 系统支持最大DO板数
#define MAX_DI_BOARD                            ( 3 ) // 系统支持最大DI板数
#define MAX_AO_BOARD                            ( 3 ) // 系统支持最大AO板数
#define DI_NUM_PER_BOARD                        ( 16 ) // 每个I/O板有16个DI点
#define DO_NUM_PER_BOARD                        ( 16 ) // 每个I/O版有16个DO点
#define JOINT_MIN_STEP_ANGLE                    ( 0.05 * deg2rad ) //轴JOG最小步进定义为0.05度
#define CART_MIN_STEP_DISTANCE                  ( 0.05 ) //单位mm
#define CART_MIN_STEP_ROTATE                    ( 0.05 * deg2rad )	//单位rad

#define MANUL_MAX_AXIS_SPEED_PER                ( 10 )              //手动单轴限速10%
#define MANUL_MAX_TCP_SPEED                     ( 250 )             //单位mm/s
#define MANUL_MAX_ROTATE_SPEED                  ( 10 * deg2rad )	//单位rad/s

#define SAFTY_IO_NUM                            ( 32 )

#define MIN_LINEAR_SPEED                        (0.001)    //最小速度 mm/s
#define MIN_ROTATION_SPEED                      (0.001)    //最小旋转速度/s

// 语言定义
#define LANG_CN                                 ( 1 )       /* 中文 */
#define LANG_ENG                                ( 2 )       /* 英文 */

#define IN
#define OUT

//#ifndef BUILD_EMBEDDED
//    #define X86Platform
//#endif

#ifdef BUILD_WITH_DEBUG
#define HMI_DEBUG_LEVEL                         DEBUG_NORMAL
#endif

#ifdef  X86Platform
#define SYSTEM_PLATFMOR                         (0)
#define DISPLAY_LEFT_X                          (400)
#define DISPLAY_LEFT_Y                          (150)
//x86的比实际缩小0.9倍
//#define SYSTEM_WIDTH                            (921)
//#define SYSTEM_HEIGHT                           (691)
#define SYSTEM_WIDTH                            (1024)
#define SYSTEM_HEIGHT                           (768)

#else
//arm platform
#define SYSTEM_PLATFORM                         (1)
#define DISPLAY_LEFT_X                          (0)
#define DISPLAY_LEFT_Y                          (0)

#define SYSTEM_WIDTH                            (1024)
#define SYSTEM_HEIGHT                           (768)
#endif

#define     WIDTH_MAIN       (300)
#define     WIDTH_IO         (300)
#define     WIDTH_MATH       (300)
#define     WIDTH_MOVE       (300)
#define     WIDTH_OPERATOR   (300)
#define     WIDTH_PROCESS    (300)
#define     WIDTH_VAR        (300)
#define     WIDTH_NETWORK    (300)
#define     WIDTH_FUNCTION   (300)


#define MAX_INNER_AXIS_CNT                      ( 6 )
#define MAX_OUTTER_AXIS_CNT                     ( 7 )

#define EDITOR_KEYWORDS_FILE                    ( QDir::currentPath()+ QDir::separator() + QString("keywords"))
#define HMI_RECENT_FILE                         ( QString("recent_file") )
#define HMI_PREDEFINE_MOD                       ( QString("predefine.mod") )
#define HMI_SIGNAL_DI                           ( QString("SignalDI") )
#define HMI_SIGNAL_DO                           ( QString("SignalDO") )
#define HMI_SIGNAL_GI                           ( QString("SignalGI") )
#define HMI_SIGNAL_GO                           ( QString("SignalGO") )

#define HMI_TRASH_DIR                           ( QDir::currentPath() + QDir::separator() + QString("Trash") )

// 弧度和度的转换系数
#define RAD_2_DEG                               ( 57.2957795130823208767981548141052 )

#define KEYBOARD_EVENT_0                        ( 0 )
#define KEYBOARD_EVENT_1                        ( 1 )
#define KEYBOARD_EVENT_MULTI                    ( 2 )
// 薄膜键盘按键定义  适用于旧版示教器
//#define THIN_FILM_KEY_START                     ( 59 )
//#define THIN_FILM_KEY_STOP                      ( 106 )
//#define THIN_FILM_KEY_BACK                      ( 139 )
//#define THIN_FILM_KEY_FORWARD                   ( 158 )
//#define THIN_FILM_KEY_F1                        ( 60 )
//#define THIN_FILM_KEY_F2                        ( 105 )
//#define THIN_FILM_KEY_F3                        ( 108 )
//#define THIN_FILM_KEY_F4                        ( 61 )
//#define THIN_FILM_KEY_POW_ON                    ( THIN_FILM_KEY_F1 )
//#define THIN_FILM_KEY_POW_OFF                   ( THIN_FILM_KEY_F2 )
//#define THIN_FILM_KEY_VREP                      ( THIN_FILM_KEY_F3 )
//#define THIN_FILE_KEY_RUN_MODE                  ( THIN_FILM_KEY_F4 )

//#define THIN_FILM_KEY_JOG1_SUB                  ( 62 )
//#define THIN_FILM_KEY_JOG1_ADD                  ( 1 )
//#define THIN_FILM_KEY_JOG2_SUB                  ( 61 )
//#define THIN_FILM_KEY_JOG2_ADD                  ( 105 )
//#define THIN_FILM_KEY_JOG3_SUB                  ( 60 )
//#define THIN_FILM_KEY_JOG3_ADD                  ( 158 )
//#define THIN_FILM_KEY_JOG4_SUB                  ( 64 )
//#define THIN_FILM_KEY_JOG4_ADD                  ( 116 )
//#define THIN_FILM_KEY_JOG5_SUB                  ( 65 )
//#define THIN_FILM_KEY_JOG5_ADD                  ( 108 )
//#define THIN_FILM_KEY_JOG6_SUB                  ( 66 )
//#define THIN_FILM_KEY_JOG6_ADD                  ( 103 )
//#define THIN_FILM_KEY_MENU                      ( 139 )

//薄膜键盘按键定义，临时发邢台用,最新示教器也用该版本(20161011)

//#define THIN_FILM_KEY_START                     ( 59 )
//#define THIN_FILM_KEY_STOP                      ( 60 )
//#define THIN_FILM_KEY_BACK                      ( 61 )
//#define THIN_FILM_KEY_FORWARD                   ( 108 )
//#define THIN_FILM_KEY_F1                        ( 106 )
//#define THIN_FILM_KEY_F2                        ( 105 )
//#define THIN_FILM_KEY_F3                        ( 158 )
//#define THIN_FILM_KEY_F4                        ( 139 )
//#define THIN_FILM_KEY_POW_ON                    ( THIN_FILM_KEY_F1 )
//#define THIN_FILM_KEY_POW_OFF                   ( THIN_FILM_KEY_F2 )
//#define THIN_FILM_KEY_VREP                      ( THIN_FILM_KEY_F3 )
//#define THIN_FILE_KEY_RUN_MODE                  ( THIN_FILM_KEY_F4 )

//#define THIN_FILM_KEY_JOG1_SUB                  ( 158 )
//#define THIN_FILM_KEY_JOG1_ADD                  ( 1 )
//#define THIN_FILM_KEY_JOG2_SUB                  ( 116 )
//#define THIN_FILM_KEY_JOG2_ADD                  ( 103 )
//#define THIN_FILM_KEY_JOG3_SUB                  ( 60 )
//#define THIN_FILM_KEY_JOG3_ADD                  ( 62 )
//#define THIN_FILM_KEY_JOG4_SUB                  ( 59 )
//#define THIN_FILM_KEY_JOG4_ADD                  ( 61 )
//#define THIN_FILM_KEY_JOG5_SUB                  ( 106 )
//#define THIN_FILM_KEY_JOG5_ADD                  ( 108 )
//#define THIN_FILM_KEY_JOG6_SUB                  ( 139 )
//#define THIN_FILM_KEY_JOG6_ADD                  ( 105 )
//#define THIN_FILM_KEY_JOG7_SUB                  ( 63 )
//#define THIN_FILM_KEY_JOG7_ADD                  ( 64 )
//#define THIN_FILM_KEY_MENU                      ( 66 )

//最新的薄膜按键 2017.2.17
#define THIN_FILM_KEY_BACK                      ( 59 )
#define THIN_FILM_KEY_FORWARD                   ( 60 )
#define THIN_FILM_KEY_F1                        ( 61 )
#define THIN_FILM_KEY_F2                        ( 108 )
#define THIN_FILM_KEY_F3                        ( 106 )
#define THIN_FILM_KEY_F4                        ( 105 )
#define THIN_FILM_KEY_F5                        ( 158 )
#define THIN_FILM_KEY_F6                        ( 139 )
#define THIN_FILM_KEY_POW_ON                    ( THIN_FILM_KEY_F1 )
#define THIN_FILM_KEY_POW_OFF                   ( THIN_FILM_KEY_F2 )
#define THIN_FILM_KEY_VREP                      ( THIN_FILM_KEY_F3 )
#define THIN_FILE_KEY_RUN_MODE                  ( THIN_FILM_KEY_F4 )

#define THIN_FILM_KEY_START                     ( 1 )
#define THIN_FILM_KEY_STOP                      ( 158 )
#define THIN_FILM_KEY_JOG1_SUB                  ( 116 )
#define THIN_FILM_KEY_JOG1_ADD                  ( 103 )
#define THIN_FILM_KEY_JOG2_SUB                  ( 60 )
#define THIN_FILM_KEY_JOG2_ADD                  ( 62 )
#define THIN_FILM_KEY_JOG3_SUB                  ( 59 )
#define THIN_FILM_KEY_JOG3_ADD                  ( 61 )
#define THIN_FILM_KEY_JOG4_SUB                  ( 106 )
#define THIN_FILM_KEY_JOG4_ADD                  ( 108 )
#define THIN_FILM_KEY_JOG5_SUB                  ( 139 )
#define THIN_FILM_KEY_JOG5_ADD                  ( 105 )
#define THIN_FILM_KEY_JOG6_SUB                  ( 63 )
#define THIN_FILM_KEY_JOG6_ADD                  ( 64 )
#define THIN_FILM_KEY_MENU                      ( 66 )

static const QString KEY_USER_OPERATOR = QString("operator");
static const QString KEY_USER_ADMIN = QString("admin");
static const QString KEY_USER_GOD = QString("god");

//定义的按键类型
enum CUSTOMED_KEY_TYPE
{
    CUSTOMED_KEY_NONE = 0,
    CUSTOMED_KEY_KEYBOARDS,
    CUSTOMED_KEY_MONITOR,
    CUSTOMED_KEY_SCREENSHOT_TO_USB,      //截图到usb
    CUSTOMED_KEY_PRINT,                  //显示或者隐藏print框
    CUSTOMED_KEY_TRACK,                  //切换机械单元，导轨
    CUSTOMED_KEY_SWITCH_MODE,            //切换力矩模式/位置模式
    CUSTOMED_KEY_INCREASE_JOG_RATE,      //增加jog速率
    CUSTOMED_KEY_DECREASE_JOG_RATE,      //降低jog速率
    CUSTOMED_KEY_INCERASE_PROGRAM_RATE,  //增加程序运行速率
    CUSTOMED_KEY_DECERASE_PROGRAM_RATE,  //降低程序运行速率
    CUSTOMED_KEY_FORCE_OUTPUT            //强制输出
};



//定义rate变化类型
enum RATE_CHANGE_TYPE {
    RATE_PLUS,
    RATE_MINIUS
};

//辅助编程界面类型
enum ExpressionType {
    EXPRESSION_MOTION,
    EXPRESSION_LOGIC,
    EXPRESSION_VAR,
    EXPRESSION_IO,
    EXPRESSION_MATH,
    EXPRESSION_OPERATOR,
    EXPRESSION_NETWORK,
    EXPRESSION_FUNCTION,
    EXPRESSION_TRIGGER,
    EXPRESSION_CONVEYOR,
    EXPRESSION_FORCE_CONTROL,
    EXPRESSION_CLOCK,
    EXPRESSION_LEAD_THROUGH,
    EXPRESSION_TRAP,
    EXPRESSION_MOTION_ADV,
    EXPRESSION_BIT_OPERATION,
    EXPRESSION_STRING_OPERATION,
    EXPRESSION_MEMORY_IO,
    EXPRESSION_TYPE_CONVERSION,
    EXPRESSION_HOME,
    EXPRESSION_DETECTION_AREA,
    EXPRESSION_SAFE_AREA,
    EXPRESSION_PALLET,
    EXPRESSION_MPALLET,
    EXPRESSION_TOTAL_NUM,
    EXPRESSION_SERIAL
};
// 用户级别
enum UserLevel {
    UL_SUPER_ADMINISTRATOR = 0,    /* 超级管理员 */
    UL_ADMINISTRATOR,              /* 管理员 */
    UL_OPERATOR,                   /* 操作员 */
    UL_NONE                        /* 未登录 */
};

// 日志级别
enum LogLevel{
    LOG_LEVEL_DEBUG,       //0
    LOG_LEVEL_ERROR,       //1
    LOG_LEVEL_WARNING,     //2
    LOG_LEVEL_INFO,        //3
    LOG_LEVEL_ALL          //4
};

// 日志类型
enum LogLocation {
    LOCATION_GENERAL = 0,                       // 一般（通用）
    LOCATION_SYSTEM,                            // 系统
    LOCATION_HARDWARE,                          // 硬件
    LOCATION_SRL,                               // 机器人语言
    LOCATION_MOVEMENT,                          // 运动
    LOCATION_IO,                                // IO与通信
    LOCATION_SERVED                             // 保留位置
};

// 控制器操作模式
enum OperatingMode {
    OPERATING_MODE_MANUAL,      /* 手动 */
    OPERATING_MODE_AUTO,        /* 自动 */
    OPERATING_MODE_WAITING_ACK  /* 等待 */
};

// 增量模式
enum IncrementalMode {
    IM_CONTINUOUS,           // 连续onCmbConveyorNameClicked
    IM_10,                  // 10mm/10°
    IM_1,                   // 1mm/1°
    IM_0_1,                 // 0.1mm/0.1°
    IM_0_0_1,               // 0.01mm/0.01°
    IM_0_0_0_5,             // 0.005mm/0.005°
    IM_CUSTOM              // 自定义
};

// 坐标系类型
enum CoordinateSystemType {
    COORDTYPE_WORLD,            /* 世界坐标系 */
    COORDTYPE_BASE,             /* 基坐标系 */
    COORDTYPE_WOBJ,             /* 工件坐标系 */
    COORDTYPE_TOOL,             /* 工具坐标系 */
    COORDTYPE_WORKBENCH,        /* 用户坐标系 */
    COORDTYPE_CONVEYOR,           //传送带坐标系
    CALIBRATE_ZERO                //零点标定
};

// 界面分类
enum UIClassify{
    UIC_LOGIN,                 /* 登录界面 */
    UIC_WELCOME,               /* 欢迎界面 */
    UIC_LOG,                   /* 日志界面 */
    UIC_CALIBRATION,           /* 标定界面 */
    UIC_SETTINGS,              /* 系统设定 */
    UIC_MONITOR,               /* 监控界面 */
    UIC_EDIT,                  /* 编辑器界面 */
    UIC_PROGRAM_MNG,           /* 项目管理界面 */
    UIC_ADMIN,                 /* 超级用户界面 */
    UIC_DEBUG,                 /* 调试界面 */
    UIC_HELP,                  /* 帮助界面 */
    UIC_VAR_MANAGE,            /* 变量管理界面 */
    UIC_LOG_DETAILS,           //日志详情页面    12

    UIC_NONE = -1
};

// 启动模式
enum LaunchMode {
    LAUNCHMODE_REAL,
    LAUNCHMODE_SIMU
};

// 轴模式
enum AxisMode {
    JOINT_AXIS,         // 单轴模式
    CART_AXIS,          // 迪卡尔模式
    WORLD_AXIS,
    BASE_AXIS,
    TOOL_AXIS,
    WOBJ_AXIS
};

// 方向
enum Direction {
    DIR_POSITIVE,                   // 正向
    DIR_NEGATIVE                    // 负向
};

//传送带抑制或者激活
enum ConveyorState {
    CONVEYOR_ACTIVATE,            //激活
    CONVEYOR_DEACTIVATE,           //抑制
    ALL                             //所有
};

// 运行参数扩展命令
enum RunParaCMDExt{
    DIRECTION_MODE,             /* 方向模式 */
    RUN_MODE,                   /* 单步连续模式 */
    LOOP_MODE,                  /* 循环模式 */
    PROG_SPEED_PERCENT
};

// 程序方向模式
enum ProgDirMode{
    PROGRAM_POSITIVE,                   /* 正向模式 */
    PROGRAM_NEGATIVE                    /* 逆向模式 */
};

// 程序运行模式
enum ProgRunMode{
    PROGRAM_STEP,                       /* 单步执行 */
    PROGRAM_CONSTANT                    /* 连续执行 */
};

// 程序循环模式
enum ProgLoopMode{
    PROGRAM_SINGLE,             // 单次模式
    PROGRAM_LOOP                // 循环模式
};

// 机械单元类型
enum MecUnitType {
    INDUSTRYROBOT,
    POSITIONER
};

// 机械单元信息
struct MechUnit {
    MecUnitType type;                   // 机械单元类型
    char model[ MAX_STRING_LENGTH ];    // 型号
    char name[ MAX_STRING_LENGTH ];     // 单元名称
};

// 手动控制命令扩展字
enum ManualCmdExt {
    TOOL_CORD = 0,                  // 工具坐标系
    WOBJ_CORD,                      //工件坐标系
    WORKBENCH_CORD,                 //工作台坐标系
    CARTESIAN_CORD_FRAME,           // JOG运动参考坐标系
    CARTESIAN_CORD_TYPE_DISPLAY,    // 显示参考坐标系
    AXIS_MODE,                      // 轴模式
    STEPPING_OVERRIDE,              // 步进速率
    SPEED_PERCENT                   // 速度百分比
};

// IO类型
enum IOType {
    DI,
    DO,
    AI,
    AO
};

enum AnalogType {
    CURRENT,
    VOLTAGE
};

// RC回复的命令类型
enum CmdSolicitedType {
    SOLICITED,              // 请求回复类型 需回复
    UNSOLICITED             // 主动发送类型 不需要回复
};

/*HMI和RC之间的通信命令*/
enum CmdType {
    HMI_GET_SYS_CONFIG = 1,            // 查询系统配置信息
    HMI_SET_SYS_CONFIG,                // 设置系统配置信息
    HMI_GET_CUR_PROJECT,               // 查询当前的工程
    HMI_OPEN_PROJECT,                  // 打开工程
    HMI_CLOSE_PROJECT,                 // 关闭工程
    HMI_SET_ACTIVE_MU,                 // 设置当前控制机械单元
    HMI_GET_SYSTEM_STATE,              // 查询系统状态
    HMI_SEND_SYSTEM_STATE,             // 发送系统状态
    HMI_GET_POSITION,                  // 查询位置
    HMI_GET_PROG_POSITION,			   // 获取程序更新位置
    HMI_GET_MECH_UNITS,                // 查询机械单元信息
    HMI_GET_CURRENT_LOADED_PROG,       // 获取当前加载程序
    HMI_GET_TOOLS,                     // 查询当前的工具坐标
    HMI_GET_WOBJS,                     // 查询当前的工件坐标
    HMI_GET_EROBPOSE,                  // 查询带外轴的机器人笛卡尔格式位置点
    HMI_GET_EROBJOINT,                 // 带外轴的机器人轴位置点
    HMI_MOTOR_ON,                      // 电机是否上电
    HMI_MOTOR_OFF,                     // 电机是否断电
    HMI_SET_MANU_PARA,                 // 设置当前机械单元的手动配置信息
    HMI_GET_MANU_PARA,                 // 获取当前机械单元的手动配置信息
    HMI_START_JOG,                     // 启动机械单元运动
    HMI_STOP_JOG,                      // 停止机械单元运动
    HMI_SET_RUN_PARA,                  // 设置程序运行参数
    HMI_GET_RUN_PARA,					//获取程序运行参数
    HMI_LOAD_PROJECT,                     // 加载任务
    HMI_START_RUN,                     // 启动程序运行
    HMI_STOP_RUN,                      // 停止程序运行
    HMI_RUN_TO_CURSOR,					//运行到当前行 TODO caohua 可以考虑和HMI_START_RUN合并，该功能完善之后合并
    HMI_ACK,                           // 握手应答
    HMI_ERR,                           // 答复失败
    HMI_CALIBRATE,                      // 发送标定命令
    HMI_SET_DO,                         // 设定DO
    HMI_GET_DI,                         // 获取DI
    HMI_SELECT_PROG_FILE,               // 选择任务
    HMI_USER_MANAGE,                    // 用户管理
    HMI_GET_LIRCOS_VERSION,             // 获取当前控制器版本信息
    HMI_GET_RC_TIME,                    //获取RC的时间
    HMI_SET_MODE,                       // 设置工作模式，手动/自动
    HMI_ALARM_CLEAR,                    // 清除报警
    HMI_READ_SYSTEM_VARIABLE,           // 读系统变量
    HMI_WRITE_SYSTEM_VARIABLE,          // 写系统变量             //目前木有使用
    HMI_READ_GLOBAL_VARIABLE,
    HMI_WRITE_GLOBAL_VARIABLE,
    HMI_RESET_PROG,                     // 程序复位
    HMI_SET_SIMU_MODE,                  // 设置仿真模式
    HMI_SHUT_DOWN_RC,                   // 关闭控制器
    HMI_REBOOT_RC,                      // 重启控制器
    HMI_SEND_PRINT_INFO,
    HMI_SAVE_SYSTEM_VARIABLE,           // 保存系统变量       //目前木有使用
    HMI_VREP,                           // VREP仿真
    HMI_ROS,                            //ROS仿真
    HMI_GET_WORKBENCH,                  //查询工作台坐标系
    HMI_SET_WORKBENCH,                  //修改指定的工作台坐标系
    HMI_SET_TOOL_COORD,					//修改指定的工具坐标系
    HMI_SET_WOBJ_COORD,					//修改指定的工件坐标系
    HMI_GET_SOFT_LIMIT,					//读取机械单元的软限位
    HMI_SET_SOFT_LIMIT,					//配置机械单元的软限位
    HMI_UPDATE_RC,  					//通知RC更新程序
    HMI_GET_JOINT_LIMIT,				//读取机械单元的软限位
    HMI_GET_ROS_RETURN_HOME,			//读取ros是否回零点标志位
    HMI_SET_ROS_RETURN_HOME,            //设置ros回零点标志位
    HMI_GET_ROS_RECEIVE_PERIOD,         //读取ros接收指令周期
    HMI_SET_ROS_RECEIVE_PERIOD,         //设置ros接收指令周期
    HMI_GET_ROS_TIMEOUT,                //获取ros超时时间
    HMI_SET_ROS_TIMEOUT,                //设置ros超时时间
    HMI_GET_ROS_INTERP_TYPE,            //获取ros插补类型
    HMI_SET_ROS_INTERP_TYPE,            //设置ros插补类型
    HMI_GO_HOME,                        //回到零点
    HMI_IS_HOME,                        //是否已回到零点
    HMI_SAVE_CURRENT_JOINT,             //存储当前关节角
    HMI_GET_IS_SINGULAR,                //获取是否奇异点
    HMI_GET_MAC_ADDRESS,                //获取mac地址
    HMI_SET_AUTHORIZATION_CODE,         //发送授权码
    HMI_GET_AUTHORIZATION_CODE,         //获取授权码
    HMI_GET_AXLE_MOUIT_PARAM,           //获取轴运动参数
    HMI_GET_SPACE_MOUIT_PARAM,          //获取空间运动参数
    HMI_GET_COUPLING_COEFFICIENT,       //获取耦合系数
    HMI_SET_AXLE_MOUIT_PARAM,           //发送轴运动参数
    HMI_SET_SPACE_MOUIT_PARAM,          //发送空间运动参数
    HMI_SET_COUPLING_COEFFICIENT,       //发送耦合系数
    HMI_GET_AXLE_MOVEMENT_APEED_PARAM,  //获取轴运动速度参数
    HMI_SET_AXLE_MOVEMENT_APEED_PARAM,  //发送轴运动速度参数
    HMI_GET_ENCODER_DATA,               //获取编码器数值
    HMI_SET_ENCODER_DATA,               //设置编码器数值
    HMI_GET_PROG_POINTER,               //获取程序指针
    HMI_GET_DO,                         //获取DO
    HMI_SET_SOFT_LIMIT_ENABLE,          //设置软限位是否使能
    HMI_SET_GUIDE_STATE,                //设置导轨状态
    HMI_GET_GUIDE_STATE,                //获取导轨状态
    HMI_REFRESH_VAR_CONTENT,            //刷新变量内容
    HMI_GET_ROBOT_TYPE,                 //获取机器人类型
    HMI_SET_ROBOT_TYPE,                 //设置机器人类型
    HMI_SET_DI_MODEL,                   //设置DI模式
    HMI_SET_DI,                         //设定DI值
    HMI_GET_SIMU_MODE,                  //获取HMI模式
    HMI_GET_DIRVE_TYPE,                 //获取驱动器类型
    HMI_SET_DIRVE_TYPE,                 //设置驱动器类型
    HMI_GET_SUPPORT_ROBOT_MODELS,       //获取系统支持的机器人类型
    HMI_GET_TRACK_CONFIG_PARA,          //获取导轨配置参数
    HMI_SET_TRACK_CONFIG_PARA,          //设置导轨配置参数
    HMI_GET_IO_CONFIG_PARA,             //获取IO配置参数
    HMI_SET_IO_CONFIG_PARA,             //设置IO配置参数
    HMI_GET_SYS_IO_SIGNAL_CONTENT,      //获取系统输入信号状态
    HMI_SET_SYS_IO_SIGNAL_CONTENT,      //设置系统输入信号状态
    HMI_GET_OCCUPIED_IO_PORTS,          //获取被占用的端口号
    HMI_START_LOAD_IDENT,               //HMI启动负载辨识过程
    RC_NOTIFY_LOAD_IDENT_RES,           //RC通知HMI负载辨识的结果
    HMI_GET_PIO_CONFIG_PARA,            //HMI获取Profinet IO配置参数
    HMI_SET_PIO_CONFIG_PARA,            //HMI设置Profinet IO配置参数
    HMI_GET_PIO_RUNNING_STATES,         //HMI获取Profinet IO运行状态
    HMI_GET_SIGNAL_ID_SET,              //HMI获取信号ID集合
    HMI_CONFIGURE_SIGANL_CONTENT,       //HMI配置信号
    HMI_SET_LOAD_THROUGH_PROG_PARA,     //设置拖动示教参数
    HMI_GET_LOAD_THROUGH_PROG_PARA,     //获取拖动示教参数
    HMI_SET_MOTOR_CONTROL_MODE,         //设置电机控制模式
    HMI_START_DYNAMIC_IDENT,            //HMI启动本体参数辨识
    RC_NOTIFY_DYNAMIC_IDENT_RES,        //RC通知HMI本体参数辨识的结果
    HMI_SAVE_DYNAMIC_IDENT_RES,         //保存本体参数辨识结果
    HMI_SET_INPUT_SIGNAL_SIMU_MODE,     //状态监控界面设置DI或者GI模式
    HMI_SET_IO_SIGNAL_VALUE,            //设置IO信号值
    HMI_GET_IO_SIGNAL_VALUE,            //获取IO信号值
    HMI_SET_MOTION_CONTROL_PARA,        //设置运动控制参数
    HMI_GET_MOTION_CONTROL_PARA,        //获取运动控制参数
    RC_NOTIFY_CLEAR_ALARM,	            //RC通知HMI清除报警
    HMI_SET_STATE_PUBLISH_SOCKET,       //设置状态发布socket信息
    HMI_GET_COLLISION_DETECTION_STATE,                 //获取碰撞检测开关状态
    HMI_SET_COLLISION_DETECTION_STATE,                 //设置碰撞检测开关状态
    HMI_GET_COLLISION_DETECTION_LEVEL,                 //获取碰撞检测级别
    HMI_SET_COLLISION_DETECTION_LEVEL,                 //设置碰撞检测级别
    HMI_GET_MOTION_SUPERVISION_MAX_LEVEL,              //获取碰撞检测最大运动监控级别
    HMI_SET_MOTION_SUPERVISION_MAX_LEVEL,              //设置碰撞检测最大运动监控级别
    HMI_GET_COLLISION_DETECTION_MEMORY,                //获取碰撞检测缓存时间
    HMI_SET_COLLISION_DETECTION_MEMORY,                //设置碰转检测缓存时间
    HMI_GET_MOTOR_STATE_AFTER_BAKCSPACING,             //获取回退完成后电机状态
    HMI_SET_MOTOR_STATE_AFTER_BAKCSPACING,             //设置回退完成后电机状态
    HMI_GET_AUTO_RESTART_SWITCH_STATE_AFTER_COLLISION, //获取碰撞后自恢复运行开关状态
    HMI_SET_AUTO_RESTART_SWITCH_STATE_AFTER_COLLISION, //设置碰撞后自恢复运行开关状态
    HMI_GET_RECOVERY_TIME_AFTER_COLLISION,             //获取碰撞后自恢复等待时间
    HMI_SET_RECOVERY_TIME_AFTER_COLLISION,             //设置碰撞后自恢复等待时间
    HMI_GET_EIO_CONFIG_PARA,                           //HMI获取Ethernet/IP IO配置参数
    HMI_SET_EIO_CONFIG_PARA,                           //HMI设置Ethernet/IP IO配置参数
    HMI_GET_EIO_RUNNING_STATES,                        //HMI获取Ethernet/IP IO信号状态
    HMI_RESET_COLLISION_DETECTION_PARA,                //HMI重置碰撞检测参数为默认值
    HMI_PP_TO_MAIN,                                    //pp to main
    HMI_PP_TO_ROUTINE,                                 //pp to routine
    HMI_CHECK_RL_MOD_GRAMMAR,                          //检查程序
    HMI_SET_GRAVITY_COEFFICIENT,                       //用户设置地面安装或者吊装
    HMI_GET_GRAVITY_COEFFICIENT,                       //HMI获得重力系数参数
    HMI_GET_STATE_PUBLISH_SOCKET,                      //HMI获得状态发布socket信息
    RC_NOTIFY_PP_REFRESH,                              //RC通知HMI程序指针刷新
    HMI_CHECK_SUPER_AUTHORIZATION_CODE,                //HMI传给RC的用户输入的超级管理权限的授权码,RC给HMI返回结果
    HMI_GET_SUPER_AUTHORIZATION_CODE_RESULT,           //RC传送给用户的关于授权码的结果
    HMI_RESET_SOFT_LIMIT,			            	   //重置读取机械单元的软限位
    HMI_GET_LOAD_MASS_LIMIT,                           //HMI获得RC发送的负载质量限制
    HMI_GET_SYS_IO_LIST,                               //HMI获得RC发送的系统IO LIS
    HMI_LOG_MODULE_INIT_FINISH,                        //HMI通知RC日志模块初始化完毕
    HMI_GET_SUPER_AUTHORIZATION_CODE,                  //HMI从RC获取超级授权码
    RC_NOTIFY_DATA_CHANGE,                             //RC通知HMI数据变化
    HMI_GET_CONVEYOR_ID_LIST,                          //HMI获得传送带的id表
    HMI_CONFIGURE_CONVEYOR,                            //HMI对新建传送带进行配置!!!!!!!!!!!!!!!!
    HMI_GET_CONVEYOR_MOTION_STATE,                     //HMI获取传送带的速度和X位置
    HMI_GET_LEAD_THROUGH_TEACHING_PARA,                //HMI从RC获取拖动示教的参数
    HMI_SET_LEAD_THROUGH_TEACHING_STATE,               //设置拖动示教再现开始或者停止
    HMI_SET_LEAD_THROUGH_TEACHING_PLAYBACK_STATE,      //拖动示教开始回放和停止回放
    HMI_GET_LEAD_THROUGH_TEACHING_BUFFER_PERCENT,      //获取拖动示教buffer的使用百分比
    HMI_GET_LEAD_THROUGH_TEACHING_SAMPLING_AMOUNT,     //获取拖动示教采样点总数
    HMI_GET_LEAD_THROUGH_TEACHING_SAMPLING_VALUE,      //获取拖动示教采样点值
    HMI_GET_LEAD_THROUGH_TEACHING_PLAYBACK_LINE,       //获取拖动示教回放程序执行行号
    HMI_GET_LEAD_THROUGH_TEACHING_SPEED,               //获取拖动示教回放轨迹速度
    HMI_CLEAR_R29_ALARM,                               //获取拖动示教回放轨迹速度
    HMI_SET_CONVEYOR_CALIBRATION_START,                //传送带开始标定函数
    HMI_GET_CONVEYOR_ENCODER_COUNTS,                   //HMI获得RC发过来的编码器的counts int型数据
    HMI_SAVE_CONVEYOR_COUNTS_PER_METER,                //HMI向RC发送double的counts per meter
    HMI_SET_CONVEYOR_ENABLE_STATUS,                    //HMI设定传送带开启状态
    HMI_GET_CONVEYOR_ENABLE_STATUS,                    //HMI获取传送带开启状态
    HMI_GET_LAST_LOAD_PROG_PATH,                       //HMI获取上一次加载的工程的路径
    HMI_SET_RC_TIME,                                   //HMI设置时间
    HMI_SET_COORPERATIVE_MODE,                         //设置协作模式状态!!!!!!!!!!!
    HMI_GET_COOPERATIVE_MODE,                          //获取协作模式状态
    HMI_SET_COORPERATIVE_DI,                           //设置协作模式DI设置
    HMI_GET_COOPERATIVE_DI,                            //获取协作模式DI设置
    HMI_GET_LEAD_THROUGH_PROJ,                         //获取拖动示教工程列表
    HMI_GET_LEAD_THROUGH_PROJ_SETTING,                 //获取拖动示教工程的参数
    HMI_OPERATION_LEAD_THROUGH_PROJ,                   //删除或者运行拖动示教工程
    HMI_SET_LEAD_THROUGH_PROJ_PARA,                    //设置生成拖动示教工程参数()
    HMI_SET_LEAD_THROUGH_PROJ_RUN_PARA,                //设置拖动示教运行参数
    HMI_GET_LEAD_THROUGH_TEACHING_TIME_PERCENT,        //获取拖动示教时间使用率
    HMI_SET_LEAD_THROUGH_TEACHING_RECORD_STATE,        //设置拖动示教轨迹记录状态
    HMI_CLEAR_LEAD_THROUGH_TEACHING_RECORD,            //清空拖动示教轨迹


    HMI_SET_LTT_BASE_ON_TIME_PLAYBACK_STATE,           //设置拖动示教回放状态

    HMI_GET_LEAD_THROUGH_TEACHING_PLAYBACK_PERCENT,    //获取拖动示教回放百分比

    RC_NOTIFY_LEAD_THROUGH_TEACHING_PROJ_RUN,          //RC通知拖动示教工程运行

    HMI_SET_LTT_PROJ_RUN_STATE,						   //设置拖动示教工程运行开始/停止

    HMI_GET_LTT_PROJ_RUN_MAX_RATE,                     //获取拖动工程运行的最大速率
    RC_NOTIFY_LTT_RECORD_STATE,            			   //RC通知拖动示教开始/停止记录
    HMI_SET_LTT_PROJ_RUN_SIGNAL_LISTEN,                //设置启动或者停止监听拖动工程运行信号
    RC_NOTIFY_LTT_SAVE_ERROR,						   //拖动示教记录已满，保存出错
    HMI_NOTIFY_LTT_ERROR_CLEAR,                        //HMI通知RC通过DI控制拖动状态/程序运行状态的错误已经清除
    HMI_GET_R29_ALARM_STATE,						   //获取R29告警状态
    RC_SEND_LTT_PROJ_PARA,                             //上报拖动回放工程参数
    RC_NOTIFY_LTT_RECORD_TO_FULL,                      //上报拖动回放记录已满
    RC_NOTIFY_LTT_PROJ_SAVE_SUCC,                      //生成拖动工程成功
    HMI_GET_IS_JNT_OVER_RANGE,                         //获取关节是否超限
    HMI_SET_VREP_CONTENT_AND_STATE,                    //HMI设置vrep连接状态和内容
    HMI_GET_VREP_CONTENT,                              //HMI获得vrep连接内容
    RC_NOTIFY_VREP_STATE,                              //RC通知HMI vrep连接状态
    HMI_SET_TO_SPECIFIED_GESTURE,					   //HMI通知RC设置回到指定姿态
    HMI_STOP_TO_SPECIFIED_GESTURE,                     //HMI通知RC停止到姿态的移动
    HMI_GET_FC_USER_DATE,                              //获取力控用户数据
    HMI_SET_FC_USER_DATE,                              //设置力控用户数据
    HMI_GET_FC_SENSOR_DATE,                            //获取力控传感器数据
    HMI_SET_FC_SENSOR_DATE,                            //设置力控传感器数据
    HMI_GET_FC_STATUS,                                 //HMI获取FC开启状态
    HMI_SET_FC_STATUS,                                 //HMI设定FC开启状态
    HMI_GET_FC_MONITOR_DATA,                           //HMI获取FC在状态监控页面的参数
    HMI_GET_TASK_LIST,                                 //获取task的ID列表
    HMI_GET_TASK_CONFIG_PARA,                          //根据ID获取task
    HMI_GET_TASK_RUNNING_STATE,                        //获取task运行状态
    HMI_SET_TASK_SELECT_STATE,                         //设置task通过薄膜按键控制的列表
    HMI_GET_TASK_SELECT_STATE,                         //获取task通过薄膜按键控制的列表
    HMI_SET_TASK_SELECT_SETTING,                       //设置Only Normal/All Tasks
    HMI_GET_TASK_SELECT_SETTING,                       //获取Only Normal/All Tasks
    HMI_OPERATION_TASK,                                //新建、修改、删除task
    HMI_GET_MULTI_TASKING_STATE,                        //获取多任务启动或者关闭状态
    HMI_GET_DYNAMIC_PARA_TYPE,                          //获取机器人运动规划使用的的动力学参数类型
    HMI_SET_DYNAMIC_PARA_TYPE,                          //设置机器人运动规划使用的的动力学参数类型
    HMI_SET_IP_CONFIG_INFORMATION,                      //设置调试或视觉的IP地址
    HMI_GET_IP_CONFIG_INFORMATION,                      //获取调试或视觉的IP地址
    HMI_GET_MONITOR_POSITION,                           //根据当前参考坐标系获取监控界面位置信息
    HMI_GET_CONTROL_PERIOD,                             //获取控制周期
    HMI_SET_CONTROL_PERIOD,                             //设置控制周期
    HMI_GET_ROBOT_DIMENSIONS,                           //获取连杆参数
    HMI_SET_ROBOT_DIMENSIONS,                           //设置连杆参数
    HMI_SET_ROKAE_CONNECT_ENABLED_STATE,                //设置ROKAE.CONNECT的启用状态
    HMI_GET_ROKAE_CONNECT_PUBLISH_STATE,                //获取ROKAE.CONNECT是否发布状态信息
    HMI_SET_ROKAE_CONNECT_PUBLISH_STATE,                //设置ROKAE.CONNECT是否发布状态信息
    HMI_GET_ROKAE_CONNECT_PUBLISH_PERIOD,               //获取ROKAE.CONNECT发布周期
    HMI_SET_ROKAE_CONNECT_PUBLISH_PERIOD,               //设置ROKAE.CONNECT发布周期
    HMI_GET_GRAVITY_VECTOR,                             //获取重力场向量
    HMI_SET_GRAVITY_VECTOR,                             //设置重力场向量
    HMI_CAL_GRAVITY_VECTOR,                             //计算重力场投影
    HMI_SET_FEEDFORWARD_COMPENSATION,                   //HMI设置是否开启补偿前馈状态
    HMI_GET_BASE_FRAME,                                 //HMI获取基座标系，只获取姿态
    HMI_SET_BASE_FRAME,                                 //HMI设置基座标系，设置姿态、重力投影
    HMI_GET_PALLET_ID_LIST,                              //获取料盘ID集合
    HMI_CONFIGURE_PALLET,                                //配置料盘
    HMI_GET_LAST_DYNAMIC_IDENTIFY_TIME,                  //HMI获取RC是否动力学辨识状态
    HMI_SET_EULER_ANGLE_ORDER,                           //HMI设置欧拉角顺序
    HMI_GET_EULER_ANGLE_ORDER,                           //HMI获取欧拉角顺序
    HMI_GET_TEACH_PENDANT_ENABLE_STATE,                  //获取HMI是否使用的状态
    HMI_SET_TEACH_PENDANT_ENABLE_STATE,                  //设置HMI是否使用的状态
    HMI_SET_EMERGENCY_STOP_STATE,                        //HMI设置示教器急停状态
    HMI_GET_EMERGENCY_STOP_STATE,                        //HMI获取示教器急停状态
    HMI_GET_SOFT_LIMIT_ENABLE,                            //HMI获取软限位是否使能
    RC_NOTIFY_COOPERATIVE_WORK_STATE,                      //RC通知HMI协作模式工作状态

    HMI_GET_AIO_CONFIG_PARA,                              //HMI获取AO配置参数
    HMI_SET_AIO_CONFIG_PARA,                              //HMI设置AO配置参数
    HMI_SET_HOME_STATUS,                                //HMI设置作业原点开启状态
    HMI_GET_HOME_STATUS,                                //HMI获取作业原点开启状态
    HMI_SET_HOME_RECORD,					            //HMI设置作业原点的数据
    HMI_GET_HOME_RECORD, 					            //HMI获取作业原点的数据

    HMI_EULER_TO_QUATERNION,                             //欧拉角转换四元数
    HMI_QUATERNION_TO_EULER,                            //四元数转欧拉

    HMI_GET_SOCKET_INFO_LIST,                           //HMI获取socket list的信息        //未使用
    HMI_GET_MODBUS_CONFIG_PARA,                         //获取modbus配置参数
    HMI_SET_MODBUS_CONFIG_PARA,                         //设置modbus配置参数
    HMI_SET_MODBUS_CONNECT_STATE,                       //设置modbus连接状态
    HMI_GET_MODBUS_CONNECT_STATE,                       //获取modbus连接状态
    RC_NOTIFY_MODBUS_STATE,                             //RC主动上送，modbus连接终断事件
    HMI_NOTIFY_LANGUAGE,                                //HMI主动通知语言
    RC_NOTIFY_SOCKET_LOAD_PROJECT,                           //socket加载工程
    RC_NOTIFY_NONMOTION_TASK,                           //刷新静态任务的运行状态

    HMI_GET_MATERIAL_PALLET_ID_LIST,
    HMI_CONFIGURE_MATERIAL_PALLET,

    HMI_GET_ENCODER_PULSE_VALUE,                        //获取编码脉冲值
    HMI_GET_CALIEBRATE_ANGLE,                           //获取标定角度
    HMI_SET_CALIEBRATE_ANGLE,                           //设定标定角度

    HMI_GET_SERVO_CONNECT,                             //获取伺服接通启用状态,0使用，1不使用
    HMI_SET_SERVO_CONNECT,                             //设置伺服接通启用状态,0使用，1不使用

    HMI_RESTORATION,                                    //急停复位
    HMI_INSERT_LOG,                                      //主动插入指令

    HMI_GET_CARVING_MACHINE_ROBTARGET,                     //HMI获取精雕机robtarget 列表
    HMI_SET_CARVING_MACHINE_ROBTARGET,                   //HMI设置精雕机robtarget所有数据
    HMI_GET_CARVING_MACHINE_TIME,                      //HMI获取精雕机TIME所有数据
    HMI_SET_CARVING_MACHINE_TIME,                      //HMI设置精雕机TIME所有数据
    HMI_GET_CARING_MACHINE_NAME,                       //HMI精雕机名字获取
    HMI_SET_CARING_MACHINE_NAME,                         //HMI精雕机名字设置 287

    RC_NOTIFY_LOCK_STATE,                               //rc上送锁机状态
    HMI_SET_UNLOCK_LICENSE_CODE,                        //设置激活码
    HMI_GET_REMAIN_HOUR,                                //获取剩余可用时长
    HMI_SET_RANDOM_CODE,                            	//设置随机码
    HMI_GET_RANDOM_CODE,                                //获取随机码
    RC_NOTIFY_PERIOD_VALIDE,                            //当可用时长只剩一天时，提醒用户续时长
    RC_NOTIFY_UNCLEARED_BLOCK_ALARM,                    //控制器推送未清除阻塞机器人的告警
    HMI_GET_LOCK_STATE,                                 //获取锁机状态
    HMI_CONFIGURE_SERIAL_CONTENT,                       //HMI配置串口属性， 296
    HMI_GET_FOUND_SERIALS_DEVICE,                       //HMI获取找到的串口设备 297
    HMI_GET_SERIAL_ID_LIST,                              //获取串口列表
    RC_NOTIFY_SET_STEP_OVERRIDE,                       //rc主动上送程序运行速率

    HMI_GET_ROBOT_INSTALLATION_MODE,                    //临时增加，获取机器人安装方式，0倒装，1正装
    HMI_SET_ROBOT_INSTALLATION_MODE,                    //临时增加，设置机器人安装方式

    HMI_GET_MATERIAL_PALLET_POINT,                      //获取料盘点坐标信息
    RC_NOTIFY_SHUT_DOWN,                                //控制器即将关机

    //    HMI_SET_DB2ROPC,                                    //刷新电子铭牌
    //    HMI_SET_ROPC2DB,                                    //刷新数据库
    //    RC_NOTIFY_ROPC_SYNC,                                //同步数据结果回复
    //    HMI_GET_ROPCMATCHDB                                //电子铭牌与数据库是否匹配

    HMI_GET_BLENDING_PATH_STATE,                        //转弯区状态获取
    HMI_SET_BLENDING_PATH_STATE,                        //转弯区状态设置

    HMI_SET_AIO_SIGNALTYPE,                             //设置MINI板上AIO各通道的类型：电流OR电压
    HMI_GET_AIO_SIGNALTYPE,                             //获取各通道的类型
    HMI_GET_XBC5_STATE,                                 //获取XBC5控制柜状态
    HMI_GET_MINI_IOBOARDLIST,                           //获取MINI板IO列表
    HMI_NOTIFY_READY_TO_HOT_PLUG,                       //通知HMI准备进入无示教器状态

    HMI_GET_DYNAMICS_FEEDFORWARD_STATE,                 //动力学开关状态获取
    HMI_SET_DYNAMICS_FEEDFORWARD_STATE,                 //动力学开关状态设置

    HMI_NOTIFY_MINIBOARD_STATE_CHANGED,                 //通知HMI MINI IO 板发生拔插事件

    HMI_XPAD2_MODE,                                     //是否为XPAD2  bool  false:xpad1  true:xpad2

    HMI_COMMAND_MAX_COUNT

    // 以上命令为自己定义SignalType
};

/**
 * @brief 机器人调整到某一姿态的类型
 */
enum RobotToSpecifiedGestureType {
    ROBOT_TO_PARALLEL_TO_GROUND_TYPE,                          //法兰平面与地面平行
    ROBOT_TO_X_PERPENDICULAR_TO_GROUND_AND_GOING_DOWN_TYPE,    //X轴垂直于地面，正向朝下
    ROBOT_TO_X_PERPENDICULAR_TO_GROUND_AND_GOING_UP_TYPE,      //X轴垂直于地面，负向朝下
    ROBOT_TO_Y_PERPENDICULAR_TO_GROUND_AND_GOING_DOWN_TYPE,    //Y轴垂直于地面，正向朝下
    ROBOT_TO_Y_PERPENDICULAR_TO_GROUND_AND_GOING_UP_TYPE,      //Y轴垂直于地面，负向朝下
    ROBOT_TO_Z_PERPENDICULAR_TO_GROUND_AND_GOING_DOWN_TYPE,    //Z轴垂直于地面，正向朝下
    ROBOT_TO_Z_PERPENDICULAR_TO_GROUND_AND_GOING_UP_TYPE,      //Z轴垂直于地面，负向朝下
    ROBOT_TO_ZERO_GESTURE_TYPE,                                //机器人回到零点位置
    ROBOT_TO_TRANSSHIPMENT_GESTURE_TYPE,                        //机器人回到转运位置
    ROBOT_TO_HOME_GESTURE_TYPE                                 //机器人回到起始点位置
};

//协作模式的级别
enum COOPERATIVE_MODE_LEVEL {
    COOPERATIVE_MODE_LEVEL_FIRST,         //第一级别
    COOPERATIVE_MODE_LEVEL_SECOND         //第二级别
};

//ros插补类型
enum ROS_INTERP_TYPE {
    ROS_INTERP_ROSTIME = 0,		//插补以ros时钟为准
    ROS_INTERP_RCTIME			//插补以rc时钟为准
};

enum IF_SAVE_SYSTEM_VARIABLE{
    NOT_SAVE_SYSTEM_VARIABLE,
    SAVE_SYSTEM_VARIABLE
};

//modbus连接
enum ModbusConnectState
{
    MODBUS_CONNCET,            //连接modbus
    MODBUS_DISCONNECT            //关闭modbus
};

enum HMI_NETWORK_CONFIG_EXTENSION{
    IP_CONFIG,
    PORT_CONFIG
};

// 用户管理相关的扩展命令
enum UserManagerExe {
    ADD_USER,                   // 登录后添加用户
    DELETE_USER,                // 删除用户
    CHECK_USER,                 // 用户合法性检查
    CHANGE_PW,                  // 修改用户密码
    LOGOUT                      // 注销
};

enum HMI_ERR_REASON{
    HMI_ERR_NO_ACTIVE_MU,
    HMI_ERR_BAD_CMD,
    HMI_ERR_BAD_CMD_EXT,
    HMI_ERR_BAD_POINT_FOR_CALIBRATION
};

// 系统扩展字
enum SystemStateCmdExt {
    SYSTEM_STATE_ROBOT,                     /* 电机上电状态 */
    SYSTEM_STATE_PROGRAM,                   /* 查询程序状态 */
    SYSTEM_STATE_ALARM,                     /* 查询系统报警信息 */
    SYSTEM_STATE_MOVE_POINTER,              /* 查询运行语句指针 */
    SYSTEM_STATE_CONTROL_MODE,              /* 查询当前控制模式 */
    SYSTEM_STATE_ADVANCE_RUN_POINTER        /* 查询当前编译语句指针 */
};

//机器人状态位
enum ROBOT_State
{
    ROBOT_Initializing,          //初始化状态
    ROBOT_MOTOR_OFF,             //电机停止状态，初始化后进入电机停止状态
    ROBOT_MOTOR_ON,              //电机上电状态，此时可以jog或运行程序
    ROBOT_GENERAL_STOPPED,       //常规停止，由于安全保护装置被打开造成的停止
    ROBOT_EMERGENCY_STOPPED,     //紧急停止，按下急停造成的停止
    ROBOT_ERROR                  //机器人处于故障状态，需要重启
};

//锁机状态
enum LockState
{
    ROBOT_UNLOCK,                 //机器人未锁定
    ROBOT_LOCK                     //机器人锁定
};

// 程序状态
enum ProgExecState{
    PROG_EXECUTE_STATE_NOT_LOAD,                // 程序未加载
    PROG_EXECUTE_STATE_RUNNING,                 // 程序执行
    PROG_EXECUTE_STATE_STOP                     // 程序停止
};

// 标定方法
enum CALIBRATE_TYPE{
    ZERO_CALIBRATION,                             // 软标定
    THREE_POINT_CALIBRAT_USER_COOR,               //3点法用户坐标系
    MANUAL_CALIBRATE_WOBJ_COOR,                   //手动标定工件坐标系
    THREE_POINT_CALIBRAT_WOBJ_COOR,               //3点法标定工件坐标系
    MANUAL_CALIBRATE_TOOL_COOR,                   //手动标定工具坐标系
    THREE_POINT_CALIBRATE_TOOL_COOR,               //3点法标定工具坐标系
    FOUR_POINT_CALIBRAT_TOOL_COOR,                 //4点法标定工具坐标系
    SIX_POINT_CALIBRAT_TOOL_COOR,                  //6点法标定工具坐标系
    SIX_POINT_CALIBRAT_BASE_COOR,                  //6点法基础坐标系标定
    FOUR_POINT_CALBRAT_CONVEYOR_COOR,              //4点法标定传送带基坐标系、用户坐标系
    NINE_POINT_CALBRAT_CONVEYOR_CAMERA_COOR,                    //9点法标定相机
    ZERO_CALIBRATION_M,                              // 机械标定
    TWO_POINT_CALIBRATE_TOOL_COOR,                  //2点法标定工具坐标系
    ONE_POINT_CALIBRAT_WOBJ_COOR               //1点法标定工件坐标系

};

// ReturnValue为函数返回值列表
enum ReturnValue {
    ERROR_NONE = 0,             //无错误
    ERROR_NEW_FAIL,             //调用new失败，资源分配失败
    ERROR_IO_RW,                //IO读写失败
    ERROR_INVALID_PARA,         //函数输入参数无效
    ERROR_THREAD_CREATE_FAIL,   //线程创建失败
    ERROR_GET_ADDRESS,          //获取地址失败
    ERROR_CREATE,               //创建失败
    ERROR_BUFFER,               //数据缓冲区有误
    ERROR_FILE,                 //文件操作失败
    ERROR_SYSTEM_PARAM,         //系统参数错误. 添加：陈晓颖，2012-5-17
    ERROR_PARAM,
    COMPILE_COMPLETE,           //编译完成
    COMPILE_ERROR,              //编译错误,添加:王丽,2012-6-20
    PROCEDURE_OVER
};

// 轴类型
enum AxisType {
    EAT_INNER,                 // 内部轴
    EAT_OUTTER                 // 外部轴
};

//程序指令
//enum ProgramCommand {
//    COMMAND_NON,
//    COMMAND_MOVEJ,
//    COMMAND_MOVEABSJ,
//    COMMAND_MOVEL,
//    COMMAND_MOVC,
//    COMMAND_SETDO,
//    COMMAND_SETAO,
//    COMMAND_WAITDI,
//    COMMAND_WAITTIME,
//    COMMAND_INTERRUPT
//};

// 系统变量定义 NOTE： 移动定义的位置后需修改 QCommonSet::FillSystemVariableText 接口
enum SystemVariable {
    ESV_PROG_DIR_MODE = 0,                      // 程序方向模式
    ESV_PROG_STEP_MODE,                         // 程序单步连续模式
    ESV_PROG_LOOP_MODE,                         // 循环模式

    ESV_DO_BOARD_NUM,                           // DO板数量
    ESV_DO_COMMENT,                             // DO注释
    ESV_DO_SIG,
    ESV_DI_BOARD_NUM,                           // DI板数量
    ESV_DI_COMMENT,                             // DI注释
    ESV_DI_SIG,
    ESV_AO_BOARD_NUM,                           // AO板数量
    ESV_AO_COMMENT,                             // AO注释
    ESV_AO_SIG,

    ESV_CHANNEL_HOME_POS_MAX,                   // 安全区域最大范围
    ESV_CHANNEL_HOME_POS_MIN,                   // 安全区域最小范围

    ESV_USER_SIMU_MODE,                         // 用户仿真模式
    ESV_SOFT_STROKE_LIMIT_ENABLE,               // 软限位是否使能

    ESV_EX_JOINT_NUM,                           // 外轴数量
    ESV_MOTOR_NUM                               // 内轴数量
};

// 薄膜按键状态
enum ThinFilmButtonState {
    EFBS_RELEASE = 0,                           // 抬起
    EFBS_PRESS = 1,                             // 按下
    EFBS_HOLD = 2                               // 长按
};

enum ProgramState {
    MANUAL_PAUSE,
    MANUAL_RUNNING,
    AUTO_PAUSE,
    AUTO_RUNNING
};

//传感器模型
enum ForceControlModel {
    FCMODLE_ATI_AXIS80,
    FCMODLE_OPTOFORCE_HEX_70_CG_1000N,
    FCMOLE_HPS
};

//外部轴状态
enum ExternalAxisState
{
    GENERAL_STATE,           //普通状态
    EXTERNAL_AXLE_STATE      //存在外部轴的状态
};

//刷新变量内容的扩展命令
enum RefreshVarContentEx
{
    //无效类型
    REFRESH_VAR_TYPE_NONE,
    //基础类型
    REFRESH_KEY_INT,
    REFRESH_KEY_DOUBLE,
    REFRESH_KEY_BOOL,
    REFRESH_KEY_STRING,
    //复杂类型
    REFRESH_KEY_SPEED,
    REFRESH_VAR_TYPE_TOOL,
    REFRESH_VAR_TYPE_WOBJ,
    REFRESH_KEY_ZONE,
    REFRESH_VAR_TYPE_ROBTARGET,
    REFRESH_VAR_TYPE_JOINTTARGET,
    REFRESH_KEY_CLOCK,
    REFRESH_KEY_SYNCIDENT,
    REFRESH_KEY_TASKS,
    //力控变量
    REFRESH_KEY_LOAD,              // Load14
    REFRESH_KEY_FCBOXVOL,          // FCBoxVol
    REFRESH_KEY_FCSPHEREVOL,       //FCSphereVol
    REFRESH_KEY_FCCYLINDERVOL,     //FCCylinderVol
    REFRESH_KEY_FCXYZNUM,          //FCXyzNum
    REFRESH_KEY_FCCARTNUM,         //FCCartNum
    REFRESH_KEY_POSE,              //Pose

    REFRESH_KEY_BYTE               //byte
};

/**
 * @brief 发送byte数据类型枚举
 */
enum SendByteDataType {
    SEND_BYTE_DATA_SINGLE_BYTE,        //单个byte
    SEND_BYTE_DATA_ARRAY_BYTE,         //数组byte
    SEND_BYTE_DATA_INPUT_BYTE          //输入byte值
};


//机器人类型
enum RobotType
{
    ROBOT_TYPE_NONE,   //错误类型
    ROBOT_TYPE_XB04,   //4KG类型的机器人
    ROBOT_TYPE_XB06,   //6KG类型的机器人
    ROBOT_TYPE_XB07,   //7KG类型的机器人
    ROBOT_TYPE_UR10    //UR类型机器人
};

//机器人安装方式
enum InstallType
{
    INSTALL_TYPE_GROUND,    //地面安装
    INSTALL_TYPE_LIFT       //吊装
};

//DI模式
enum DIModel
{
    ACTUAL_MODLE,      //实际模式
    SIMULATION_MODEL   //仿真模式
};

//用户选择的升级选项
enum UpgradeOptions
{
    KEEP_USER_DATE,              //保留用户数据
    RESTORE_FACTORY_SETTINGS     //清除用户数据
};

//驱动器类型
enum DirveType
{
    DIRVE_TSINO_DYNATRON,     //清能德创
    DIRVE_PANASONIC,          //松下
    DIRVE_SERVOTRONIX         //高创
};
//IO信息扩展位，用于在获取或设置IO信号状态时区分IO

enum OccupiedSignalType {
    IO_SIGNAL_TYPE_DI,  //DI信号
    IO_SIGNAL_TYPE_DO,  //DO信号
    IO_SIGNAL_TYPE_PDI, //PDI信号
    IO_SIGNAL_TYPE_PDO  //PDO信号
};
enum SysIOSignalType {
    SYS_IO_SIG_INPUT,  ///系统输入
    SYS_IO_SIG_OUTPUT  ///系统输出
};

//负载辨识结果
enum LoadIdentifyRes
{
    LOAD_IDENT_RES_FAILED,              //负载辨识失败
    LOAD_IDENT_RES_RUNING,              //已到达零点位置，负载辨识正式执行
    LOAD_IDENT_RES_NONE_LOAD_SUCCESS,   //无负载辨识成功
    LOAD_IDENT_RES_WITH_LOAD_SUCCES     //有负载辨识过程完成，将传递识别的负载参数
};
//负载辨识工作模式
enum LoadIdentifyMode
{
    LOAD_IDENTIFY_NONE_LOAD, //无负载模式运行
    LOAD_IDENTIFY_WITH_LOAD  //有负载模式运行
};
//profinet工作状态
enum ProfinetIOStates
{
    PROFINET_IO_NOT_ENABLE, //未启用
    PROFINET_IO_RUNNING,    //工作正常
    PROFINET_IO_NOT_START   //工作异常
};
//ethernet工作状态
enum EthernetIOStates
{
    ETHERNET_IO_NOT_ENABLE, //未启用
    ETHERNET_IO_RUNNING,    //工作正常
    ETHERNET_IO_NOT_START   //工作异常
};

//机械单元
enum MechanicalUnit
{
    MECHANICAL_ARM,     //机械臂
    GUIDE               //导轨
};
//IO信号类别
enum SignalType
{
    SIGNAL_TYPE_DI,
    SIGNAL_TYPE_DO,
    SIGNAL_TYPE_GI,
    SIGNAL_TYPE_GO,
    SIGNAL_TYPE_AI,
    SIGNAL_TYPE_AO,
    SIGNAL_TYPE_ALL_SIGNAL ///获取ID集合时用到
};

/**
 * @brief io处理操作模版
 */
enum IoHandledType {
    IO_HANDED_TYPE_NEW_IO,         //新建IO
    IO_HANDED_TYPE_MODIFY_IO,      //修改IO
    IO_HANDED_TYPE_DELETE_IO       //删除IO
};

//IO信号配置命令拓展命令
enum SignalHandleCommand
{
    CONFIG_SIGNAL_INSERT_SIGNAL,      //插入一个Signal
    CONFIG_SIGNAL_REMOVE_SIGANL,      //删除一个Signal
    CONFIG_SIGNAL_UPDATE_SIGNAL,      //更新一个Signal
    CONIFG_SIGNAL_GET_SIGNAL_CONTENT  //根据ID查询一个Signal的配置详情
};

//传送带配置命令拓展命令
enum ConveyorConfigureCommand {
    CONFIGURE_ADD_CONVEYOR,      //添加一个Conveyor
    CONFIGURE_DEL_CONVEYOR,      //删除一个Conveyor
    CONFIGURE_MODIFY_CONVEYOR,      //更新一个Conveyor
    CONIFGURE_GET_CONVEYOR_CONTENT  //根据ID查询一个Conveyor的配置详情
};

enum SerialCommand {
    CONFIG_SERIAL_INSERT_SERIAL,      //插入一个串口属性
    CONFIG_SERIAL_REMOVE_SERIAL,      //删除一个串口属性
    CONFIG_SERIAL_UPDATE_SERIAL,      //更新一个串口属性
    CONIFG_SERIAL_GET_SERIAL_CONTENT //根据ID查询一个串口的配置详情
};

enum ConveyorCalibrationPoint8  {
    CONVEYOR_CALIBRATION_POINT8_1 = 1,     //传送带8点法标定第一点
    CONVEYOR_CALIBRATION_POINT8_2,     //传送带8点法标定第二点
    CONVEYOR_CALIBRATION_POINT8_3,     //传送带8点法标定第三点
    CONVEYOR_CALIBRATION_POINT8_4,     //传送带8点法标定第四点
    CONVEYOR_CALIBRATION_POINT8_5,     //传送带8点法标定第五点
    CONVEYOR_CALIBRATION_POINT8_6,     //传送带8点法标定第六点
    CONVEYOR_CALIBRATION_POINT8_7,     //传送带8点法标定第七点
    CONVEYOR_CALIBRATION_POINT8_8      //传送带8点法标定第八点
};

//电机控制模式
enum MotorControlMode
{
    position_control = 1,   //位置模式  拖动示教关闭
    veloctiy_control = 2,   //速度模式
    torque_control = 3      //零力模式  拖动示教开启  (开启拖动示教，不能jog) force-free mode
};
//获取拖动示教参数的类别
enum LeadThroughProgParaType
{
    DEFAULT_PARA,     //默认参数
    USER_PARA         //用户参数
};

//本体参数辨识工作模式DynamicIdentifyMode
enum DynamicIdentifyMode
{
    PILOT_RUN,       //试运行
    FORMALLY_RUN     //正式运行
};

//本体参数辨识结果
enum DynamicIdentifyRes {
    DYNAMIC_IDENT_TESTTING_RUN_FAILED,            //本体参数辨识试运行失败
    DYNAMIC_IDENT_FORMALLY_RUN_FAILED,            //本体参数辨识失败
    DYNAMIC_IDENT_RES_PILOT_RUN_SUCCESS,          //本体参数辨识试运行成功
    DYNAMIC_IDENT_RES_FORMALLY_RUN_SUCCES         //本体参数辨识成功
};

enum ProfinetIOMode
{
    PROFINET_IO_MODE_SERIAL,           //串口模式
    PROFINET_IO_MODE_ETHERCAT_SLAVE    //Ethercat从栈
};

//程序打开方式
enum ProjectOpenType
{
    LOAD_PROJECT,      //加载工程
    OPEN_MODUL,        //打开mod文件，打开未打开的.mod文件
    SHOW_MODUL         //显示mod文件，只可读
};

enum VarScopeType
{
    VAR_SCOPE_GLOBAL,  //全局变量
    VAR_SCOPE_LOCAL,   //本地变量
    VAR_SCOPE_ROUTINE  //子函数变量
};
//状态发布socket状态
enum StatePublishSocketState
{
    SOCKET_STATE_ON,            //连接socket
    SOCKET_STATE_OFF            //关闭socket
};

//vrep状态
enum VrepState {
    VREP_STATE_ON,            //连接VREP
    VREP_STATE_OFF            //断开VREP
};

//ip模式(调试或视觉)
enum IPType  {
    DEBUG_TYPE_IP,    //获取或设置调试IP
    VISION_TYPE_IP    //获取或设置视觉IP
};


//碰撞检测类别
enum CollisionDetectionType
{
    PATH_COLLISION_DETECTION,   //路径碰撞检测
    JOG_COLLISION_DETECTION     //jog碰撞检测
};
// 电机上电状态
enum MotorState
{
    MOTOR_STATE_ON,                     /* 电机上电 */
    MOTOR_STATE_OFF                     /* 电机断电 */
};
//更新数据类型
enum DataChangeType {
    TOOL_CHANGED,
    WOBJ_CHANGED
};

//日志的错误类型
enum ALARM_SOURCE {
    ALARM_SOURCE_NONE = 0,
    ALARM_SOURCE_OPEN_DEVICE_FAIL,
    ALARM_SOURCE_INTERP,
    ALARM_SOURCE_COMPILE,
    ALARM_SOURCE_EMERGENCY_PUSH,
    ALARM_SOURCE_CHANGE_CONTROL_MODE_WHEN_RUN,
    ALARM_SOURCE_START_POS_OVER_LIMIT,
    ALARM_SOURCE_DEV_SYS,
    ALARM_SOURCE_BATTERY_LOW_VOLTAGE,
    ALARM_SOURCE_DEV_SYS_UNMIND,
    ALARM_SOURCE_UPS,
    ALARM_SOURCE_HMI,
    ALARM_SOURCE_PROG_START_FAILED,
    ALARM_SOURCE_DETECT_COLLISION,
    ALARM_SOURCE_DETECT_COLLISION_AND_AUTO_RESTART,
    ALARM_SOURCE_CLOSE_PATH_DETECT_COLLISION,
    ALARM_SOURCE_CLOSE_JOG_DETECT_COLLISION,
    ALARM_SOURCE_EXCEED_MAX_PATH_DETECT_LEVEL,
    ALARM_SOURCE_ZERO_FORCE_CONTROL,
    ALARM_SOURCE_DYNAMIC_IDENT,
    ALARM_SOURCE_CONVEYOR_EXEC    //传送带执行问题
};

//拖动示教工程的相关操作
enum LeadThroughProjOperation {
    LEAD_THROUGH_PROJ_DELETE = 0,     //删除拖动示教工程
    LEAD_THROUGH_PROJ_CANCEL,         //取消创建拖动示教工程
    LEAD_THROUGH_PROJ_SAVE            //保存工程
};

//拖动回放工程开始和停止触发方式
enum LeadThroughProjTriggerType {
    LEAD_THROUGH_MANUAL_TRIGG = 0,          //手动按下按钮触发
    LEAD_THROUGH_AUTO_TRIGG                 //通过DI信号触发
};

//基于时间的拖动轨迹回放功能轨迹自动保存原因
enum LeadThroughProjAutoSaveReason {
    LTT_TIME_EXCEEDED = 0 ,          //超出最大时间
    LTT_MOTOT_OFF                    //电机下电
};

//控制器适配
enum ControllerAdaptation {
    CONTROLLER_ROKAE = 0,    //rokae机器人
    CONTROLLER_OTHER         //其他机器人
};
//机器人处于运行状态的具体原因
enum ProgStatusCause {
    NO_VALID_CAUSE,                          //机器人不是运行状态 0
    PET_RUNNING,                             //正在运行程序
    DRAG_REPLAY_POSITION_BASED_RUNNING,      //正在运行基于位置的拖动回放
    DRAG_REPLAY_TIME_BASED_RUNNING,          //正在运行基于时间的拖动回放
    DYNAMIC_IDENTIFY_RUNNING,                //正在运行本体参数辨识
    LOAD_IDENTIFY_RUNNING,                   //正在运行负载参数辨识
    OTHER_RUNNING                            //其他运行状态，JOG，重回路径，碰撞回退等6
};

//多任务的任务类型
enum TaskType {
    NORMAL_TASK,
    SEMISTATIC_TASK,
    STATIC_TASK
};

//多任务中任务的信任等级
enum TrustLevel {
    NONE_TRUST_LEVEL,
    NO_SAFETY,
    SYS_STOP,
    SYS_HALT,
    SYS_FAIL
};

//多任务配置操作
enum TaskOperation {
    MULTI_TASKING_NEW_TASK,       //新建
    MULTI_TASKING_MODIFY_TASK,    //修改
    MULTI_TASKING_DELETE_TASK     //删除
};

//状态监控界面task选择模式
enum TaskSelectMode {
    MULTI_TASKING_ONLY_NORMAL,     //仅常规任务
    MULTI_TASKING_ALL              //所有任务
};

/**
 * @brief 机器人运动规划时使用的动力学参数类型
 */
enum DynamicParametersType {
    DYNAMIC_PARA_AVERAGE_NORMINAL_PARA,
    DYNAMIC_PARA_FACTORY_IDENTIFICATION_PARA
};
/**
 * @brief 通过窗口创建的新建或者修改的类型
 */
enum DialogNewOrModifyType {
    NEW_TYPE,
    MODIFY_TYPE
};
/**
 * @brief 变量类型枚举
 */
enum VariableType {
    JOINTTARGET_TYPE,      //jointtarget
    ROBTARGET_TYPE,        //robtarget
    TRIGG_TYPE,            //trigg
    CLOCK_TYPE,            //clock
    LOAD_TYPE,              //load
    SPEED_TYPE               //speed
};
/**
 * @brief 变量管理界面对界面的操作
 */
enum HandleTypeOfVariable {
    DELETE_VAR,       //删除变量
    COPY_VAR,         //复制变量
    CREATE_VAR,       //新建变量
    MODIFY_VAR,       //修改变量
    NONE_HANDLE       //没有操作
};

/**
 * @brief 机器人类型图分类
 */
enum RobotCompanyType {
    XB_SERIES,       //xb系列
    YANGTIAN_SERIES  //扬天系列
};

/**
 * @brief 料盘配置命令拓展命令
 */
enum PalletConfigureCommand {
    CONFIGURE_ADD_PALLET,
    CONFIGURE_DEL_PALLET,
    CONFIGURE_MODIFY_PALLET,
    CONFIGURE_QUERY_PALLET
};

/**
 * @brief 欧拉角顺序
 */
enum EulerAngularSequence {
    EULER_ANGULAR_XYZ,      //XYZ顺序
    EULER_ANGULAR_ZYX       //ZYX顺序
};

// 监控的内容
enum E_MONITOR_CONTENT {
    EMC_POSITION,
    EMC_IO,
    EMC_TASK,
    EMC_SOCKET,
    EMC_VARIABLE
};

/**
 * @brief The POINT_TYPE enum 点类型
 */
enum POINT_TYPE {
    NONE_POINT,
    AUXILIARY_POINT,     //辅助点
    TARGET_POINT         //目标点
};

/**
 * @brief The SHOW_STATE enum  是否显示当前页面
 */
enum SHOW_STATE {
    STATE_SHOW,            //显示
    STATE_HIDE               //隐藏
};

/**
 * @brief The NEW_PROJECT_TYPE enum 项目管理界面中新建的类型
 */
enum NEW_PROJECT_TYPE {
    NEW_PROJECT,             //新建工程
    NEW_FILE                 //新建文件
};

/**
 * @brief 对工程的操作
 */
enum HandleTypeOfProject {
    DELETE_PROJECT,       //删除工程
    CREATE_PROJECT,       //新建工程
    RENAME_PROJECT,       //修改工程
    NONE_HANDLE_PROJECT  //无操作
};

#endif // HMICOMMONDEFINE_H
