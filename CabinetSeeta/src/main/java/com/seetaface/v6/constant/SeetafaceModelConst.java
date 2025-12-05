package com.seetaface.v6.constant;

/**
 * app 模型常量
 */
public class SeetafaceModelConst {

    /**
     * 眼睛状态
     * 眼睛状态评估	    2.78MB
     */
    public static final String MODEL_EYE_STATE = "eye_state.csta";
    /**
     * 年龄评估
     * 年龄预测模型	    31.3MB
     */
    public static final String MODEL_AGE_PREDICTOR = "age_predictor.csta";

    /**
     *活体检测识别器 局部检测模型   42.6MB
     */
    public static final String MODEL_FIRST = "fas_first.csta";
    /**
     *活体检测识别器 全局检测模型   18.2MB
     */
    public static final String MODEL_SECOND = "fas_second.csta";
    /**
     *人脸检测器，检测到的每个人脸位置，用矩形表示 3.86MB
     */
    public static final String DETECTOR_FACE_MODEL = "face_detector.csta";
    /**
     *5点人脸标识模型, 确定 两眼、两嘴角和鼻尖	   412KB
     */
    public static final String MODEL_LAND_MARKER_PTS5 = "face_landmarker_pts5.csta";
    /**
     *68点人脸标识模型， 人脸68个特征点     2.43MB
     */
    public static final String MODEL_LAND_MARKER_PTS68 = "face_landmarker_pts68.csta";
    /**
     *遮挡评估，判断的遮挡物为五个关键点，分别是左右眼中心、鼻尖和左右嘴角	3.13MB
     */
    public static final String MODEL_LAND_MARKER_MASK_PTS5 = "face_landmarker_mask_pts5.csta";
    /**
     * 高精度人脸识别人脸向量特征提取模型，建议阈值：0.62	 97.8MB
     */
    public static final String MODEL_RECOGNIZER = "face_recognizer.csta";
    /**
     * 轻量级人脸向量特征提取模型，建议阈值：0.55 4.51MB
     */
    public static final String MODEL_RECOGNIZER_LIGHT = "face_recognizer_light.csta";
    /**
     * 口罩模型
     * 戴口罩人脸向量特征提取模型，建议阈值：0.48	         93.8MB
     */
    public static final String MODEL_RECOGNIZER_MASK = "face_recognizer_mask.csta";
    /**
     *人脸检测器，检测到的每个人脸位置，用矩形表示 3.86MB
     */
    public static final String MODEL_TRACKER = "face_detector.csta";
    /**
     *性别识别        916KB
     */
    public static final String MODEL_GENDER_PREDICTOR = "gender_predictor.csta";
    /**
     * 口罩检测器	  916KB
     */
    public static final String MASK_DETECTOR_MODEL = "mask_detector.csta";

    /**
     * 清晰度评估模型   954KB
     */
    public static final String MODEL_LBN = "quality_lbn.csta";
    /**
     *人脸姿态评估      1.89MB
     */
    public static final String MODEL_POSE_EX = "pose_estimation.csta";


}
