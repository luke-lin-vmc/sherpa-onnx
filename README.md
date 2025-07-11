## Forked from Sherpa-Onnx
* https://github.com/k2-fsa/sherpa-onnx

## Improvements
* Upgrade [portaudio](https://github.com/PortAudio/portaudio)
* MFC example to enable loopback recording (thru WASAPI hostapi)
* Fixed MFC example project settings. MFC example now can switch debug and release build without rebuilding Sherpa-Onnx libs (see Fixed issues)

![gui](https://github.com/user-attachments/assets/3fe6d807-adc0-404e-b582-23b750318ac9)

## Work Flow
### Git clone
```
git clone https://github.com/luke-lin-vmc/sherpa-onnx
cd sherpa-onnx
mkdir build
cd build
```
### Build Release Libs and Examples
```
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```
### (Optional) Build Debug Libs and Examples
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```
### Build Release MFC example
```
cd ..\mfc-examples
msbuild .\mfc-examples.sln /property:Configuration=Release /property:Platform=x64
(or Visual C++ to open mfc-examples.sln, then build Release solution)
```
###  (Optional) Build Debug MFC example
```
cd ..\mfc-examples
msbuild .\mfc-examples.sln /property:Configuration=Debug /property:Platform=x64
(or Visual C++ to open mfc-examples.sln, then build Debug solution)
```
The `StreamingSpeechRecognition.exe` files will be under `.\x64\Release` or `.\x64\Debug`

### Prepare Zipformer Models
1. Download and decompress models and tokenizer from https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
2. Rename models and tokenizer to `decoder.onnx`, `encoder.onnx`, `joiner.onnx` and `tokens.txt`. Put them along with the `StreamingSpeechRecognition.exe`. An example below.
```
   D:\ASR\Sherpa-onnx\zipformer\>tree /F
   Folder PATH listing for volume OSDisk
   Volume serial number is C2C8-D7B9
   C:.
       decoder.onnx
       encoder.onnx
       joiner.onnx
       StreamingSpeechRecognition.exe
       tokens.txt
```
Note: You may change the model to support different languages. All the zipformer models can be found at https://github.com/k2-fsa/sherpa-onnx/releases/tag/asr-models
This app is a streaming (aka on-line or real-time) pipeline, only the streaming models (model name contains “streaming”, such like sherpa-onnx-streaming-zipformer-en-2023-02-21.tar.bz2) can be used.

### (Optional) Prepare Paraformer Models
1. You may use paraformer models instead of zipformer models
2. Download and decompress models and tokenizer from https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
3. Rename models and tokenizer to `paraformer-decoder.onnx`, `paraformer-encoder.onnx` and `tokens.txt`. Put them along with the `StreamingSpeechRecognition.exe`. An example below.

```
   D:\ASR\Sherpa-onnx\paraformer>tree /F
   Folder PATH listing for volume OSDisk
   Volume serial number is C2C8-D7B9
   C:.
       paraformer-decoder.onnx
       paraformer-encoder.onnx
       StreamingSpeechRecognition.exe
       tokens.txt
```
Note: You may change the model to support different languages. All the paraformer models can be found at https://github.com/k2-fsa/sherpa-onnx/releases/tag/asr-models
This app is a streaming (aka on-line or real-time) pipeline, only the streaming models (model name contains “streaming”, such like sherpa-onnx-streaming-paraformer-trilingual-zh-cantonese-en.tar.bz2
) can be used.

### (Optional) Traditional Chinese Support
Currently the models support Chinese output Simplified Chinese only. You may convert "tokens.txt" to Traditional Chinese to output Traditional Chinese. Several methods can do the conversion. One of the methods is to use MS Word.
https://answers.microsoft.com/en-us/msoffice/forum/all/in-ms-365-word-how-to-convert-traditional-chinese/f46cdf06-f404-429a-86cb-74f4d8bfb114

### Run the program
```
.\x64\Release\StreamingSpeechRecognition.exe
```

## Fixed issues
1. With the original [repo](https://github.com/k2-fsa/sherpa-onnx), you might get `Link Error` while "Build Debug MFC example". This is becasue Debug MFC example needs to link debug "onnxruntime.lib". However, below two CMake scripts put both debug and release "onnxruntime.lib" into the same location. Means "Build Release Libs and Examples" will overwrite the debug "onnxruntime.lib" with release "onnxruntime.lib". The LINK error will happen when Debug MFC example links to release "onnxruntime.lib"
* https://github.com/k2-fsa/sherpa-onnx/blob/master/cmake/onnxruntime-win-x64-static-debug.cmake#L67
* https://github.com/k2-fsa/sherpa-onnx/blob/master/cmake/onnxruntime-win-x86-static.cmake#L63
2. Solution
* Modify the cmake script by copying debug "onnxruntime.lib" to lib/Debug, and copying release "onnxruntime.lib" to lib/Release.
* Make MFC example to link differnet "onnxruntime.lib" per the configuration (Debug/Release)

## Known issues
* PortAudio returns "Invalid device" if using the "Speaker (Synaptics Audio) [Loopback]" device

## Reference
* https://k2-fsa.github.io/sherpa/onnx/install/windows.html#bit-windows-x64
* https://github.com/k2-fsa/sherpa-onnx/tree/v1.11.2/mfc-examples


## ------------- Original README.md -------------------


### Supported functions

|Speech recognition| Speech synthesis |
|------------------|------------------|
|   ✔️              |         ✔️        |

|Speaker identification| Speaker diarization | Speaker verification |
|----------------------|-------------------- |------------------------|
|   ✔️                  |         ✔️           |            ✔️           |

| Spoken Language identification | Audio tagging | Voice activity detection |
|--------------------------------|---------------|--------------------------|
|                 ✔️              |          ✔️    |                ✔️         |

| Keyword spotting | Add punctuation | Speech enhancement |
|------------------|-----------------|--------------------|
|     ✔️            |       ✔️         |      ✔️             |

### Supported platforms

|Architecture| Android | iOS     | Windows    | macOS | linux | HarmonyOS |
|------------|---------|---------|------------|-------|-------|-----------|
|   x64      |  ✔️      |         |   ✔️        | ✔️     |  ✔️    |   ✔️       |
|   x86      |  ✔️      |         |   ✔️        |       |       |           |
|   arm64    |  ✔️      | ✔️       |   ✔️        | ✔️     |  ✔️    |   ✔️       |
|   arm32    |  ✔️      |         |            |       |  ✔️    |   ✔️       |
|   riscv64  |         |         |            |       |  ✔️    |           |

### Supported programming languages

| 1. C++ | 2. C  | 3. Python | 4. JavaScript |
|--------|-------|-----------|---------------|
|   ✔️    | ✔️     | ✔️         |    ✔️          |

|5. Java | 6. C# | 7. Kotlin | 8. Swift |
|--------|-------|-----------|----------|
| ✔️      |  ✔️    | ✔️         |  ✔️       |

| 9. Go | 10. Dart | 11. Rust | 12. Pascal |
|-------|----------|----------|------------|
| ✔️     |  ✔️       |   ✔️      |    ✔️       |

For Rust support, please see [sherpa-rs][sherpa-rs]

It also supports WebAssembly.

## Introduction

This repository supports running the following functions **locally**

  - Speech-to-text (i.e., ASR); both streaming and non-streaming are supported
  - Text-to-speech (i.e., TTS)
  - Speaker diarization
  - Speaker identification
  - Speaker verification
  - Spoken language identification
  - Audio tagging
  - VAD (e.g., [silero-vad][silero-vad])
  - Keyword spotting

on the following platforms and operating systems:

  - x86, ``x86_64``, 32-bit ARM, 64-bit ARM (arm64, aarch64), RISC-V (riscv64), **RK NPU**
  - Linux, macOS, Windows, openKylin
  - Android, WearOS
  - iOS
  - HarmonyOS
  - NodeJS
  - WebAssembly
  - [NVIDIA Jetson Orin NX][NVIDIA Jetson Orin NX] (Support running on both CPU and GPU)
  - [NVIDIA Jetson Nano B01][NVIDIA Jetson Nano B01] (Support running on both CPU and GPU)
  - [Raspberry Pi][Raspberry Pi]
  - [RV1126][RV1126]
  - [LicheePi4A][LicheePi4A]
  - [VisionFive 2][VisionFive 2]
  - [旭日X3派][旭日X3派]
  - [爱芯派][爱芯派]
  - etc

with the following APIs

  - C++, C, Python, Go, ``C#``
  - Java, Kotlin, JavaScript
  - Swift, Rust
  - Dart, Object Pascal

### Links for Huggingface Spaces

<details>
<summary>You can visit the following Huggingface spaces to try sherpa-onnx without
installing anything. All you need is a browser.</summary>

| Description                                           | URL                                     |
|-------------------------------------------------------|-----------------------------------------|
| Speaker diarization                                   | [Click me][hf-space-speaker-diarization]|
| Speech recognition                                    | [Click me][hf-space-asr]                |
| Speech recognition with [Whisper][Whisper]            | [Click me][hf-space-asr-whisper]        |
| Speech synthesis                                      | [Click me][hf-space-tts]                |
| Generate subtitles                                    | [Click me][hf-space-subtitle]           |
| Audio tagging                                         | [Click me][hf-space-audio-tagging]      |
| Spoken language identification with [Whisper][Whisper]| [Click me][hf-space-slid-whisper]       |

We also have spaces built using WebAssembly. They are listed below:

| Description                                                                              | Huggingface space| ModelScope space|
|------------------------------------------------------------------------------------------|------------------|-----------------|
|Voice activity detection with [silero-vad][silero-vad]                                    | [Click me][wasm-hf-vad]|[地址][wasm-ms-vad]|
|Real-time speech recognition (Chinese + English) with Zipformer                           | [Click me][wasm-hf-streaming-asr-zh-en-zipformer]|[地址][wasm-hf-streaming-asr-zh-en-zipformer]|
|Real-time speech recognition (Chinese + English) with Paraformer                          |[Click me][wasm-hf-streaming-asr-zh-en-paraformer]| [地址][wasm-ms-streaming-asr-zh-en-paraformer]|
|Real-time speech recognition (Chinese + English + Cantonese) with [Paraformer-large][Paraformer-large]|[Click me][wasm-hf-streaming-asr-zh-en-yue-paraformer]| [地址][wasm-ms-streaming-asr-zh-en-yue-paraformer]|
|Real-time speech recognition (English) |[Click me][wasm-hf-streaming-asr-en-zipformer]    |[地址][wasm-ms-streaming-asr-en-zipformer]|
|VAD + speech recognition (Chinese + English + Korean + Japanese + Cantonese) with [SenseVoice][SenseVoice]|[Click me][wasm-hf-vad-asr-zh-en-ko-ja-yue-sense-voice]| [地址][wasm-ms-vad-asr-zh-en-ko-ja-yue-sense-voice]|
|VAD + speech recognition (English) with [Whisper][Whisper] tiny.en|[Click me][wasm-hf-vad-asr-en-whisper-tiny-en]| [地址][wasm-ms-vad-asr-en-whisper-tiny-en]|
|VAD + speech recognition (English) with [Moonshine tiny][Moonshine tiny]|[Click me][wasm-hf-vad-asr-en-moonshine-tiny-en]| [地址][wasm-ms-vad-asr-en-moonshine-tiny-en]|
|VAD + speech recognition (English) with Zipformer trained with [GigaSpeech][GigaSpeech]    |[Click me][wasm-hf-vad-asr-en-zipformer-gigaspeech]| [地址][wasm-ms-vad-asr-en-zipformer-gigaspeech]|
|VAD + speech recognition (Chinese) with Zipformer trained with [WenetSpeech][WenetSpeech]  |[Click me][wasm-hf-vad-asr-zh-zipformer-wenetspeech]| [地址][wasm-ms-vad-asr-zh-zipformer-wenetspeech]|
|VAD + speech recognition (Japanese) with Zipformer trained with [ReazonSpeech][ReazonSpeech]|[Click me][wasm-hf-vad-asr-ja-zipformer-reazonspeech]| [地址][wasm-ms-vad-asr-ja-zipformer-reazonspeech]|
|VAD + speech recognition (Thai) with Zipformer trained with [GigaSpeech2][GigaSpeech2]      |[Click me][wasm-hf-vad-asr-th-zipformer-gigaspeech2]| [地址][wasm-ms-vad-asr-th-zipformer-gigaspeech2]|
|VAD + speech recognition (Chinese 多种方言) with a [TeleSpeech-ASR][TeleSpeech-ASR] CTC model|[Click me][wasm-hf-vad-asr-zh-telespeech]| [地址][wasm-ms-vad-asr-zh-telespeech]|
|VAD + speech recognition (English + Chinese, 及多种中文方言) with Paraformer-large          |[Click me][wasm-hf-vad-asr-zh-en-paraformer-large]| [地址][wasm-ms-vad-asr-zh-en-paraformer-large]|
|VAD + speech recognition (English + Chinese, 及多种中文方言) with Paraformer-small          |[Click me][wasm-hf-vad-asr-zh-en-paraformer-small]| [地址][wasm-ms-vad-asr-zh-en-paraformer-small]|
|VAD + speech recognition (多语种及多种中文方言) with [Dolphin][Dolphin]-base          |[Click me][wasm-hf-vad-asr-multi-lang-dolphin-base]| [地址][wasm-ms-vad-asr-multi-lang-dolphin-base]|
|Speech synthesis (English)                                                                  |[Click me][wasm-hf-tts-piper-en]| [地址][wasm-ms-tts-piper-en]|
|Speech synthesis (German)                                                                   |[Click me][wasm-hf-tts-piper-de]| [地址][wasm-ms-tts-piper-de]|
|Speaker diarization                                                                         |[Click me][wasm-hf-speaker-diarization]|[地址][wasm-ms-speaker-diarization]|

</details>

### Links for pre-built Android APKs

<details>

<summary>You can find pre-built Android APKs for this repository in the following table</summary>

| Description                            | URL                                | 中国用户                          |
|----------------------------------------|------------------------------------|-----------------------------------|
| Speaker diarization                    | [Address][apk-speaker-diarization] | [点此][apk-speaker-diarization-cn]|
| Streaming speech recognition           | [Address][apk-streaming-asr]       | [点此][apk-streaming-asr-cn]      |
| Text-to-speech                         | [Address][apk-tts]                 | [点此][apk-tts-cn]                |
| Voice activity detection (VAD)         | [Address][apk-vad]                 | [点此][apk-vad-cn]                |
| VAD + non-streaming speech recognition | [Address][apk-vad-asr]             | [点此][apk-vad-asr-cn]            |
| Two-pass speech recognition            | [Address][apk-2pass]               | [点此][apk-2pass-cn]              |
| Audio tagging                          | [Address][apk-at]                  | [点此][apk-at-cn]                 |
| Audio tagging (WearOS)                 | [Address][apk-at-wearos]           | [点此][apk-at-wearos-cn]          |
| Speaker identification                 | [Address][apk-sid]                 | [点此][apk-sid-cn]                |
| Spoken language identification         | [Address][apk-slid]                | [点此][apk-slid-cn]               |
| Keyword spotting                       | [Address][apk-kws]                 | [点此][apk-kws-cn]                |

</details>

### Links for pre-built Flutter APPs

<details>

#### Real-time speech recognition

| Description                    | URL                                 | 中国用户                            |
|--------------------------------|-------------------------------------|-------------------------------------|
| Streaming speech recognition   | [Address][apk-flutter-streaming-asr]| [点此][apk-flutter-streaming-asr-cn]|

#### Text-to-speech

| Description                              | URL                                | 中国用户                           |
|------------------------------------------|------------------------------------|------------------------------------|
| Android (arm64-v8a, armeabi-v7a, x86_64) | [Address][flutter-tts-android]     | [点此][flutter-tts-android-cn]     |
| Linux (x64)                              | [Address][flutter-tts-linux]       | [点此][flutter-tts-linux-cn]       |
| macOS (x64)                              | [Address][flutter-tts-macos-x64]   | [点此][flutter-tts-macos-arm64-cn] |
| macOS (arm64)                            | [Address][flutter-tts-macos-arm64] | [点此][flutter-tts-macos-x64-cn]   |
| Windows (x64)                            | [Address][flutter-tts-win-x64]     | [点此][flutter-tts-win-x64-cn]     |

> Note: You need to build from source for iOS.

</details>

### Links for pre-built Lazarus APPs

<details>

#### Generating subtitles

| Description                    | URL                        | 中国用户                   |
|--------------------------------|----------------------------|----------------------------|
| Generate subtitles (生成字幕)  | [Address][lazarus-subtitle]| [点此][lazarus-subtitle-cn]|

</details>

### Links for pre-trained models

<details>

| Description                                 | URL                                                                                   |
|---------------------------------------------|---------------------------------------------------------------------------------------|
| Speech recognition (speech to text, ASR)    | [Address][asr-models]                                                                 |
| Text-to-speech (TTS)                        | [Address][tts-models]                                                                 |
| VAD                                         | [Address][vad-models]                                                                 |
| Keyword spotting                            | [Address][kws-models]                                                                 |
| Audio tagging                               | [Address][at-models]                                                                  |
| Speaker identification (Speaker ID)         | [Address][sid-models]                                                                 |
| Spoken language identification (Language ID)| See multi-lingual [Whisper][Whisper] ASR models from  [Speech recognition][asr-models]|
| Punctuation                                 | [Address][punct-models]                                                               |
| Speaker segmentation                        | [Address][speaker-segmentation-models]                                                |
| Speech enhancement                          | [Address][speech-enhancement-models]                                                  |

</details>

#### Some pre-trained ASR models (Streaming)

<details>

Please see

  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-paraformer/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-ctc/index.html>

for more models. The following table lists only **SOME** of them.


|Name | Supported Languages| Description|
|-----|-----|----|
|[sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20][sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20]| Chinese, English| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#csukuangfj-sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20-bilingual-chinese-english)|
|[sherpa-onnx-streaming-zipformer-small-bilingual-zh-en-2023-02-16][sherpa-onnx-streaming-zipformer-small-bilingual-zh-en-2023-02-16]| Chinese, English| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#sherpa-onnx-streaming-zipformer-small-bilingual-zh-en-2023-02-16-bilingual-chinese-english)|
|[sherpa-onnx-streaming-zipformer-zh-14M-2023-02-23][sherpa-onnx-streaming-zipformer-zh-14M-2023-02-23]|Chinese| Suitable for Cortex A7 CPU. See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#sherpa-onnx-streaming-zipformer-zh-14m-2023-02-23)|
|[sherpa-onnx-streaming-zipformer-en-20M-2023-02-17][sherpa-onnx-streaming-zipformer-en-20M-2023-02-17]|English|Suitable for Cortex A7 CPU. See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#sherpa-onnx-streaming-zipformer-en-20m-2023-02-17)|
|[sherpa-onnx-streaming-zipformer-korean-2024-06-16][sherpa-onnx-streaming-zipformer-korean-2024-06-16]|Korean| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#sherpa-onnx-streaming-zipformer-korean-2024-06-16-korean)|
|[sherpa-onnx-streaming-zipformer-fr-2023-04-14][sherpa-onnx-streaming-zipformer-fr-2023-04-14]|French| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#shaojieli-sherpa-onnx-streaming-zipformer-fr-2023-04-14-french)|

</details>


#### Some pre-trained ASR models (Non-Streaming)

<details>

Please see

  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-paraformer/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-ctc/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/telespeech/index.html>
  - <https://k2-fsa.github.io/sherpa/onnx/pretrained_models/whisper/index.html>

for more models. The following table lists only **SOME** of them.

|Name | Supported Languages| Description|
|-----|-----|----|
|[Whisper tiny.en](https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-whisper-tiny.en.tar.bz2)|English| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/whisper/tiny.en.html)|
|[Moonshine tiny][Moonshine tiny]|English|See [also](https://github.com/usefulsensors/moonshine)|
|[sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17][sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17]|Chinese, Cantonese, English, Korean, Japanese| 支持多种中文方言. See [also](https://k2-fsa.github.io/sherpa/onnx/sense-voice/index.html)|
|[sherpa-onnx-paraformer-zh-2024-03-09][sherpa-onnx-paraformer-zh-2024-03-09]|Chinese, English| 也支持多种中文方言. See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-paraformer/paraformer-models.html#csukuangfj-sherpa-onnx-paraformer-zh-2024-03-09-chinese-english)|
|[sherpa-onnx-zipformer-ja-reazonspeech-2024-08-01][sherpa-onnx-zipformer-ja-reazonspeech-2024-08-01]|Japanese|See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#sherpa-onnx-zipformer-ja-reazonspeech-2024-08-01-japanese)|
|[sherpa-onnx-nemo-transducer-giga-am-russian-2024-10-24][sherpa-onnx-nemo-transducer-giga-am-russian-2024-10-24]|Russian|See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/nemo-transducer-models.html#sherpa-onnx-nemo-transducer-giga-am-russian-2024-10-24-russian)|
|[sherpa-onnx-nemo-ctc-giga-am-russian-2024-10-24][sherpa-onnx-nemo-ctc-giga-am-russian-2024-10-24]|Russian| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-ctc/nemo/russian.html#sherpa-onnx-nemo-ctc-giga-am-russian-2024-10-24)|
|[sherpa-onnx-zipformer-ru-2024-09-18][sherpa-onnx-zipformer-ru-2024-09-18]|Russian|See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#sherpa-onnx-zipformer-ru-2024-09-18-russian)|
|[sherpa-onnx-zipformer-korean-2024-06-24][sherpa-onnx-zipformer-korean-2024-06-24]|Korean|See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#sherpa-onnx-zipformer-korean-2024-06-24-korean)|
|[sherpa-onnx-zipformer-thai-2024-06-20][sherpa-onnx-zipformer-thai-2024-06-20]|Thai| See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#sherpa-onnx-zipformer-thai-2024-06-20-thai)|
|[sherpa-onnx-telespeech-ctc-int8-zh-2024-06-04][sherpa-onnx-telespeech-ctc-int8-zh-2024-06-04]|Chinese| 支持多种方言. See [also](https://k2-fsa.github.io/sherpa/onnx/pretrained_models/telespeech/models.html#sherpa-onnx-telespeech-ctc-int8-zh-2024-06-04)|

</details>

### Useful links

- Documentation: https://k2-fsa.github.io/sherpa/onnx/
- Bilibili 演示视频: https://search.bilibili.com/all?keyword=%E6%96%B0%E4%B8%80%E4%BB%A3Kaldi

### How to reach us

Please see
https://k2-fsa.github.io/sherpa/social-groups.html
for 新一代 Kaldi **微信交流群** and **QQ 交流群**.

## Projects using sherpa-onnx

### [Open-LLM-VTuber](https://github.com/t41372/Open-LLM-VTuber)

Talk to any LLM with hands-free voice interaction, voice interruption, and Live2D taking
face running locally across platforms

See also <https://github.com/t41372/Open-LLM-VTuber/pull/50>

### [voiceapi](https://github.com/ruzhila/voiceapi)

<details>
  <summary>Streaming ASR and TTS based on FastAPI</summary>


It shows how to use the ASR and TTS Python APIs with FastAPI.
</details>

### [腾讯会议摸鱼工具 TMSpeech](https://github.com/jxlpzqc/TMSpeech)

Uses streaming ASR in C# with graphical user interface.

Video demo in Chinese: [【开源】Windows实时字幕软件（网课/开会必备）](https://www.bilibili.com/video/BV1rX4y1p7Nx)

### [lol互动助手](https://github.com/l1veIn/lol-wom-electron)

It uses the JavaScript API of sherpa-onnx along with [Electron](https://electronjs.org/)

Video demo in Chinese: [爆了！炫神教你开打字挂！真正影响胜率的英雄联盟工具！英雄联盟的最后一块拼图！和游戏中的每个人无障碍沟通！](https://www.bilibili.com/video/BV142tje9E74)

### [Sherpa-ONNX 语音识别服务器](https://github.com/hfyydd/sherpa-onnx-server)

A server based on nodejs providing Restful API for speech recognition.

### [QSmartAssistant](https://github.com/xinhecuican/QSmartAssistant)

一个模块化，全过程可离线，低占用率的对话机器人/智能音箱

It uses QT. Both [ASR](https://github.com/xinhecuican/QSmartAssistant/blob/master/doc/%E5%AE%89%E8%A3%85.md#asr)
and [TTS](https://github.com/xinhecuican/QSmartAssistant/blob/master/doc/%E5%AE%89%E8%A3%85.md#tts)
are used.

### [Flutter-EasySpeechRecognition](https://github.com/Jason-chen-coder/Flutter-EasySpeechRecognition)

It extends [./flutter-examples/streaming_asr](./flutter-examples/streaming_asr) by
downloading models inside the app to reduce the size of the app.

Note: [[Team B] Sherpa AI backend](https://github.com/umgc/spring2025/pull/82) also uses
sherpa-onnx in a Flutter APP.

### [sherpa-onnx-unity](https://github.com/xue-fei/sherpa-onnx-unity)

sherpa-onnx in Unity. See also [#1695](https://github.com/k2-fsa/sherpa-onnx/issues/1695),
[#1892](https://github.com/k2-fsa/sherpa-onnx/issues/1892), and [#1859](https://github.com/k2-fsa/sherpa-onnx/issues/1859)

### [xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server)

本项目为xiaozhi-esp32提供后端服务，帮助您快速搭建ESP32设备控制服务器
Backend service for xiaozhi-esp32, helps you quickly build an ESP32 device control server.

See also

  - [ASR新增轻量级sherpa-onnx-asr](https://github.com/xinnan-tech/xiaozhi-esp32-server/issues/315)
  - [feat: ASR增加sherpa-onnx模型](https://github.com/xinnan-tech/xiaozhi-esp32-server/pull/379)

### [KaithemAutomation](https://github.com/EternityForest/KaithemAutomation)

Pure Python, GUI-focused home automation/consumer grade SCADA.

It uses TTS from sherpa-onnx. See also [✨ Speak command that uses the new globally configured TTS model.](https://github.com/EternityForest/KaithemAutomation/commit/8e64d2b138725e426532f7d66bb69dd0b4f53693)

### [Open-XiaoAI KWS](https://github.com/idootop/open-xiaoai-kws)

Enable custom wake word for XiaoAi Speakers. 让小爱音箱支持自定义唤醒词。

Video demo in Chinese: [小爱同学启动～˶╹ꇴ╹˶！](https://www.bilibili.com/video/BV1YfVUz5EMj)

### [Making robot Paimon, Ep10 "The AI Part 1"](https://www.youtube.com/watch?v=KxPKkwxGWZs)

It is a [YouTube video](https://www.youtube.com/watch?v=KxPKkwxGWZs),
showing how the author tried to use AI so he can have a conversation with Paimon.

It uses sherpa-onnx for speech-to-text and text-to-speech.
![](https://private-user-images.githubusercontent.com/5284924/442579664-f6eea2d5-1807-42cb-9160-be8da2971e1f.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NDcwMjI5NjIsIm5iZiI6MTc0NzAyMjY2MiwicGF0aCI6Ii81Mjg0OTI0LzQ0MjU3OTY2NC1mNmVlYTJkNS0xODA3LTQyY2ItOTE2MC1iZThkYTI5NzFlMWYucG5nP1gtQW16LUFsZ29yaXRobT1BV1M0LUhNQUMtU0hBMjU2JlgtQW16LUNyZWRlbnRpYWw9QUtJQVZDT0RZTFNBNTNQUUs0WkElMkYyMDI1MDUxMiUyRnVzLWVhc3QtMSUyRnMzJTJGYXdzNF9yZXF1ZXN0JlgtQW16LURhdGU9MjAyNTA1MTJUMDQwNDIyWiZYLUFtei1FeHBpcmVzPTMwMCZYLUFtei1TaWduYXR1cmU9ODAyMDllYjNmMGU0ZjNlODI3N2RkZjdiYjU2MmY4YzQwNGJjZjM4MGZmNjk0OWRmNmYzMjJhYWIzMDRiMWNiYSZYLUFtei1TaWduZWRIZWFkZXJzPWhvc3QifQ.WP-v8oKPqQ-s487-eUU2Z4quOfWbwDJQJok89NfkTf4)

[sherpa-rs]: https://github.com/thewh1teagle/sherpa-rs
[silero-vad]: https://github.com/snakers4/silero-vad
[Raspberry Pi]: https://www.raspberrypi.com/
[RV1126]: https://www.rock-chips.com/uploads/pdf/2022.8.26/191/RV1126%20Brief%20Datasheet.pdf
[LicheePi4A]: https://sipeed.com/licheepi4a
[VisionFive 2]: https://www.starfivetech.com/en/site/boards
[旭日X3派]: https://developer.horizon.ai/api/v1/fileData/documents_pi/index.html
[爱芯派]: https://wiki.sipeed.com/hardware/zh/maixIII/ax-pi/axpi.html
[hf-space-speaker-diarization]: https://huggingface.co/spaces/k2-fsa/speaker-diarization
[hf-space-asr]: https://huggingface.co/spaces/k2-fsa/automatic-speech-recognition
[Whisper]: https://github.com/openai/whisper
[hf-space-asr-whisper]: https://huggingface.co/spaces/k2-fsa/automatic-speech-recognition-with-whisper
[hf-space-tts]: https://huggingface.co/spaces/k2-fsa/text-to-speech
[hf-space-subtitle]: https://huggingface.co/spaces/k2-fsa/generate-subtitles-for-videos
[hf-space-audio-tagging]: https://huggingface.co/spaces/k2-fsa/audio-tagging
[hf-space-slid-whisper]: https://huggingface.co/spaces/k2-fsa/spoken-language-identification
[wasm-hf-vad]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-sherpa-onnx
[wasm-ms-vad]: https://modelscope.cn/studios/csukuangfj/web-assembly-vad-sherpa-onnx
[wasm-hf-streaming-asr-zh-en-zipformer]: https://huggingface.co/spaces/k2-fsa/web-assembly-asr-sherpa-onnx-zh-en
[wasm-ms-streaming-asr-zh-en-zipformer]: https://modelscope.cn/studios/k2-fsa/web-assembly-asr-sherpa-onnx-zh-en
[wasm-hf-streaming-asr-zh-en-paraformer]: https://huggingface.co/spaces/k2-fsa/web-assembly-asr-sherpa-onnx-zh-en-paraformer
[wasm-ms-streaming-asr-zh-en-paraformer]: https://modelscope.cn/studios/k2-fsa/web-assembly-asr-sherpa-onnx-zh-en-paraformer
[Paraformer-large]: https://www.modelscope.cn/models/damo/speech_paraformer-large_asr_nat-zh-cn-16k-common-vocab8404-pytorch/summary
[wasm-hf-streaming-asr-zh-en-yue-paraformer]: https://huggingface.co/spaces/k2-fsa/web-assembly-asr-sherpa-onnx-zh-cantonese-en-paraformer
[wasm-ms-streaming-asr-zh-en-yue-paraformer]: https://modelscope.cn/studios/k2-fsa/web-assembly-asr-sherpa-onnx-zh-cantonese-en-paraformer
[wasm-hf-streaming-asr-en-zipformer]: https://huggingface.co/spaces/k2-fsa/web-assembly-asr-sherpa-onnx-en
[wasm-ms-streaming-asr-en-zipformer]: https://modelscope.cn/studios/k2-fsa/web-assembly-asr-sherpa-onnx-en
[SenseVoice]: https://github.com/FunAudioLLM/SenseVoice
[wasm-hf-vad-asr-zh-en-ko-ja-yue-sense-voice]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-en-ja-ko-cantonese-sense-voice
[wasm-ms-vad-asr-zh-en-ko-ja-yue-sense-voice]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-zh-en-jp-ko-cantonese-sense-voice
[wasm-hf-vad-asr-en-whisper-tiny-en]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-en-whisper-tiny
[wasm-ms-vad-asr-en-whisper-tiny-en]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-en-whisper-tiny
[wasm-hf-vad-asr-en-moonshine-tiny-en]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-en-moonshine-tiny
[wasm-ms-vad-asr-en-moonshine-tiny-en]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-en-moonshine-tiny
[wasm-hf-vad-asr-en-zipformer-gigaspeech]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-en-zipformer-gigaspeech
[wasm-ms-vad-asr-en-zipformer-gigaspeech]: https://www.modelscope.cn/studios/k2-fsa/web-assembly-vad-asr-sherpa-onnx-en-zipformer-gigaspeech
[wasm-hf-vad-asr-zh-zipformer-wenetspeech]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-zipformer-wenetspeech
[wasm-ms-vad-asr-zh-zipformer-wenetspeech]: https://www.modelscope.cn/studios/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-zipformer-wenetspeech
[ReazonSpeech]: https://research.reazon.jp/_static/reazonspeech_nlp2023.pdf
[wasm-hf-vad-asr-ja-zipformer-reazonspeech]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-ja-zipformer
[wasm-ms-vad-asr-ja-zipformer-reazonspeech]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-ja-zipformer
[GigaSpeech2]: https://github.com/SpeechColab/GigaSpeech2
[wasm-hf-vad-asr-th-zipformer-gigaspeech2]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-th-zipformer
[wasm-ms-vad-asr-th-zipformer-gigaspeech2]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-th-zipformer
[TeleSpeech-ASR]: https://github.com/Tele-AI/TeleSpeech-ASR
[wasm-hf-vad-asr-zh-telespeech]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-telespeech
[wasm-ms-vad-asr-zh-telespeech]: https://www.modelscope.cn/studios/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-telespeech
[wasm-hf-vad-asr-zh-en-paraformer-large]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-en-paraformer
[wasm-ms-vad-asr-zh-en-paraformer-large]: https://www.modelscope.cn/studios/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-en-paraformer
[wasm-hf-vad-asr-zh-en-paraformer-small]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-en-paraformer-small
[wasm-ms-vad-asr-zh-en-paraformer-small]: https://www.modelscope.cn/studios/k2-fsa/web-assembly-vad-asr-sherpa-onnx-zh-en-paraformer-small
[Dolphin]: https://github.com/DataoceanAI/Dolphin
[wasm-ms-vad-asr-multi-lang-dolphin-base]: https://modelscope.cn/studios/csukuangfj/web-assembly-vad-asr-sherpa-onnx-multi-lang-dophin-ctc
[wasm-hf-vad-asr-multi-lang-dolphin-base]: https://huggingface.co/spaces/k2-fsa/web-assembly-vad-asr-sherpa-onnx-multi-lang-dophin-ctc

[wasm-hf-tts-piper-en]: https://huggingface.co/spaces/k2-fsa/web-assembly-tts-sherpa-onnx-en
[wasm-ms-tts-piper-en]: https://modelscope.cn/studios/k2-fsa/web-assembly-tts-sherpa-onnx-en
[wasm-hf-tts-piper-de]: https://huggingface.co/spaces/k2-fsa/web-assembly-tts-sherpa-onnx-de
[wasm-ms-tts-piper-de]: https://modelscope.cn/studios/k2-fsa/web-assembly-tts-sherpa-onnx-de
[wasm-hf-speaker-diarization]: https://huggingface.co/spaces/k2-fsa/web-assembly-speaker-diarization-sherpa-onnx
[wasm-ms-speaker-diarization]: https://www.modelscope.cn/studios/csukuangfj/web-assembly-speaker-diarization-sherpa-onnx
[apk-speaker-diarization]: https://k2-fsa.github.io/sherpa/onnx/speaker-diarization/apk.html
[apk-speaker-diarization-cn]: https://k2-fsa.github.io/sherpa/onnx/speaker-diarization/apk-cn.html
[apk-streaming-asr]: https://k2-fsa.github.io/sherpa/onnx/android/apk.html
[apk-streaming-asr-cn]: https://k2-fsa.github.io/sherpa/onnx/android/apk-cn.html
[apk-tts]: https://k2-fsa.github.io/sherpa/onnx/tts/apk-engine.html
[apk-tts-cn]: https://k2-fsa.github.io/sherpa/onnx/tts/apk-engine-cn.html
[apk-vad]: https://k2-fsa.github.io/sherpa/onnx/vad/apk.html
[apk-vad-cn]: https://k2-fsa.github.io/sherpa/onnx/vad/apk-cn.html
[apk-vad-asr]: https://k2-fsa.github.io/sherpa/onnx/vad/apk-asr.html
[apk-vad-asr-cn]: https://k2-fsa.github.io/sherpa/onnx/vad/apk-asr-cn.html
[apk-2pass]: https://k2-fsa.github.io/sherpa/onnx/android/apk-2pass.html
[apk-2pass-cn]: https://k2-fsa.github.io/sherpa/onnx/android/apk-2pass-cn.html
[apk-at]: https://k2-fsa.github.io/sherpa/onnx/audio-tagging/apk.html
[apk-at-cn]: https://k2-fsa.github.io/sherpa/onnx/audio-tagging/apk-cn.html
[apk-at-wearos]: https://k2-fsa.github.io/sherpa/onnx/audio-tagging/apk-wearos.html
[apk-at-wearos-cn]: https://k2-fsa.github.io/sherpa/onnx/audio-tagging/apk-wearos-cn.html
[apk-sid]: https://k2-fsa.github.io/sherpa/onnx/speaker-identification/apk.html
[apk-sid-cn]: https://k2-fsa.github.io/sherpa/onnx/speaker-identification/apk-cn.html
[apk-slid]: https://k2-fsa.github.io/sherpa/onnx/spoken-language-identification/apk.html
[apk-slid-cn]: https://k2-fsa.github.io/sherpa/onnx/spoken-language-identification/apk-cn.html
[apk-kws]: https://k2-fsa.github.io/sherpa/onnx/kws/apk.html
[apk-kws-cn]: https://k2-fsa.github.io/sherpa/onnx/kws/apk-cn.html
[apk-flutter-streaming-asr]: https://k2-fsa.github.io/sherpa/onnx/flutter/asr/app.html
[apk-flutter-streaming-asr-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/asr/app-cn.html
[flutter-tts-android]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-android.html
[flutter-tts-android-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-android-cn.html
[flutter-tts-linux]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-linux.html
[flutter-tts-linux-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-linux-cn.html
[flutter-tts-macos-x64]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-macos-x64.html
[flutter-tts-macos-arm64-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-macos-x64-cn.html
[flutter-tts-macos-arm64]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-macos-arm64.html
[flutter-tts-macos-x64-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-macos-arm64-cn.html
[flutter-tts-win-x64]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-win.html
[flutter-tts-win-x64-cn]: https://k2-fsa.github.io/sherpa/onnx/flutter/tts-win-cn.html
[lazarus-subtitle]: https://k2-fsa.github.io/sherpa/onnx/lazarus/download-generated-subtitles.html
[lazarus-subtitle-cn]: https://k2-fsa.github.io/sherpa/onnx/lazarus/download-generated-subtitles-cn.html
[asr-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/asr-models
[tts-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/tts-models
[vad-models]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/silero_vad.onnx
[kws-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/kws-models
[at-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/audio-tagging-models
[sid-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/speaker-recongition-models
[slid-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/speaker-recongition-models
[punct-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/punctuation-models
[speaker-segmentation-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/speaker-segmentation-models
[GigaSpeech]: https://github.com/SpeechColab/GigaSpeech
[WenetSpeech]: https://github.com/wenet-e2e/WenetSpeech
[sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
[sherpa-onnx-streaming-zipformer-small-bilingual-zh-en-2023-02-16]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-small-bilingual-zh-en-2023-02-16.tar.bz2
[sherpa-onnx-streaming-zipformer-korean-2024-06-16]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-korean-2024-06-16.tar.bz2
[sherpa-onnx-streaming-zipformer-zh-14M-2023-02-23]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-zh-14M-2023-02-23.tar.bz2
[sherpa-onnx-streaming-zipformer-en-20M-2023-02-17]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-en-20M-2023-02-17.tar.bz2
[sherpa-onnx-zipformer-ja-reazonspeech-2024-08-01]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-zipformer-ja-reazonspeech-2024-08-01.tar.bz2
[sherpa-onnx-zipformer-ru-2024-09-18]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-zipformer-ru-2024-09-18.tar.bz2
[sherpa-onnx-zipformer-korean-2024-06-24]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-zipformer-korean-2024-06-24.tar.bz2
[sherpa-onnx-zipformer-thai-2024-06-20]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-zipformer-thai-2024-06-20.tar.bz2
[sherpa-onnx-nemo-transducer-giga-am-russian-2024-10-24]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-nemo-transducer-giga-am-russian-2024-10-24.tar.bz2
[sherpa-onnx-paraformer-zh-2024-03-09]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-paraformer-zh-2024-03-09.tar.bz2
[sherpa-onnx-nemo-ctc-giga-am-russian-2024-10-24]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-nemo-ctc-giga-am-russian-2024-10-24.tar.bz2
[sherpa-onnx-telespeech-ctc-int8-zh-2024-06-04]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
[sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
[sherpa-onnx-streaming-zipformer-fr-2023-04-14]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-fr-2023-04-14.tar.bz2
[Moonshine tiny]: https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-moonshine-tiny-en-int8.tar.bz2
[NVIDIA Jetson Orin NX]: https://developer.download.nvidia.com/assets/embedded/secure/jetson/orin_nx/docs/Jetson_Orin_NX_DS-10712-001_v0.5.pdf?RCPGu9Q6OVAOv7a7vgtwc9-BLScXRIWq6cSLuditMALECJ_dOj27DgnqAPGVnT2VpiNpQan9SyFy-9zRykR58CokzbXwjSA7Gj819e91AXPrWkGZR3oS1VLxiDEpJa_Y0lr7UT-N4GnXtb8NlUkP4GkCkkF_FQivGPrAucCUywL481GH_WpP_p7ziHU1Wg==&t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczovL3d3dy5nb29nbGUuY29tLmhrLyJ9
[NVIDIA Jetson Nano B01]: https://www.seeedstudio.com/blog/2020/01/16/new-revision-of-jetson-nano-dev-kit-now-supports-new-jetson-nano-module/
[speech-enhancement-models]: https://github.com/k2-fsa/sherpa-onnx/releases/tag/speech-enhancement-models
