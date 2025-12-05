//
// Created by gu on 2021/7/14 0014.
//

#ifndef SEETAFACE6DEMO_OBJECTMAPPING_H
#define SEETAFACE6DEMO_OBJECTMAPPING_H

#include <jni.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/CTrackingFaceInfo.h>
#include <seeta/QualityStructure.h>
#include "Common/Struct.h"
#include "CFaceInfo.h"

namespace gu {
    class ObjectMapping {
    public:
        static jboolean
        getSeetaImageData(JNIEnv *env, jobject imageDataObj, SeetaImageData *nativeImageData);

        static jobject
        createFaceImageDataObject(JNIEnv *env, SeetaImageData nativeImageData, int dataLength);

        static void
        setFaceImageDataObject(JNIEnv *env, SeetaImageData nativeImageData, int dataLength,
                               jobject remoteImageDataObj);

        static jobjectArray
        createFaceInfoObjectArray(JNIEnv *env, SeetaFaceInfoArray &nativeFaceInfoArray);

        // SeetaRect
        static jboolean getSeetaRect(JNIEnv *env, jobject jFaceRect, SeetaRect *nativeFaceRect);

        // SeetaPointF
        static jboolean
        getSeetaPointFArray(JNIEnv *env, jobjectArray pointFArray, SeetaPointF *nativePointFArray,
                            int length);

        static jobjectArray createPointFArrayObject(JNIEnv *env, std::vector<SeetaPointF> points);

        static void setPointFArrayObject(JNIEnv *env, SeetaPointF *seetaPointF, int length,
                                         jobjectArray pointFArray);

        static jobjectArray createPointFWithMaskArrayObject(JNIEnv *env,
                                                            std::vector<seeta::v6::FaceLandmarker::PointWithMask> points);

        static void
        setPointFWithMaskArrayObject(JNIEnv *env, SeetaPointF *seetaPointF, int32_t *maskArray,
                                     int length, jobjectArray pointFArray);

        // SeetaTrackingFaceInfoArray
        static jobjectArray createTrackingFaceInfoObjectArray(JNIEnv *env,
                                                              SeetaTrackingFaceInfoArray &nativeTrackingFaceInfoArray);

        // QualityResult
        static jobject createQualityResultObject(JNIEnv *env, seeta::QualityResult result);
    };
}

#endif //SEETAFACE6DEMO_OBJECTMAPPING_H
