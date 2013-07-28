
// LasViewerDoc.cpp : CLasViewerDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "LasViewer.h"
#endif

#include <algorithm>
#include <string>

#include "LasViewerDoc.h"
#include "MainFrm.h"
#include "LasViewerView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLasViewerDoc

IMPLEMENT_DYNCREATE(CLasViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CLasViewerDoc, CDocument)
END_MESSAGE_MAP()


// CLasViewerDoc ����/����

CLasViewerDoc::CLasViewerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CLasViewerDoc::~CLasViewerDoc()
{

}

BOOL CLasViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CLasViewerDoc ���л�

void CLasViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CLasViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CLasViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CLasViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLasViewerDoc ���

#ifdef _DEBUG
void CLasViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLasViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLasViewerDoc ����


BOOL CLasViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

#ifdef UNICODE
	std::string filename = CW2A(lpszPathName);
#else
	std::string filename(lpszPathName);
#endif

	int cntPnt;
	if ((cntPnt = m_lasDataSet.Open(filename)) > 0)
	{
		try
		{
			m_points.resize(cntPnt);
			m_colors.resize(cntPnt);
		}
		catch(std::bad_alloc)
		{
			return FALSE;
		}
		
		m_lasDataSet.GetXYZ((double*)m_points.data());

		UpdateAABB();
		AddColor();

		return TRUE;
	}
	
	return FALSE;
}

void CLasViewerDoc::UpdateAABB()
{
	using namespace std;

	m_minPnt = m_points[0];
	m_maxPnt = m_points[0];

	for (size_t i = 1; i < m_points.size(); ++i)
	{
		const GPoint& pnt = m_points[i];
		
		m_minPnt._x = min(m_minPnt._x, pnt._x);
		m_minPnt._y = min(m_minPnt._y, pnt._y);
		m_minPnt._z = min(m_minPnt._z, pnt._z);

		m_maxPnt._x = max(m_maxPnt._x, pnt._x);
		m_maxPnt._y = max(m_maxPnt._y, pnt._y);
		m_maxPnt._z = max(m_maxPnt._z, pnt._z);
	}

	CView *pView = ((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CLasViewerView *pViewerView = (CLasViewerView*)pView;
	pViewerView->UpdateAABB(m_minPnt, m_maxPnt);
}

void CLasViewerDoc::AddColor()
{
	// ��ɫ��
	COLORREF clrs[] =
	{
		RGB(0, 0, 255),
		RGB(0, 255, 255),
		RGB(0, 255, 0),
		RGB(255, 255,0),
		RGB(255, 0, 0),
	};

	double dz = m_maxPnt._z - m_minPnt._z;
	if (dz < 1e-2)
	{
		for (auto i = begin(m_colors); i != end(m_colors); ++i)	*i = clrs[0];
		return;
	}

	double minZ = m_minPnt._z;
	double maxZ = m_maxPnt._z;

	// ��������ɫ
	const int nColor = _countof(clrs);
	double step = (maxZ - minZ) * 1.0001 / (nColor - 1);

	for (size_t i = 0; i < m_points.size(); ++i)
	{
		double idx = (m_points[i]._z - minZ) / step;
		int pos = int(idx);
		double rate = idx - pos;
		ASSERT(pos >= 0 && pos < nColor - 1);

		BYTE b[2][3] = 
		{
			GetRValue(clrs[pos]), GetGValue(clrs[pos]), GetBValue(clrs[pos]),
			GetRValue(clrs[pos+1]), GetGValue(clrs[pos+1]), GetBValue(clrs[pos+1])
		};

		BYTE red	= BYTE(b[0][0] * (1 - rate) + b[1][0] * rate);
		BYTE green	= BYTE(b[0][1] * (1 - rate) + b[1][1] * rate);
		BYTE blue	= BYTE(b[0][2] * (1 - rate) + b[1][2] * rate);

		m_colors[i] = RGB(red, green, blue);
	}
}

void CLasViewerDoc::GetData(double* &pXYZ, COLORREF* &pClr, size_t &cntPnt)
{
	pXYZ = (double*)m_points.data();
	pClr = m_colors.data();
	cntPnt = m_points.size();
}