
// GameView.cpp : implementation of the CGameView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_COMMAND(ID_DIFFICULTYLEVEL_EASY, &CGameView::OnDifficultylevelEasy)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTYLEVEL_EASY, &CGameView::OnUpdateDifficultylevelEasy)
	ON_COMMAND(ID_DIFFICULTYLEVEL_HARD, &CGameView::OnDifficultylevelHard)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTYLEVEL_HARD, &CGameView::OnUpdateDifficultylevelHard)
	ON_COMMAND(ID_DIFFICULTYLEVEL_MEDIUM, &CGameView::OnDifficultylevelMedium)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTYLEVEL_MEDIUM, &CGameView::OnUpdateDifficultylevelMedium)
	ON_COMMAND(ID_FILE_CLOSEVIEW, &CGameView::OnFileCloseview)
	ON_COMMAND(ID_PAUSEGAME, &CGameView::OnPausegame)
	ON_COMMAND(ID_RESTARTGAME, &CGameView::OnRestartgame)
END_MESSAGE_MAP()

// CGameView construction/destruction

CGameView::CGameView()
{
	stepsOf.cx = 25; 
	stepsOf.cy = -25;
	mouseCaptured = false;
	racket.SetRect(0, 0, 100, 10);	
	isTimerSet = false;
	frequency = -1; // SILLY INITIAL VALUE FOR THE TIMER
	gameMaxed = false;
	close = false;
}
	

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGameView drawing

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!close)
	{
		COLORREF clientColor= RGB(180, 230, 255);
		COLORREF ballColor;
		COLORREF racketColor = RGB(0, 0, 0);

		CRect clientRect;
		GetClientRect (&clientRect);
		pDC->FillSolidRect(clientRect, clientColor);
	
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen); 

		ballColor = pDoc->getBallColor();
		CBrush ballBrush(ballColor);	
		CBrush* pOldBrush = pDC->SelectObject(&ballBrush);

		pDC->SetMapMode (MM_LOENGLISH);

		CSize ballSize(50,50);
		ballStartPoint = pDoc->getBallPosition();
		ballEndPoint.x = ballStartPoint.x + ballSize.cx; 
		ballEndPoint.y = ballStartPoint.y - ballSize.cy;
		pDC->Ellipse(ballStartPoint.x, ballStartPoint.y, ballEndPoint.x, ballEndPoint.y);

		CBrush racketBrush(racketColor);	
		pDC->SelectObject(&racketBrush);	
	
		CSize racketSize(100,10);	
		racketStartPoint = pDoc->getRacketPosition();
		racketEndPoint.x = racketStartPoint.x + racketSize.cx; 
		racketEndPoint.y = racketStartPoint.y - racketSize.cy;	
		racket.SetRect(racketStartPoint.x, racketStartPoint.y, racketEndPoint.x, racketEndPoint.y);
		pDC->Rectangle(racketStartPoint.x, racketStartPoint.y, racketEndPoint.x, racketEndPoint.y);
	
		CFont fontW;
		fontW.CreateFont(30,13,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,0,_T("Arial Black Upper"));	
		pDC->SelectObject(&fontW);
		pDC->SetBkMode(TRANSPARENT);	
		CRect missedScreen;
		CRect catchedScreen;
		missedScreen.SetRect(1000, 0, 1220, -40 );
		catchedScreen.SetRect(20,0, 100, -40 );	
		pDC->DrawText(_T("MISSED"), missedScreen, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		pDC->DrawText(_T("CAUGHT"), catchedScreen, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);

		CFont fontS;
		fontS.CreateFont(110,50,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,0,_T("Trebuchet Ms"));
		pDC->SelectObject(&fontS);
		pDC->SetBkMode(TRANSPARENT);
		CRect screenNumMissed;
		CRect screenNumCatched;
		CRect screenCount;
		screenNumMissed.SetRect(1000,-20, 1220, -125 );
		screenNumCatched.SetRect(18,-20, 100, -125 );
		screenCount.SetRect(540,-15, 700, -100 );
		CString num;
		num.Format(_T("%d"), pDoc->getMissedScore());
		pDC->DrawText(num, screenNumMissed, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		num.Format(_T("%d"), pDoc->getCatchedScore());
		pDC->DrawText(num, screenNumCatched, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		num.Format(_T("%d"), pDoc->getCount());
		CFont fontT;
		fontT.CreateFont(130,60,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,0,_T("Trebuchet Ms"));
		pDC->SelectObject(&fontT);
		pDC->SetBkMode(TRANSPARENT);
		if (pDoc->getCount() <= 10)
		{
			COLORREF dramaticColor= RGB(255, 0, 0);			
			pDC->SetTextColor(dramaticColor);
		}
		pDC->DrawText(num, screenCount, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
	}
	else
		close = false;
}

void CGameView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CGameView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGameView diagnostics

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView message handlers


void CGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGameDoc* pDoc = GetDocument();		
	CSize offset(40,40); // LATERAL STEP SIZE
            
    if (nChar == VK_LEFT)
	{    
		if (racketStartPoint.x >= 0) // SETTING LIMIT FOR LATERAL EXCURSION
		{
			racketStartPoint.x -= offset.cx;
			racketStartPoint.y = -780;
			pDoc->setRacketPosition(racketStartPoint);
			if (!isTimerSet && !pDoc->getGameOver()) // INITIAL SETTING OF TIMER
			{
				frequency = pDoc->getTimerFrequency();
				SetTimer(1, frequency, NULL);	////////////////////////////////////TIMER	
				SetTimer(2, 1000, NULL);
				isTimerSet = true;
				pDoc->setGameRunning(1);
			}
		}
	}                   
 
    if (nChar == VK_RIGHT)  
	{           
		if (racketEndPoint.x <= 1230) // SETTING LIMIT FOR LATERAL EXCURSION
		{
			racketStartPoint.x += offset.cx;
			racketStartPoint.y = -780;
			pDoc->setRacketPosition(racketStartPoint);
			if (!isTimerSet && !pDoc->getGameOver())  // INITIAL SETTING OF TIMER
			{
				frequency = pDoc->getTimerFrequency();
				SetTimer(1, frequency, NULL);	////////////////////////////////////TIMER	
				SetTimer(2, 1000, NULL);
				isTimerSet = true;
				pDoc->setGameRunning(1);
			}
		}	
	}
}


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	mouseCaptured = true;
	CClientDC dc(this);	
	dc.SetMapMode(MM_LOENGLISH);
	dc.DPtoLP(&point);
	CGameDoc* pDoc = GetDocument();
	CPoint newRacketStartPoint(0,0);
	if ((point.x - 50) >=0 && (point.x + 50) <= 1230)
	{
		newRacketStartPoint.x = point.x - 50;
		newRacketStartPoint.y = -780;
		pDoc->setRacketPosition(newRacketStartPoint);
	}
	if (!isTimerSet && !pDoc->getGameOver())  // INITIAL SETTING OF TIMER
		{
			frequency = pDoc->getTimerFrequency();
			SetTimer(1, frequency, NULL);	////////////////////////////////////////////////TIMER	
			SetTimer(2, 1000, NULL);
			isTimerSet = true;
			pDoc->setGameRunning(1);
		}	
}


void CGameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(mouseCaptured)
	{
		ReleaseCapture();
		mouseCaptured = false;	
	}
}


void CGameView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mouseCaptured)
	{	
		CClientDC dc(this);		
		dc.SetMapMode(MM_LOENGLISH);
		CGameDoc* pDoc = GetDocument();
		dc.DPtoLP(&point);		
		CPoint newRacketStartPoint(0,0);
		if ((point.x - 50) >=0 && (point.x + 50) <= 1230) // SETTING LIMIT FOR LATERAL EXCURSION
		{
			newRacketStartPoint.x = point.x - 50;
			newRacketStartPoint.y = -780;
			pDoc->setRacketPosition(newRacketStartPoint);
		}
	}
}


void CGameView::OnTimer(UINT_PTR nIDEvent)
{
	CGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
	if (pDoc->getMustStopTimersForNewDocument()) // CHECK IF A NEW DOCUMENT WAS CREATED AND IF TIMERS WERE ALREADY 
	{	                                         // RUNNING. IF SO, DESTROY TIMERS TO CREATE A STILL NEW GAME
	    KillTimer(1);
		KillTimer(2);
		isTimerSet = false;
		pDoc->setMustStopTimersForNewDocument(); // SET THIS VARIABLE TO FALSE TO CLEAR PASSAGE
	}
	else
	{
		if (nIDEvent == 1) // SERVICE BALL TIMER
		{	
			CClientDC dc (this);
			dc.SetMapMode (MM_LOENGLISH);    
	
			CRect rcClient;
			GetClientRect(&rcClient);	
			dc.DPtoLP(&rcClient);
			CPoint racketTop(0,-760);
			CPoint racketBottom(0, -770);
			CPoint ballMidPoint(ballStartPoint.x + 25, ballStartPoint.y + 25);
			CPoint racketMidPoint(racketStartPoint.x + 50, racketStartPoint.y + 5);
			CSize xSteps(30,0);	
	
			if (ballStartPoint.x <= 0)
			{
				if (stepsOf.cx < 0) // TO AVOID STUCK BALLS
				{
					stepsOf.cx = -stepsOf.cx;  	
				}
			}
			if (ballEndPoint.x >= 1250) 
			{
				if (stepsOf.cx > 0) // TO AVOID STUCK BALLS
				{
					stepsOf.cx = -stepsOf.cx;  	
				}
			}

	
			if (ballStartPoint.y >= 0)
			{
				if (stepsOf.cy > 0) // TO AVOID STUCK BALLS
				{
					stepsOf.cy = -stepsOf.cy;	
				}
			}
			if (ballEndPoint.y <= -880)
			{
				if (stepsOf.cy < 0) // TO AVOID STUCK BALLS
				{
					stepsOf.cy = -stepsOf.cy;	
					pDoc->setMissedScore(); // INCREASE SCORE for MISSED BALLS			
				}
			}
	


			if ((ballEndPoint.x >= racketStartPoint.x) && (ballStartPoint.x <= racketEndPoint.x) && (ballEndPoint.y <= racketTop.y))
			{		
				if (stepsOf.cy < 0) // TO AVOID STUCK BALLS
				{
					pDoc->setCatchedScore(); // INCREASE SCORE for CATCHED BALLS
					stepsOf.cy = -stepsOf.cy;  	
					stepsOf.cx = xSteps.cx * ( (ballMidPoint.x - racketMidPoint.x) / (float)40 ); // ALGORITHM TO COMPLICATE REBOUND
					UINT f = frequency; 
					f -= 5;
					if (f > 10) 
					{
						frequency = f; // SPEEDING UP GAME AT EVERY BOUNCE
						pDoc->setTimerFrequency(frequency);
						frequency = pDoc->getTimerFrequency();
						KillTimer(1);
						SetTimer(1, frequency, NULL);
					}
					else
						gameMaxed = true; // GAME REACHED MAXIMUM SPEED
				}    
			}	

			ballStartPoint.x += stepsOf.cx;
			ballStartPoint.y += stepsOf.cy;
			pDoc->setBallPosition(ballStartPoint);	
		}
		else if(nIDEvent == 2) // SERVICE COUNTDOWN TIMER
		{
			if (pDoc->getCount() > 0)
			{
				pDoc->setCount();				
			}
			else // at end countdown
			{			
				KillTimer(2); //Stop CountDown
				KillTimer(1); //Stop Ball
				isTimerSet = false;
				pDoc->setGameRunning(0);  
				pDoc->setNumOfGames();
				pDoc->setGameOver(); // record end of game
			}
		}
	}
}


void CGameView::OnDifficultylevelEasy()
{
	CGameDoc* pDoc = GetDocument();	
	if (isTimerSet)
	{
		KillTimer(1);
	}
	pDoc->setTimerFrequency(130);
	frequency = pDoc->getTimerFrequency();
	if (isTimerSet)   // IF BALL IS ALREADY MOVING THEN DON"T STOP
		SetTimer(1, frequency, NULL);
}


void CGameView::OnUpdateDifficultylevelEasy(CCmdUI *pCmdUI)
{
	CGameDoc* pDoc = GetDocument();
	// Set Checked if the current setting is EASY
	pCmdUI->SetCheck( pDoc->getTimerFrequency() == 130 );
}


void CGameView::OnDifficultylevelMedium()
{
	CGameDoc* pDoc = GetDocument();	
	if (isTimerSet)
	{
		KillTimer(1);
	}
	pDoc->setTimerFrequency(90);
	frequency = pDoc->getTimerFrequency();
	if (isTimerSet)   // IF BALL IS ALREADY MOVING THEN DON"T STOP
		SetTimer(1, frequency, NULL);
}


void CGameView::OnUpdateDifficultylevelMedium(CCmdUI *pCmdUI)
{
	CGameDoc* pDoc = GetDocument();
	// Set Checked if the current setting is MEDIUM
	pCmdUI->SetCheck( pDoc->getTimerFrequency() == 90);
}


void CGameView::OnDifficultylevelHard()
{
	CGameDoc* pDoc = GetDocument();	
	if (isTimerSet)
	{
		KillTimer(1);
	}
	pDoc->setTimerFrequency(50);
	frequency = pDoc->getTimerFrequency();
	if (isTimerSet)  // IF BALL IS ALREADY MOVING THEN DON"T STOP
		SetTimer(1, frequency, NULL);
}


void CGameView::OnUpdateDifficultylevelHard(CCmdUI *pCmdUI)
{
	CGameDoc* pDoc = GetDocument();
	// Set Checked if the current setting is HARD
	pCmdUI->SetCheck( pDoc->getTimerFrequency() == 50);
}




void CGameView::OnFileCloseview()
{
	CGameDoc* pDoc = GetDocument();
	KillTimer(2); //Stop CountDown
	KillTimer(1); //Stop Ball
	isTimerSet = false;
	pDoc->setGameRunning(0);  
	int choice = 0;	
	choice = AfxMessageBox(_T("Close the document? \nIf yes, the current document will be saved when closing \nthe application or when opening a new file."), MB_OKCANCEL);	
	if (choice == 1)
	{		
		close = true;
		pDoc->UpdateAllViews(NULL); 
	}
	else //IF USER CANCELS OPERATION RESUME GAME
	{
		if (!isTimerSet && !pDoc->getGameOver())
		{
			frequency = pDoc->getTimerFrequency();
			SetTimer(1, frequency, NULL);
			SetTimer(2, 1000, NULL);
			isTimerSet = true;
			pDoc->setGameRunning(1);
		}
	}

	
}


void CGameView::OnPausegame()
{
	CGameDoc* pDoc = GetDocument();
	KillTimer(2); //Stop CountDown
	KillTimer(1); //Stop Ball
	isTimerSet = false;
	pDoc->setGameRunning(0);  
}


void CGameView::OnRestartgame()
{
	CGameDoc* pDoc = GetDocument();
	if (!isTimerSet && !pDoc->getGameOver())
	{
		frequency = pDoc->getTimerFrequency();
		SetTimer(1, frequency, NULL);
		SetTimer(2, 1000, NULL);
		isTimerSet = true;
		pDoc->setGameRunning(1);
	}
}
