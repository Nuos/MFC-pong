
// GameView.h : interface of the CGameView class
//

#pragma once


class CGameView : public CView
{
protected: // create from serialization only
	CGameView();
	DECLARE_DYNCREATE(CGameView)

// Attributes
public:
	CGameDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint ballStartPoint; 
	CPoint ballEndPoint;
	CPoint racketStartPoint; 
	CPoint racketEndPoint;	
	CSize stepsOf;
	CRect racket;
	bool mouseCaptured;
	bool isTimerSet;
	UINT frequency;
	bool gameMaxed;
	bool close;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDifficultylevelEasy();
	afx_msg void OnUpdateDifficultylevelEasy(CCmdUI *pCmdUI);
	afx_msg void OnDifficultylevelHard();
	afx_msg void OnUpdateDifficultylevelHard(CCmdUI *pCmdUI);
	afx_msg void OnDifficultylevelMedium();
	afx_msg void OnUpdateDifficultylevelMedium(CCmdUI *pCmdUI);
	afx_msg void OnFileCloseview();
	afx_msg void OnPausegame();
	afx_msg void OnRestartgame();
};

#ifndef _DEBUG  // debug version in GameView.cpp
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

