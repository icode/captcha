import 'package:capatcha/capatcha.dart';

main() {
  Capatcha.generate(generated:(c) {
    //print(c.file.readAsBytesSync());
    print(c.text);
  });
}