//
// Created by Administrator on 2021/7/15 0015.
//
#include "com_seetaface_v6_FaceTracker.h"
#include <android/log.h>
#include "FaceTracker.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::FaceTracker *gFaceTracker = nullptr;

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;II)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceTracker_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring model, jint width, jint height) {
//    LOGD("face--- [native_FaceTracker]->nativeCreateEngine");
    if (gFaceTracker != nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeDestroyEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_FaceTracker]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (result == EXIT_SUCCESS) {
        gFaceTracker = new seeta::FaceTracker(modelSetting, width, height);
    }
    result = gFaceTracker != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint) result;
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceTracker_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceTracker]->nativeDestroyEngine");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gFaceTracker;
    gFaceTracker = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeSetSingleCalculationThreads
 * Signature: (I)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceTracker_nativeSetSingleCalculationThreads
        (JNIEnv *env, jobject obj, jint number) {
//    LOGD("face--- [native_FaceTracker]->nativeSetSingleCalculationThreads");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeSetSingleCalculationThreads engine has not been created");
        return;
    }
    gFaceTracker->SetSingleCalculationThreads(number);
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeSetMinFaceSize
 * Signature: (I)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceTracker_nativeSetMinFaceSize
        (JNIEnv *env, jobject obj, jint size) {
//    LOGD("face--- [native_FaceTracker]->nativeSetMinFaceSize");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeSetMinFaceSize engine has not been created");
        return;
    }
    gFaceTracker->SetMinFaceSize(size);
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeGetMinFaceSize
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceTracker_nativeGetMinFaceSize
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceTracker]->nativeGetMinFaceSize");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeGetMinFaceSize engine has not been created");
        return 0;
    }
    return gFaceTracker->GetMinFaceSize();
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeSetThreshold
 * Signature: (F)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceTracker_nativeSetThreshold
        (JNIEnv *env, jobject obj, jfloat thresh) {
//    LOGD("face--- [native_FaceTracker]->nativeSetThreshold");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeSetThreshold engine has not been created");
        return;
    }
    gFaceTracker->SetThreshold(thresh);
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeGetScoreThreshold
 * Signature: ()F
 */
extern "C" JNIEXPORT jfloat JNICALL Java_com_seetaface_v6_FaceTracker_nativeGetScoreThreshold
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceTracker]->nativeGetScoreThreshold");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeGetScoreThreshold engine has not been created");
        return 0;
    }
    return gFaceTracker->GetThreshold();
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeSetVideoStable
 * Signature: (Z)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceTracker_nativeSetVideoStable
        (JNIEnv *env, jobject obj, jboolean stable) {
//    LOGD("face--- [native_FaceTracker]->nativeSetVideoStable");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeSetVideoStable engine has not been created");
        return;
    }
    gFaceTracker->SetVideoStable(stable);
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeGetVideoStable
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_FaceTracker_nativeGetVideoStable
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_FaceTracker]->nativeGetVideoStable");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeGetVideoStable engine has not been created");
        return 0;
    }
    return gFaceTracker->GetVideoStable();
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeTrack
 * Signature: (Lcom/seetaface/v6/SeetaImageData;)[Lcom/seetaface/v6/SeetaTrackingFaceInfo;
 */
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_seetaface_v6_FaceTracker_nativeTrack__Lcom_seetaface_v6_SeetaImageData_2
        (JNIEnv *env, jobject obj, jobject image) {
//    LOGD("face--- [native_FaceTracker]->nativeTrack");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeTrack engine has not been created");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_FaceTracker]->nativeTrack get image data failed");
        return nullptr;
    }

    SeetaTrackingFaceInfoArray trackingFaceInfoArray{};
    try {
        trackingFaceInfoArray = gFaceTracker->Track(nativeImageData);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);

    jobjectArray trackingFaceInfo = nullptr;
    if (trackingFaceInfoArray.size > 0) {
        trackingFaceInfo = gu::ObjectMapping::createTrackingFaceInfoObjectArray(env, trackingFaceInfoArray);
    }
    return trackingFaceInfo;
}

/*
 * Class:     com_seetaface_v6_FaceTracker
 * Method:    nativeTrack
 * Signature: (Lcom/seetaface/v6/SeetaImageData;I)[Lcom/seetaface/v6/SeetaTrackingFaceInfo;
 */
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_seetaface_v6_FaceTracker_nativeTrack__Lcom_seetaface_v6_SeetaImageData_2I
        (JNIEnv *env, jobject obj, jobject image, jint frameIndex) {
//    LOGD("face--- [native_FaceTracker]->nativeTrack");
    if (gFaceTracker == nullptr) {
//        LOGW("face--- [native_FaceTracker]->nativeTrack engine has not been created");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_FaceTracker]->nativeTrack get image data failed");
        return nullptr;
    }

    SeetaTrackingFaceInfoArray trackingFaceInfoArray{};
    try {
        trackingFaceInfoArray = gFaceTracker->Track(nativeImageData, frameIndex);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);

    jobjectArray trackingFaceInfo = nullptr;
    if (trackingFaceInfoArray.size > 0) {
        trackingFaceInfo = gu::ObjectMapping::createTrackingFaceInfoObjectArray(env, trackingFaceInfoArray);
    }
    return trackingFaceInfo;
}