# 起摆 PID 控制工程

基于 STM32F103C8 的起摆与 PID 控制实验工程，使用 STM32CubeMX 生成基础外设代码，并通过 Keil MDK-ARM 构建。

## 工程内容

- `Core/`：应用层源码与头文件
- `Drivers/`：STM32 HAL 和 CMSIS 驱动
- `MDK-ARM/`：Keil 工程配置与启动文件
- `pid.ioc`：STM32CubeMX 工程配置

## 使用方式

1. 使用 STM32CubeMX 打开 `pid.ioc` 查看或重新生成外设配置。
2. 使用 Keil MDK-ARM 打开 `MDK-ARM/pid.uvprojx`。
3. 编译并下载到 STM32F103C8 目标板。

## 存档说明

仓库保留源码、驱动和工程配置。Keil 编译产物及个人界面状态由 `.gitignore` 排除，避免把可再生成文件和本机配置写入版本历史。

