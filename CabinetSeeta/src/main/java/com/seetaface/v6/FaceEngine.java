package com.seetaface.v6;

import android.util.Log;

import com.seetaface.v6.config.FaceConfig;
import com.seetaface.v6.constant.SeetafaceModelConst;


public class FaceEngine {

    private static final String TAG = "FaceEngine";

    private static FaceEngine faceEngine;

    private FaceDetector faceDetector;

    private FaceLandMarker faceLandMarker;

    private FaceRecognizer faceRecognizer;

    private FaceAntiSpoofing faceAntiSpoofing;

    private FaceImagePreprocessor faceImagePreprocessor;


    public static FaceEngine getInstance() {
        if (null == faceEngine) {
            faceEngine = new FaceEngine();
            faceEngine.init();
        }
        return faceEngine;
    }


    private void init() {
        try {
            if (faceDetector == null || faceLandMarker == null || faceRecognizer == null) {
                faceDetector = new FaceDetector();
                faceLandMarker = new FaceLandMarker(SeetafaceModelConst.MODEL_LAND_MARKER_PTS5);
                faceRecognizer = new FaceRecognizer(SeetafaceModelConst.MODEL_RECOGNIZER);
                faceAntiSpoofing = new FaceAntiSpoofing(FaceConfig.ANTI_TYPE);
            }
        } catch (Exception e) {
            Log.e(TAG, "init exception:" + e);
        }
    }


    public FaceDetector getFaceDetector() {
        return faceDetector;
    }

    public FaceLandMarker getFaceLandMarker() {
        return faceLandMarker;
    }

    public FaceRecognizer getFaceRecognizer() {
        return faceRecognizer;
    }

    public FaceAntiSpoofing getFaceAntiSpoofing() {
        return faceAntiSpoofing;
    }
}
