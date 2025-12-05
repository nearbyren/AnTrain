//
// Created by Administrator on 2021/7/16 0016.
//
#include "com_seetaface_v6_MaskDetector.h"
#include <android/log.h>
#include "MaskDetector.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

seeta::MaskDetector *gMaskDetector = nullptr;

/*
 * Class:     com_seetaface_v6_MaskDetector
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_MaskDetector_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("seetaface6--- [native_MaskDetector]->nativeCreateEngine");
    if (gMaskDetector != nullptr) {
//        LOGW("seetaface6--- [native_MaskDetector]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("seetaface6--- [native_MaskDetector]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (result == EXIT_SUCCESS) {
        gMaskDetector = new seeta::MaskDetector(modelSetting);
    }
    result = gMaskDetector != nullptr;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_MaskDetector
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_MaskDetector_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("seetaface6--- [native_MaskDetector]->nativeDestroyEngine");
    if (gMaskDetector == nullptr) {
//        LOGW("seetaface6--- [native_MaskDetector]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gMaskDetector;
    gMaskDetector = nullptr;

    int ret = EXIT_SUCCESS;
    return (jint) ret;
}

/*
 * Class:     com_seetaface_v6_MaskDetector
 * Method:    nativeDetectMask
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[F)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_MaskDetector_nativeDetectMask
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jfloatArray scoreArray) {
//    LOGD("seetaface6--- [native_MaskDetector]->nativeDetectMask");
    if (gMaskDetector == nullptr) {
//        LOGW("seetaface6--- [native_MaskDetector]->nativeDetectMask engine has not been created");
        return EXIT_FAILURE;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("seetaface6--- [native_MaskDetector]->nativeDetectMask get image data failed");
        return JNI_FALSE;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("seetaface6--- [native_MaskDetector]->nativeDetectMask get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    int scoreLength = env->GetArrayLength(scoreArray);
    float *scores = new float[scoreLength];

    bool result = JNI_FALSE;
    try {
        result = gMaskDetector->detect(nativeImageData, nativeFaceRect, scores);
    } catch (std::exception& e) {
    }
//    LOGD("seetaface6--- [native_MaskDetector]->nativeDetectMask result:%d", result);

    // release imageData.data
    free(nativeImageData.data);

    if (result) {
        env->SetFloatArrayRegion(scoreArray, 0, scoreLength, scores);
    }

    delete [] scores;

    return result;
}

