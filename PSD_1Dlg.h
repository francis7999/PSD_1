
// PSD_1Dlg.h : ͷ�ļ�
//

#pragma once
#include "LawicelCanUsb.h"
#include "afxwin.h"
#include<iostream>
#include<fstream>
#include<string>   
#include <sstream>
#include <vector>
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

// CPSD_1Dlg �Ի���
class CPSD_1Dlg : public CDialogEx
{
// ����
public:
	CPSD_1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PSD_1_DIALOG };
	enum { TID = 0x9822             };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void HuiZhiZuoBiaoXi(); 

public:
	static UINT ThreadFun(LPVOID para);  //�̺߳���
	float StHeight;                          //��̬�ı���ĸ�
	float StWidth;                        //��̬�ı���ĳ�
	CanMsgObject msg_obj_7;             //��ʼ��������
	CanMsgObject msg_obj_8;              //ֹͣ��������
	CanMsgObject msg_obj_9;              //��λ
	float PerHeight;
	float PerWeight;
	CString filename;
	int ti;
	CTime t;
	BOOL SFlag;
	UINT SaveFlag;
	int test;

protected:
	int ThreadFunKernal();                //�߳�ִ�к���
private:
	CLawicelCanUsb* m_pPort;
	BOOL            m_bConnect;

	// ��λ�û��ǵ�ѹ��Ϣ
	int				m_iType;
	float           m_Length;
	int*            m_iChX;
	int*            m_iChY;
	int*            m_iAngleA;
	int*            m_iAngleB;
	int*            m_iChXZL;
	int*            m_iChYZL;
	CWinThread*		m_pThread;

	// ������ȡ����ֵ
	union msgx
	{
		long x[2];
		unsigned char ch[8];
	};
	union msgx _msg;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStop();
	int m_Ch_x;
	int m_Ch_y;
	int m_ch_xzl;
	int m_ch_yzl;
	int m_xAngle;
	int m_yAngle;
	afx_msg void OnDestroy();
	CEdit m_Lengh;
	afx_msg void OnBnClickedZhiling();
	afx_msg void OnBnClickedKaishicaiji();
	afx_msg void OnBnClickedQuxiaozhiling();
	afx_msg void OnBnClickedFuwei();
	afx_msg void OnBnClickedLianjieshebei();
	double angle0;
	double angle1;
	double angle2;
	double h;
	double yucecanshu;
	double ReAngle;
	double NoAngle;
	int flag;
	void InactWithLabview(void);
	vector<int> vec_x;
	vector<int> vec_y;
	static CPSD_1Dlg* pthis;
	static void CALLBACK TimeProc(UINT uID,UINT uMsg,DWORD dwUsers,DWORD dw1,DWORD dw2);
	UINT wTimerRes;
	UINT timerID;
	TIMECAPS ts;
	afx_msg void OnBnClickedCancel();
};
