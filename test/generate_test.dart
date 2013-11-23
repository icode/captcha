import 'package:captcha/captcha.dart';

main() {
  Captcha.generate(generated:(c) {
    //print(c.file.readAsBytesSync());
    print(c.text);
  });
}