/**
 * @file freertos.c
 * @brief 센서 수집, 자율주행, 수동제어 Task와 공유 mutex 구성.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "Sensor.h"        // StartSensorTask, sensorMutexHandle
#include "AutoDrive.h"     // StartDriveTask
#include "ManualControl.h" // StartManualTask

osThreadId_t SensorTaskHandle;
osThreadId_t DriveTaskHandle;
osThreadId_t ManualTaskHandle;
osMutexId_t  sensorMutexHandle;

const osThreadAttr_t SensorTask_attributes = {
    .name       = "SensorTask",
    .stack_size = 512 * 4,
    /* 거리 측정 주기를 안정적으로 유지하기 위해 가장 높은 우선순위를 부여한다. */
    .priority   = (osPriority_t) osPriorityHigh,
};
const osThreadAttr_t DriveTask_attributes = {
    .name       = "DriveTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t) osPriorityNormal,
};
const osThreadAttr_t ManualTask_attributes = {
    .name       = "ManualTask",
    .stack_size = 512 * 4,
    /* 사용자 정지·전환 명령을 자율주행 판단보다 먼저 처리한다. */
    .priority   = (osPriority_t) osPriorityAboveNormal,
};
const osMutexAttr_t sensorMutex_attributes = {
    .name = "sensorMutex"
};

void MX_FREERTOS_Init(void)
{
    /* 세 방향 거리값을 SensorTask와 DriveTask가 안전하게 공유하도록 보호한다. */
    sensorMutexHandle = osMutexNew(&sensorMutex_attributes);

    /* 각 기능을 독립 Task로 실행해 센서 수집과 차량 제어를 분리한다. */
    SensorTaskHandle  = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);
    DriveTaskHandle   = osThreadNew(StartDriveTask,  NULL, &DriveTask_attributes);
    ManualTaskHandle  = osThreadNew(StartManualTask, NULL, &ManualTask_attributes);
}
