# FreeRTOS 기반 자율주행 로보택시

> STM32F411RE와 다중 센서를 활용해 장애물 회피 자율주행과 Bluetooth 수동 제어를 구현한 소형 모빌리티 시스템입니다.

## 프로젝트 정보

- 개발 기간: 2026.06.08 ~ 2026.06.17
- 개발 형태: 개인 프로젝트
- 개발 환경: STM32CubeIDE, FreeRTOS

## 프로젝트 개요

센서 측정, 자율주행 판단, 수동 제어를 3개의 FreeRTOS Task로 분리했습니다. 3방향 초음파 센서로 주변 장애물을 감지하고, 좌·우 거리 차이에 따라 양쪽 모터의 속도를 보정해 중앙 주행과 장애물 회피를 수행합니다.

Bluetooth 명령으로 자율주행과 수동주행을 전환할 수 있으며, IR 센서로 승객 탑승을 확인하고 LED와 부저로 차량의 동작 상태를 표시합니다.

## 주요 기능

- 3방향 초음파 센서 기반 장애물 감지 및 회피
- 좌·우 거리 오차를 이용한 P 제어 방식의 차동 조향
- 전방 거리에 따른 긴급 회피·코너 회피·일반 주행 분기
- Bluetooth 앱을 통한 자율·수동 모드 전환 및 방향 제어
- IR 센서를 이용한 승객 탑승 감지
- 후진·회전 상태에 따른 LED 및 부저 알림
- Mutex를 이용한 센서 데이터 동기화

## Task 구성

| Task | 우선순위 | 역할 |
|---|---:|---|
| `SensorTask` | High | 3방향 초음파 거리 측정 및 IR 센서 상태 확인 |
| `ManualTask` | Above Normal | Bluetooth 명령 처리 및 자율·수동 모드 전환 |
| `DriveTask` | Normal | 장애물 회피 판단 및 차동 구동 기반 자율주행 제어 |

## 시스템 동작 흐름

```text
초음파 센서 · IR 센서
          ↓
      SensorTask
          ↓ Mutex
       DriveTask ─────────→ 모터 제어

Bluetooth 명령
          ↓
      ManualTask ─────────→ 모터 · LED · 부저 제어
```

- `SensorTask`가 좌·전방·우측 초음파 센서를 순차적으로 측정합니다.
- 측정값은 Mutex로 보호하여 `DriveTask`와 안전하게 공유합니다.
- 자율 모드에서는 거리값에 따라 주행 상태와 좌·우 모터 속도를 결정합니다.
- 수동 모드에서는 Bluetooth 명령에 따라 차량의 방향과 상태 표시 장치를 제어합니다.


## 핵심 소스

| 파일 | 역할 |
|---|---|
| `freertos.c` | Task와 센서 공유용 Mutex 생성 |
| `Sensor.c` | 초음파·IR 센서 측정 및 공유 데이터 관리 |
| `AutoDrive.c` | 자율주행 판단, 장애물 회피 및 P 제어 |
| `ManualControl.c` | Bluetooth 명령 처리와 주행 모드 전환 |
| `Bluetooth.c` | UART 인터럽트 기반 명령 수신 및 오류 복구 |
| `Motor.c` | 모터 방향 및 PWM 속도 제어 |
| `LED.c` / `Buzzer.c` | 차량 동작 상태 알림 |
| `DHT11.c` | 온·습도 데이터 수신 및 Bluetooth 출력 |

## 기술 스택

`STM32F411RE` · `FreeRTOS` · `C` · `STM32 HAL` · `UART` · `PWM` · `GPIO` · `Bluetooth` · `HC-SR04` · `IR Sensor` · `DHT11`

<details>
<summary>개발 참고</summary>

DHT11 데이터 수신 로직은 구현했으나, FreeRTOS 환경에서 통신 타이밍의 안정성을 확보하지 못해 현재 Bluetooth 출력에는 테스트용 고정값을 사용합니다.

</details>
