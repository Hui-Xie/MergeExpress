
// MergeExpressionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MergeExpression.h"
#include "MergeExpressionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMergeExpressionDlg dialog



CMergeExpressionDlg::CMergeExpressionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMergeExpressionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMergeExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_shellTree);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_shellList);
	DDX_Control(pDX, IDC_BUTTON1, m_btnMergeAll);
	DDX_Control(pDX, IDC_BUTTON2, m_btnMergeChose);
	DDX_Control(pDX, IDC_COMBO1, m_comboxStatus);
}

BEGIN_MESSAGE_MAP(CMergeExpressionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CMergeExpressionDlg::OnShellTreeSelchanged)
	ON_BN_CLICKED(IDC_BUTTON2, &CMergeExpressionDlg::OnBnClickedMergeChoseFiles)
	ON_BN_CLICKED(IDC_BUTTON1, &CMergeExpressionDlg::OnBnClickedMergeAllFiles)
END_MESSAGE_MAP()


// CMergeExpressionDlg message handlers

BOOL CMergeExpressionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//m_shellList.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_shellList.SetItemTypes(SHCONTF_NONFOLDERS);
	int chracterWidth = 8;
	m_shellList.SetColumnWidth(0, 40 * chracterWidth);//Name
	m_shellList.SetColumnWidth(1, 10 * chracterWidth);//Size
	m_shellList.SetColumnWidth(2, 19 * chracterWidth);//type
	m_shellList.SetColumnWidth(3, 23 * chracterWidth);//Modify time

	m_comboxStatus.SetWindowText(_T("Please Choose a directory and then Choose StreamOutput files."));

	ShowWindow(SW_SHOWNORMAL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMergeExpressionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMergeExpressionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMergeExpressionDlg::OnShellTreeSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString strPath;
	if (m_shellTree.GetItemPath(strPath)){
		m_shellList.DisplayFolder(strPath);
		filterFileDisplay();
		m_shellList.Sort(3); //sort by ascending of modifying time;
	}

	*pResult = 0;

}

void CMergeExpressionDlg::filterFileDisplay(){
	CString strFileName;
	CString strFileType;
	//CString const csvExtName(".csv");
	CString const streamOutputName("StreamOutput_");
	CString const acceptedFileType("Comma Separated Values File");
	for (int i = 0; i < m_shellList.GetItemCount(); i++){
		strFileName = m_shellList.GetItemText(i, 0);
		strFileType = m_shellList.GetItemText(i, 2);
		//if (0 != csvExtName.CompareNoCase(strFileName.Right(4))
		//	|| 0 != streamOutputName.CompareNoCase(strFileName.Left(13))){
		if (-1 == strFileType.Find(acceptedFileType)
			|| 0 != streamOutputName.CompareNoCase(strFileName.Left(13))){	
			m_shellList.DeleteItem(i);
			i--;
		}
	}
}


void CMergeExpressionDlg::OnBnClickedMergeChoseFiles()
{
	// TODO: Add your control notification handler code here
	int nFiles = m_shellList.GetSelectedCount();
	if (nFiles <= 1){
		AfxMessageBox(_T("You must select at least 2 files."));
		return;
	}

	m_comboxStatus.SetWindowText(_T("Merging Files, Please Wait ......"));
	
	m_shellList.GetCurrentFolder(m_merger.m_strDir);
	SetCurrentDirectory(m_merger.m_strDir);

	m_merger.m_vectorFiles.clear();
	CString fileName;
	int i = 0;
	int nItem = -1;
	POSITION pos = m_shellList.GetFirstSelectedItemPosition();
	while (i < nFiles){
		nItem = m_shellList.GetNextSelectedItem(pos);
		fileName = m_shellList.GetItemText(nItem, 0);
		m_merger.m_vectorFiles.push_back(fileName);
		i++;
	}

	m_merger.mergeFiles();

	m_comboxStatus.SetWindowText(CString("File Output:  ") + m_merger.m_strDir+ "\\"+ m_merger.m_strMergingFileName);
}


void CMergeExpressionDlg::OnBnClickedMergeAllFiles()
{
	// TODO: Add your control notification handler code here
	int nFiles = m_shellList.GetItemCount();
	if (nFiles <= 1){
		AfxMessageBox(_T("You must have at least 2 files."));
		return;
	}
	m_comboxStatus.SetWindowText(_T("Merging Files, Please Wait ......"));

	m_shellList.GetCurrentFolder(m_merger.m_strDir);
	SetCurrentDirectory(m_merger.m_strDir);

	m_merger.m_vectorFiles.clear();
	CString fileName;
	int i = 0;
	while (i < nFiles){
		fileName = m_shellList.GetItemText(i, 0);
		m_merger.m_vectorFiles.push_back(fileName);
		i++;
	}

	m_merger.mergeFiles();

	m_comboxStatus.SetWindowText(CString("File Output:  ") + m_merger.m_strDir + "\\" + m_merger.m_strMergingFileName);

}
