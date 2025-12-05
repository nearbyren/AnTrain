//
// Created by Administrator on 2021/7/15 0015.
//
#include "com_seetaface_v6_FaceAntiSpoofing.h"
#include <android/log.h>
#include "FaceAntiSpoofing.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::FaceAntiSpoofing *gFaceAntiSpoofing = nullptr;

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring firstModel, jstring secondModel) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeCreateEngine");
    if (gFaceAntiSpoofing != nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (firstModel != nullptr) {
        const char *firstModelFile = env->GetStringUTFChars(firstModel, 0);
//        LOGD("face--- [native_FaceAntiSpoofing]->nativeCreateEngine firstModelFile:%s", firstModelFile);
        if (firstModelFile != nullptr) {
            modelSetting.append(firstModelFile);
            env->ReleaseStringUTFChars(firstModel, firstModelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (secondModel != nullptr) {
        const char *secondModelFile = env->GetStringUTFChars(secondModel, 0);
//        LOGD("face--- [native_FaceAntiSpoofing]->nativeCreateEngine secondModelFile:%s", secondModelFile);
        if (secondModelFile != nullptr) {
            modelSetting.append(secondModelFile);
            env->ReleaseStringUTFChars(secondModel, secondModelFile);
        }
    }
    if (result == EXIT_SUCCESS) {
        gFaceAntiSpoofing = new seeta::FaceAntiSpoofing(modelSetting);
    }
    result = gFaceAntiSpoofing != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeDestroyEngine");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gFaceAntiSpoofing;
    gFaceAntiSpoofing = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativePredict
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativePredict
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativePredict");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredict engine has not been created");
        return -1;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredict get image data failed");
        return -1;
    }

    SeetaRect nativeRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeRect)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredict get rect failed");
        // release native image data
        free(nativeImageData.data);
        return -1;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredict get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return -1;
    }

    int ret = -1;
    try {
        seeta::FaceAntiSpoofing::Status status = gFaceAntiSpoofing->Predict(nativeImageData, nativeRect, nativePointFs);
        ret = status;
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_FaceAntiSpoofing]->nativePredict status:%d", ret);

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    return ret;
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativePredictVideo
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativePredictVideo
        (JNIEnv *env, jobject obj, jobject image,
         jobject rect, jobjectArray points) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativePredictVideo");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredictVideo engine has not been created");
        return -1;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredictVideo get image data failed");
        return -1;
    }

    SeetaRect nativeRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeRect)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredictVideo get rect failed");
        // release native image data
        free(nativeImageData.data);
        return -1;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativePredictVideo get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return -1;
    }

    int ret = -1;
    try {
        seeta::FaceAntiSpoofing::Status status = gFaceAntiSpoofing->PredictVideo(nativeImageData, nativeRect, nativePointFs);
        ret = status;
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_FaceAntiSpoofing]->nativePredictVideo status:%d", ret);

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    return ret;
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeResetVideo
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeResetVideo
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeResetVideo");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeResetVideo engine has not been created");
        return;
    }
    gFaceAntiSpoofing->ResetVideo();
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeGetPreFrameScore
 * Signature: ([F[F)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeGetPreFrameScore
        (JNIEnv *env, jobject obj, jfloatArray clarityArray, jfloatArray realityArray) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeGetPreFrameScore");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeGetPreFrameScore engine has not been created");
        return;
    }

    int nativeClarityLength = env->GetArrayLength(clarityArray);
    float *nativeClarityArray = new float[nativeClarityLength];
    int nativeRealityLength = env->GetArrayLength(clarityArray);
    float *nativeRealityArray = new float[nativeRealityLength];

    gFaceAntiSpoofing->GetPreFrameScore(nativeClarityArray, nativeRealityArray);

    env->SetFloatArrayRegion(clarityArray, 0, nativeClarityLength, nativeClarityArray);
    env->SetFloatArrayRegion(realityArray, 0, nativeRealityLength, nativeRealityArray);

    delete [] nativeRealityArray;
    delete [] nativeClarityArray;
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeSetVideoFrameCount
 * Signature: (I)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeSetVideoFrameCount
        (JNIEnv *env, jobject obj, jint number) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeSetVideoFrameCount");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeSetVideoFrameCount engine has not been created");
        return;
    }
    gFaceAntiSpoofing->SetVideoFrameCount(number);
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeGetVideoFrameCount
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeGetVideoFrameCount
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeGetVideoFrameCount");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeGetVideoFrameCount engine has not been created");
        return 0;
    }
    return gFaceAntiSpoofing->GetVideoFrameCount();
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeSetThreshold
 * Signature: (FF)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeSetThreshold
        (JNIEnv *env, jobject obj, jfloat clarity, jfloat reality) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeSetThreshold");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeSetThreshold engine has not been created");
        return;
    }
    gFaceAntiSpoofing->SetThreshold(clarity, reality);
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeGetThreshold
 * Signature: (Ljava/lang/Float;Ljava/lang/Float;)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeGetThreshold
        (JNIEnv *env, jobject obj,
                jobject remoteClarity, jobject remoteReality) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeGetThreshold");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeGetThreshold engine has not been created");
        return;
    }
    float clarity = 0;
    float reality = 0;
    gFaceAntiSpoofing->GetThreshold(&clarity, &reality);

    jclass floatClass = env->FindClass("java/lang/Float");
    jfieldID valueField = env->GetFieldID(floatClass, "value", "F");
    env->SetFloatField(remoteClarity, valueField, clarity);
    env->SetFloatField(remoteReality, valueField, reality);
    env->DeleteLocalRef(floatClass);
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeSetProperty
        (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeSetProperty");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeSetProperty engine has not been created");
        return;
    }
    gFaceAntiSpoofing->set(static_cast<seeta::FaceAntiSpoofing::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_FaceAntiSpoofing
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_FaceAntiSpoofing_nativeGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_FaceAntiSpoofing]->nativeGetProperty");
    if (gFaceAntiSpoofing == nullptr) {
//        LOGW("face--- [native_FaceAntiSpoofing]->nativeGetProperty engine has not been created");
        return 0;
    }
    return gFaceAntiSpoofing->get(static_cast<seeta::FaceAntiSpoofing::Property>(property));
}