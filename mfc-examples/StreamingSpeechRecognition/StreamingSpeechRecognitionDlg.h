
// StreamingSpeechRecognitionDlg.h : header file
//

#pragma once

#include <string>
#include <map>

#include "portaudio.h"
#include "sherpa-onnx/c-api/c-api.h"

class Microphone {
 public:
  Microphone();
  ~Microphone();
};

class RecognizerThread;

// CStreamingSpeechRecognitionDlg dialog
class CStreamingSpeechRecognitionDlg : public CDialogEx {
  // Construction
 public:
  CStreamingSpeechRecognitionDlg(
      CWnd *pParent = nullptr);  // standard constructor
  ~CStreamingSpeechRecognitionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_STREAMINGSPEECHRECOGNITION_DIALOG };
#endif

 protected:
  virtual void DoDataExchange(CDataExchange *pDX);  // DDX/DDV support

  // Implementation
 protected:
  HICON m_hIcon;

  // Generated message map functions
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
 private:
  Microphone mic_;

  const SherpaOnnxOnlineRecognizer *recognizer_ = nullptr;

  int pa_device_ = -1;
  std::map<int, int> idx_to_pa_device;
  PaStream *pa_stream_ = nullptr;
  RecognizerThread *thread_ = nullptr;
  CButton my_btn_;
  CEdit my_text_;
  CComboBox my_combo_devices_;

 public:
  bool started_ = false;
  const SherpaOnnxOnlineStream *stream_ = nullptr;

 public:
  int RunThread();
  afx_msg void OnBnClickedOk();
  afx_msg void OnCbnSelchangeCombo1();

 private:
  void AppendTextToEditCtrl(const std::string &s);
  void AppendLineToMultilineEditCtrl(const std::string &s);
  void InitMicrophone();

  bool Exists(const std::string &filename);
  void InitRecognizer();
  void InitParaformer();
  void ShowInitRecognizerHelpMessage();
};

class RecognizerThread : public CWinThread {
 public:
  RecognizerThread(CStreamingSpeechRecognitionDlg *dlg) : dlg_(dlg) {}
  virtual BOOL InitInstance() { return TRUE; }
  virtual int Run() { return dlg_->RunThread(); }

 private:
  CStreamingSpeechRecognitionDlg *dlg_;
};
