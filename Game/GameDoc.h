
// GameDoc.h : interface of the CGameDoc class
//


#pragma once


class CGameDoc : public CDocument
{
protected: // create from serialization only
	CGameDoc();
	DECLARE_DYNCREATE(CGameDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGameDoc();
	void setMissedScore();
	int getMissedScore();
	void setCatchedScore();
	int getCatchedScore();
	void setCount();
	int getCount();
	void setBallPosition(CPoint);
	void setRacketPosition(CPoint);
	CPoint getBallPosition();
	CPoint getRacketPosition();	
	void setTimerFrequency(UINT);
	UINT getTimerFrequency();
	COLORREF getBallColor();
	bool getMustStopTimersForNewDocument();
	void setMustStopTimersForNewDocument();
	void setGameRunning(bool);
	void checkBestScore();
	void setNumOfGames();
	void setGameOver();
	bool getGameOver();
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int missedScore;
	int catchedScore;
	int count;
	CPoint ballPosition;
	CPoint racketPosition;
	UINT timerFrequency;
	COLORREF ballColor;
	bool mustStopTimersForNewDocument;
	bool firstDocumentCreation;
	bool gameRunning;
	int bestScore;
	int numOfGames;
	bool gameOver;
	


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnBallcolorRed();
	afx_msg void OnUpdateBallcolorRed(CCmdUI *pCmdUI);
	afx_msg void OnBallcolorBlue();
	afx_msg void OnUpdateBallcolorBlue(CCmdUI *pCmdUI);
	afx_msg void OnBallcolorPurple();
	afx_msg void OnUpdateBallcolorPurple(CCmdUI *pCmdUI);
};
