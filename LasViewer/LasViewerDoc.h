
// LasViewerDoc.h : CLasViewerDoc ��Ľӿ�
//


#pragma once

#include "LasDataSet.h"
#include "GPoint.h"
#include <vector>

using std::vector;

class CLasViewerDoc : public CDocument
{
protected: // �������л�����
	CLasViewerDoc();
	DECLARE_DYNCREATE(CLasViewerDoc)

// ����
public:
	void		GetData(double*	&pXYZ, COLORREF* &pClr, size_t &cntPnt);

// ����
public:
	bool		ReadData();
	void		UpdateAABB();
	void		AddColor();

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
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

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
