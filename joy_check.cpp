#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int32.h>

// 定数
const int MD_ID = 1;

// グローバル変数
int32_t g_motor_0 = 0;
int32_t g_motor_1 = 0;

// リソースの初期化
ros::NodeHandle nh;
CAN can1(PA_11, PA_12);
Ticker ticker;
DigitalOut led(LED1);

void message_cb_motor_0(const std_msgs::Int32 &motor_msg)
{
  g_motor_0 = motor_msg.data;
}

void message_cb_motor_1(const std_msgs::Int32 &motor_msg)
{
  g_motor_1 = motor_msg.data;
}

// サブスクライバの定義
ros::Subscriber<std_msgs::Int32> sub_motor_0("motor_0", &message_cb_motor_0);
ros::Subscriber<std_msgs::Int32> sub_motor_1("motor_1", &message_cb_motor_1);

// パブリッシャの定義
std_msgs::Int32 encoder_0;
std_msgs::Int32 encoder_1;
ros::Publisher pub_encoder_0("encoder_0", &encoder_0);
ros::Publisher pub_encoder_1("encoder_1", &encoder_1);

void send_callback()
{
  const int len = 3;
  char data[len];

  // データ送信
  data[0] = (0b11 << 6) | (g_motor_0 >> 5 & 0b111000) | (g_motor_1 >> 8 & 0b111);
  data[1] = g_motor_0 & 0XFF;
  data[2] = g_motor_1 & 0XFF;
  can1.write(CANMessage(MD_ID, data, len));
}

int main()
{
  // ROSノード初期化
  nh.getHardware()->setBaud(115200); // ボーレート設定 (注意：最初にこの関数を持ってくる！)
  nh.initNode();
  nh.subscribe(sub_motor_0);
  nh.subscribe(sub_motor_1);
  nh.advertise(pub_encoder_0);
  nh.advertise(pub_encoder_1);

  // CAN通信の初期化
  can1.frequency(1000000); // ボーレート設定 (注意：この関数を最初に持ってくる！)
  // can1.mode(CAN::LocalTest);
  // 受信セットアップ
  can1.filter(0, 0b11111);
  wait_ms(1000); // ハードウェア初期化が完了するまで待つ（いらないかも）

  // MDセットアップ
  const int len = 4;
  char motor_setup_data_0[len] = {0b00000000, 0, 0, 0};
  char motor_setup_data_1[len] = {0b01000000, 0, 0, 0};
  can1.write(CANMessage(MD_ID, motor_setup_data_0, len));
  can1.write(CANMessage(MD_ID, motor_setup_data_1, len));

  // Timerの初期設定
  ticker.attach(&send_callback, 0.005);

  // エンコーダの値を格納する変数の初期化
  encoder_0.data = 0;
  encoder_1.data = 0;

  while (1) {
    nh.spinOnce();

    CANMessage msg;
    while (can1.read(msg)) {
      int md_data_type = (msg.data[0] >> 6) & 0b11;
      int md_id = (msg.id >> 5) & 0b11111;

      if (msg.len != 3 || md_data_type != 0b11) continue;

      if ((msg.data[0] >> 5 & 1) == 0) {
        // エンコーダの値が正
        encoder_0.data = ((msg.data[0] & 0b111000) << 5) | msg.data[1];
      }
      else {
        // エンコーダの値が負
        encoder_0.data = int16_t(0b11111 << 11 | (((msg.data[0] & 0b111000) << 5) | msg.data[1]));
      }

      if ((msg.data[0] >> 2 & 1) == 0) {
        // エンコーダの値が正
        encoder_1.data = ((msg.data[0] & 0b111) << 8) | msg.data[2];
      }
      else {
        // エンコーダの値が負
        encoder_1.data = int16_t(0b11111 << 11 | (((msg.data[0] & 0b111) << 8) | msg.data[2]));
      }

      pub_encoder_0.publish(&encoder_0);
      pub_encoder_1.publish(&encoder_1);

      led = !led;
    }

    // wait_ms(5); //通信速度が早すぎると良くないらしいのでwaitを入れる
  }
}
