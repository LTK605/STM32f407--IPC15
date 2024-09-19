/*
 * @Author: Dragon
 * @Date: 2022-08-27 20:56:30
 * @LastEditTime: 2022-08-29 20:27:45
 * @FilePath: \USERf:\WorkSpace\Engineering training\Intelligent handling vehicle\code\Stepper motor S control\HARDWARE\MOTOR\motor.h
 * @custom_steing_obkoro1:  Read only, do not modify place!!!
 */
#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
/*Wheel operation information structure*/
#define TIM_FREQ            84000000U                      /* 定时器主频 */
#define T1_FREQ             (TIM_FREQ/PSC)                   /* 计数频率*/
#define ARR 0XFFFF
#define PSC 84-1
#define WHEEL_R 7.62                                    // Wheel radius
#define ALPHA 1.8                                    // Step angle
#define MICRO 4                                     // subdivide 步进电机细分
#define L ((PI * WHEEL_R * 2 *  ALPHA ) / (360*MICRO )) // 步进电机每一步的实际距离
typedef struct
{
    __IO uint8_t run_state; /* 电机旋转状态 */
    __IO uint8_t dir;       /* 电机旋转方向 */
    __IO uint8_t offline;   /* 电机脱机 */

    __IO uint32_t step;/* 轮子已经走步数 */
	  __IO uint32_t accel_count;    	
	  __IO int32_t max_speed;
    __IO int32_t sum_step;       /* 轮子需要走的总步数 */
    __IO uint32_t accel_step;     /* 停止加速步数 */
    __IO uint32_t accel_max_step; /* 极限加速步数 */
    __IO uint32_t decel_step;     /* 减速步数 */
     __IO uint32_t decel_start; /* 开始减速步数 */
    __IO uint32_t step_delay; /* 下个脉冲周期（时间间隔），启动时为加速度 */
    __IO uint32_t decel_val;  /* 减速阶段步数 */
    __IO uint32_t min_delay;  /* 速度最快，计数值最小的值(最大速度，即匀速段速度) */
} WheelRunningData;

/*run_state optional value*/
#define STOP   0  //停止
#define ACCEL  1 //加速
#define DECEL  2 //减速
#define RUN    3   //匀速
/*dir optional value*/
#define CW     0
#define CCW    1
/*offline optation value*/
#define EN     0
#define DIS    1

/*Vehicle operation information structure*/
#define LENGTH 6
#define WIDTH 5
typedef struct
{
    __IO float x;       /*车辆x轴运行信息*/
    __IO float y;       /*车辆y轴运行信息*/
    __IO float yaw;     /*车辆偏航信息*/
    __IO float speed_x; /*车辆偏航信息*/
    __IO float speed_y; /*车辆偏航信息*/
    __IO float speed_w; /*车辆偏航信息*/

} VehicleRunningData;

/*Top level function*/

/**
 * @brief The vehicle moves to the specified absolute coordinate point
 *
 * @description:The vehicle moves to the specified absolute coordinate point
 * @param {float} coordinate_X X-axis coordinate information
 * @param {float} coordinate_Y Y-axis coordinate information
 * @param {float} coordinate_W Absolute angle of z-axis rotation
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void Absolute_Move(float coordinate_X, float coordinate_Y, float coordinate_W, uint32_t v_accel, uint32_t v_decel, uint32_t v_speed);

/**
 * @brief Relative movement of vehicles
 *
 * @description: Relative movement of vehicles
 * @param {float} distance_X Relative movement distance of x-axis
 * @param {float} distance_Y Relative movement distance of y-axis
 * @param {float} distance_W Rotation angle relative to z-axis
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void Relative_Move( float32_t distance_X,  float32_t distance_Y, float32_t distance_W, uint32_t v_accel, uint32_t v_W,uint32_t v_decel, float32_t v_speed);

/**
 * @brief Stepper motor initialization
 *
 * @description: Stepper motor initialization
 * @return {*}
 */
void StepperMotor_Init(void);

/*********************************************************************************************************/
/*The underlying function is not external*/

/**
 * @brief Left front wheel control
 *
 * @description:
 * @param {uint32_t} sum_step Number of wheel revolutions
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void StepperMotor_LF_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed);

/**
 * @brief Left rear wheel control
 *
 * @description:
 * @param {uint32_t} sum_step Number of wheel revolutions
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void StepperMotor_LB_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed);

/**
 * @brief Right front wheel control
 *
 * @description:
 * @param {uint32_t} sum_step Number of wheel revolutions
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void StepperMotor_RF_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed);

/**
 * @brief  Right rear wheel control
 *
 * @description:
 * @param {uint32_t} sum_step Number of wheel revolutions
 * @param {uint32_t} accel Acceleration of acceleration section
 * @param {uint32_t} decel Acceleration of deceleration section
 * @param {uint32_t} speed Maximum operating speed
 * @return {*}
 */
void StepperMotor_RB_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed);
//void control(WheelRunningData *wheel);
void Pulse_Control(WheelRunningData *Wheel);
#endif
