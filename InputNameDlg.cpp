// InputNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "FileManager.h"
#include "InputNameDlg.h"
#include "afxdialogex.h"


// InputNameDlg 대화 상자

IMPLEMENT_DYNAMIC(InputNameDlg, CDialogEx)

InputNameDlg::InputNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_NAME_DLG, pParent)
{

}

InputNameDlg::~InputNameDlg()
{
}

void InputNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InputNameDlg, CDialogEx)
END_MESSAGE_MAP()


// InputNameDlg 메시지 처리기
