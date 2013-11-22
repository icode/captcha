library capatcha;

import 'dart-ext:capatcha';

void _generate(String captchaText, String fileOut, int count, int width, int height, int offset, int quality,
               int isjpeg, int fontSize) native 'Generate';

class Capatcha {
// The simplest way to call native code: top-level functions.
  static void generate() {

  }
}
