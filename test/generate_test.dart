import 'package:captcha/captcha.dart';

main() {
  Captcha.generate(removeFile:false,generated:(c) {
    //print(c.file.readAsBytesSync());
    print(c.text);
  });
}