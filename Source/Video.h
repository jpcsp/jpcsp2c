#pragma once

enum PspDisplayPixelFormat {
	PSP_DISPLAY_PIXEL_FORMAT_565  = 0,
	PSP_DISPLAY_PIXEL_FORMAT_5551 = 1,
	PSP_DISPLAY_PIXEL_FORMAT_4444 = 2,
	PSP_DISPLAY_PIXEL_FORMAT_8888 = 3,

	_PspDisplayPixelFormat_force_u32 = 0x7FFFFFFF,
};

enum PspDisplaySetBufSync {
	PSP_DISPLAY_SETBUF_IMMEDIATE = 0,
	PSP_DISPLAY_SETBUF_NEXTFRAME = 1,

	_PspDisplaySetBufSync_force_u32 = 0x7FFFFFFF,
};

enum PspDisplayErrorCodes {
	SCE_DISPLAY_ERROR_OK = 0,
	SCE_DISPLAY_ERROR_POINTER = 0x80000103,
	SCE_DISPLAY_ERROR_ARGUMENT = 0x80000107
};

namespace video {
    bool Initialize();
	void Finalize();
	void Step();

	void Draw();

	PspDisplayErrorCodes sceSetDisplayMode(u32 mode, u32 width, u32 height);
	PspDisplayErrorCodes sceDisplaySetFrameBuf(
		u32 topaddr, u32 bufferwidth,
		PspDisplayPixelFormat pixelformat,
		PspDisplaySetBufSync sync);

	void ConvertFormat565();
	void ConvertFormat5551();
	void ConvertFormat4444();
  	void ConvertFormat8888();


}
