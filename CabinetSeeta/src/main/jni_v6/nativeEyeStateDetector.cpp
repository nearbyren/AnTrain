//
// Created by Administrator on 2021/7/15 0015.
//
#include "com_seetaface_v6_EyeStateDetector.h"
#include <android/log.h>
#include "EyeStateDetector.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::EyeStateDetector *gEyeStateDetector = nullptr;

/*
 * Class:     com_seetaface_v6_EyeStateDetector
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_EyeStateDetector_nativeCreateEngine
    (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_EyeStateDetector]->nativeCreateEngine");
    if (gEyeStateDetector != nullptr) {
//        LOGW("face--- [native_EyeStateDetector]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_EyeStateDetector]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }

    if (result == EXIT_SUCCESS) {
        gEyeStateDetector = new seeta::EyeStateDetector(modelSetting);
    }

    result = gEyeStateDetector != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_EyeStateDetector
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_EyeStateDetector_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_EyeStateDetector]->nativeDestroyEngine");
    if (gEyeStateDetector == nullptr) {
//        LOGW("face--- [native_EyeStateDetector]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gEyeStateDetector;
    gEyeStateDetector = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_EyeStateDetector
 * Method:    nativeEyeStateDetect
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;Ljava/lang/Integer;Ljava/lang/Integer;)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_EyeStateDetector_nativeEyeStateDetect
        (JNIEnv *env, jobject obj, jobject image,
                jobjectArray points, jobject leftObj, jobject rightObj) {
//    LOGD("face--- [native_EyeStateDetector]->nativeEyeStateDetect");
    if (gEyeStateDetector == nullptr) {
//        LOGW("face--- [native_EyeStateDetector]->nativeEyeStateDetect engine has not been created");
        return;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_EyeStateDetector]->nativeEyeStateDetect get image data failed");
        return;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_EyeStateDetector]->nativeEyeStateDetect get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return;
    }

    seeta::EyeStateDetector::EYE_STATE leftEyeState = seeta::EyeStateDetector::EYE_STATE::EYE_UNKNOWN;
    seeta::EyeStateDetector::EYE_STATE rightEyeState = seeta::EyeStateDetector::EYE_STATE::EYE_UNKNOWN;
    try {
        gEyeStateDetector->Detect(nativeImageData, nativePointFs, leftEyeState, rightEyeState);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jclass integerClass = env->FindClass("java/lang/Integer");
    jfieldID valueField = env->GetFieldID(integerClass, "value", "I");
    env->SetIntField(leftObj, valueField, (jint)leftEyeState);
    env->SetIntField(rightObj, valueField, (jint)rightEyeState);
    env->DeleteLocalRef(integerClass);
}

/*
 * Class:     com_seetaface_v6_EyeStateDetector
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_EyeStateDetector_nativeSetProperty
        (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_EyeStateDetector]->nativeSetProperty");
    if (gEyeStateDetector == nullptr) {
//        LOGW("face--- [native_EyeStateDetector]->nativeSetProperty engine has not been created");
        return;
    }
    gEyeStateDetector->set(static_cast<seeta::EyeStateDetector::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_EyeStateDetector
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_EyeStateDetector_nativeGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_EyeStateDetector]->nativeGetProperty");
    if (gEyeStateDetector == nullptr) {
//        LOGW("face--- [native_EyeStateDetector]->nativeGetProperty engine has not been created");
        return 0;
    }
    return gEyeStateDetector->get(static_cast<seeta::EyeStateDetector::Property>(property));
}