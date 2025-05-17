function(download_portaudio)
  include(FetchContent)

  # Get the source code from commit ccd16c8 (4/25/2025). https://github.com/PortAudio/portaudio/commit/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad
  set(portaudio_URL  "https://github.com/PortAudio/portaudio/archive/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip")
  set(portaudio_HASH "SHA256=5f5389c3e3b906a8a1d2cf765c69f1c448e04deab044bc627e8ec6b31dc2b54c")

  # If you don't have access to the Internet, please download it to your
  # local drive and modify the following line according to your needs.
  set(possible_file_locations
    $ENV{HOME}/Downloads/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip
    $ENV{HOME}/asr/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip
    ${CMAKE_SOURCE_DIR}/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip
    ${CMAKE_BINARY_DIR}/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip
    /tmp/ccd16c85e64d8c1a5462541388b6fbcaedbb1cad.zip
  )

  foreach(f IN LISTS possible_file_locations)
    if(EXISTS ${f})
      set(portaudio_URL  "${f}")
      file(TO_CMAKE_PATH "${portaudio_URL}" portaudio_URL)
      message(STATUS "Found local downloaded portaudio: ${portaudio_URL}")
      set(portaudio_URL2)
      break()
    endif()
  endforeach()

  # Always use static build
  set(DBUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(portaudio
    URL
      ${portaudio_URL}
      ${portaudio_URL2}
    URL_HASH          ${portaudio_HASH}
  )

  FetchContent_GetProperties(portaudio)
  if(NOT portaudio_POPULATED)
    message(STATUS "Downloading portaudio from ${portaudio_URL}")
    FetchContent_Populate(portaudio)
  endif()
  message(STATUS "portaudio is downloaded to ${portaudio_SOURCE_DIR}")
  message(STATUS "portaudio's binary dir is ${portaudio_BINARY_DIR}")

  if(APPLE)
    set(CMAKE_MACOSX_RPATH ON) # to solve the following warning on macOS
  endif()

  add_subdirectory(${portaudio_SOURCE_DIR} ${portaudio_BINARY_DIR} EXCLUDE_FROM_ALL)

  set_target_properties(portaudio PROPERTIES OUTPUT_NAME "sherpa-onnx-portaudio_static")
  if(NOT WIN32)
    target_compile_options(portaudio PRIVATE "-Wno-deprecated-declarations")
  endif()

  if(NOT BUILD_SHARED_LIBS AND SHERPA_ONNX_ENABLE_BINARY)
    install(TARGETS
      portaudio
    DESTINATION lib)
  endif()

endfunction()

download_portaudio()

# Note
# See http://portaudio.com/docs/v19-doxydocs/tutorial_start.html
# for how to use portaudio
