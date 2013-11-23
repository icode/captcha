#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "lib/CImg/CImg.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

using namespace cimg_library;

//第一次调用本地函数被调用时，给定一个名称解释成成一个C的本机函数名称。
Dart_NativeFunction ResolveName(Dart_Handle name, int argc);

//Called when the extension is loaded.
DART_EXPORT Dart_Handle captcha_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library,
			ResolveName);
	if (Dart_IsError(result_code))
		return result_code;

	return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
	if (Dart_IsError(handle))
		Dart_PropagateError(handle);
	return handle;
}

struct FunctionLookup {
	const char* name;
	Dart_NativeFunction function;
};

void _generate(const char* captcha_text,const char* file_o, int count,
		int width, int height, int offset, int quality, int isjpeg,
		int fontSize) {
	// Create captcha image
	//----------------------

	// Write colored and distorted text
	CImg<unsigned char> captcha(width, height, 1, 3, 0), color(3);

	char letter[2] = { 0 };
	for (unsigned int k = 0; k < count; ++k) {
		CImg<unsigned char> tmp;
		*letter = captcha_text[k];
		if (*letter) {
			cimg_forX(color,i)
				color[i] = (unsigned char) (128 + (std::rand() % 127));
			tmp.draw_text((int) (2 + 8 * cimg::rand()),
					(int) (12 * cimg::rand()), letter, color.data(), 0, 1,
					fontSize).resize(-100, -100, 1, 3);
//      const unsigned int dir = std::rand()%4, wph = tmp.width()+tmp.height();
//      cimg_forXYC(tmp,x,y,v) {
//        const int val = dir==0?x+y:(dir==1?x+tmp.height()-y:(dir==2?y+tmp.width()-x:tmp.width()-x+tmp.height()-y));
//        tmp(x,y,v) = (unsigned char)cimg::max(0.0f,cimg::min(255.0f,1.5f*tmp(x,y,v)*val/wph));
//      }
			if (std::rand() % 2)
				tmp = (tmp.get_dilate(3) -= tmp);
			tmp.blur((float) cimg::rand() * 0.8f).normalize(0, 255);
			const float sin_offset = (float) cimg::crand() * 3, sin_freq =
					(float) cimg::crand() / 7;
			cimg_forYC(captcha,y,v)
				captcha.get_shared_row(y, 0, v).shift(
						(int) (4 * std::cos(y * sin_freq + sin_offset)));
			captcha.draw_image(count + offset * k, tmp);
		}
	}

	// Add geometric and random noise
	CImg<unsigned char> copy = (+captcha).fill(0);
	for (unsigned int l = 0; l < 3; ++l) {
		if (l)
			copy.blur(0.5f).normalize(0, 148);
		for (unsigned int k = 0; k < 10; ++k) {
			cimg_forX(color,i)
				color[i] = (unsigned char) (128 + cimg::rand() * 127);
			if (cimg::rand() < 0.5f)
				copy.draw_circle((int) (cimg::rand() * captcha.width()),
						(int) (cimg::rand() * captcha.height()),
						(int) (cimg::rand() * 30), color.data(), 0.6f, ~0U);
			else
				copy.draw_line((int) (cimg::rand() * captcha.width()),
						(int) (cimg::rand() * captcha.height()),
						(int) (cimg::rand() * captcha.width()),
						(int) (cimg::rand() * captcha.height()), color.data(),
						0.6f);
		}
	}
	captcha |= copy;
	captcha.noise(10, 2);

	captcha = (+captcha).fill(255) - captcha;

	// Write output image and captcha text

	if (isjpeg) {
		captcha.save_jpeg(file_o, quality);
	} else {
		captcha.save(file_o);
	}
}

/**
 * 生成验证码
 **/
void Generate(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	//bool success = false;

	Dart_Handle captcha_text_object = HandleError(
			Dart_GetNativeArgument(arguments, 0));
	Dart_Handle file_out_object = HandleError(
			Dart_GetNativeArgument(arguments, 1));
	Dart_Handle count_object = HandleError(
			Dart_GetNativeArgument(arguments, 2));
	Dart_Handle width_object = HandleError(
			Dart_GetNativeArgument(arguments, 3));
	Dart_Handle height_object = HandleError(
			Dart_GetNativeArgument(arguments, 4));
	Dart_Handle offset_object = HandleError(
			Dart_GetNativeArgument(arguments, 5));
	Dart_Handle quality_object = HandleError(
			Dart_GetNativeArgument(arguments, 6));
	Dart_Handle isjpeg_object = HandleError(
			Dart_GetNativeArgument(arguments, 7));
	Dart_Handle fontSize_object = HandleError(
			Dart_GetNativeArgument(arguments, 8));

	const char* captcha_text;
	HandleError(Dart_StringToCString(captcha_text_object, &captcha_text));

	const char* file_o;
	HandleError(Dart_StringToCString(file_out_object, &file_o));

	int64_t count;
	HandleError(Dart_IntegerToInt64(count_object, &count));

	int64_t width;
	HandleError(Dart_IntegerToInt64(width_object, &width));

	int64_t height;
	HandleError(Dart_IntegerToInt64(height_object, &height));

	int64_t offset;
	HandleError(Dart_IntegerToInt64(offset_object, &offset));

	int64_t quality;
	HandleError(Dart_IntegerToInt64(quality_object, &quality));

	int64_t isjpeg;
	HandleError(Dart_IntegerToInt64(isjpeg_object, &isjpeg));

	int64_t fontSize;
	HandleError(Dart_IntegerToInt64(fontSize_object, &fontSize));

	_generate(captcha_text, file_o, count, width, height, offset, quality,
			isjpeg, fontSize);
	//success = true;

	//Dart_Handle result = HandleError(Dart_NewBoolean(success));
	//Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

//dart到c的转换映射
FunctionLookup function_list[] = { { "Generate", Generate }, { NULL, NULL } };

//调用本地函数解释为C函数的实现
Dart_NativeFunction ResolveName(Dart_Handle name, int argc) {
	if (!Dart_IsString(name))
		return NULL;
	Dart_NativeFunction result = NULL;
	Dart_EnterScope();
	const char* cname;
	HandleError(Dart_StringToCString(name, &cname));

	for (int i = 0; function_list[i].name != NULL; ++i) {
		if (strcmp(function_list[i].name, cname) == 0) {
			result = function_list[i].function;
			break;
		}
	}
	Dart_ExitScope();
	return result;
}

