﻿
// CircularMeasuringInstrumentDlg.h: 헤더 파일
//

#pragma once
#include <random>
#include <mutex>

// CCircularMeasuringInstrumentDlg 대화 상자
class CCircularMeasuringInstrumentDlg : public CDialogEx
{
// 생성입니다.
private:
	CImage m_image;
	int m_num;
	int m_Width = 910;
	int m_Height = 420;

public:
	CCircularMeasuringInstrumentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCULARMEASURINGINSTRUMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	CStatic m_CenterPos;

	std::mutex m_imageMutex;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool CreateFilePathIfNotExists(const CString& filePath);
public:
	afx_msg void OnBnClickedMakeCircle();
	afx_msg void OnBnClickedLoadImageBnt();
	afx_msg void OnBnClickedActionButton();

	bool IsAction = false;
	bool IsLoad = false;

	void InitMakeCircle();

	void DrawBackground(int nBpp);

	void UpdateDisplay();

	// 파일 저장시, 순서매기기
	void GetNewFileName(CString toFilePath);
	void MakeCircle();
	void OnOpenImage();
	void DrawCircleAnnotations(CDC* pImageDC, double centerX, double centerY, double radius);
	void PerformAction();
	void DrawCircleAt(int x, int y, int radius);
	std::random_device rd;       // 시드 생성
	std::mt19937 gen;

	CEdit m_interval;
	std::unique_ptr<CImage> m_ptrImage;
	std::unique_ptr<CImage> m_ptrLoadImage;
};
