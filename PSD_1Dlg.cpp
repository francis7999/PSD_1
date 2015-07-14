
// PSD_1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PSD_1.h"
#include "PSD_1Dlg.h"
#include "afxdialogex.h"
#include "math.h"
#include <iomanip>
#include "fstream"
#include <windows.h>
#define TIME1   1
#define TIME2   2
#define TIME3   3
#define TIME4   4
#define TIME5   5

using namespace std;
#define PI  3.1415926

fstream file;
ofstream file1;
ofstream fout;
ofstream tempfile;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPSD_1Dlg 对话框

CPSD_1Dlg* CPSD_1Dlg::pthis=NULL;

CPSD_1Dlg::CPSD_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPSD_1Dlg::IDD, pParent)
	,m_Ch_x(0)
	,m_Ch_y(0)
	,m_ch_xzl(0)
	,m_ch_yzl(0)
	,m_xAngle(0)
	,m_yAngle(0)
	, angle0(0)
	, angle1(0)
	, angle2(0)
	, h(1.5)
	, yucecanshu(0)
	, ReAngle(0)
	, NoAngle(0)
	, flag(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pthis=this;
}

void CPSD_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XZUOBIAO, m_Ch_x);
	DDX_Text(pDX, IDC_YZUOBIAO, m_Ch_y);
	DDX_Text(pDX, IDC_XPIANYI, m_ch_xzl);
	DDX_Text(pDX, IDC_YPIANYI, m_ch_yzl);
}

BEGIN_MESSAGE_MAP(CPSD_1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ZHILING, &CPSD_1Dlg::OnBnClickedZhiling)
	ON_BN_CLICKED(IDC_KAISHICAIJI, &CPSD_1Dlg::OnBnClickedKaishicaiji)
	ON_BN_CLICKED(IDC_TINGZHICAIJI, &CPSD_1Dlg::OnStop)
	ON_BN_CLICKED(IDC_QUXIAOZHILING, &CPSD_1Dlg::OnBnClickedQuxiaozhiling)
	ON_BN_CLICKED(IDC_FUWEI, &CPSD_1Dlg::OnBnClickedFuwei)
	ON_BN_CLICKED(IDC_LIANJIESHEBEI, &CPSD_1Dlg::OnBnClickedLianjieshebei)
END_MESSAGE_MAP()


// CPSD_1Dlg 消息处理程序

BOOL CPSD_1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	msg_obj_7.id=7;
	msg_obj_7.msg[0]=0x01;              //发送
	msg_obj_7.size=8;
	msg_obj_7.ext_id=false;
	msg_obj_7.rtr=false;

	msg_obj_8.id=7;               //停止发送
	msg_obj_8.msg[0]=0x02;
	msg_obj_8.size=8;
	msg_obj_8.ext_id=false;
	msg_obj_8.rtr=false;

	msg_obj_9.id=7;                       //复位
	msg_obj_9.msg[0]=0x03;
	msg_obj_9.size=8;
	msg_obj_9.ext_id=false;
	msg_obj_9.rtr=false;

	m_bConnect=FALSE;
	m_pPort=new CLawicelCanUsb();
	SaveFlag=0;
	//((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);//选上
	test=0;

	m_iChX=&m_Ch_x; //位置x信息
	m_iChY=&m_Ch_y;//位置y信息
	m_iChXZL=&m_ch_xzl;//x置零
	m_iChYZL=&m_ch_yzl;//y置零

//	CString strTemp;
// 	m_Length=1000;
// 	strTemp.Format(_T("%.0f"),m_Length);
 	//m_Lengh.SetWindowText(strTemp);
	HuiZhiZuoBiaoXi();
//	SetTimer(TIME3,1,NULL);           //这个是用来初始化坐标系的

//	file.open("Inact.txt");
//	SetTimer(TIME4,1,NULL);
	

//	SetTimer(TIME5,1,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPSD_1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPSD_1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPSD_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPSD_1Dlg::HuiZhiZuoBiaoXi()
{

	CWnd *pWnd=GetDlgItem(IDC_STATIC1);
	CDC *pControlDC=pWnd->GetDC();

	CRect rect;
	pWnd->GetClientRect(&rect);

	CDC MemDC;
	CBitmap bitmap;
	StHeight=rect.Height();
	StWidth=rect.Width();
	PerHeight=(float)StHeight/21000;
	PerWeight=(float)StWidth/21000;
	float xFen=(float)StWidth/21;
	float yFen=(float)StHeight/21;
	MemDC.CreateCompatibleDC(pControlDC);
	bitmap.CreateCompatibleBitmap(pControlDC,StWidth,StHeight);
	CBitmap*pBitmap=MemDC.SelectObject(&bitmap);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.FillSolidRect(rect,RGB(255,255,255));

	MemDC.MoveTo(0,0);
	MemDC.LineTo(0,StHeight);
	MemDC.LineTo(StWidth,StHeight);   //坐标系1
	MemDC.LineTo(StWidth,0);
	MemDC.LineTo(0,0);

	MemDC.MoveTo(0,StHeight/2);
	MemDC.LineTo(StWidth,StHeight/2);   //x轴

	MemDC.MoveTo(StWidth/2,0);
	MemDC.LineTo(StWidth/2,StHeight);  //y轴

	CPen pen(PS_SOLID,1,RGB(0,255,0));
	CPen* pOldPen =MemDC.SelectObject(&pen);
	///////////////////////////////////////////////以下是画网格
	
	for (int i=1;i<11;i++)         

	{
		MemDC.MoveTo(0,StHeight/2+i*yFen);
		MemDC.LineTo(StWidth,StHeight/2+i*yFen);     
	}

	for (int i=1;i<11;i++)
	{
		MemDC.MoveTo(StWidth/2+i*xFen,0);
		MemDC.LineTo(StWidth/2+i*xFen,StHeight);
	}

	for (int i=1;i<11;i++)
	{
		MemDC.MoveTo(0,StHeight/2-i*yFen);
		MemDC.LineTo(StWidth,StHeight/2-i*yFen);     
	}

	for (int i=1;i<11;i++)
	{
		MemDC.MoveTo(StWidth/2-i*xFen,0);
		MemDC.LineTo(StWidth/2-i*xFen,StHeight);
	}
	////////////////////////////////////以上是画网格

	CBrush brush,*pOldBrush; 
	brush.CreateSolidBrush(RGB(255,0,0));
	pOldBrush=MemDC.SelectObject(&brush); 

	MemDC.Ellipse((*m_iChX)*PerWeight+StWidth/2-7,StHeight/2-7-(*m_iChY)*PerHeight,(*m_iChX)*PerWeight+StWidth/2+7,StHeight/2+7-(*m_iChY)*PerHeight);//psd的位置点
	MemDC.SelectObject(pOldBrush);
	brush.DeleteObject();


	pControlDC->BitBlt(rect.left,rect.top,StWidth,StHeight,&MemDC,0,0,SRCCOPY);
	MemDC.SelectObject(pBitmap);
	MemDC.DeleteDC();
	bitmap.DeleteObject();

	pWnd->ReleaseDC(pControlDC);

}



void CPSD_1Dlg::OnBnClickedZhiling()
{
	// TODO: 在此添加控件通知处理程序代码
	*m_iChXZL+=*m_iChX;
	*m_iChYZL+=*m_iChY;
}

void CPSD_1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case TID:
		UpdateData(FALSE);
		break;
/*	case TIME1:
		{
			COleDateTime m_odtTime = COleDateTime::GetCurrentTime();//得到当前时间
			CString str=m_odtTime.Format("%H:%M:%S");

			file<<str<<'\t';
			
			
			file<<*m_iChX<<'\t';
			file<<*m_iChY<<'\t';
			
			file<<endl;	
		}
		break;
	case TIME2:
		{
			COleDateTime m_odtTime = COleDateTime::GetCurrentTime();//得到当前时间
			CString str=m_odtTime.Format("%H:%M:%S");

			file1<<str<<'\t';

			file1<<*m_iAngleA<<'\t';
			file1<<*m_iAngleB<<'\t';
			file1<<endl;	
		}
		break;
*/
/*	case TIME3:
		{
			HuiZhiZuoBiaoXi();
		}
		break;
*/
//	case TIME4:
//		{
//			InactWithLabview();
//		}
//		break;
/*	case TIME5:
		{
			vec_x.push_back(*m_iChX);
			vec_y.push_back(*m_iChY);
		}
		break;
*/

	}

	CDialogEx::OnTimer(nIDEvent);
}


void CPSD_1Dlg::OnBnClickedKaishicaiji()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ERROR_CANUSB_OK==m_pPort->Transmit(msg_obj_7))           //向底层传达发送数据命令
	{
		AfxBeginThread(ThreadFun,this);
		SetTimer(TID, 1, NULL);
		//确定多媒体定时器提供的最大和最小定时器事件周期

		timeGetDevCaps(&ts, sizeof(ts));

		wTimerRes = 1;

		//建立最小定时器精度

		timeBeginPeriod(wTimerRes);

		//启动定时器事件，设置定时周期为1ms，分辨率是1毫秒
		
		timerID = timeSetEvent(1,1,TimeProc,NULL,TIME_PERIODIC);
	}
	else
	{
		MessageBox(_T("发送数据失败!"), _T("错误!"), MB_ICONERROR); 

	}
}

void CPSD_1Dlg::OnStop()                    //停止采集
{
	// TODO: 在此添加命令处理程序代码
    m_pPort->Transmit(msg_obj_8);        //向底层传达发送数据命令
// 	m_bConnect = FALSE;
// 	m_pPort->Close();
// 	KillTimer(TID);
	//关闭定时器事件

	timeKillEvent(timerID);

	//清除最小定时器精度

	timeEndPeriod(wTimerRes);
	COleDateTime m_odtTime = COleDateTime::GetCurrentTime();//得到当前时间
	string str1=m_odtTime.Format("%H-%M-%S");
	string str2=m_odtTime.Format("%H:%M:%S");
	fout.open("PSD数据"+str1+".csv");
	fout<<str2<<'\t'
		<<'\n';	
	fout<<"x"<<','
	    <<"y"<<','
		<<'\n';
	for(int i=0;i<vec_x.size();i++)
	{
		fout<<vec_x[i]<<','
	        <<vec_y[i]<<','
		    <<'\n';
	}
	fout.close();
}

UINT CPSD_1Dlg::ThreadFun(LPVOID para)
{
	CPSD_1Dlg* pDlg=( CPSD_1Dlg*)para;
	return pDlg->ThreadFunKernal();

}

int CPSD_1Dlg::ThreadFunKernal()
{
	CanMsgObject msg_obj;
	while(1)                                                                // 读数据，显示
	{
		if (ERROR_CANUSB_OK == m_pPort->Receive(msg_obj))
		{


			if ((msg_obj.id&0x40)==0)
				m_iType=0;      //电压
			else
				m_iType=1;

			for (int i = 0;i<8;i++)
				_msg.ch[i] = msg_obj.msg[i];

			if(_msg.ch[2] & 0x80)
				_msg.ch[3] = 0xFF;
			else
				_msg.ch[3] = 0x00;

			if(_msg.ch[6] & 0x80)
				_msg.ch[7] = 0xFF;
			else
				_msg.ch[7] = 0x00;

			// SetDlgItemText(IDC_MSG_BOX, s);

			// 取通道号
			int iIndex = msg_obj.msg[3] & 0x0F;

			// 将位置信息取出来
			if(m_iType == 1&&iIndex==0)
			{

                *m_iChX=(int)((float)_msg.x[1]/10/0.8275-*m_iChXZL+0.5);
				*m_iChY=(int)((float)_msg.x[0]/10/0.8412-*m_iChYZL+0.5);


				

//				*m_iAngleA=(int)(((float)*m_iChX*324)/(m_Length*PI)+0.5);                  // s=2l*b;          
//				*m_iAngleB=(int)(((float)*m_iChY*324)/(m_Length*PI)+0.5);

			}
		}
	}

	return 0;
}


void CPSD_1Dlg::OnBnClickedQuxiaozhiling()
{
	// TODO: 在此添加控件通知处理程序代码
    *m_iChXZL=0;
	*m_iChYZL=0;

}


void CPSD_1Dlg::OnBnClickedFuwei()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pPort->Transmit(msg_obj_9); 
}


void CPSD_1Dlg::OnBnClickedLianjieshebei()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bConnect=FALSE;
	m_pPort=new CLawicelCanUsb();
	test++;
	if (!m_bConnect)
	{   
		// 连接到适配器
		if (m_pPort->Init(BAUD_1000_KBPS))
		{
			m_bConnect = TRUE; 

		}
		else
		{

			m_bConnect=FALSE;
			m_pPort->Close();
			delete m_pPort;
			MessageBox(_T("打开设备失败!"), _T("错误!"), MB_ICONERROR); 
			//Sleep(500);
			//On32771();
		}
	}
	else
	{
		m_bConnect=FALSE;
		m_pPort->Close();
		delete m_pPort;

		//KillTimer(TID);
	}
	
}

void CPSD_1Dlg::OnDestroy()
{
	m_pPort->Transmit(msg_obj_8); 
	m_bConnect=FALSE;
	m_pPort->Close();
	delete m_pPort;
//	KillTimer(TID);
//	KillTimer(TIME1);
//	KillTimer(TIME2);
//	KillTimer(TIME3);
//	KillTimer(TIME4);
//	KillTimer(TIME5);
	// 	file.close();
	// 	file1.close();
//	file.close();
	//关闭定时器事件

	timeKillEvent(timerID);

	//清除最小定时器精度

	timeEndPeriod(wTimerRes);

	COleDateTime m_odtTime = COleDateTime::GetCurrentTime();//得到当前时间
	string str1=m_odtTime.Format("%H-%M-%S");
	string str2=m_odtTime.Format("%H:%M:%S");
	fout.open("PSD数据"+str1+".csv");
	fout<<str2<<'\t'
		<<'\n';	
	fout<<"x"<<','
	    <<"y"<<','
		<<'\n';
	for(int i=0;i<vec_x.size();i++)
	{
		fout<<vec_x[i]<<','
	        <<vec_y[i]<<','
		    <<'\n';
	}
	fout.close();

	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CALLBACK CPSD_1Dlg::TimeProc(UINT uID,UINT uMsg,DWORD dwUsers,DWORD dw1,DWORD dw2)
{
//	pthis->UpdateData(FALSE);
	pthis->HuiZhiZuoBiaoXi();
	pthis->vec_x.push_back(*pthis->m_iChX);
	pthis->vec_y.push_back(*pthis->m_iChY);
}



void CPSD_1Dlg::InactWithLabview(void)
{
	double tempangle;
	string line;
	string line2;
	if(file.is_open())
	{
		file.seekg(0);
		while(!file.eof())
		{
			getline(file, line, '\n');
			getline(file, line2,'\n');
			istringstream istr(line);
			istringstream istr2(line2);
    		while( !istr.eof() )
    		{
      			istr >> tempangle ;
        		cout << tempangle << '\t' ;		     	
   			}
			while( !istr2.eof() )
			{
				istr2 >> flag ;
        		cout << flag<< endl ;	
			}
		}
		if(flag==1)//角度需要改变
		{
			file.seekg(0);
			angle0=angle1;
			angle1=tempangle;
			ReAngle=atan((*m_iChX+h*tan(2*angle1))/h);
			NoAngle=angle1+ReAngle/2;
			yucecanshu=(NoAngle/2-angle1)/(angle1-angle0);
			angle2=angle1+(NoAngle/2-angle1)/yucecanshu;
			file.seekg(0);
			file<< angle2 <<endl << 0 ;
			angle0=angle1;
			angle1=angle2;
		}
	}
}



