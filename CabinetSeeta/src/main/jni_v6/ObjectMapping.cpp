//
// Created by gu on 2021/7/14 0014.
//
#include "ObjectMapping.h"

namespace gu {

    jboolean ObjectMapping::getSeetaImageData(JNIEnv *env, jobject imageDataObj,
                                              SeetaImageData *nativeImageData) {
        jboolean result = JNI_FALSE;
        jclass jImageClass = env->FindClass("com/seetaface/v6/SeetaImageData");
        jfieldID jWidth = env->GetFieldID(jImageClass, "width", "I");
        nativeImageData->width = env->GetIntField(imageDataObj, jWidth);
        jfieldID jHeight = env->GetFieldID(jImageClass, "height", "I");
        nativeImageData->height = env->GetIntField(imageDataObj, jHeight);
        jfieldID jChannels = env->GetFieldID(jImageClass, "channels", "I");
        nativeImageData->channels = env->GetIntField(imageDataObj, jChannels);
        jfieldID jDataID = env->GetFieldID(jImageClass, "data", "[B");
        jbyteArray jDataArray = static_cast<jbyteArray>(env->GetObjectField(imageDataObj, jDataID));
        if (jDataArray != nullptr) {
            int imageLength = env->GetArrayLength(jDataArray);
            nativeImageData->data = static_cast<unsigned char *>(malloc(imageLength));
            if (nativeImageData->data != nullptr) {
                result = JNI_TRUE;
                env->GetByteArrayRegion(jDataArray, 0, imageLength,
                        reinterpret_cast<jbyte *>(nativeImageData->data));
            }
            // release byte array ref
            env->DeleteLocalRef(jDataArray);
        }
        // release class ref
        env->DeleteLocalRef(jImageClass);
        return result;
    }

    jobject ObjectMapping::createFaceImageDataObject(JNIEnv *env, SeetaImageData nativeImageData,
                                                     int dataLength) {
        jclass jFaceImageClass = env->FindClass("com/seetaface/v6/SeetaImageData");
        jmethodID jFaceImageContructor = env->GetMethodID(jFaceImageClass, "<init>", "()V");
        jobject jFaceImageObj = env->NewObject(jFaceImageClass, jFaceImageContructor);
        jfieldID jWidth = env->GetFieldID(jFaceImageClass, "width", "I");
        env->SetIntField(jFaceImageObj, jWidth, (jint) nativeImageData.width);
        jfieldID jHeight = env->GetFieldID(jFaceImageClass, "height", "I");
        env->SetIntField(jFaceImageObj, jHeight, (jint) nativeImageData.height);
        jfieldID jChannels = env->GetFieldID(jFaceImageClass, "channels", "I");
        env->SetIntField(jFaceImageObj, jChannels, (jint) nativeImageData.channels);
        jfieldID jData = env->GetFieldID(jFaceImageClass, "data", "[B");
        jbyteArray jDataArray = env->NewByteArray(dataLength);
        env->SetByteArrayRegion(jDataArray, 0, dataLength,
                reinterpret_cast<const jbyte *>(nativeImageData.data));
        env->SetObjectField(jFaceImageObj, jData, jDataArray);
        // release class ref
        env->DeleteLocalRef(jFaceImageClass);
        return jFaceImageObj;
    }

    void ObjectMapping::setFaceImageDataObject(JNIEnv *env, SeetaImageData nativeImageData,
                                               int dataLength, jobject remoteImageDataObj) {
        jclass jFaceImageClass = env->FindClass("com/seetaface/v6/SeetaImageData");
        jfieldID jWidth = env->GetFieldID(jFaceImageClass, "width", "I");
        env->SetIntField(remoteImageDataObj, jWidth, (jint) nativeImageData.width);
        jfieldID jHeight = env->GetFieldID(jFaceImageClass, "height", "I");
        env->SetIntField(remoteImageDataObj, jHeight, (jint) nativeImageData.height);
        jfieldID jChannels = env->GetFieldID(jFaceImageClass, "channels", "I");
        env->SetIntField(remoteImageDataObj, jChannels, (jint) nativeImageData.channels);
        jfieldID jData = env->GetFieldID(jFaceImageClass, "data", "[B");
        jbyteArray jDataArray = env->NewByteArray(dataLength);
        env->SetByteArrayRegion(jDataArray, 0, dataLength,
                reinterpret_cast<const jbyte *>(nativeImageData.data));
        env->SetObjectField(remoteImageDataObj, jData, jDataArray);
        // release class ref
        env->DeleteLocalRef(jFaceImageClass);
    }

    jobjectArray
    ObjectMapping::createFaceInfoObjectArray(JNIEnv *env, SeetaFaceInfoArray &nativeFaceInfoArray) {
        // face info class
        jclass jFaceInfoClass = env->FindClass("com/seetaface/v6/SeetaFaceInfo");
        jmethodID jFaceInfoContructor = env->GetMethodID(jFaceInfoClass, "<init>", "()V");

        // face rect class
        jclass jFaceRectClass = env->FindClass("com/seetaface/v6/SeetaRect");
        jmethodID jFaceRectContructor = env->GetMethodID(jFaceRectClass, "<init>", "()V");

        jobjectArray jFaceInfoArray = env->NewObjectArray(nativeFaceInfoArray.size, jFaceInfoClass,
                nullptr);

        for (int i = 0; i < nativeFaceInfoArray.size; i++) {
            jobject jFaceInfoObj = env->NewObject(jFaceInfoClass, jFaceInfoContructor);
            // add face rect object
            SeetaRect seetaRect = nativeFaceInfoArray.data[i].pos;
            jfieldID jFaceRectField = env->GetFieldID(jFaceInfoClass, "position",
                    "Lcom/seetaface/v6/SeetaRect;");
            jobject jFaceRectObj = env->NewObject(jFaceRectClass, jFaceRectContructor);
            jfieldID jx = env->GetFieldID(jFaceRectClass, "x", "I");
            env->SetIntField(jFaceRectObj, jx, (jint) seetaRect.x);
            jfieldID jy = env->GetFieldID(jFaceRectClass, "y", "I");
            env->SetIntField(jFaceRectObj, jy, (jint) seetaRect.y);
            jfieldID jw = env->GetFieldID(jFaceRectClass, "width", "I");
            env->SetIntField(jFaceRectObj, jw, (jint) seetaRect.width);
            jfieldID jh = env->GetFieldID(jFaceRectClass, "height", "I");
            env->SetIntField(jFaceRectObj, jh, (jint) seetaRect.height);
            env->SetObjectField(jFaceInfoObj, jFaceRectField, jFaceRectObj);
            // add face score
            jfieldID jScoreField = env->GetFieldID(jFaceInfoClass, "score", "F");
            env->SetFloatField(jFaceInfoObj, jScoreField,
                    (jfloat) nativeFaceInfoArray.data[i].score);

            // add face info to array
            env->SetObjectArrayElement(jFaceInfoArray, i, jFaceInfoObj);
        }

        // release class ref
        env->DeleteLocalRef(jFaceRectClass);
        // release class ref
        env->DeleteLocalRef(jFaceInfoClass);

        return jFaceInfoArray;
    }

    jboolean
    ObjectMapping::getSeetaRect(JNIEnv *env, jobject jFaceRect, SeetaRect *nativeFaceRect) {
        jclass jFaceRectClass = env->FindClass("com/seetaface/v6/SeetaRect");
        jfieldID jx = env->GetFieldID(jFaceRectClass, "x", "I");
        nativeFaceRect->x = env->GetIntField(jFaceRect, jx);
        jfieldID jy = env->GetFieldID(jFaceRectClass, "y", "I");
        nativeFaceRect->y = env->GetIntField(jFaceRect, jy);
        jfieldID jw = env->GetFieldID(jFaceRectClass, "width", "I");
        nativeFaceRect->width = env->GetIntField(jFaceRect, jw);
        jfieldID jh = env->GetFieldID(jFaceRectClass, "height", "I");
        nativeFaceRect->height = env->GetIntField(jFaceRect, jh);
        // release class ref
        env->DeleteLocalRef(jFaceRectClass);
        return JNI_TRUE;
    }

    jboolean ObjectMapping::getSeetaPointFArray(JNIEnv *env, jobjectArray pointFArray,
                                                SeetaPointF *nativePointFArray, int length) {
        jclass pointFClass = env->FindClass("com/seetaface/v6/SeetaPointF");
        for (int i = 0; i < length; i++) {
            jobject pointFObj = env->GetObjectArrayElement(pointFArray, i);
            jfieldID jx = env->GetFieldID(pointFClass, "x", "D");
            nativePointFArray[i].x = env->GetDoubleField(pointFObj, jx);
            jfieldID jy = env->GetFieldID(pointFClass, "y", "D");
            nativePointFArray[i].y = env->GetDoubleField(pointFObj, jy);
            // release object array ref
            env->DeleteLocalRef(pointFObj);
        }
        // release class ref
        env->DeleteLocalRef(pointFClass);
        return JNI_TRUE;
    }

    jobjectArray
    ObjectMapping::createPointFArrayObject(JNIEnv *env, std::vector<SeetaPointF> points) {
        jclass pointFClass = env->FindClass("com/seetaface/v6/SeetaPointF");
        jmethodID pointFContructor = env->GetMethodID(pointFClass, "<init>", "()V");
        jobjectArray pointFArray = env->NewObjectArray(points.size(), pointFClass, nullptr);
        for (int i = 0; i < points.size(); ++i) {
            // SeetaPointF point = nativeFacePoints.at(i);
            SeetaPointF point = points.at(i);
            jobject jFacePointObj = env->NewObject(pointFClass, pointFContructor);
            jfieldID jx = env->GetFieldID(pointFClass, "x", "D");
            env->SetDoubleField(jFacePointObj, jx, (jdouble) point.x);
            jfieldID jy = env->GetFieldID(pointFClass, "y", "D");
            env->SetDoubleField(jFacePointObj, jy, (jdouble) point.y);

            env->SetObjectArrayElement(pointFArray, i, jFacePointObj);
        }
        // release class ref
        env->DeleteLocalRef(pointFClass);
        return pointFArray;
    }

    void ObjectMapping::setPointFArrayObject(JNIEnv *env, SeetaPointF *seetaPointF, int length,
                                             jobjectArray pointFArray) {
        jclass pointFClass = env->FindClass("com/seetaface/v6/SeetaPointF");
        jmethodID pointFContructor = env->GetMethodID(pointFClass, "<init>", "()V");
        for (int i = 0; i < length; ++i) {
            SeetaPointF point = seetaPointF[i];
            jobject jFacePointObj = env->NewObject(pointFClass, pointFContructor);
            jfieldID jx = env->GetFieldID(pointFClass, "x", "D");
            env->SetDoubleField(jFacePointObj, jx, (jdouble) point.x);
            jfieldID jy = env->GetFieldID(pointFClass, "y", "D");
            env->SetDoubleField(jFacePointObj, jy, (jdouble) point.y);

            env->SetObjectArrayElement(pointFArray, i, jFacePointObj);
        }
        // release class ref
        env->DeleteLocalRef(pointFClass);
    }

    jobjectArray ObjectMapping::createPointFWithMaskArrayObject(JNIEnv *env,
                                                                std::vector<seeta::v6::FaceLandmarker::PointWithMask> points) {
        // SeetaPointsWithMask
        jclass pointsWithMaskClass = env->FindClass("com/seetaface/v6/SeetaPointsWithMask");
        jmethodID pointsWithMaskContructor = env->GetMethodID(pointsWithMaskClass, "<init>", "()V");
        // SeetaPointF
        jclass pointsClass = env->FindClass("com/seetaface/v6/SeetaPointF");
        jmethodID pointsContructor = env->GetMethodID(pointsClass, "<init>", "()V");
        // new object array
        jobjectArray pointFArray = env->NewObjectArray(points.size(), pointsWithMaskClass, nullptr);
        for (int i = 0; i < points.size(); ++i) {
            seeta::v6::FaceLandmarker::PointWithMask pointWithMask = points.at(i);
            jobject pointsWithMaskObj = env->NewObject(pointsWithMaskClass,
                    pointsWithMaskContructor);
            // set points to SeetaPointsWithMask
            jobject pointObj = env->NewObject(pointsClass, pointsContructor);
            jfieldID jx = env->GetFieldID(pointsClass, "x", "D");
            env->SetDoubleField(pointObj, jx, (jdouble) pointWithMask.point.x);
            jfieldID jy = env->GetFieldID(pointsClass, "y", "D");
            env->SetDoubleField(pointObj, jy, (jdouble) pointWithMask.point.y);
            jfieldID pointsId = env->GetFieldID(pointsWithMaskClass, "points",
                    "Lcom/seetaface/v6/SeetaPointF;");
            env->SetObjectField(pointsWithMaskObj, pointsId, pointObj);
            // set mask to SeetaPointsWithMask
            jfieldID maskId = env->GetFieldID(pointsWithMaskClass, "mask", "Z");
            env->SetBooleanField(pointsWithMaskObj, maskId, pointWithMask.mask);
            // add SeetaPointsWithMask to object array
            env->SetObjectArrayElement(pointFArray, i, pointsWithMaskObj);
        }
        // release class ref
        env->DeleteLocalRef(pointsClass);
        env->DeleteLocalRef(pointsWithMaskClass);
        return pointFArray;
    }

    void ObjectMapping::setPointFWithMaskArrayObject(JNIEnv *env, SeetaPointF *seetaPointF,
                                                     int32_t *maskArray, int length,
                                                     jobjectArray pointFArray) {
        // SeetaPointsWithMask
        jclass pointsWithMaskClass = env->FindClass("com/seetaface/v6/SeetaPointsWithMask");
        jmethodID pointsWithMaskContructor = env->GetMethodID(pointsWithMaskClass, "<init>", "()V");
        // SeetaPointF
        jclass pointsClass = env->FindClass("com/seetaface/v6/SeetaPointF");
        jmethodID pointsContructor = env->GetMethodID(pointsClass, "<init>", "()V");
        for (int i = 0; i < length; ++i) {
            SeetaPointF points = seetaPointF[i];
            jobject pointsWithMaskObj = env->NewObject(pointsWithMaskClass,
                    pointsWithMaskContructor);
            // set points to SeetaPointsWithMask
            jobject pointObj = env->NewObject(pointsClass, pointsContructor);
            jfieldID jx = env->GetFieldID(pointsClass, "x", "D");
            env->SetDoubleField(pointObj, jx, (jdouble) points.x);
            jfieldID jy = env->GetFieldID(pointsClass, "y", "D");
            env->SetDoubleField(pointObj, jy, (jdouble) points.y);
            jfieldID pointsId = env->GetFieldID(pointsWithMaskClass, "points",
                    "Lcom/seetaface/v6/SeetaPointF;");
            env->SetObjectField(pointsWithMaskObj, pointsId, pointObj);
            // set mask to SeetaPointsWithMask
            int32_t mask = maskArray[i];
            jfieldID maskId = env->GetFieldID(pointsWithMaskClass, "mask", "Z");
            env->SetBooleanField(pointsWithMaskObj, maskId, mask == 1);
            // add SeetaPointsWithMask to object array
            env->SetObjectArrayElement(pointFArray, i, pointsWithMaskObj);
        }
        // release class ref
        env->DeleteLocalRef(pointsClass);
        env->DeleteLocalRef(pointsWithMaskClass);
    }

    jobjectArray ObjectMapping::createTrackingFaceInfoObjectArray(JNIEnv *env,
                                                                  SeetaTrackingFaceInfoArray &nativeTrackingFaceInfoArray) {
        // face info class
        jclass trackingFaceInfoClass = env->FindClass("com/seetaface/v6/SeetaTrackingFaceInfo");
        jmethodID trackingFaceInfoContructor = env->GetMethodID(trackingFaceInfoClass, "<init>",
                "()V");

        // face rect class
        jclass faceRectClass = env->FindClass("com/seetaface/v6/SeetaRect");
        jmethodID faceRectContructor = env->GetMethodID(faceRectClass, "<init>", "()V");

        jobjectArray trackingFaceInfoArray = env->NewObjectArray(nativeTrackingFaceInfoArray.size,
                trackingFaceInfoClass, nullptr);

        for (int i = 0; i < nativeTrackingFaceInfoArray.size; i++) {
            jobject trackingFaceInfoObj = env->NewObject(trackingFaceInfoClass,
                    trackingFaceInfoContructor);
            // add face rect object
            SeetaRect seetaRect = nativeTrackingFaceInfoArray.data[i].pos;
            jfieldID faceRectField = env->GetFieldID(trackingFaceInfoClass, "rect",
                    "Lcom/seetaface/v6/SeetaRect;");
            jobject faceRectObj = env->NewObject(faceRectClass, faceRectContructor);
            jfieldID jx = env->GetFieldID(faceRectClass, "x", "I");
            env->SetIntField(faceRectObj, jx, (jint) seetaRect.x);
            jfieldID jy = env->GetFieldID(faceRectClass, "y", "I");
            env->SetIntField(faceRectObj, jy, (jint) seetaRect.y);
            jfieldID jw = env->GetFieldID(faceRectClass, "width", "I");
            env->SetIntField(faceRectObj, jw, (jint) seetaRect.width);
            jfieldID jh = env->GetFieldID(faceRectClass, "height", "I");
            env->SetIntField(faceRectObj, jh, (jint) seetaRect.height);
            env->SetObjectField(trackingFaceInfoObj, faceRectField, faceRectObj);
            // add score
            jfieldID scoreField = env->GetFieldID(trackingFaceInfoClass, "score", "F");
            env->SetFloatField(trackingFaceInfoObj, scoreField,
                    (jfloat) nativeTrackingFaceInfoArray.data[i].score);
            // add frame
            jfieldID frameField = env->GetFieldID(trackingFaceInfoClass, "frame", "I");
            env->SetIntField(trackingFaceInfoObj, frameField,
                    nativeTrackingFaceInfoArray.data[i].frame_no);
            // add pid
            jfieldID pidField = env->GetFieldID(trackingFaceInfoClass, "pid", "I");
            env->SetIntField(trackingFaceInfoObj, pidField,
                    nativeTrackingFaceInfoArray.data[i].PID);
            // add step
            jfieldID stepField = env->GetFieldID(trackingFaceInfoClass, "step", "I");
            env->SetIntField(trackingFaceInfoObj, stepField,
                    nativeTrackingFaceInfoArray.data[i].step);

            // add face info to array
            env->SetObjectArrayElement(trackingFaceInfoArray, i, trackingFaceInfoObj);
        }

        // release class ref
        env->DeleteLocalRef(faceRectClass);
        // release class ref
        env->DeleteLocalRef(trackingFaceInfoClass);

        return trackingFaceInfoArray;
    }

    jobject ObjectMapping::createQualityResultObject(JNIEnv *env, seeta::QualityResult result) {
        jclass qualityResultClass = env->FindClass("com/seetaface/v6/QualityResult");
        jmethodID qualityResultContructor = env->GetMethodID(qualityResultClass, "<init>", "()V");
        jobject qualityResultObj = env->NewObject(qualityResultClass, qualityResultContructor);
        jfieldID levelField = env->GetFieldID(qualityResultClass, "level", "I");
        env->SetIntField(qualityResultObj, levelField, (int) result.level);
        jfieldID scoreField = env->GetFieldID(qualityResultClass, "score", "F");
        env->SetFloatField(qualityResultObj, scoreField, result.score);
        // release class ref
        env->DeleteLocalRef(qualityResultClass);
    }
}
