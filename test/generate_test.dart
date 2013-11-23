import 'package:capatcha/capatcha.dart';

main() {
  Capatcha.generate(generated:(c) {
    //print(c.file.readAsBytes());
    print(c.text);
  });
}