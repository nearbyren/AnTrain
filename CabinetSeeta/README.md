模型	                                    模型说明	                                                                备注
face_recognizer.csta	                高精度人脸识别人脸向量特征提取模型，建议阈值：0.62	 97.8MB                        返回1024长度向量特征
face_recognizer_mask.csta	            戴口罩人脸向量特征提取模型，建议阈值：0.48	         93.8MB                           返回512长度向量特征
face_recognizer_light.csta	            轻量级人脸向量特征提取模型，建议阈值：0.55	         4.51MB                           返回512长度向量特征
age_predictor.csta	                    年龄预测模型	    31.3MB                                                        返回int[0]
face_landmarker_pts5.csta	            5点人脸标识模型, 确定 两眼、两嘴角和鼻尖	   412KB                                 SeetaPointF[] 即 x，y坐标数组
face_landmarker_pts68.csta	            68点人脸标识模型， 人脸68个特征点     2.43MB                                    	SeetaPointF[] 即 x，y坐标数组
pose_estimation.csta	                人脸姿态评估      1.89MB
eye_state.csta	                        眼睛状态评估	    2.78MB                                                        打开 关闭状态
face_detector.csta	                    人脸检测器，检测到的每个人脸位置，用矩形表示 3.86MB
face_landmarker_mask_pts5.csta	        遮挡评估，判断的遮挡物为五个关键点，分别是左右眼中心、鼻尖和左右嘴角	3.13MB             1：遮挡， 0：没遮挡
mask_detector.csta	                    口罩检测器	  916KB                                                          false:0.0089 或 true:0.985
gender_predictor.csta	                性别识别        916KB
quality_lbn.csta	                    清晰度评估模型   954KB
fas_first.csta	                        活体检测识别器 局部检测模型   42.6MB
fas_second.csta	                        活体检测识别器 全局检测模型   18.2MB
————————————————


index
|-- asserts               一些图片，无视掉     
|-- docs                  各功能接口说明
|-- example               包含了一个qt 人脸识别demo
|-- FaceAntiSpoofingX6    静默活体模块
|-- FaceBoxes             人脸检测模块
|-- FaceRecognizer6       人脸识别模块
|-- FaceTracker6          人脸跟踪
|-- Landmarker            人脸特征点
|-- OpenRoleZoo           *常用操作的集合*
|-- PoseEstimator6        人脸朝向姿态估计
|-- QualityAssessor3      图像质量评估（亮度评估、清晰度评估、完整度评估、清晰度评估（深度）、姿态评估、姿态评估（深度）、分辨率评估）
|-- README.md
|-- SeetaAgePredictor     年龄识别
|-- SeetaAuthorize	      *模型解析工程*
|-- SeetaEyeStateDetector 眼睛状态检测：睁眼，闭眼
|-- SeetaGenderPredictor  性别识别
|-- SeetaMaskDetector     口罩人脸检测识别
|-- TenniS                *前向计算框架*


OpenRoleZoo
SeetaAuthorize
TenniS

Android 平台编译说明
 **下载配置Android的NDK，
 **配置环境变量ndk-build
 **编译
  cd到各个模块的jni目录,例如
```
    #cd到jni目录下
    ./FaceRecognizer6/FaceRecognizer/android/jni
    #执行命令
    ndk-build -j4
```

