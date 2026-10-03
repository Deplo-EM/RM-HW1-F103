# 电控第一次作业 · RM 嵌入式（STM32F103C8T6）

> 三道题做在**同一个 CubeMX 工程**里：GPIO 点灯 → 定时器 1 ms 中断 → 独立看门狗。
> 详细说明（含推导、逐行讲解、验证与附录图）见 **[docs/说明文档.md](docs/说明文档.md)**。

---

## 一、三题做了什么

| 题 | 要求 | 实现位置 | 现象 |
|---|---|---|---|
| 1 | PC13 输出指定电平点亮板载 LED | `Tasks/src/Tasks.cpp` → `HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)` | 板载蓝灯**常亮** |
| 2 | 用一个定时器的更新中断，周期 **1 ms**；中断里 `tick` 自增并喂狗 | TIM2：`PSC=71`、`ARR=999`；回调里 `tick++` + `HAL_IWDG_Refresh` | `tick` 每秒约 **+1000**（实测 999.2/秒） |
| 3 | 删掉喂狗，看门狗超时复位 | 注释掉回调里的 `HAL_IWDG_Refresh`，其余不动 | `tick` 涨到约 **2048** → 芯片复位、`tick` 归零 → 反复 |

业务代码**只在 `Tasks/` 目录里**（`Tasks/src/Tasks.cpp`、`Tasks/inc/Tasks.h`）；
`main.c` 只在 `USER CODE` 区域加了 `#include "Tasks.h"` 与 `TasksInit()`，`while (1)` 保持为空。

---

## 二、关键配置（怎么算出来的）

| 项 | 值 | 计算 |
|---|---|---|
| 系统时钟 | **72 MHz** | HSE 8 MHz × PLL 9 |
| 定时器时钟 | **72 MHz** | APB1 = 36 MHz，**APB1 分频 ≠1 时定时器时钟自动 ×2** |
| TIM2 更新中断 | **1 kHz（1 ms）** | 72 MHz ÷ (71+1) ÷ (999+1) = 1000 Hz |
| IWDG 超时 | **2.000 s** | LSI 40 kHz ÷ 64 = 625 Hz；1250 个计数 ÷ 625 = 2 s |
| 板载 LED | PC13 推挽输出，**低电平点亮** | 3.3 V → 电阻 → LED → PC13，拉低形成回路 |

---

## 三、目录结构

| 路径 | 内容 |
|---|---|
| `Tasks/` | **业务代码**（三题的全部实现） |
| `Core/`、`Drivers/` | CubeMX 生成的初始化与 HAL 库 |
| `docs/` | 说明文档、附录图、要求对照表 |
| `CMakeLists.txt` | 老师发放的模板（`user_folders = "Tasks"`） |
| `HW1.ioc` | CubeMX 工程文件（时钟树、引脚、TIM2、IWDG 配置都在里面） |

---

## 四、怎么编译

```powershell
# 工程根目录；VS Code 里把构建类型选成 Debug 也一样
cmake --build --preset Debug     # 产物：build/HW1.elf
```

观察运行时变量：用 Ozone 打开 `build/HW1.elf`。

---

## 五、文档索引

| 文件 | 内容 |
|---|---|
| **[docs/说明文档.md](docs/说明文档.md)** | **提交给老师的说明文档**：三题实现、1 ms 与 2 s 的推导、验证、建议；截图与照片统一放在附录 |
| `docs/说明文档.html` | 上面那份的可打印版本（用浏览器 `Ctrl+P` 导出 PDF） |
| `docs/附录图/` | 图①灯亮照片、图②`tick` 增长、图③看门狗复位 |
| `docs/作业要求对照表.md` | 作业要求逐条对照与完成状态 |
| `docs/调试过程记录.md` | 调试过程中的问题与本机命令（存档） |
