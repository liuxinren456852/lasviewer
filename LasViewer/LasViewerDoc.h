
// LasViewerDoc.h : CLasViewerDoc 类的接口
//


#pragma once

#include "LasDataSet.h"
#include "GPoint.h"
#include <vector>

using std::vector;

class CLasViewerDoc : public CDocument
{
protected: // 仅从序列化创建
	CLasViewerDoc();
	DECLARE_DYNCREATE(CLasViewerDoc)

// 特性
public:
	void		GetData(double*	&pXYZ, COLORREF* &pClr, size_t &cntPnt);

// 操作
public:
	bool		ReadData();
	void		UpdateAABB();
	void		AddColor();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CLasViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	CLasDataSet			m_lasDataSet;

	vector<GPoint>		m_points;
	vector<COLORREF>	m_colors;

	GPoint				m_minPnt, m_maxPnt;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
