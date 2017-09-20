
// MergeExpressionDlg.h : header file
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include "afxwin.h"
#include "merger.h"


// CMergeExpressionDlg dialog
class CMergeExpressionDlg : public CDialogEx
{
// Construction
public:
	CMergeExpressionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MERGEEXPRESSION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	Merger m_merger;

public:
	CMFCShellTreeCtrl m_shellTree;
	CMFCShellListCtrl m_shellList;
	CButton m_btnMergeAll;
	CButton m_btnMergeChose;
	CComboBox m_comboxStatus;
	void filterFileDisplay();
		
	afx_msg void OnShellTreeSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMergeChoseFiles();
	afx_msg void OnBnClickedMergeAllFiles();
};
