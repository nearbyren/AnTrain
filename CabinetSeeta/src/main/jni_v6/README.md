# SO库 C C++ 知识

*** so 文件名称
    **** 最终会生成 build/intermediates/merged_native_libs/release/out/lib/${abi}/lib-xxx.so 。基于配置的 abi 会有对应类型的 so 生成。

*** SHARED 共享库，这个一般是固定的


add_library(libTenniS SHARED IMPORTED)
set_target_properties(
libTenniS
PROPERTIES IMPORTED_LOCATION 
"${libs}/${ANDROID_ABI}/libTenniS.so")

第一行声明了一个名为libTenniS的共享库，并标记为 IMPORTED，即它是外部导入的。
set_target_properties用于设置这个库的属性，这里，IMPORTED_LOCATION属性指明了库文件的实际位置，
${libs}so文件所在的目录，${ANDROID_ABI}自动对应于构建的目标架构。



target_link_libraries(
FacePreprocessor
android
jnigraphics
${log-lib})
target_link_libraries 命令用于指定native-lib需要链接哪些库。
这里，除了之前声明的第三方库外，还有log-lib，一个常用的日志库，通常包含在NDK中。









/***************************************说明例子*******************************/

add_library(
FacePreprocessor # 库的名字
SHARED # 库的类型  
ObjectMapping.cpp # 库的源文件
nativeFacePreprocessor.cpp)# 库的源文件

# 引用NDK中的库log，命名为log-lib
find_library(
log-lib # 库路径对应的变量名
log)    # NDK中的库名

# 关联库，确保 FacePreprocessor 中 能使用 log 库
target_link_libraries(
FacePreprocessor
android
jnigraphics
${log-lib})

# 在defaultConfig闭包内，添加对外部本地构建的配置 例如生成arm64-v8a、armeabi-v7a格式的库

    defaultConfig {
        externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "arm64-v8a", "armeabi-v7a"
            }
        }
    }

/***************************************说明例子*******************************/


# 年龄估计器要求输入原始图像数据和人脸特征点（或者裁剪好的人脸数据），对输入的人脸进行年龄估计。
[nativeAgePredictor.cpp](nativeAgePredictor.cpp)

# 眼睛检测器要求输入原始图像数据和人脸特征点，返回左眼和右眼的状态。
[nativeEyeStateDetector.cpp](nativeEyeStateDetector.cpp)

# 静默活体识别根据输入的图像数据、人脸位置和人脸特征点，对输入人脸进行活体的判断，并返回人脸活体的状态。
[nativeFaceAntiSpoofing.cpp](nativeFaceAntiSpoofing.cpp)

# 人脸检测器会对输入的彩色图片或者灰度图像进行人脸检测，并返回所有检测到的人脸位置。
[nativeFaceDetector.cpp](nativeFaceDetector.cpp)

# 人脸特征点检测器要求输入原始图像数据和人脸位置，返回人脸 5 个或者其他数量的的特征点的坐标（特征点的数量和加载的模型有关）。
[nativeFaceLandMarker.cpp](nativeFaceLandMarker.cpp)

# 数据格式转换器
[nativeFacePreprocessor.cpp](nativeFacePreprocessor.cpp)

# 人脸识别器要求输入原始图像数据和人脸特征点（或者裁剪好的人脸数据），对输入的人脸提取特征值数组，根据提取的特征值数组对人脸进行相似度比较。
[nativeFaceRecognizer.cpp](nativeFaceRecognizer.cpp)

# 人脸跟踪器会对输入的彩色图像或者灰度图像中的人脸进行跟踪，并返回所有跟踪到的人脸信息。
[nativeFaceTracker.cpp](nativeFaceTracker.cpp)

# 性别估计器要求输入原始图像数据和人脸特征点（或者裁剪好的人脸数据），对输入的人脸进行性别估计。
[nativeGenderPredictor.cpp](nativeGenderPredictor.cpp)

# 口罩检测器根据输入的图像数据、人脸位置，返回是否佩戴口罩的检测结果。
[nativeMaskDetector.cpp](nativeMaskDetector.cpp)

# 质量评估器包含不同的质量评估模块，包括人脸亮度、人脸清晰度（非深度方法）、 人脸清晰度（深度方法）、人脸姿态（非深度方法）、人脸姿态（深度方法）、人脸分辨率和人脸完整度评估模块。
[nativeQualityChecker.cpp](nativeQualityChecker.cpp)

