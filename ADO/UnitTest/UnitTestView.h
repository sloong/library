
// UnitTestView.h : interface of the CUnitTestView class
//

#pragma once


class CUnitTestView : public CView
{
protected: // create from serialization only
	CUnitTestView();
	DECLARE_DYNCREATE(CUnitTestView)

// Attributes
public:
	CUnitTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CUnitTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UnitTestView.cpp
inline CUnitTestDoc* CUnitTestView::GetDocument() const
   { return reinterpret_cast<CUnitTestDoc*>(m_pDocument); }
#endif

