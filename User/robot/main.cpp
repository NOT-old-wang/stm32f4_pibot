#include "board.h"
#include "os_include.h"
#include "data_holder.h"

#include "robot.h"

#define START_TASK_PRIO		3
#define START_TASK_STK_SIZE 		512
OS_TCB  START_TASK_TCB;
CPU_STK START_TASK_STK[START_TASK_STK_SIZE];
void start_task(void *p_arg);

#define BSP_TASK_PRIO		4
#define BSP_TASK_STK_SIZE 		256
OS_TCB  BSP_TASK_TCB;
CPU_STK BSP_TASK_STK[BSP_TASK_STK_SIZE];
void bsp_task(void *p_arg);

#define CHECK_COMMAND_TASK_PRIO		5
#define CHECK_COMMAND_TASK_STK_SIZE 		256
OS_TCB  CHECK_COMMAND_TASK_TCB;
CPU_STK CHECK_COMMAND_TASK_STK[CHECK_COMMAND_TASK_STK_SIZE];
void check_command_task(void *p_arg);

#define DO_KINMATICS_TASK_PRIO		6
#define DO_KINMATICS_TASK_STK_SIZE 		512
OS_TCB  DO_KINMATICS_TASK_TCB;
CPU_STK DO_KINMATICS_TASK_STK[DO_KINMATICS_TASK_STK_SIZE];
void do_kinmatics_task(void *p_arg);

#define IMU_TASK_PRIO		7
#define IMU_TASK_STK_SIZE 		256
OS_TCB  IMU_TASK_TCB;
CPU_STK IMU_TASK_STK[IMU_TASK_STK_SIZE];
void imu_task(void *p_arg);

#define CALC_ODOM_TASK_PRIO		8
#define CALC_ODOM_TASK_STK_SIZE 		256
OS_TCB  CALC_ODOM_TASK_TCB;
CPU_STK CALC_ODOM_TASK_STK[CALC_ODOM_TASK_STK_SIZE];
void calc_odom_task(void *p_arg);

int main(void)
{
    
    OS_ERR err;
    CPU_SR_ALLOC();

    Robot::get()->init();

    OSInit(&err);
    OS_CRITICAL_ENTER();

    //Create start task
    OSTaskCreate((OS_TCB 	* )&START_TASK_TCB,
                 (CPU_CHAR	* )"start task",
                 (OS_TASK_PTR )start_task,
                 (void		* )0,
                 (OS_PRIO	  )START_TASK_PRIO,
                 (CPU_STK   * )&START_TASK_STK[0],
            (CPU_STK_SIZE)START_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)START_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);
    OS_CRITICAL_EXIT();
    OSStart(&err);

    while(1);
}


void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN
    OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

    OS_CRITICAL_ENTER();

    OSTaskCreate((OS_TCB 	* )&BSP_TASK_TCB,
                 (CPU_CHAR	* )"bsp task",
                 (OS_TASK_PTR )bsp_task,
                 (void		* )0,
                 (OS_PRIO	  )BSP_TASK_PRIO,
                 (CPU_STK   * )&BSP_TASK_STK[0],
            (CPU_STK_SIZE)BSP_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)BSP_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);


    OSTaskCreate((OS_TCB 	* )&CHECK_COMMAND_TASK_TCB,
                 (CPU_CHAR	* )"command task",
                 (OS_TASK_PTR )check_command_task,
                 (void		* )0,
                 (OS_PRIO	  )CHECK_COMMAND_TASK_PRIO,
                 (CPU_STK   * )&CHECK_COMMAND_TASK_STK[0],
            (CPU_STK_SIZE)CHECK_COMMAND_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)CHECK_COMMAND_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);


    OSTaskCreate((OS_TCB 	* )&DO_KINMATICS_TASK_TCB,
                 (CPU_CHAR	* )"do_kinmatics task",
                 (OS_TASK_PTR )do_kinmatics_task,
                 (void		* )0,
                 (OS_PRIO	  )DO_KINMATICS_TASK_PRIO,
                 (CPU_STK   * )&DO_KINMATICS_TASK_STK[0],
            (CPU_STK_SIZE)DO_KINMATICS_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)DO_KINMATICS_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&IMU_TASK_TCB,
                 (CPU_CHAR	* )"imu task",
                 (OS_TASK_PTR )imu_task,
                 (void		* )0,
                 (OS_PRIO	  )IMU_TASK_PRIO,
                 (CPU_STK   * )&IMU_TASK_STK[0],
            (CPU_STK_SIZE)IMU_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)IMU_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);

    OSTaskCreate((OS_TCB 	* )&CALC_ODOM_TASK_TCB,
                 (CPU_CHAR	* )"calc_odom task",
                 (OS_TASK_PTR )calc_odom_task,
                 (void		* )0,
                 (OS_PRIO	  )CALC_ODOM_TASK_PRIO,
                 (CPU_STK   * )&CALC_ODOM_TASK_STK[0],
            (CPU_STK_SIZE)CALC_ODOM_TASK_STK_SIZE/10,
            (CPU_STK_SIZE)CALC_ODOM_TASK_STK_SIZE,
            (OS_MSG_QTY  )0,
            (OS_TICK	  )0,
            (void   	* )0,
            (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
            (OS_ERR 	* )&err);
            
    OS_TaskSuspend((OS_TCB*)&START_TASK_TCB,&err);

    OS_CRITICAL_EXIT();
}

void bsp_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    float bsp_task_i=0;
    Board *board = Board::get();
    while(1)
    {
        bsp_task_i++;
        board->update();
        if(bsp_task_i >= 5)
        {
            bsp_task_i=0;
            board->setDO_state(0,2);
        }
        
        OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //delay 10ms   100hz
    }
}

void check_command_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    
    while(1)
    {
        Robot::get()->check_command();
        OSTimeDly(1,OS_OPT_TIME_PERIODIC,&err);
    }
}

void do_kinmatics_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    while(1)
    {
        Robot::get()->do_kinmatics();
        OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

void imu_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;
   // HWController* hw_controller = HWController::getInstance();
    //Board *board = Board::get();

    while(1)
    {
        Robot::get()->get_imu_data();
        //OSTimeDly(1,OS_OPT_TIME_PERIODIC,&err);
        OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //delay 10ms  20hz
    }
}

void calc_odom_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    while(1)
    {
        Robot::get()->calc_odom();
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}
