# jisc_ssd
[JISC-SSD(Jisaku In-Storage Computation SSD学習ボード)](https://crane-elec.co.jp/products/vol-28/)の動作サンプルというか実装例というかデバッグツールというかです。

参考: [https://note.com/gpsnmeajp/n/n7a85a5118065](https://note.com/gpsnmeajp/n/n7a85a5118065)

Arduino IDE + [earlephilhower/arduino-pico](https://github.com/earlephilhower/arduino-pico) を導入した環境で、  
JISC-SSDに転送すると動作します。

誤りや、バグが色々とある可能性があります。

# ssd.ino
デバッグツールです

SDKをPico SDKに設定してください。

書き込むと、USBからの接続待ちとなり、LEDが高速に点滅します。  
TeraTermなど端末エミュレータでCOMポートに接続してください。

操作内容は画面を参照してください。

![](screen.png)

![](config.png)

# msc.ino
USBメモリとして認識させるサンプルです

誤り訂正をしていないため、ビットエラーが発生します。  
**決して大切なデータを保存しないで下さい**

SDKをTinyUSBに設定してください。

125MBのUSBマスストレージデバイスとして認識します。

1024ブロック中、1000ブロックを使用します。  
24ブロックはBad Block用の予備です。

誤り訂正なし。1ページあたり2176byte中2048byte使用します。  
(128byteは将来の誤り訂正に備える)

Bad Blockの対応は固定テーブル方式です。
先にssd.inoなどを使って、Bad Blockをチェックしておいてください。  
その上で、下記関数のBad Block置き換えを適切に書き換えてください。  
int bad_block_replace(int page)

![](check.png)

![](disc.png)
