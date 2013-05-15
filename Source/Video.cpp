//#include "StdAfx.h"

#include "Common.h"

#include "Video.h"
#include "Memory.h"
#include "Allegrex/Allegrex.h"
#include <tchar.h>
#include <emmintrin.h>
#include "HLE/Modules.h"
extern void* hMainWindow;


#include <gl/glew.h>

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
GLuint texFb;
float texS, texT;



	u8 *vram;

	u32 pspMode;
	u32 pspWidth;
	u32 pspPitch;
	u32 pspHeight;
	u32 pspAddr;
	u32 pixelFormatFb;
	u32 bufferWidthFb;

	TCHAR windowTitle[1024];
	u64 cntFreq;
	u64 cntStart;
	u64 cntEnd;
	u32 cntFrame;

	u32 prevCycles;
	bool geUpdate;
	u64  cntLastUpdate;
	u64  cntFrequency;

static u32 makePow2(u32 n) {
    --n;
    n = (n >>  1) | n;
    n = (n >>  2) | n;
    n = (n >>  4) | n;
    n = (n >>  8) | n;
    n = (n >> 16) | n;
    return ++n;
}

static int getPixelFormatBytes(int pixelformat) {
    return pixelformat == PSP_DISPLAY_PIXEL_FORMAT_8888 ? 4 : 2;
}

static int getPixelFormatGL(int pixelformat) {
    switch (pixelformat) {
    case PSP_DISPLAY_PIXEL_FORMAT_565:
        return GL_UNSIGNED_SHORT_5_6_5_REV;
    case PSP_DISPLAY_PIXEL_FORMAT_5551:
        return GL_UNSIGNED_SHORT_1_5_5_5_REV;
    case PSP_DISPLAY_PIXEL_FORMAT_4444:
        return GL_UNSIGNED_SHORT_4_4_4_4_REV;
    default:
        return GL_UNSIGNED_BYTE;
    }
}

bool video::Initialize() {
	GLuint		PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC((HWND)hMainWindow)))							// Did We Get A Device Context?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		//KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	GLenum glewErr;
	if((glewErr = glewInit()) != GLEW_OK) {
		MessageBox(NULL, (char *)glewGetErrorString(glewErr), "ERROR", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	vram = Memory::videoMemory;

	glGenTextures(1, &texFb);

	QueryPerformanceFrequency((LARGE_INTEGER *)&cntFreq);
	QueryPerformanceCounter((LARGE_INTEGER *)&cntStart);
	cntFrame = 0;
	prevCycles = 0;

	return true;
}

void video::Finalize() {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC((HWND)hMainWindow, hDC);
}

void video::Draw() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    
	glViewport(0, 0, pspWidth, pspHeight);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);

    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, getPixelFormatBytes(pixelFormatFb));
    glPixelStorei(GL_UNPACK_ROW_LENGTH, pspPitch);

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0.0, pspWidth, pspHeight, 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    glBindTexture(GL_TEXTURE_2D, texFb);
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(texS, texT);
    glVertex2i(pspWidth, pspHeight);

    glTexCoord2f(0.0f, texT);
    glVertex2i(0, pspHeight);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, 0);

    glTexCoord2f(texS, 0.0f);
    glVertex2i(pspWidth, 0);

    glEnd();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();

	::SwapBuffers(hDC);
}


PspDisplayErrorCodes video::sceSetDisplayMode(u32 mode, u32 width, u32 height) {
		Modules::log.debug(
		"sceSetDisplayMode(mode = %d, width = %d, height = %d)",
		mode, width, height);
	if (mode != 0 || width == 0 || height == 0 || width > 480 || height > 272)
		return SCE_DISPLAY_ERROR_ARGUMENT;



	pspMode = mode;
	pspWidth = width;
	pspHeight = height;

	return SCE_DISPLAY_ERROR_OK;
}

PspDisplayErrorCodes video::sceDisplaySetFrameBuf(
	u32 topaddr, u32 bufferwidth,
	PspDisplayPixelFormat pixelformat,
	PspDisplaySetBufSync sync)
{	
	Modules::log.debug(
			"sceDisplaySetFrameBuf(topAddr = 0x%08X, bufferWidth = %d, pixelFormat = %d, sync = %d",
			topaddr, bufferwidth, (PspDisplayPixelFormat)pixelformat, (PspDisplaySetBufSync)sync);
	topaddr &= 0x0FFFFFFF;
	if (topaddr < Memory::VideoMemoryStart || (topaddr + bufferwidth * pspHeight * 4) > Memory::VideoMemoryEnd ||
		topaddr & (bufferwidth - 1))
	{
		return SCE_DISPLAY_ERROR_POINTER;
	}
	pspAddr = topaddr - Memory::VideoMemoryStart;
	
	if(pixelFormatFb != pixelformat ||
		pspPitch != bufferwidth) {
		glBindTexture(GL_TEXTURE_2D, texFb);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RGB,
			bufferwidth, makePow2(pspHeight), 0,
			GL_RGBA,
			getPixelFormatGL(pixelformat), NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		pixelFormatFb = pixelformat;
		pspPitch = bufferwidth;
	}

	glBindTexture(GL_TEXTURE_2D, texFb);
	int pixelFormatGL = getPixelFormatGL(pixelformat);
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		0, 0, bufferwidth, pspHeight,
		pixelFormatGL == GL_UNSIGNED_SHORT_5_6_5_REV ? GL_RGB : GL_RGBA,
		pixelFormatGL, vram + pspAddr);

	texS = (float)pspWidth / (float)bufferwidth;
    texT = (float)pspHeight / (float)makePow2(pspHeight);

	Draw();
	
	++cntFrame;
	geUpdate = true;
	/*QueryPerformanceCounter((LARGE_INTEGER *)&cntEnd);
	if ((cntEnd - cntStart) > cntFreq) {
		double fix = (double)cntFreq / (cntEnd - cntStart);
		_stprintf_s(
			windowTitle,
			sizeof(windowTitle) / sizeof(TCHAR),
			_T("FPS: %.2lf MIPS: %.5lf"),
			cntFrame * fix,
			(pspRegs.cycle - prevCycles) * fix /
			1000000.0f);
		puts(windowTitle);
		cntFrame = 0;
		cntStart = cntEnd;
		prevCycles = pspRegs.cycle;
	}*/

	/*MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		//if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	return SCE_DISPLAY_ERROR_OK;
}


void video::Step() { //TODO!!
/*	u64 cntNow;
	QueryPerformanceCounter((LARGE_INTEGER *)&cntNow);
	if ((cntNow - cntLastUpdate) > (cntFrequency / 60)) {
		if (geUpdate) {
			Draw();

			// TODO: temporal hack, because I have to add some
			// detection of writes to video ram in Memory.cpp
			//geUpdate = false;
		}
		cntLastUpdate = cntNow;
	}*/
}