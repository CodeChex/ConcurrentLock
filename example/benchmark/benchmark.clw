; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBenchmarkDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "benchmark.h"
LastPage=0

ClassCount=4
Class1=CBenchmarkApp
Class2=CAboutDlg
Class3=CBenchmarkDlg
Class4=BenchmarkThreadDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDD_THREAD
Resource3=IDD_BENCHMARK_DIALOG

[CLS:CBenchmarkApp]
Type=0
BaseClass=CWinApp
HeaderFile=benchmark.h
ImplementationFile=benchmark.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=benchmarkDlg.cpp
ImplementationFile=benchmarkDlg.cpp

[CLS:CBenchmarkDlg]
Type=0
BaseClass=CDialog
HeaderFile=benchmarkDlg.h
ImplementationFile=benchmarkDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CBenchmarkDlg

[CLS:BenchmarkThreadDlg]
Type=0
BaseClass=CDialog
HeaderFile=BenchmarkThreadDlg.h
ImplementationFile=BenchmarkThreadDlg.cpp
LastObject=BenchmarkThreadDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_BENCHMARK_DIALOG]
Type=1
Class=CBenchmarkDlg
ControlCount=50
Control1=IDC_STATIC,button,1342308359
Control2=IDC_LOCK_CONCUR,button,1342377993
Control3=IDC_LOCK_MUTEX,button,1342246921
Control4=IDC_LOCK_CRITSECT,button,1342246921
Control5=IDC_STATIC,button,1342308359
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NUM_READ_THREADS,edit,1350762624
Control8=IDC_STATIC,static,1342308352
Control9=IDC_MSECS_R,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_TIMEOUT_R,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_S_TRIES_R,static,1342312450
Control14=IDC_STATIC,static,1342308352
Control15=IDC_S_SUCCESS_R,static,1342312450
Control16=IDC_STATIC,static,1342308352
Control17=IDC_S_FAILED_R,static,1342312450
Control18=IDC_STATIC,static,1342308352
Control19=IDC_S_MIN_R,static,1342312450
Control20=IDC_STATIC,static,1342308352
Control21=IDC_S_AVG_R,static,1342312450
Control22=IDC_STATIC,static,1342308352
Control23=IDC_S_MAX_R,static,1342312450
Control24=IDC_STATIC,button,1342308359
Control25=IDC_STATIC,static,1342308352
Control26=IDC_NUM_WRITE_THREADS,edit,1350762624
Control27=IDC_STATIC,static,1342308352
Control28=IDC_MSECS_W,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_TIMEOUT_W,edit,1350631552
Control31=IDC_STATIC,static,1342308352
Control32=IDC_S_TRIES_W,static,1342312450
Control33=IDC_STATIC,static,1342308352
Control34=IDC_S_SUCCESS_W,static,1342312450
Control35=IDC_STATIC,static,1342308352
Control36=IDC_S_FAILED_W,static,1342312450
Control37=IDC_STATIC,static,1342308352
Control38=IDC_S_MIN_W,static,1342312450
Control39=IDC_STATIC,static,1342308352
Control40=IDC_S_AVG_W,static,1342312450
Control41=IDC_STATIC,static,1342308352
Control42=IDC_S_MAX_W,static,1342312450
Control43=IDC_START,button,1342373888
Control44=IDC_STOP,button,1342242816
Control45=IDC_SHOW_THREADS,button,1476460544
Control46=IDCANCEL,button,1342242816
Control47=IDC_SUCCESS_RATE_R,static,1350701569
Control48=IDC_SUCCESS_RATE_W,static,1350701569
Control49=IDC_STATIC,static,1342308352
Control50=IDC_DURATION,edit,1350631552

[DLG:IDD_THREAD]
Type=1
Class=BenchmarkThreadDlg
ControlCount=15
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST1,SysListView32,1082228737
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_S_TRIES,static,1342312450
Control10=IDC_S_SUCCESS,static,1342312450
Control11=IDC_S_FAILED,static,1342312450
Control12=IDC_S_MIN,static,1342312450
Control13=IDC_S_AVG,static,1342312450
Control14=IDC_S_MAX,static,1342312450
Control15=IDC_DETAILS,button,1342242816

