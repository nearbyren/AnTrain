 # 设置CMake的最低版本
 cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 添加头文件
include_directories(include)
include_directories(include/seeta)

find_library(log-lib log)

# 添加人脸相关共享库文件，会自动把so打包到apk中，无需在build.gradle中添加jniLibs的路径，
# 也不能把so放到main路径下的jniLibs中，否则会报'More than one file was found with OS independent path' 错误
# libs设置的路径一定不能出错，否则编译时会找不到文件
set(libs "${CMAKE_SOURCE_DIR}/libs")

# SeetaAuthorize
add_library(libSeetaAuthorize SHARED IMPORTED)
set_target_properties(
        libSeetaAuthorize
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaAuthorize.so")

# TenniS
add_library(libTenniS SHARED IMPORTED)
set_target_properties(
        libTenniS
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libTenniS.so")

# 添加生成人脸图片预处理共享库，并添加链接依赖
add_library(
        FacePreprocessor
        SHARED
        ObjectMapping.cpp
        nativeFacePreprocessor.cpp)
target_link_libraries(
        FacePreprocessor
        android
        jnigraphics
        ${log-lib})

# SeetaFaceDetector
add_library(libSeetaFaceDetector600 SHARED IMPORTED)
set_target_properties(
        libSeetaFaceDetector600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaFaceDetector600.so")
# 添加生成人脸检测共享库，并添加链接依赖
add_library(
        FaceDetector
        SHARED
        ObjectMapping.cpp
        nativeFaceDetector.cpp)
target_link_libraries(
        FaceDetector
        android
        libSeetaAuthorize
        libTenniS
        libSeetaFaceDetector600
        ${log-lib})

# SeetaFaceLandmarker
add_library(libSeetaFaceLandmarker600 SHARED IMPORTED)
set_target_properties(
        libSeetaFaceLandmarker600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaFaceLandmarker600.so")
# 添加生成人脸特征提取共享库，并添加链接依赖
add_library(
        FaceLandMarker
        SHARED
        ObjectMapping.cpp
        nativeFaceLandMarker.cpp)
target_link_libraries(
        FaceLandMarker
        android
        libSeetaAuthorize
        libTenniS
        libSeetaFaceLandmarker600
        ${log-lib})

# SeetaFaceRecognizer
add_library(libSeetaFaceRecognizer600 SHARED IMPORTED)
set_target_properties(
        libSeetaFaceRecognizer600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaFaceRecognizer600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        FaceRecognizer
        SHARED
        ObjectMapping.cpp
        nativeFaceRecognizer.cpp)
target_link_libraries(
        FaceRecognizer
        android
        libSeetaAuthorize
        libTenniS
        libSeetaFaceRecognizer600
        ${log-lib})

# SeetaAgePredictor
add_library(libSeetaAgePredictor600 SHARED IMPORTED)
set_target_properties(
        libSeetaAgePredictor600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaAgePredictor600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        AgePredictor
        SHARED
        ObjectMapping.cpp
        nativeAgePredictor.cpp)
target_link_libraries(
        AgePredictor
        android
        libSeetaAuthorize
        libTenniS
        libSeetaAgePredictor600
        ${log-lib})

# SeetaEyeStateDetector
add_library(libSeetaEyeStateDetector600 SHARED IMPORTED)
set_target_properties(
        libSeetaEyeStateDetector600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaEyeStateDetector600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        EyeStateDetector
        SHARED
        ObjectMapping.cpp
        nativeEyeStateDetector.cpp)
target_link_libraries(
        EyeStateDetector
        android
        libSeetaAuthorize
        libTenniS
        libSeetaEyeStateDetector600
        ${log-lib})

# SeetaFaceAntiSpoofing
add_library(libSeetaFaceAntiSpoofingX600 SHARED IMPORTED)
set_target_properties(
        libSeetaFaceAntiSpoofingX600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaFaceAntiSpoofingX600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        FaceAntiSpoofing
        SHARED
        ObjectMapping.cpp
        nativeFaceAntiSpoofing.cpp)
target_link_libraries(
        FaceAntiSpoofing
        android
        libSeetaAuthorize
        libTenniS
        libSeetaFaceAntiSpoofingX600
        ${log-lib})

# SeetaFaceTracking
add_library(libSeetaFaceTracking600 SHARED IMPORTED)
set_target_properties(
        libSeetaFaceTracking600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaFaceTracking600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        FaceTracker
        SHARED
        ObjectMapping.cpp
        nativeFaceTracker.cpp)
target_link_libraries(
        FaceTracker
        android
        libSeetaAuthorize
        libTenniS
        libSeetaFaceTracking600
        ${log-lib})

# SeetaGenderPredictor
add_library(libSeetaGenderPredictor600 SHARED IMPORTED)
set_target_properties(
        libSeetaGenderPredictor600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaGenderPredictor600.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        GenderPredictor
        SHARED
        ObjectMapping.cpp
        nativeGenderPredictor.cpp)
target_link_libraries(
        GenderPredictor
        android
        libSeetaAuthorize
        libTenniS
        libSeetaGenderPredictor600
        ${log-lib})

# SeetaMaskDetector
add_library(libSeetaMaskDetector200 SHARED IMPORTED)
set_target_properties(
        libSeetaMaskDetector200
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libSeetaMaskDetector200.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        MaskDetector
        SHARED
        ObjectMapping.cpp
        nativeMaskDetector.cpp)
target_link_libraries(
        MaskDetector
        android
        libSeetaAuthorize
        libTenniS
        libSeetaMaskDetector200
        ${log-lib})

# PoseEstimation
add_library(libPoseEstimation600 SHARED IMPORTED)
set_target_properties(
        libPoseEstimation600
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libPoseEstimation600.so")
# QualityAssessor
add_library(libQualityAssessor300 SHARED IMPORTED)
set_target_properties(
        libQualityAssessor300
        PROPERTIES IMPORTED_LOCATION
        "${libs}/${ANDROID_ABI}/libQualityAssessor300.so")
# 添加生成人脸识别共享库，并添加链接依赖
add_library(
        QualityChecker
        SHARED
        ObjectMapping.cpp
        nativeQualityChecker.cpp)
target_link_libraries(
        QualityChecker
        android
        libSeetaAuthorize
        libTenniS
        libPoseEstimation600
        libQualityAssessor300
        ${log-lib})