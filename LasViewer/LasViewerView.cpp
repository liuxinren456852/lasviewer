
// LasViewerView.cpp : CLasViewerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LasViewer.h"
#endif

#include "LasViewerDoc.h"
#include "LasViewerView.h"

#include <gl/GL.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLasViewerView

IMPLEMENT_DYNCREATE(CLasViewerView, CView)

BEGIN_MESSAGE_MAP(CLasViewerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MENUITEM_SET_BK_COLOR, &CLasViewerView::OnMenuitemSetBkColor)
END_MESSAGE_MAP()

// CLasViewerView 构造/析构

CLasViewerView::CLasViewerView()
{
	// TODO: 在此处添加构造代码
	
}

CLasViewerView::~CLasViewerView()
{
}

BOOL CLasViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLasViewerView 绘制

void CLasViewerView::OnDraw(CDC* /*pDC*/)
{
	CLasViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	m_gl.OnInitRenderOpenGL();

	Draw();

	m_gl.SwapBufferOpenGL();
}

void CLasViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLasViewerView 诊断

#ifdef _DEBUG
void CLasViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CLasViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLasViewerDoc* CLasViewerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLasViewerDoc)));
	return (CLasViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLasViewerView 消息处理程序


int CLasViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_gl.SetProjectionMode(1);
	m_gl.SetTwoView(false);

	m_gl.OnInitOpenGL(GetSafeHwnd());

	return 0;
}


void CLasViewerView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	m_gl.OnDestroyOpenGL();
}


void CLasViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnLButtonDown(nFlags, point);
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CLasViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnLButtonUp(nFlags, point);
	Invalidate();

	CView::OnLButtonUp(nFlags, point);
}

void CLasViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnRButtonDown(nFlags, point);
	Invalidate();

	CView::OnRButtonDown(nFlags, point);
}

void CLasViewerView::OnRButtonUp(UINT  nFlags, CPoint point)
{
	// 	ClientToScreen(&point);
	// 	OnContextMenu(this, point);
	m_gl.OnRButtonUp(nFlags, point);
	Invalidate();
}

void CLasViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnMButtonDown(nFlags, point);
	Invalidate();

	CView::OnMButtonDown(nFlags, point);
}

void CLasViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnMButtonUp(nFlags, point);
	Invalidate();

	CView::OnMButtonUp(nFlags, point);
}

void CLasViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnMouseMove(nFlags, point);
	Invalidate();

	CView::OnMouseMove(nFlags, point);
}


BOOL CLasViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnMouseWheel(nFlags, zDelta, pt);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLasViewerView::UpdateAABB(GPoint minPnt, GPoint maxPnt)
{
	tagCVector org;
	org.x = float(minPnt._x + maxPnt._x) / 2;
	org.y = float(minPnt._y + maxPnt._y) / 2;
	org.z = float(minPnt._z + maxPnt._z) / 2;

	tagCVector box;
	box.x = float(maxPnt._x - minPnt._x) / 2;
	box.y = float(maxPnt._y - minPnt._y) / 2;
	box.z = float(maxPnt._z - minPnt._z) / 2;

	m_gl.SetBox(org, box);
}

void CLasViewerView::Draw()
{
	double *pXYZ;
	COLORREF *pClr;
	size_t cntPnt;

	((CLasViewerDoc*)GetDocument())->GetData(pXYZ, pClr, cntPnt);

	if (cntPnt <= 0)	return;

	glPointSize(2);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(COLORREF), pClr);
	glVertexPointer(3, GL_DOUBLE, sizeof(GPoint), pXYZ);

	glDrawArrays(GL_POINTS, 0, cntPnt);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void CLasViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_gl.OnSizeOpenGL(nType, cx, cy);	
}


BOOL CLasViewerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CLasViewerView::OnMenuitemSetBkColor()
{
	// TODO: Add your command handler code here

}
