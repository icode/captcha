library capatcha;

import 'dart-ext:capatcha';
import 'dart:math';
import 'dart:io';
import 'dart:async';

typedef String TextGenerator();

typedef void CapatchaGenerated(Capatcha capatcha);

void _generate(String captchaText, String fileOut, int count, int width, int height, int offset, int quality, int isjpeg, int fontSize) native 'Generate';

class Capatcha {

  _Capatcha() {
  }

  String _text;

  File _file;

  String get text {
    return _text;
  }

  File get file {
    return _file;
  }

  static final Arrays<String> charDic = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'];

  static final Random random = new Random();

  static Future<Capatcha> generate({TextGenerator textGenerator, CapatchaGenerated generated, String fileOut, int width:230, int height:60, int offset:35, int quality:50, int fontSize:57, bool removeFile:true}) {
    var completer = new Completer();
    var str_num = 6, r_num = charDic.length, text = '';
    if (textGenerator == null) {
      for (var i = 0;i < str_num;i++) {
        text += charDic[random.nextInt(r_num)];
      }
    } else {
      text = textGenerator().toUpperCase();
    }

    if (fileOut == null) {
      var pakRoot = new File(Platform.packageRoot);
      var tempDir = pakRoot.parent.path + Platform.pathSeparator + 'tmp';
      var tmpDirectory = new Directory(tempDir);
      if (!tmpDirectory.exists()) {
        tmpDirectory.createSync(recursive:true);
      }
      fileOut = tempDir + Platform.pathSeparator + new DateTime.now().millisecondsSinceEpoch.toString() + text + '.jpg';
    }

    _generate(text, fileOut, text.length, width, height, offset, quality, fileOut.endsWith('.jpg') || fileOut.endsWith('.jpeg') ? 1 : 0, fontSize);
    var c = new Capatcha();
    c._text = text;
    c._file = new File(fileOut);
    try {
      generated(c);
    }
    finally{
      if (removeFile)Timer.run(() {
        c._file.delete();
      });
    }
  }
}
