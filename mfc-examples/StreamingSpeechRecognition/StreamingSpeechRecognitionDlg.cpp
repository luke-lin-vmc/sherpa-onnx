
// StreamingSpeechRecognitionDlg.cpp : implementation file
//
// clang-format off
#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"
// clang-format on

#include "StreamingSpeechRecognitionDlg.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "StreamingSpeechRecognition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Microphone::Microphone() {
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "portaudio error: %s\n", Pa_GetErrorText(err));
    exit(-2);
  }
}

Microphone::~Microphone() {
  PaError err = Pa_Terminate();
  if (err != paNoError) {
    fprintf(stderr, "portaudio error: %s\n", Pa_GetErrorText(err));
    exit(-2);
  }
}

// CStreamingSpeechRecognitionDlg dialog

CStreamingSpeechRecognitionDlg::CStreamingSpeechRecognitionDlg(
    CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_STREAMINGSPEECHRECOGNITION_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStreamingSpeechRecognitionDlg::~CStreamingSpeechRecognitionDlg() {
  if (recognizer_) {
    SherpaOnnxDestroyOnlineRecognizer(recognizer_);
    recognizer_ = nullptr;
  }
}

void CStreamingSpeechRecognitionDlg::DoDataExchange(CDataExchange *pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDOK, my_btn_);
  DDX_Control(pDX, IDC_EDIT1, my_text_);
  DDX_Control(pDX, IDC_COMBO1, my_combo_devices_);
}

BEGIN_MESSAGE_MAP(CStreamingSpeechRecognitionDlg, CDialogEx)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDOK, &CStreamingSpeechRecognitionDlg::OnBnClickedOk)
ON_CBN_SELCHANGE(IDC_COMBO1,
                 &CStreamingSpeechRecognitionDlg::OnCbnSelchangeCombo1)
ON_WM_GETMINMAXINFO()
ON_WM_SIZE()
END_MESSAGE_MAP()

// CStreamingSpeechRecognitionDlg message handlers

BOOL CStreamingSpeechRecognitionDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);   // Set big icon
  SetIcon(m_hIcon, FALSE);  // Set small icon

  // Calculate and keep the margins
  CRect rect, edit_rect;
  GetClientRect(&rect);
  minDiagWidth_ = rect.Width();
  my_text_.GetWindowRect(&edit_rect);
  ScreenToClient(&edit_rect);
  marginEditLeft_ = edit_rect.left - rect.left;
  marginEditTop_ = edit_rect.top - rect.top;
  marginEditRight_ = rect.right - edit_rect.right;
  marginEditBottom_ = rect.bottom - edit_rect.bottom;

  // TODO: Add extra initialization here
  SetWindowText(_T("Real-time speech recogntion with Next-gen Kaldi"));
  InitMicrophone();

  return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStreamingSpeechRecognitionDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this);  // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
                0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user
// drags
//  the minimized window.
HCURSOR CStreamingSpeechRecognitionDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

static int32_t RecordCallback(const void *input_buffer,
                              void * /*output_buffer*/,
                              unsigned long frames_per_buffer,  // NOLINT
                              const PaStreamCallbackTimeInfo * /*time_info*/,
                              PaStreamCallbackFlags /*status_flags*/,
                              void *user_data) {
  auto dlg = reinterpret_cast<CStreamingSpeechRecognitionDlg *>(user_data);

  auto stream = dlg->stream_;
  if (stream) {
    SherpaOnnxOnlineStreamAcceptWaveform(stream, 48000, reinterpret_cast<const float *>(input_buffer),
                   frames_per_buffer);
  }

  return dlg->started_ ? paContinue : paComplete;
}

void CStreamingSpeechRecognitionDlg::OnBnClickedOk() {
  if (!recognizer_) {
    AppendLineToMultilineEditCtrl("Creating recognizer...");
    AppendLineToMultilineEditCtrl("It will take several seconds. Please wait");
    InitRecognizer();
    if (!recognizer_) {
      // failed to create the recognizer
      return;
    }
    AppendLineToMultilineEditCtrl("Recognizer created!");
  }

  if (!started_) {
    started_ = true;

    if (stream_) {
      SherpaOnnxDestroyOnlineStream(stream_);
      stream_ = nullptr;
    }

    stream_ = SherpaOnnxCreateOnlineStream(recognizer_);

    AppendLineToMultilineEditCtrl(std::string("Selected device ") + Pa_GetDeviceInfo(pa_device_)->name);
    PaStreamParameters param;
    param.device = pa_device_;
    const PaDeviceInfo *info = Pa_GetDeviceInfo(param.device);
    param.channelCount = 1;
    param.sampleFormat = paFloat32;
    param.suggestedLatency = info->defaultLowInputLatency;
    param.hostApiSpecificStreamInfo = nullptr;
    float sample_rate = 48000;
    pa_stream_ = nullptr;
    PaError err =
        Pa_OpenStream(&pa_stream_, &param, nullptr, /* &outputParameters, */
                      sample_rate,
                      0,          // frames per buffer
                      paClipOff,  // we won't output out of range samples
                                  // so don't bother clipping them
                      RecordCallback, this);
    if (err != paNoError) {
      AppendLineToMultilineEditCtrl(std::string("PortAudio error: ") +
                                    Pa_GetErrorText(err));
      my_btn_.EnableWindow(FALSE);
      return;
    }

    err = Pa_StartStream(pa_stream_);
    if (err != paNoError) {
      AppendLineToMultilineEditCtrl(std::string("PortAudio error: ") +
                                    Pa_GetErrorText(err));
      my_btn_.EnableWindow(FALSE);
      return;
    }
    AppendLineToMultilineEditCtrl("Started! Please speak");
    my_btn_.SetWindowText(_T("Stop"));

    thread_ = new RecognizerThread(this);
    thread_->CreateThread(CREATE_SUSPENDED);
    thread_->m_bAutoDelete = false;  // Let me delete it.
    thread_->ResumeThread();
  } else {
    started_ = false;
    Pa_Sleep(200);  // sleep for 200ms
    if (pa_stream_) {
      PaError err = Pa_CloseStream(pa_stream_);
      if (err != paNoError) {
        AppendLineToMultilineEditCtrl(std::string("PortAudio error: ") +
                                      Pa_GetErrorText(err));
        my_btn_.EnableWindow(FALSE);
        return;
      }
    }
    pa_stream_ = nullptr;

    WaitForSingleObject(thread_->m_hThread, INFINITE);
    delete thread_;
    thread_ = nullptr;

    // AfxMessageBox("stopped", MB_OK);
    my_btn_.SetWindowText(_T("Start"));
    AppendLineToMultilineEditCtrl("Stopped");
  }
}

static std::wstring Utf8ToUtf16(const std::string &utf8);

void CStreamingSpeechRecognitionDlg::InitMicrophone() {
  int numHostApis = Pa_GetHostApiCount();
  int numDevices = Pa_GetDeviceCount();

  // Find default WASAPI input/output device
  idx_to_pa_device.clear();
  int defaultWasapiInputDevice = -1;
  int defaultWasapiOutputDevice = -1;
  for (int i = 0; i < numHostApis; ++i) {
    const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(i);
    if (apiInfo->type == PaHostApiTypeId::paWASAPI) {
      if (apiInfo->defaultInputDevice != paNoDevice) {
        defaultWasapiInputDevice = apiInfo->defaultInputDevice;
        defaultWasapiOutputDevice = apiInfo->defaultOutputDevice;
        break;
      }
    }
  }
  if (defaultWasapiInputDevice == -1) {
    AppendLineToMultilineEditCtrl("No default input device found!");
    my_btn_.EnableWindow(FALSE);
    return;
  }
  if (defaultWasapiOutputDevice == -1) {
    AppendLineToMultilineEditCtrl("No default output device found!");
    my_btn_.EnableWindow(FALSE);
    return;
  }

  // Find default WASAPI loopback device i.e. default output device with postfix "[Loopback]" 
  int defaultWasapiLoopbackDevice = -1;
  std::string strDefaultOutputDeviceName = std::string(Pa_GetDeviceInfo(defaultWasapiOutputDevice)->name);
  for (int i = 0; i < numDevices; ++i) {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
    const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
    if (apiInfo->type == PaHostApiTypeId::paWASAPI) {
      if (deviceInfo->maxInputChannels > 0) {
        std::string strDeviceName = std::string(deviceInfo->name);
        if (strDeviceName.find(std::string("[Loopback]")) != std::string::npos) {
          if (strDeviceName.find(strDefaultOutputDeviceName) != std::string::npos) {
            defaultWasapiLoopbackDevice = i;
            break;
          }
        }
      }
    }
  }
  if (defaultWasapiLoopbackDevice == -1) {
    AppendLineToMultilineEditCtrl("No default loopback device found!");
    my_btn_.EnableWindow(FALSE);
    return;
  }

  // List all WASAPI input devices
  int numWasapiInputDevices = 0;
  my_combo_devices_.ResetContent();
  idx_to_pa_device[numWasapiInputDevices++] = defaultWasapiInputDevice; // item 0 is the default device
  my_combo_devices_.AddString(_T("Default"));
  idx_to_pa_device[numWasapiInputDevices++] = defaultWasapiLoopbackDevice; // item 1 is the default loopback device
  my_combo_devices_.AddString(_T("Default [Loopback]"));
  // Enumerate all WASAPI input devices
  for (int i = 0; i < numDevices; ++i) {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
    const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
    if (apiInfo->type == PaHostApiTypeId::paWASAPI) {
      if (deviceInfo->maxInputChannels > 0) {
        idx_to_pa_device[numWasapiInputDevices++] = i;
        auto str = Utf8ToUtf16(std::string(deviceInfo->name));
        my_combo_devices_.AddString(str.c_str());
      }
    }
  }

  // Select item 0 as the default device
  my_combo_devices_.SetCurSel(0);
  pa_device_ = idx_to_pa_device[my_combo_devices_.GetCurSel()];
}

bool CStreamingSpeechRecognitionDlg::Exists(const std::string &filename) {
  std::ifstream is(filename);
  return is.good();
}

void CStreamingSpeechRecognitionDlg::ShowInitRecognizerHelpMessage() {
    my_btn_.EnableWindow(FALSE);
    std::string msg =
        "\r\nPlease go to\r\n"
        "https://k2-fsa.github.io/sherpa/onnx/pretrained_models/index.html "
        "\r\n";
    msg += "to download a streaming model, i.e., an online model.\r\n";
    msg += "You need to rename them after downloading\r\n\r\n";
    msg += "It supports both transducer and paraformer models.\r\n\r\n";
    msg +=
      "We give two examples below to show you how to download models\r\n\r\n";
    msg += "(1) Transducer\r\n\r\n";
    msg +=
        "https://huggingface.co/pkufool/"
        "icefall-asr-zipformer-streaming-wenetspeech-20230615";
    msg += "\r\n\r\n";
    msg +=
        "wget https:// "
        "huggingface.co/pkufool/"
        "icefall-asr-zipformer-streaming-wenetspeech-20230615/resolve/main/exp/"
        "encoder-epoch-12-avg-4-chunk-16-left-128.onnx\r\n";
    msg +=
        "wget https:// "
        "huggingface.co/pkufool/"
        "icefall-asr-zipformer-streaming-wenetspeech-20230615/resolve/main/exp/"
        "decoder-epoch-12-avg-4-chunk-16-left-128.onnx\r\n";
    msg +=
        "wget https:// "
        "huggingface.co/pkufool/"
        "icefall-asr-zipformer-streaming-wenetspeech-20230615/resolve/main/exp/"
        "joiner-epoch-12-avg-4-chunk-16-left-128.onnx\r\n";
    msg +=
        "wget "
        "https://huggingface.co/pkufool/"
        "icefall-asr-zipformer-streaming-wenetspeech-20230615/resolve/main/"
        "data/lang_char/tokens.txt\r\n";

    msg += "\r\nNow rename them.\r\n";
    msg += "mv encoder-epoch-12-avg-4-chunk-16-left-128.onnx encoder.onnx\r\n";
    msg += "mv decoder-epoch-12-avg-4-chunk-16-left-128.onnx decoder.onnx\r\n";
    msg += "mv joiner-epoch-12-avg-4-chunk-16-left-128.onnx joiner.onnx\r\n";
    msg += "\r\n";
    msg += "(2) Paraformer\r\n\r\n";
    msg +=
        "wget "
        "https://huggingface.co/csukuangfj/"
        "sherpa-onnx-streaming-paraformer-bilingual-zh-en/resolve/main/"
        "encoder.int8.onnx\r\n";
    msg +=
        "wget "
        "https://huggingface.co/csukuangfj/"
        "sherpa-onnx-streaming-paraformer-bilingual-zh-en/resolve/main/"
        "decoder.int8.onnx\r\n";
    msg +=
        "wget "
        "https://huggingface.co/csukuangfj/"
        "sherpa-onnx-streaming-paraformer-bilingual-zh-en/resolve/main/"
        "tokens.txt\r\n";
    msg += "\r\nNow rename them.\r\n";
    msg += "mv encoder.int8.onnx paraformer-encoder.onnx\r\n";
    msg += "mv decoder.int8.onnx paraformer-decoder.onnx\r\n\r\n";
    msg += "That's it!\r\n";

    AppendLineToMultilineEditCtrl(msg);
}

void CStreamingSpeechRecognitionDlg::InitParaformer() {
  std::string paraformer_encoder = "./paraformer-encoder.onnx";
  std::string paraformer_decoder = "./paraformer-decoder.onnx";

  std::string tokens = "./tokens.txt";

  bool is_ok = true;

  if (Exists("./paraformer-encoder.int8.onnx")) {
    paraformer_encoder = "./paraformer-encoder.int8.onnx";
  } else if (!Exists(paraformer_encoder)) {
    std::string msg = paraformer_encoder + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (Exists("./paraformer-decoder.int8.onnx")) {
    paraformer_decoder = "./paraformer-decoder.int8.onnx";
  } else if (!Exists(paraformer_decoder)) {
    std::string msg = paraformer_decoder + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!Exists(tokens)) {
    std::string msg = tokens + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!is_ok) {
    ShowInitRecognizerHelpMessage();
    return;
  }

  SherpaOnnxOnlineRecognizerConfig config;
  memset(&config, 0, sizeof(config));
  config.model_config.debug = 0;
  config.model_config.num_threads = 1;
  config.model_config.provider = "cpu";

  config.decoding_method = "greedy_search";
  config.max_active_paths = 4;

  config.feat_config.sample_rate = 16000;
  config.feat_config.feature_dim = 80;

  config.enable_endpoint = 1;
  config.rule1_min_trailing_silence = 1.2f;
  config.rule2_min_trailing_silence = 0.8f;
  config.rule3_min_utterance_length = 300.0f;

  config.model_config.tokens = tokens.c_str();
  config.model_config.paraformer.encoder = paraformer_encoder.c_str();
  config.model_config.paraformer.decoder = paraformer_decoder.c_str();

  recognizer_ = SherpaOnnxCreateOnlineRecognizer(&config);
}

void CStreamingSpeechRecognitionDlg::InitRecognizer() {
  if (Exists("./paraformer-encoder.onnx") || Exists("./paraformer-encoder.int8.onnx")) {
    InitParaformer();
    return;
  }

  std::string encoder = "./encoder.onnx";
  std::string decoder = "./decoder.onnx";
  std::string joiner = "./joiner.onnx";
  std::string tokens = "./tokens.txt";

  bool is_ok = true;
  if (!Exists(encoder)) {
    std::string msg = encoder + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!Exists(decoder)) {
    std::string msg = decoder + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!Exists(joiner)) {
    std::string msg = joiner + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!Exists(tokens)) {
    std::string msg = tokens + " does not exist!";
    AppendLineToMultilineEditCtrl(msg);
    is_ok = false;
  }

  if (!is_ok) {
    ShowInitRecognizerHelpMessage();
    return;
  }

  SherpaOnnxOnlineRecognizerConfig config;
  memset(&config, 0, sizeof(config));
  config.model_config.debug = 0;
  config.model_config.num_threads = 1;
  config.model_config.provider = "cpu";

  config.decoding_method = "greedy_search";
  config.max_active_paths = 4;

  config.feat_config.sample_rate = 16000;
  config.feat_config.feature_dim = 80;

  config.enable_endpoint = 1;
  config.rule1_min_trailing_silence = 1.2f;
  config.rule2_min_trailing_silence = 0.8f;
  config.rule3_min_utterance_length = 300.0f;

  config.model_config.tokens = tokens.c_str();
  config.model_config.transducer.encoder = encoder.c_str();
  config.model_config.transducer.decoder = decoder.c_str();
  config.model_config.transducer.joiner = joiner.c_str();

  recognizer_ = SherpaOnnxCreateOnlineRecognizer(&config);
}

// see
// https://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring
static std::wstring Utf8ToUtf16(const std::string &utf8) {
  std::vector<unsigned long> unicode;
  size_t i = 0;
  while (i < utf8.size()) {
    unsigned long uni;
    size_t todo;
    bool error = false;
    unsigned char ch = utf8[i++];
    if (ch <= 0x7F) {
      uni = ch;
      todo = 0;
    } else if (ch <= 0xBF) {
      throw std::logic_error("not a UTF-8 string");
    } else if (ch <= 0xDF) {
      uni = ch & 0x1F;
      todo = 1;
    } else if (ch <= 0xEF) {
      uni = ch & 0x0F;
      todo = 2;
    } else if (ch <= 0xF7) {
      uni = ch & 0x07;
      todo = 3;
    } else {
      throw std::logic_error("not a UTF-8 string");
    }
    for (size_t j = 0; j < todo; ++j) {
      if (i == utf8.size()) throw std::logic_error("not a UTF-8 string");
      unsigned char ch = utf8[i++];
      if (ch < 0x80 || ch > 0xBF) throw std::logic_error("not a UTF-8 string");
      uni <<= 6;
      uni += ch & 0x3F;
    }
    if (uni >= 0xD800 && uni <= 0xDFFF)
      throw std::logic_error("not a UTF-8 string");
    if (uni > 0x10FFFF) throw std::logic_error("not a UTF-8 string");
    unicode.push_back(uni);
  }
  std::wstring utf16;
  for (size_t i = 0; i < unicode.size(); ++i) {
    unsigned long uni = unicode[i];
    if (uni <= 0xFFFF) {
      utf16 += (wchar_t)uni;
    } else {
      uni -= 0x10000;
      utf16 += (wchar_t)((uni >> 10) + 0xD800);
      utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
    }
  }
  return utf16;
}

void CStreamingSpeechRecognitionDlg::AppendTextToEditCtrl(
    const std::string &s) {
  // get the initial text length
  int nLength = my_text_.GetWindowTextLength();
  // put the selection at the end of text
  my_text_.SetSel(nLength, nLength);
  // replace the selection

  std::wstring wstr = Utf8ToUtf16(s);

  // my_text_.ReplaceSel(wstr.c_str());
  my_text_.ReplaceSel(wstr.c_str());
}

void CStreamingSpeechRecognitionDlg::AppendLineToMultilineEditCtrl(
    const std::string &s) {
  AppendTextToEditCtrl("\r\n" + s);
}

static std::string Cat(const std::vector<std::string> &results,
                       const std::string &s) {
  std::ostringstream os;
  std::string sep;

  int i = 0;
  for (i = 0; i != results.size(); ++i) {
    os << sep << i << ": " << results[i];
    sep = "\r\n";
  }

  if (!s.empty()) {
    os << sep << i << ": " << s;
  }
  return os.str();
}

int CStreamingSpeechRecognitionDlg::RunThread() {
  std::vector<std::string> results;

  std::string last_text;
  while (started_) {
    while (SherpaOnnxIsOnlineStreamReady(recognizer_, stream_)) {
      SherpaOnnxDecodeOnlineStream(recognizer_, stream_);
    }

    auto r = SherpaOnnxGetOnlineStreamResult(recognizer_, stream_);
    std::string text = r->text;
    SherpaOnnxDestroyOnlineRecognizerResult(r);
    if (!text.empty() && last_text != text) {
      // CString str;
      // str.Format(_T("%s"), Cat(results, text).c_str());
      auto str = Utf8ToUtf16(Cat(results, text).c_str());
      my_text_.SetWindowText(str.c_str());
      my_text_.SetFocus();
      my_text_.SetSel(-1);
      last_text = text;
    }
    int is_endpoint = SherpaOnnxOnlineStreamIsEndpoint(recognizer_, stream_);
    if (is_endpoint) {
      SherpaOnnxOnlineStreamReset(recognizer_, stream_);
      if (!text.empty()) {
        results.push_back(std::move(text));
      }
    }

    Pa_Sleep(100);  // sleep for 100ms
  }

  return 0;
}

void CStreamingSpeechRecognitionDlg::OnCbnSelchangeCombo1() {
  // TODO: Add your control notification handler code here
  pa_device_ = idx_to_pa_device[my_combo_devices_.GetCurSel()];
}

void CStreamingSpeechRecognitionDlg::OnGetMinMaxInfo(MINMAXINFO *lpMMI) {
  // TODO: Add your message handler code here and/or call default

  // Minimum width and height in pixels
  const int minWidth = minDiagWidth_;
  const int minHeight = marginEditTop_ + 140;  // keep at least 3 lines for display

  lpMMI->ptMinTrackSize.x = minWidth;
  lpMMI->ptMinTrackSize.y = minHeight;

  CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CStreamingSpeechRecognitionDlg::OnSize(UINT nType, int cx, int cy) {
  CDialogEx::OnSize(nType, cx, cy);
  // TODO: Add your message handler code here
  
  // Ensure my_text_ is valid (only after dialog is created)
  if (my_text_.GetSafeHwnd())
  {
    CRect rect;
    GetClientRect(&rect);
    // Resize edit control window to fit dialog client area
    my_text_.MoveWindow(marginEditLeft_, marginEditTop_,
                        rect.Width() - marginEditLeft_ - marginEditRight_,
                        rect.Height() - marginEditTop_ - marginEditBottom_);
  }
}
