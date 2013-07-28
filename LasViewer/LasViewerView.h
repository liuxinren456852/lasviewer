
// LasViewerView.h : CLasViewerView 类的接口
//

#pragma once

#include "COpenGL.h"

class CLasViewerView : public CView
{
protected: // 仅从序列化创建
	CLasViewerView();
	DECLARE_DYNCREATE(CLasViewerView)

// 特性
public:
	CLasViewerDoc* GetDocument() const;

// 操作
public:
	void			UpdateAABB(GPoint minPnt, GPoint maxPnt);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CLasViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void			Draw();

private:
	COpenGL			m_gl;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMenuitemSetBkColor();
};

#ifndef _DEBUG  // LasViewerView.cpp 中的调试版本
inline CLasViewerDoc* CLasViewerView::GetDocument() const
   { return reinterpret_cast<CLasViewerDoc*>(m_pDocument); }
#endif

