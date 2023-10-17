
// OpenGL_TestView.h: COpenGLTestView 클래스의 인터페이스
//

#pragma once
#include "Sphere.h"
#include "ImageLoad.h"

class COpenGLTestView : public CView
{
protected: // serialization에서만 만들어집니다.
	COpenGLTestView() noexcept;
	DECLARE_DYNCREATE(COpenGLTestView)

// 특성입니다.
public:
	COpenGLTestDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~COpenGLTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

// Test OpenGL
public:
	HDC m_hDC; // Divice Context
	HGLRC m_hglRC; // Rendering Context

public:
	// Camera
	GLfloat position[3] = { 0.0f,15.0f,15.0f };
	GLfloat lookAt[3] = { 0.0f,0.0f,0.0f };
	GLfloat upVector[3] = { 0.0f,1.0f,0.0f };
	GLfloat lookVector[3];
	GLfloat leftVector[3];
	bool multiView;
	bool mouseClick;
	GLfloat rotation[3][3];
	GLfloat mouse_x;
	GLfloat mouse_y;

private:
	int sceneWidth;
	int sceneHeight;

	// Planet
	Sphere Sun;
	Sphere Mercury;
	Sphere Venus;
	Sphere Earth;
	Sphere Moon;
	Sphere Mars;

	// Light
	GLfloat lightAmbient[4] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat lightDiffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightSpecular[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightPosition[4] = { 0.0f,0.0f,0.0f,1.0f };

public:
	void SetSize(int width, int height);
	void InitGL();
	void ReShape(GLsizei width, GLsizei height, int num);
	void COpenGLTestView::DrawSolarSystem(int num);
	BOOL SetPixelformat(HDC hdc);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void Display();

public:
	// Orbit
	void DrawPlanetOrbit(float radius);

	// Lighting
	void InitLight();

	// CameraMove
	void LookVector();
	void LeftVector();
	void rotMatrix(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

	// Texture
	GLuint LoadTexture(const char *fileName);
	GLuint tex_Sun;
	GLuint tex_Mercury;
	GLuint tex_Venus;
	GLuint tex_Earth;
	GLuint tex_Moon;
	GLuint tex_Mars;

	unsigned int acc;
};

#ifndef _DEBUG  // OpenGL_TestView.cpp의 디버그 버전
inline COpenGLTestDoc* COpenGLTestView::GetDocument() const
   { return reinterpret_cast<COpenGLTestDoc*>(m_pDocument); }
#endif