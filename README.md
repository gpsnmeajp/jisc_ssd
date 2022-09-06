# jisc_ssd
[JISC-SSD(Jisaku In-Storage Computation SSD学習ボード)](https://crane-elec.co.jp/products/vol-28/)の動作サンプルというか実装例というかデバッグツールというかです。

Arduino IDE + [earlephilhower/arduino-pico](https://github.com/earlephilhower/arduino-pico) を導入した環境で、  
JISC-SSDに転送すると動作します。

誤りや、バグが色々とある可能性があります。

書き込むと、USBからの接続待ちとなり、LEDが高速に点滅します。  
TeraTermなど端末エミュレータでCOMポートに接続してください。

操作内容は画面を参照してください。

![](screen.png)

![](config.png)
