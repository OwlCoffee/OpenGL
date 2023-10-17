
// OpenGL_TestView.cpp: COpenGLTestView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "OpenGL_Test.h"
#endif

#include "OpenGL_TestDoc.h"
#include "OpenGL_TestView.h"
#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLTestView

IMPLEMENT_DYNCREATE(COpenGLTestView, CView)

BEGIN_MESSAGE_MAP(COpenGLTestView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// COpenGLTestView 생성/소멸

COpenGLTestView::COpenGLTestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	multiView = false;
	mouseClick = false;

	acc = 5;
}

COpenGLTestView::~COpenGLTestView()
{
}

BOOL COpenGLTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// COpenGLTestView 그리기

void COpenGLTestView::OnDraw(CDC* /*pDC*/)
{
	COpenGLTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	Display();
	InvalidateRect(NULL, FALSE); // MFC invalid
}


// COpenGLTestView 인쇄


void COpenGLTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGLTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void COpenGLTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void COpenGLTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void COpenGLTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenGLTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenGLTestView 진단

#ifdef _DEBUG
void COpenGLTestView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLTestDoc* COpenGLTestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLTestDoc)));
	return (COpenGLTestDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLTestView 메시지 처리기


// Test OpenGL
void COpenGLTestView::InitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	tex_Sun = LoadTexture("sun.bmp");
	tex_Mercury = LoadTexture("mercury.bmp");
	tex_Venus = LoadTexture("venus.bmp");
	tex_Earth = LoadTexture("earth_daymap.bmp");
	tex_Moon = LoadTexture("moon.bmp");
	tex_Mars = LoadTexture("mars.bmp");
}

void COpenGLTestView::ReShape(GLsizei width, GLsizei height, int num)
{
	if (height == 0)
	{
		height = 1;
	}

	GLdouble h = 2 * 0.1*tan((45.0 / 2) * 3.141592 / 180.0);
	GLdouble aspect = (GLdouble)width / (GLdouble)height;

	if (num == 1)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(aspect*-h, aspect*h, -h, h, 0.1, 1000.0);
		//gluPerspective(45.0f, aspect, 0.1f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else if (num == 2)
	{
		glViewport(0, 0, width / 2, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(aspect*-h, aspect*h, -h, h, 0.1, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else if (num == 3)
	{
		glViewport(width / 2, 0, width / 2, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(aspect*-h, aspect*h, -h, h, 0.1, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void COpenGLTestView::DrawSolarSystem(int num)
{
	GLfloat angle = 360;
	static GLfloat move = 1.0f;
	move += acc;

	GLfloat ambientMat[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat diffuseMat[4] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat specularMat[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat emssiveMat_Sun[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat emssiveMat[4] = { 0.0f,0.0f,0.0f,0.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glPushMatrix(); // Sun
	glMaterialfv(GL_FRONT, GL_EMISSION, emssiveMat_Sun);
	glBindTexture(GL_TEXTURE_2D, tex_Sun);
	Sun.DrawSphere(num);

	glMaterialfv(GL_FRONT, GL_EMISSION, emssiveMat);
	glPushMatrix(); // Mercury
	DrawPlanetOrbit(3.0f);
	glRotated(angle / 87.97f / 24 * move, 0.0f, 1.0f, 0.0f);
	glTranslated(3.0f, 0.0f, 0.0f);
	glRotated(angle / 58 / 24 * move, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, tex_Mercury);
	Mercury.DrawSphere(num);
	glPopMatrix();

	glPushMatrix(); // Venus
	DrawPlanetOrbit(5.0f);
	glRotated(angle / 224 / 24 * move, 0.0f, 1.0f, 0.0f);
	glTranslated(5.0f, 0.0f, 0.0f);
	glRotated(angle / -243 / 24 * move, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, tex_Venus);
	Venus.DrawSphere(num);
	glPopMatrix();

	glPushMatrix(); // Earth
	DrawPlanetOrbit(7.0f);
	glRotated(angle / 365 / 24 * move, 0.0f, 1.0f, 0.0f);
	glTranslated(7.0f, 0.0f, 0.0f);
	glRotated(angle / 24 / 24 * move, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, tex_Earth);
	Earth.DrawSphere(num);
	glPushMatrix(); // Moon
	DrawPlanetOrbit(0.7f);
	glRotated(angle / 365 / 24 * move, 0.0f, 1.0f, 0.0f);
	glTranslated(0.7f, 0.0f, 0.0f);
	glRotated(angle / 27 / 24 * move, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, tex_Moon);
	Moon.DrawSphere(num);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix(); // Mars
	DrawPlanetOrbit(10.0f);
	glRotated(angle / 686.98f / 24 * move, 0.0f, 1.0f, 0.0f);
	glTranslated(10.0f, 0.0f, 0.0f);
	glRotated(angle / 24.5f / 24 * move, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, tex_Mars);
	Mars.DrawSphere(num);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glFlush();
}

BOOL COpenGLTestView::SetPixelformat(HDC hdc)
{
	int pixelformat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd 
		1, // default version
		PFD_DRAW_TO_WINDOW | // support window 
		PFD_SUPPORT_OPENGL | // support OpenGL 
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER, // double buffered 
		PFD_TYPE_RGBA, // RGBA type 
		32, // 32-bit color depth 
		0, 0, 0, 0, 0, 0, // color bits ignored 
		8, // no alpha buffer 
		0, // shift bit ignored 
		8, // no accumulation buffer 
		0, 0, 0, 0, // accum bits ignored 
		16, // 16-bit z-buffer
		0, // no stencil buffer 
		0, // no auxiliary buffer 
		PFD_MAIN_PLANE, // main layer 
		0, // reserved 
		0, 0, 0 // layer masks ignored 
	};

	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE)
	{
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

int COpenGLTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	// set the pixel format
	if (!SetPixelformat(m_hDC))
	{
		return -1;
	}

	// create rendering context and make it current
	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);

	InitGL();
	InitLight();


	return 0;
}

void COpenGLTestView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// deselect rendering context and delete it
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hglRC);
}

void COpenGLTestView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSize(cx, cy);
}

void COpenGLTestView::SetSize(int width, int height)
{
	sceneWidth = width;
	sceneHeight = height;
}

void COpenGLTestView::DrawPlanetOrbit(float radius)
{
	float x, z;
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 50; i++)
	{
		x = radius * (float)cos(6.28*(float)i / 50.0);
		z = radius * (float)sin(6.28*(float)i / 50.0);
		glVertex3f(x, 0.0, z);
		glNormal3f(x, 0.0, z);
	}
	glEnd();
}

void COpenGLTestView::Display()
{
	Sun.SetSphere(1.0f, 50, 25, 1.0f, 0.5f, 0.0f);
	Mercury.SetSphere(0.2f, 50, 25, 0.5f, 0.8f, 1.0f);
	Venus.SetSphere(0.275f, 50, 25, 1.0f, 1.0f, 0.0f);
	Earth.SetSphere(0.3f, 50, 25, 0.2f, 0.2f, 1.0f);
	Moon.SetSphere(0.1f, 50, 25, 0.74f, 0.74f, 0.74f);
	Mars.SetSphere(0.25f, 50, 25, 1.0f, 0.5f, 0.4f);

	if (!multiView)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		ReShape(sceneWidth, sceneHeight, 1);
		gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], upVector[0], upVector[1], upVector[2]);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		DrawSolarSystem(2); // Using vertex array
		SwapBuffers(m_hDC);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		ReShape(sceneWidth, sceneHeight, 2);
		gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], upVector[0], upVector[1], upVector[2]);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		DrawSolarSystem(2); // Using vertex array
		ReShape(sceneWidth, sceneHeight, 3);
		gluLookAt(0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		//gluLookAt(position[0], position[1], position[2], lookAt[0], lookAt[1], lookAt[2], upVector[0], upVector[1], upVector[2]);
		DrawSolarSystem(1); // Using glBegin()/glEnd()
		SwapBuffers(m_hDC);
	}
}

void COpenGLTestView::InitLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHT0);
}

void COpenGLTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case'W':
		LookVector();
		for (int i = 0; i < 3; i++)
		{
			position[i] += lookVector[i];
			lookAt[i] += lookVector[i];
		}
		break;
	case'S':
		LookVector();
		for (int i = 0; i < 3; i++)
		{
			position[i] -= lookVector[i];
			lookAt[i] -= lookVector[i];
		}
		break;
	case'A':
		LeftVector();
		for (int i = 0; i < 3; i++)
		{
			position[i] += leftVector[i];
			lookAt[i] += leftVector[i];
		}
		break;
	case'D':
		LeftVector();
		for (int i = 0; i < 3; i++)
		{
			position[i] -= leftVector[i];
			lookAt[i] -= leftVector[i];
		}
		break;
	case'V':
		multiView = !multiView;
		break;
	case'B':
		if (acc > 1)
		{
			acc--;
		}
		break;
	case'N':
		acc++;
		break;
	}

	InvalidateRect(NULL, FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenGLTestView::LookVector()
{
	// AB vector = B - A
	lookVector[0] = lookAt[0] - position[0];
	lookVector[1] = lookAt[1] - position[1];
	lookVector[2] = lookAt[2] - position[2];

	GLfloat unit = sqrt(lookVector[0] * lookVector[0] + lookVector[1] * lookVector[1] + lookVector[2] * lookVector[2]);
	lookVector[0] = lookVector[0] / unit;
	lookVector[1] = lookVector[1] / unit;
	lookVector[2] = lookVector[2] / unit;
}

void COpenGLTestView::LeftVector()
{
	LookVector();
	// cross product
	leftVector[0] = (upVector[1] * lookVector[2]) - (upVector[2] * lookVector[1]);
	leftVector[1] = (upVector[2] * lookVector[0]) - (upVector[0] * lookVector[2]);
	leftVector[2] = (upVector[0] * lookVector[1]) - (upVector[1] * lookVector[0]);

	GLfloat unit = sqrt(leftVector[0] * leftVector[0] + leftVector[1] * leftVector[1] + leftVector[2] * leftVector[2]);
	leftVector[0] = leftVector[0] / unit;
	leftVector[1] = leftVector[1] / unit;
	leftVector[2] = leftVector[2] / unit;
}

void COpenGLTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	mouseClick = true;
	mouse_x = (GLfloat)point.x;
	mouse_y = (GLfloat)point.y;
	CView::OnLButtonDown(nFlags, point);
}

void COpenGLTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	mouseClick = false;
	CView::OnLButtonUp(nFlags, point);
}

void COpenGLTestView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (mouseClick)
	{
		GLfloat angle = 0;
		GLfloat tempV1[3] = { 0,0,0 };
		GLfloat tempV2[3] = { 0,0,0 };

		// Yaw
		LookVector();
		angle = (mouse_x - point.x)*(GLfloat)0.001;
		rotMatrix(angle, 0, 1, 0);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				tempV1[i] += rotation[i][j] * lookVector[j];
			}
		}

		// Pitch
		LeftVector();
		angle = (point.y - mouse_y)*(GLfloat)0.001;
		rotMatrix(angle, leftVector[0], leftVector[1], leftVector[2]);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				tempV2[i] += rotation[i][j] * tempV1[j];
			}
		}
		for (int i = 0; i < 3; i++)
		{
			lookAt[i] = position[i] + tempV2[i];
		}
		mouse_x = (GLfloat)point.x;
		mouse_y = (GLfloat)point.y;

		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}

void COpenGLTestView::rotMatrix(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	// Axis-angle method
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// x postion
	rotation[0][0] = ((1 - cosTheta)*x*x + cosTheta);
	rotation[0][1] = ((1 - cosTheta)*x*y - sinTheta * z);
	rotation[0][2] = ((1 - cosTheta)*x*z + sinTheta * y);

	// y postion
	rotation[1][0] = ((1 - cosTheta)*x*y + sinTheta * z);
	rotation[1][1] = ((1 - cosTheta)*y*y + cosTheta);
	rotation[1][2] = ((1 - cosTheta)*y*z - sinTheta * x);

	// z postion
	rotation[2][0] = ((1 - cosTheta)*x*z - sinTheta * y);
	rotation[2][1] = ((1 - cosTheta)*y*z + sinTheta * x);
	rotation[2][2] = ((1 - cosTheta)*z*z + cosTheta);
}

GLuint COpenGLTestView::LoadTexture(const char *fileName)
{
	ImageLoad imageLoad;
	if (!imageLoad.Load(fileName))
	{
		return 0;
	}

	GLuint texture;

	int width = imageLoad.getWidth();
	int height = imageLoad.getHeight();
	const unsigned char *data = imageLoad.getDataRGB();
	GLenum type = GL_UNSIGNED_BYTE;
	GLenum format;
	int bpp = imageLoad.getBitCount();
	if (bpp == 8)
	{
		format = GL_LUMINANCE;
	}
	else if (bpp == 24)
	{
		format = GL_RGB;
	}
	else if (bpp == 32)
	{
		format = GL_RGBA;
	}
	else
	{
		return 0;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);

	switch (bpp)
	{
	case 8:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
		break;
	case 24:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
		break;
	case 32:
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
		break;
	}

	return texture;
}