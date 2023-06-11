# m5stack_ssl_referee_pubsub

M5StackでSSL GC Referee Messageを送受信するためのプログラムです。

メッセージを受信すると、パケットサイズとメッセージ内のcommandとstageを表示します。

```sh
72
Received multicast message: command=1, stage=1
```

M5Stackのボタンを押すと、下記のcommandをセットしたメッセージを送信します。

- 左ボタン(Aボタン): ホルト
- 中ボタン(Bボタン): ストップ
- 右ボタン(Cボタン): 青チームボールプレースメント

## Requirements

- Arduino IDE 2.1.0
- Arduino Library:
  - M5Stack 0.4.3

## Installation

m5stack_ssl_referee_pubsubをクローンします。

```sh
$ cd ~
$ git clone https://github.com/ShotaAk/m5stack_ssl_referee_pubsub
```

nanopbをクローンします

```sh
$ cd ~
$ git clone https://github.com/nanopb/nanopb.git
```

nanopb内のヘッダーファイルとソースファイルをコピーします。

```sh
$ cp ~/nanopb/*.h ~/m5stack_ssl_referee_pubsub/
$ cp ~/nanopb/*.c ~/m5stack_ssl_referee_pubsub/
```

protoファイルをコンパイルします。

```sh
$ python3 ~/nanopb/generator/nanopb_generator.py *.proto
```
