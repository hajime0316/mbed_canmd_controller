# mbed_canmd_controller

CANMDを制御するためにMbedで開発したサンプルプログラム．

## 概要

CANMDを制御するためにMbedで開発したサンプルプログラム．
MbedボードにはNUCLEO-F303REを用いることを想定している．

PC（raspberry Pi）からCANMDを操作する際に，通信を中継する役割を果たす．
PCとのデータのやり取りにはrosserialを用いる．
CANMDとのデータのやり取りにはCAN通信を用いる．

## 開発環境

|           | バージョンまたは`git hash` |
| --------- | -------------------------- |
| Ubuntu    | 18.04                      |
| ROS       | Melodic                    |
| rosserial | `5ff3`                     |
| Mbed      | `0e9c`                     |

「[ROS⇔mbedのシリアル通信実践(rosserial編) – Keio Robotics Association](https://keiorogiken.wordpress.com/2020/12/16/ros%E2%87%94mbed%E3%81%AE%E3%82%B7%E3%83%AA%E3%82%A2%E3%83%AB%E9%80%9A%E4%BF%A1%E5%AE%9F%E8%B7%B5rosserial%E7%B7%A8/)」を参考に，開発環境を構築した．
