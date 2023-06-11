#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "ssl_gc_referee_message.pb.h" // 生成されたprotobufヘッダファイル

const char* ssid = "ここにSSID";
const char* password = "ここにパスワード";
const IPAddress multicastIP(224, 5, 23, 1); // マルチキャストグループのIPアドレス
const unsigned int multicastPort = 10003;   // マルチキャストポート

WiFiUDP udp;

void setup() {
  M5.begin();
  Serial.begin(115200);

  // WiFiに接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP: ");
  Serial.println(WiFi.localIP());

  // UDPソケットを開く
  if (!udp.beginMulticast(multicastIP, multicastPort)) {
    Serial.println("Failed to begin multicast");
    while (1) {
      delay(1000);
    }
  }
}

void send_referee_command(Referee_Command command) {
  Referee message = Referee_init_zero;
  message.command = command;

  uint8_t buffer[256];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  if (pb_encode(&stream, Referee_fields, &message)) {
    udp.beginPacket(multicastIP, multicastPort);
    udp.write(buffer, stream.bytes_written);
    udp.endPacket();
    Serial.println("Sent multicast message");
  } else {
    Serial.println("Failed to encode message");
  }
}

void loop() {
  // ボタンを押したらパケットを送信する
  if (M5.BtnA.wasPressed()) {
    send_referee_command(Referee_Command_HALT);
  } else if (M5.BtnB.wasPressed()) {
    send_referee_command(Referee_Command_STOP);
  } else if (M5.BtnC.wasPressed()) {
    send_referee_command(Referee_Command_BALL_PLACEMENT_BLUE);
  }

  // パケットを受信したらシリアルモニタにメッセージを表示する
  int packetSize = udp.parsePacket();
  if (packetSize) {
    uint8_t receivedBuffer[256];
    int bytesRead = udp.read(receivedBuffer, sizeof(receivedBuffer));
    Serial.println(bytesRead);
    pb_istream_t stream = pb_istream_from_buffer(receivedBuffer, bytesRead);
    Referee receivedMessage = Referee_init_zero;
    if (pb_decode(&stream, Referee_fields, &receivedMessage)) {
      Serial.print("Received multicast message: command=");
      Serial.print(receivedMessage.command);
      Serial.print(", stage=");
      Serial.println(receivedMessage.stage);
    } else {
      Serial.println("Failed to decode message");
    }
  }

  M5.update();
}
