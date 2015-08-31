
// GameDoc.cpp : implementation of the CGameDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
	ON_COMMAND(ID_BALLCOLOR_RED, &CGameDoc::OnBallcolorRed)
	ON_UPDATE_COMMAND_UI(ID_BALLCOLOR_RED, &CGameDoc::OnUpdateBallcolorRed)
	ON_COMMAND(ID_BALLCOLOR_BLUE, &CGameDoc::OnBallcolorBlue)
	ON_UPDATE_COMMAND_UI(ID_BALLCOLOR_BLUE, &CGameDoc::OnUpdateBallcolorBlue)
	ON_COMMAND(ID_BALLCOLOR_PURPLE, &CGameDoc::OnBallcolorPurple)
	ON_UPDATE_COMMAND_UI(ID_BALLCOLOR_PURPLE, &CGameDoc::OnUpdateBallcolorPurple)
END_MESSAGE_MAP()


// CGameDoc construction/destruction

CGameDoc::CGameDoc()
{
	firstDocumentCreation = true;
	mustStopTimersForNewDocument = false;
	gameRunning = false;
	numOfGames = 0;
	bestScore = 0;
}

CGameDoc::~CGameDoc()
{
}

BOOL CGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// DOCUMENT INITIALIZATION
	gameOver = false;
	missedScore = 0;
	catchedScore = 0;
	count = 60;
	ballPosition.x = 190; 
	ballPosition.y = -200;  
	racketPosition.x = 500;  
	racketPosition.y = -780;  
	timerFrequency = 130;      //  INITIAL TIMER FREQUENCY
	ballColor = RGB(150, 0, 25); // INITIAL BALL COLOR IS RED
	if (firstDocumentCreation)
	{
		firstDocumentCreation = false;
	}
	else
	{
		if (gameRunning)
			mustStopTimersForNewDocument = true;
	}

	return TRUE;
}




// CGameDoc serialization

void CGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << missedScore << catchedScore << count << ballPosition << racketPosition << timerFrequency 
			<< ballColor << firstDocumentCreation << numOfGames << bestScore << gameOver;
	}
	else
	{
		ar >> missedScore >> catchedScore >> count >> ballPosition >> racketPosition >> timerFrequency 
			>> ballColor >> firstDocumentCreation >> numOfGames >> bestScore >> gameOver;
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
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

// Support for Search Handlers
void CGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGameDoc::SetSearchContent(const CString& value)
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

// CGameDoc diagnostics

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameDoc commands

void CGameDoc::setMissedScore()
{
	missedScore++;
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}
int CGameDoc::getMissedScore()
{
	return missedScore;
}

void CGameDoc::setCatchedScore()
{
	catchedScore++;
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}
int CGameDoc::getCatchedScore()
{
	return catchedScore;
}

void CGameDoc::setCount()
{
	count--;
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}
int CGameDoc::getCount()
{
	return count;
}


void CGameDoc::setBallPosition(CPoint newPos)
{
	ballPosition = newPos;
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}
CPoint CGameDoc::getBallPosition()
{
	return ballPosition;
}


void CGameDoc::setRacketPosition(CPoint newPos)
{
	racketPosition = newPos;
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}
CPoint CGameDoc::getRacketPosition()
{
	return racketPosition;
}


void CGameDoc::setTimerFrequency(UINT f)
{
	timerFrequency = f;
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}
UINT CGameDoc::getTimerFrequency()
{
	return timerFrequency;
}


COLORREF CGameDoc::getBallColor()
{
	return ballColor;
}


void CGameDoc::OnBallcolorRed()
{
	ballColor = RGB(150, 0, 25);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}


void CGameDoc::OnUpdateBallcolorRed(CCmdUI *pCmdUI)
{
	// Set Checked if the current color is RED
	pCmdUI->SetCheck(ballColor == RGB(150, 0, 25));
}


void CGameDoc::OnBallcolorBlue()
{
	ballColor = RGB(15, 0, 205);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}


void CGameDoc::OnUpdateBallcolorBlue(CCmdUI *pCmdUI)
{
	// Set Checked if the current color is BLUE
	pCmdUI->SetCheck(ballColor == RGB(15, 0, 205));
}


void CGameDoc::OnBallcolorPurple()
{
	ballColor = RGB(150, 0, 205);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}


void CGameDoc::OnUpdateBallcolorPurple(CCmdUI *pCmdUI)
{
	// Set Checked if the current color is PURPLE
	pCmdUI->SetCheck(ballColor == RGB(150, 0, 205));
}


bool CGameDoc::getMustStopTimersForNewDocument()
{
	return mustStopTimersForNewDocument;
}
void CGameDoc::setMustStopTimersForNewDocument()
{
	mustStopTimersForNewDocument = false;
}

void CGameDoc::setGameRunning(bool answer)
{
	if (answer)
		gameRunning = true;
	else 
		gameRunning = false;
}


void CGameDoc::setNumOfGames()
{
	numOfGames++;
	checkBestScore();
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}


void CGameDoc::checkBestScore()
{
	CString s;    
	if (numOfGames == 1)
	{
		bestScore = catchedScore;
		s.Format(_T("Your new best score is: %d. Get to work."), bestScore);
		AfxMessageBox(s, MB_OK);	
	}
	else if (bestScore < catchedScore)
	{
		s.Format(_T("This is a glorious moment!! Your Best Score was: %d but we will update it for you right away!"), bestScore);		
		AfxMessageBox(s, MB_OK);
		bestScore = catchedScore;
	}
	else if (bestScore > catchedScore)
	{
		s.Format(_T("Your Best Score was: %d. What happened???"), bestScore);
		AfxMessageBox(s, MB_OK);
	}
	else if (bestScore = catchedScore)
	{
		s.Format(_T("You matched your Best Score of %d! But you can do better..."), bestScore);
		AfxMessageBox(s, MB_OK);
	}
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}


void CGameDoc::setGameOver()
{
	gameOver = true;
	SetModifiedFlag(TRUE);
	//UpdateAllViews(NULL);
}
bool CGameDoc::getGameOver()
{
	return gameOver;
}


