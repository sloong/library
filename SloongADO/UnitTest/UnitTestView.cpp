
// UnitTestView.cpp : implementation of the CUnitTestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "UnitTest.h"
#endif

#include "UnitTestDoc.h"
#include "UnitTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUnitTestView

IMPLEMENT_DYNCREATE(CUnitTestView, CView)

BEGIN_MESSAGE_MAP(CUnitTestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CUnitTestView construction/destruction

CUnitTestView::CUnitTestView()
{
	// TODO: add construction code here

}

CUnitTestView::~CUnitTestView()
{
}

BOOL CUnitTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUnitTestView drawing

void CUnitTestView::OnDraw(CDC* /*pDC*/)
{
	CUnitTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CUnitTestView printing

BOOL CUnitTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUnitTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CUnitTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CUnitTestView diagnostics

#ifdef _DEBUG
void CUnitTestView::AssertValid() const
{
	CView::AssertValid();
}

void CUnitTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUnitTestDoc* CUnitTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUnitTestDoc)));
	return (CUnitTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CUnitTestView message handlers
