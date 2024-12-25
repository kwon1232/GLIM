
// CircularMeasuringInstrumentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircularMeasuringInstrument.h"
#include "CircularMeasuringInstrumentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
#include <thread>
#include <time.h>


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCircularMeasuringInstrumentDlg 대화 상자



CCircularMeasuringInstrumentDlg::CCircularMeasuringInstrumentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCULARMEASURINGINSTRUMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_num = 0;
}

void CCircularMeasuringInstrumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CENTER_POS, m_CenterPos);
	DDX_Control(pDX, IDC_INTERVAL, m_interval);
}

BEGIN_MESSAGE_MAP(CCircularMeasuringInstrumentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MAKE_CIRCLE_BNT, &CCircularMeasuringInstrumentDlg::OnBnClickedMakeCircle)
	ON_BN_CLICKED(IDC_LOAD_IMAGE_BNT, &CCircularMeasuringInstrumentDlg::OnBnClickedLoadImageBnt)
	ON_BN_CLICKED(IDC_ACTION_BNT, &CCircularMeasuringInstrumentDlg::OnBnClickedActionButton)
END_MESSAGE_MAP()


// CCircularMeasuringInstrumentDlg 메시지 처리기

BOOL CCircularMeasuringInstrumentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	srand((time(NULL)));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCircularMeasuringInstrumentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCircularMeasuringInstrumentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircularMeasuringInstrumentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCircularMeasuringInstrumentDlg::OnBnClickedMakeCircle()
{
	// 멤버 함수 InitMakeCircle을 스레드에서 실행
	std::thread _thread0(&CCircularMeasuringInstrumentDlg::InitMakeCircle, this);

	// 스레드 분리
	_thread0.detach();
}

void CCircularMeasuringInstrumentDlg::InitMakeCircle()
{
	CEdit* countEdit = (CEdit*)GetDlgItem(IDC_SAMPLE_CNT);
	CString str;

	countEdit->GetWindowTextW(str);
	m_Width = 910;
	m_Height = 420;
	int nBpp = 8;

	for (int i = 0; i < _ttoi(str); i++)
	{
		DrawBackground(nBpp);

		MakeCircle();

		CString savePath = _T("C:\\Image\\save_");

		GetNewFileName(savePath);
	}
}

void CCircularMeasuringInstrumentDlg::DrawBackground(int nBpp)
{
	if (m_image.IsNull())
		m_image.Create(m_Width, m_Height, nBpp);

	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 255, rgb);
		UpdateDisplay();
	}
}

void CCircularMeasuringInstrumentDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 10, 50);
}


void CCircularMeasuringInstrumentDlg::GetNewFileName(CString toFilePath)
{
	CString str;
	CString filePath = toFilePath;
	m_num = 0;
	while (true)
	{
		filePath += _T(".bmp");
		if (PathFileExists(filePath)) {
			m_num++;
			str.Format(_T("%d"), m_num);
			filePath.Format(_T("C:\\Image\\save_"));
			filePath += str;
		}
		else {
			m_image.Save(filePath);
			break;
		}
	}

}

void CCircularMeasuringInstrumentDlg::MakeCircle()
{

	// CImage 배경을 검은색으로 초기화
	HDC hdcImage = m_image.GetDC();
	CDC* pImageDC = CDC::FromHandle(hdcImage);
	CBrush blackBrush(RGB(0, 0, 0)); 
	CRect rect(0, 0, m_Width, m_Height);

	pImageDC->FillRect(rect, &blackBrush);

	// 랜덤한 원의 위치 및 반지름 설정
	int nRadius = rand() % 110 + 10;
	int nCircleX = rand() % (m_Width - (nRadius * 2)) + nRadius;
	int nCircleY = rand() % (m_Height - (nRadius * 2)) + nRadius;


	CPen redPen(PS_SOLID, 2, RGB(80, 80, 80));
	CPen* pOldPen = pImageDC->SelectObject(&redPen);

	// 원 내부를 채울 브러시
	CBrush transparentBrush(RGB(80, 80, 80)); // 채우지 않음
	CBrush* pOldBrush = pImageDC->SelectObject(&transparentBrush);


	pImageDC->Ellipse(nCircleX - nRadius, nCircleY - nRadius, nCircleX + nRadius, nCircleY + nRadius);
	redPen.DeleteObject();
	pImageDC->SelectObject(pOldPen);

	UpdateDisplay();

	// 디버깅용
	//CString msg;
	//msg.Format(L"원의 중심 좌표: (%d, %d)\n반지름: %d", nCircleX, nCircleY, nRadius);
	//AfxMessageBox(msg);

	m_image.ReleaseDC();
	Sleep(300);

}

void CCircularMeasuringInstrumentDlg::OnOpenImage()
{
	CString initialDir = _T("C:\\Image");
	CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST, L"Image Files (*.bmp;*.jpg)|*.bmp;*.jpg|All Files (*.*)|*.*||");
	dlg.m_ofn.lpstrInitialDir = initialDir;
	if (dlg.DoModal() != IDOK) return;

	CString selectedFilePath = dlg.GetPathName();

	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}

	HRESULT hr = m_image.Load(selectedFilePath);
	// 디버깅용 코드
	//if (SUCCEEDED(hr))
	//{
	//	AfxMessageBox(L"이미지를 성공적으로 로드했습니다.");
	//}
	//else
	//{
	//	AfxMessageBox(L"이미지 로드에 실패했습니다.");
	//}

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nBpp = m_image.GetBPP(); // 비트 깊이

	if (nBpp != 8 ) // 8비트 컬러만 처리
	{
		AfxMessageBox(L"지원되지 않는 이미지 형식입니다.");
		return;
	}

	unsigned char* pBits = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch(); 

	// 원 경계선의 픽셀 좌표 저장용
	std::vector<CPoint> boundaryPoints;


	// 경계선 탐지 색상 및 허용 오차 설정
	int targetRed = 80, targetGreen = 80, targetBlue = 80; // 회색 기준
	int tolerance = 10; // 허용 오차

	// 이미지 스캔 (전체 픽셀 탐색)
	for (int y = 0; y < nHeight; y++)
	{
		for (int x = 0; x < nWidth; x++)
		{
			int index = y * nPitch + x * (nBpp / 8);

			unsigned char blue = pBits[index];
			unsigned char green = pBits[index + 1];
			unsigned char red = pBits[index + 2];

			// 경계 조건 (색상과 허용 오차 기준)
			if (abs(red - targetRed) <= tolerance &&
				abs(green - targetGreen) <= tolerance &&
				abs(blue - targetBlue) <= tolerance)
			{
				boundaryPoints.push_back(CPoint(x, y));
			}
		}
	}

	if (boundaryPoints.empty())
	{
		AfxMessageBox(L"원을 찾을 수 없습니다.");
		return;
	}

	// 중심 좌표 계산
	double sumX = 0, sumY = 0;
	for (const auto& point : boundaryPoints)
	{
		sumX += point.x;
		sumY += point.y;
	}

	double centerX = sumX / boundaryPoints.size();
	double centerY = sumY / boundaryPoints.size();

	// 최대 반지름 계산
	double maxRadius = 0.0;
	for (const auto& point : boundaryPoints)
	{
		double distance = sqrt(pow(point.x - centerX, 2) + pow(point.y - centerY, 2));
		if (distance > maxRadius)
		{
			maxRadius = distance;
		}
	}
	
	HDC hdcImage = m_image.GetDC();
	CDC* pImageDC = CDC::FromHandle(hdcImage);

	// 중심 좌표에 X 모양 추가 (두 개의 대각선)
	CPen greenPen(PS_SOLID, 5, RGB(255, 0, 255));
	CPen* pOldPen = pImageDC->SelectObject(&greenPen);

	// X 모양: 대각선 그리기
	int offset = 10; // X 모양의 크기
	pImageDC->MoveTo(static_cast<int>(centerX - offset), static_cast<int>(centerY - offset)); // 좌상단에서 시작
	pImageDC->LineTo(static_cast<int>(centerX + offset), static_cast<int>(centerY + offset)); // 우하단으로

	pImageDC->MoveTo(static_cast<int>(centerX - offset), static_cast<int>(centerY + offset)); // 좌하단에서 시작
	pImageDC->LineTo(static_cast<int>(centerX + offset), static_cast<int>(centerY - offset)); // 우상단으로

	pImageDC->SelectObject(pOldPen);
	greenPen.DeleteObject();


	UpdateDisplay();

	// 디버깅용 코드
	//CString msg;
	//msg.Format(L"원의 중심 좌표: (%.0f, %.0f)\n최대 반지름: %.2f", centerX, centerY, maxRadius);
	//AfxMessageBox(msg);

	CString pos;
	pos.Format(L"x : %.0f, y : %.0f", centerX, centerY);
	m_CenterPos.SetWindowTextW(pos);

	DrawCircleAnnotations(pImageDC, centerX, centerY, maxRadius);

	double scale = 1.2;

	int zoomedWidth = static_cast<int>(nWidth * scale);
	int zoomedHeight = static_cast<int>(nHeight * scale);

	double zoomedCenterX = zoomedWidth / 2.0;
	double zoomedCenterY = zoomedHeight / 2.0;

	// 원 중심을 확대된 이미지 중심에 맞추기 위한 오프셋 계산
	double offsetX = zoomedCenterX - (centerX * scale);
	double offsetY = zoomedCenterY - (centerY * scale);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	try
	{
		Gdiplus::Bitmap bitmap(m_image.GetWidth(), m_image.GetHeight(), nPitch, PixelFormat8bppIndexed, pBits);

		Gdiplus::ColorPalette* palette = (Gdiplus::ColorPalette*)malloc(sizeof(Gdiplus::ColorPalette) + 255 * sizeof(Gdiplus::ARGB));
		palette->Count = 256;
		palette->Flags = Gdiplus::PaletteFlagsGrayScale;
		for (int i = 0; i < 256; i++)
		{
			palette->Entries[i] = Gdiplus::Color::MakeARGB(255, i, i, i);
		}
		bitmap.SetPalette(palette);
		free(palette);

		// 확대된 Bitmap 생성
		Gdiplus::Bitmap zoomedBitmap(zoomedWidth, zoomedHeight, PixelFormat8bppIndexed);
		Gdiplus::Graphics graphics(&zoomedBitmap);
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);


		// 확대 영역 설정 및 이미지 렌더링
		Gdiplus::RectF destRect(
			static_cast<float>(offsetX), static_cast<float>(offsetY),
			static_cast<float>(zoomedWidth), static_cast<float>(zoomedHeight)
		);
		graphics.DrawImage(&bitmap, destRect);

		// 확대된 이미지 출력
		Gdiplus::Graphics screenGraphics(hdcImage);
		screenGraphics.DrawImage(&zoomedBitmap, 0, 0);
	}
	catch (...)
	{
		AfxMessageBox(L"Bitmap 생성 중 예외가 발생했습니다.");
	}

	UpdateDisplay();

	Gdiplus::GdiplusShutdown(gdiplusToken);
	m_image.ReleaseDC();

}

void CCircularMeasuringInstrumentDlg::DrawCircleAnnotations(CDC* pImageDC, double centerX, double centerY, double radius)
{
	CPen whiteDashedPen(PS_DOT, 1, RGB(255, 255, 0)); // 점선 펜
	CPen* pOldPen = pImageDC->SelectObject(&whiteDashedPen);

	// X축
	pImageDC->MoveTo(0, static_cast<int>(centerY)); // 화면 왼쪽 끝
	pImageDC->LineTo(m_image.GetWidth(), static_cast<int>(centerY)); // 화면 오른쪽 끝

	// Y축
	pImageDC->MoveTo(static_cast<int>(centerX), 0); // 화면 위쪽
	pImageDC->LineTo(static_cast<int>(centerX), m_image.GetHeight()); // 화면 아래쪽 끝

	CString radiusText;
	radiusText.Format(L"radius = %.2f", radius);

	CFont textFont;
	textFont.CreateFont(
		16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"Arial");

	CFont* pOldFont = pImageDC->SelectObject(&textFont);
	pImageDC->SetTextColor(RGB(255, 255, 255)); 
	pImageDC->SetBkMode(TRANSPARENT);
	pImageDC->TextOutW(static_cast<int>(centerX + radius + 10), static_cast<int>(centerY), radiusText);

	pImageDC->SelectObject(pOldFont);
	pImageDC->SelectObject(pOldPen);

	textFont.DeleteObject();
	whiteDashedPen.DeleteObject();
	pOldPen->DeleteObject();
	pOldFont->DeleteObject();
}

void CCircularMeasuringInstrumentDlg::PerformAction()
{
	if (m_image.IsNull()) return;

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nBpp = m_image.GetBPP();

	unsigned char* pBits = (unsigned char*)m_image.GetBits(); 
	int nPitch = m_image.GetPitch();

	// 원 경계선의 픽셀 좌표 저장용
	std::vector<CPoint> boundaryPoints;

	HDC hdcImage = m_image.GetDC();
	CDC* pImageDC = CDC::FromHandle(hdcImage);


	int targetRed = 80, targetGreen = 80, targetBlue = 80; // 회색 기준
	int tolerance = 10; // 허용 오차

	for (int y = 0; y < nHeight; y++)
	{
		for (int x = 0; x < nWidth; x++)
		{
			int index = y * nPitch + x * (nBpp / 8);

			unsigned char blue = pBits[index];
			unsigned char green = pBits[index + 1];
			unsigned char red = pBits[index + 2];

			// 경계 조건 (색상과 허용 오차 기준)
			if (abs(red - targetRed) <= tolerance &&
				abs(green - targetGreen) <= tolerance &&
				abs(blue - targetBlue) <= tolerance)
			{
				boundaryPoints.push_back(CPoint(x, y));
			}
		}
	}

	// 중심 좌표 계산
	double sumX = 0, sumY = 0;
	for (const auto& point : boundaryPoints)
	{
		sumX += point.x;
		sumY += point.y;
	}

	double centerX = sumX / boundaryPoints.size();
	double centerY = sumY / boundaryPoints.size();

	// 최대 반지름 계산
	double maxRadius = 0.0;
	for (const auto& point : boundaryPoints)
	{
		double distance = sqrt(pow(point.x - centerX, 2) + pow(point.y - centerY, 2));
		if (distance > maxRadius)
		{
			maxRadius = distance;
		}
	}

	double startPosX = centerX, startPosY = centerY;
	int endPosY, endPosX, randomOffset;

	randomOffset = rand() % nWidth + (maxRadius);
	endPosX = randomOffset + (int)startPosX;

	randomOffset = rand() % nHeight + (maxRadius * 2);
	endPosY = randomOffset + (int)startPosY;

	CString strInterval;
	m_interval.GetWindowText(strInterval);

	int step = _ttoi(strInterval);

	int imageIndex = 0;
	for (int x = startPosX, y = startPosY; x <= endPosX && y <= endPosY; x += step, y += step)
	{
		DrawCircleAt(x, y, maxRadius);

		CString savePath;
		while (true)
		{
			savePath.Format(_T("C:\\image\\action\\image_%d.bmp"), imageIndex++);
			if (!PathFileExists(savePath)) {
				m_image.Save(savePath);
				break;
			}
		}

		Sleep(100);
		UpdateDisplay();
	}

}

void CCircularMeasuringInstrumentDlg::DrawCircleAt(int x, int y, int radius)
{
	int nBpp = m_image.GetBPP();


	HDC hdc = m_image.GetDC();
	CDC* pDC = CDC::FromHandle(hdc);


	CBrush backgroundBrush(RGB(0, 0, 0)); 
	CBrush* pOldBrush = pDC->SelectObject(&backgroundBrush);

	CRect rect(0, 0, m_image.GetWidth(), m_image.GetHeight());
	pDC->FillRect(rect, &backgroundBrush);

	CBrush circleBrush(RGB(80, 80, 80)); 
	CPen circlePen(PS_SOLID, 1, RGB(80, 80, 80)); 
	CBrush* pOldCircleBrush = pDC->SelectObject(&circleBrush);
	CPen* pOldCirclePen = pDC->SelectObject(&circlePen);

	pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);

	pDC->SelectObject(pOldCircleBrush);
	pDC->SelectObject(pOldCirclePen);
	pDC->SelectObject(pOldBrush);
	m_image.ReleaseDC();

}

void CCircularMeasuringInstrumentDlg::OnBnClickedLoadImageBnt()
{
	std::thread _thread1(&CCircularMeasuringInstrumentDlg::OnOpenImage, this);
	_thread1.detach();
}


void CCircularMeasuringInstrumentDlg::OnBnClickedActionButton()
{
	std::thread actionThread(&CCircularMeasuringInstrumentDlg::PerformAction, this);
	actionThread.detach();
}
