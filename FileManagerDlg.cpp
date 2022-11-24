
// FileManagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FileManager.h"
#include "FileManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileManagerDlg 대화 상자



CFileManagerDlg::CFileManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RIGHT_LIST, m_right_list);
	DDX_Control(pDX, IDC_LEFT_LIST, m_left_list);
}

BEGIN_MESSAGE_MAP(CFileManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LEFT_LIST, &CFileManagerDlg::OnLbnDblclkLeftList)
	ON_LBN_DBLCLK(IDC_RIGHT_LIST, &CFileManagerDlg::OnLbnDblclkRightList)
END_MESSAGE_MAP()

void CFileManagerDlg::DirToList(CListBox* ap_list_box, CString a_path)
{
	// 리스트 박스에 있던 기존 목록은 제거하기
	ap_list_box->ResetContent();

	CString name;
	WIN32_FIND_DATA file_data;
	HANDLE h_item_list = FindFirstFile(a_path + L"*.*", &file_data);
	if (h_item_list != INVALID_HANDLE_VALUE)
	{
		do {
			// 리스트에서 "." 디렉토리는 제외하고 싶다면?
			// memcmp(file_data.cFileName, L".", 4);
			// if(!(file_data.cFileName[0] == '.' && file_data.cFileName[1] == 0))
			if (file_data.cFileName[0] != '.' || file_data.cFileName[1])
			{
				name = file_data.cFileName;
				if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) name = L"[" + name + L"]";
				ap_list_box->InsertString(-1, name);
			}

		} while (FindNextFile(h_item_list, &file_data));

		FindClose(h_item_list);
	}
}

// CFileManagerDlg 메시지 처리기

BOOL CFileManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// "c:\"
	wchar_t temp_path[MAX_PATH];
	// "c:\temp" , temp 뒤에 \이 없음.  그래서 + "\" 해야됨
	int len = GetCurrentDirectory(MAX_PATH, temp_path);
	temp_path[len++] = '\\';
	temp_path[len] = 0;

	SetDlgItemText(IDC_L_PATH_EDIT, temp_path);
	SetDlgItemText(IDC_R_PATH_EDIT, temp_path);

	DirToList(&m_left_list, temp_path);
	DirToList(&m_right_list, temp_path);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileManagerDlg::OnPaint()
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
HCURSOR CFileManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileManagerDlg::ChangeDir(CListBox* ap_list_box, int a_path_ctrl_id)
{
	CString str, path;						// '[]' 디렉토리인지 확인하기 위한
	int index = ap_list_box->GetCurSel(); // 어떤 항목을 더블클릭했는지 항목 확인
	ap_list_box->GetText(index, str); // 현재 위치의 text값을 읽어오기
	if (str[0] == '[') // 디렉토리라면,
	{
		GetDlgItemText(a_path_ctrl_id, path);
		str.TrimLeft('[');
		str.TrimRight(']');

		if (str == L"..")
		{
			// ex)  c:\temp\aa\ -> 'c:\temp\aa'
			path.TrimRight('\\');
			int pos = path.ReverseFind('\\');
			// ex)  c:\temp\aa\ -> 'c:\temp\'
			path.Delete(pos + 1, path.GetLength() - pos - 1);
		}
		else
		{
			// ex)  c:\temp + aa -> c:\temp\aa
			path += str;
			// ex)  c:\temp\aa -> 'c:\temp\aa\'
			path += "\\";
		}
		SetDlgItemText(a_path_ctrl_id, path);
		DirToList(ap_list_box, path);
	}
}


void CFileManagerDlg::OnLbnDblclkLeftList()
{
	ChangeDir(&m_left_list, IDC_L_PATH_EDIT);
}


void CFileManagerDlg::OnLbnDblclkRightList()
{
	ChangeDir(&m_right_list, IDC_R_PATH_EDIT);
}
