#Dart 验证码实现

##环境安装

###Ubuntu 安装所需库：

    sudo apt-get install -y graphicsmagick

###windows 下载对应版本:

    ftp://ftp.graphicsmagick.org/pub/GraphicsMagick/windows/

*windows下jpeg格式会有一个无效参数的提示，不知原因

##使用

    import 'package:captcha/captcha.dart';
    main() {
      Captcha.generate(generated:(c) {
        print(c.text);
        //print(c.file.readAsBytesSync()); // 读取文件流
      });
    }

