# 制作我自己的 EtherCAT 设备

该仓库包含了我在创建自己的 EtherCAT 设备时使用的软件、PCB 原理图和布局、参考文档等。

这个项目在一系列 YouTube 视频中进行了记录，从我最初尝试理解 EtherCAT 的工作原理，到制作自己的 PCB、编程并在 LinuxCNC 中进行测试。

=======

## 制作我自己的 EtherCAT 设备 9. 关于时间的步进发生器

这是我几乎放弃的事情。但现在它来了，一个工作的步进发生器。实际上，不止一个，而是两个步进发生器，针对 EaserCAT 2000 板。对于步进发生器来说，一切都与时序有关，时序，时序。我将带您了解我所做的工作，以使其正常运行。这可以为您提供一些关于 EtherCAT 如何在 LinuxCNC 中工作的总体见解。

这次最大的变化在于 [Firmware 文件夹](Firmware)。[Documentation 文件夹](Documentation) 中包含了第二个步进发生器的设计细节。EEPROM_generator 中修复了一个小但致命的索引错误。有关所有细节，请阅读 git 日志，太多内容无法一一提及。

[![观看视频](https://img.youtube.com/vi/lBDBcseFki8/default.jpg)](https://youtu.be/lBDBcseFki8)

## 制作我自己的 EtherCAT 设备 8. EaserCAT 3000

介绍新的 **EaserCAT 3000** 板。这是 EaserCAT 2000 的进化版，旨在用于我的等离子切割机。它具有四个步进驱动输出、一个 THCAD 弧电压卡输入、一个编码器、一个模拟输出（用于主轴 ±10V）、八个数字输入、四个数字输出，以及一些 12 个 I/O 以进行任何可能的扩展。

KiCAD 文件位于 [KiCAD 文件夹](Kicad/Ax58100-stm32-ethercat)

[![观看视频](https://img.youtube.com/vi/boanv6ihYtI/default.jpg)](https://youtu.be/boanv6ihYtI)

## 制作我自己的 EtherCAT 设备 7. 在车床上加工

我现在已经将 EaserCAT 2000 板与我的小型 CNC 车床连接在一起。两个步进发生器，一个用于 X 轴，一个用于 Z 轴，还有一个用于主轴编码器的编码器计数器都在这块小卡上。

尽管它可以工作，但由于周期时间的变化仍然存在一些问题。值得庆幸的是，我能够将变化从 80-100 微秒减少到 2-3 微秒。

[![观看视频](https://img.youtube.com/vi/Bqi1KXEVI1Q/default.jpg)](https://youtu.be/Bqi1KXEVI1Q)

## 制作我自己的 EtherCAT 设备 6. 步进电机驱动器

步进驱动器发生器出现并发出一些步进电机的声音。这次的两个主要工作是为步进脉冲设置定时器以及将 EtherCAT 周期与 linuxcnc 伺服线程周期同步。值得庆幸的是，我没有展示太多内容。

[![观看视频](https://img.youtube.com/vi/QNNEA0wO4Mw/default.jpg)](https://youtu.be/QNNEA0wO4Mw)

## 制作我自己的 EtherCAT 设备 5. 车床复活

我将 EaserCAT 2000 板连接到我的迷你车床并使其工作。文档可在此处获取，请选择 *Video5* 分支。

[![观看视频](https://img.youtube.com/vi/wOtMrlHCCic/default.jpg)](https://youtu.be/wOtMrlHCCic)

## 制作我自己的 EtherCAT 设备 4. PCB 到手

在这个视频中，事情开始变得有趣。我得到了 PCB，并尝试让它工作。现在我终于提供了文档，请查看 [这个文件夹](Pcb-1-lan9252)。

[![观看视频](https://img.youtube.com/vi/An0VrKYAv88/default.jpg)](https://youtu.be/An0VrKYAv88)

## 制作我自己的 EtherCAT 设备 3. 编码器

我制作了一个基本的 EtherCAT 编码器模块。为了测试它，我需要比我的测试设置更好的东西，所以我开始设计自己的 PCB 以便与 LinuxCNC 进行测试。

[![观看视频](https://img.youtube.com/vi/oNIBOpeTpQ4/default.jpg)](https://youtu.be/oNIBOpeTpQ4)

## 制作我自己的 EtherCAT 设备 2. MCU 和 SPI

测试 MCU 和 LAN9252 芯片之间的 SPI 连接。进入 EtherCAT 和 CIA402 的状态图。

[![观看视频](https://img.youtube.com/vi/F9HdCEG6kow/default.jpg)](https://youtu.be/F9HdCEG6kow)

## 制作我自己的 EtherCAT 设备 1. 数字 I/O

我与 LAN9252 芯片的第一次尝试。逐渐熟悉一些工具。

[![观看视频](https://img.youtube.com/vi/IGmXsXSSA4s/default.jpg)](https://youtu.be/IGmXsXSSA4s)

# 基于 LAN9252 和 STM34F407 的 EtherCAT PCB

##### 固件位于 **Firmware** 文件夹中。

在 PlatformIO 中打开此文件。如果 PlatformIO 能够安装所有必要的软件，那就太好了。如果不能，您至少需要 Arduino for STM32 (STM32duino) 和 SPI。希望 PlatformIO 能够搞定这一切。SPI 设置在 lib/soes/hal/spi.cpp 中，如果您有兴趣的话。
SOES 是来自 <https://github.com/kubabuda/ecat_servo/tree/main/examples/SOES_arduino> 的 Arduino 移植版。
我自己的代码在 main.cpp 中。编码器计数器代码在 STM34_Encoder.cpp 中。

###### PCB 设计在 **Kicad** 中。

使用 Kicad 打开此文件。我已经将符号和封装放入其中，只需设置正确的路径。该设计是我从德国 Aisler 订购 PCB 时的状态。

###### **Schematics** 文件夹

包含参考文档、多个 LAN9252 评估板和其他板的原理图。这是我从中获得设计灵感和想法的地方。是的，我并没有阅读所有的参考文档——我大部分是从这些设计中复制的，这也是为什么它第一次就能成功。[Freerouting](https://github.com/freerouting/freerouting) 被用来布线 PCB。通常我手动布线，但这次有点多，结果出来得相当不错，最重要的是，它成功了。

##### ESI XML 文件生成器在 **EEPROM_generator** 中。

使用文件浏览器访问该文件夹，点击 index.html 以创建或更新您自己的 XML 文件（以及其他必要文件，包括 EEPROM bin）。将内容复制到 Firmware/lib/soes 和 Twincat ESI XML 文件目录 `c:\twincat\3.1\io\modules\ethercat`。
原始 EEPROM_generator 可以在这里找到 <https://github.com/kubabuda/EEPROM_generator>

##### **CubeMX 文件** 仅供参考。

可以在 CubeMX 中打开 .ioc 文件。STM32F407 处理器的功能与特定引脚相关，.ioc 文件中有此信息。这只是供参考。

##### ESI（EtherCAT 从设备信息）文件

我在这里放置了 [Dig_8IN_8OUT.xml](Dig_8IN_8OUT.xml)，这是一个针对 LAN9252 IC 独立使用的示例 ESI 文件，无需 MCU。有时拥有最简单的 ESI 文件会很方便，这里就是。您可以在这里找到编码器应用的 ESI 文件 [Firmware/lib/soes/MetalMusings_EaserCAT_2000_encoder.xml](Firmware/lib/soes/MetalMusings_EaserCAT_2000_encoder.xml)。

##### **linuxcnc** 包含用于使 EaserCAT 2000 工作的修改

我使用的配置文件和 HAL 组件都放在这里。

##### LAN9252_eeprom_store_valid

Arduino 草图，用于编程 AT24C32 EEPROM 以获取有效的 EEPROM 内容。通过 I2C 连接 EEPROM 并运行程序。程序中的验证必须通过，才能进行有效编程。

### 许可证

请勿违反原始许可证。没有任何担保。您可以随意使用。