﻿#pragma once


// InputNameDlg 대화 상자

class InputNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InputNameDlg)

public:
	InputNameDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~InputNameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_NAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
