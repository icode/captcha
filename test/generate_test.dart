import 'package:capatcha/capatcha.dart';

main() {
  Capatcha.generate(generated:(c) {
    print(c.text);
  });
}