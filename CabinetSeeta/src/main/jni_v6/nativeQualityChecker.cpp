//
// Created by Administrator on 2021/7/16 0016.
//
#include "com_seetaface_v6_QualityChecker.h"
#include <android/log.h>
#include "QualityAssessor.h"
#include "QualityOfBrightness.h"
#include "QualityOfClarity.h"
#include "QualityOfIntegrity.h"
#include "QualityOfLBN.h"
#include "QualityOfPose.h"
#include "QualityOfPoseEx.h"
#include "QualityOfResolution.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

seeta::QualityOfBrightness *gQualityBrightness = nullptr;

seeta::QualityOfClarity *gQualityClarity = nullptr;

seeta::QualityOfLBN *gQualityLBN = nullptr;

seeta::QualityOfPoseEx *gQualityPoseEx = nullptr;

seeta::QualityOfResolution *gQualityResolution = nullptr;

seeta::QualityOfIntegrity *gQualityIntegrity = nullptr;

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartBrightnessChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartBrightnessChecker__
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStartBrightnessChecker");
    if (gQualityBrightness != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartBrightnessChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityBrightness = new seeta::QualityOfBrightness();
    int result = gQualityBrightness != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartBrightnessChecker
 * Signature: (FFFF)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartBrightnessChecker__FFFF
        (JNIEnv *env, jobject obj, jfloat v0, jfloat v1, jfloat v2, jfloat v3) {
//    LOGD("face--- [native_QualityChecker]->nativeStartBrightnessChecker");
    if (gQualityBrightness != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartBrightnessChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityBrightness = new seeta::QualityOfBrightness(v0, v1, v2, v3);
    int result = gQualityBrightness != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeBrightnessCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativeBrightnessCheck
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativeBrightnessCheck");
    if (gQualityBrightness == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeBrightnessCheck engine has not been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeBrightnessCheck get image data failed");
        return JNI_FALSE;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativeBrightnessCheck get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeBrightnessCheck get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    seeta::QualityResult result;
    try {
        result = gQualityBrightness->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopBrightnessChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopBrightnessChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopBrightnessChecker");
    if (gQualityBrightness == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopBrightnessChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityBrightness;
    gQualityBrightness = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartClarityChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartClarityChecker__
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStartClarityChecker");
    if (gQualityClarity != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartClarityChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityClarity = new seeta::QualityOfClarity();
    int result = gQualityClarity != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartClarityChecker
 * Signature: (FF)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartClarityChecker__FF
        (JNIEnv *env, jobject obj, jfloat low, jfloat high) {
//    LOGD("face--- [native_QualityChecker]->nativeStartClarityChecker");
    if (gQualityClarity != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartClarityChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityClarity = new seeta::QualityOfClarity(low, high);
    int result = gQualityClarity != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeClarityCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativeClarityCheck
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativeClarityCheck");
    if (gQualityClarity == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeClarityCheck engine has not been createdd");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeClarityCheck get image data failed");
        return JNI_FALSE;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativeClarityCheck get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeClarityCheck get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    seeta::QualityResult result;
    try {
        result = gQualityClarity->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopClarityChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopClarityChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopClarityChecker");
    if (gQualityClarity == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopClarityChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityClarity;
    gQualityClarity = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartLBNChecker
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartLBNChecker
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_QualityChecker]->nativeStartLBNChecker");
    if (gQualityLBN != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartLBNChecker engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_MaskDetector]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (result == EXIT_SUCCESS) {
        gQualityLBN = new seeta::QualityOfLBN(modelSetting);
    }
    result = gQualityLBN != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeLBNDetect
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;[I[I[I)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_QualityChecker_nativeLBNDetect
        (JNIEnv *env, jobject obj, jobject image,
                jobjectArray points, jintArray lightArray,
                jintArray blurArray, jintArray noiseArray) {
//    LOGD("face--- [native_QualityChecker]->nativeLBNDetect");
    if (gQualityLBN == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeLBNDetect engine has not been created");
        return;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeLBNDetect get image data failed");
        return;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeLBNDetect get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return;
    }

    int lightLen = env->GetArrayLength(lightArray);
    int *nativeLightArray = new int[lightLen];
    int blurLen = env->GetArrayLength(blurArray);
    int *nativeBlurArray = new int[blurLen];
    int noiseLen = env->GetArrayLength(noiseArray);
    int *nativeNoiseArray = new int[noiseLen];

    try {
        gQualityLBN->Detect(nativeImageData, nativePointFs, nativeLightArray, nativeBlurArray, nativeNoiseArray);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    env->SetIntArrayRegion(lightArray, 0, lightLen, nativeLightArray);
    env->SetIntArrayRegion(blurArray, 0, blurLen, nativeBlurArray);
    env->SetIntArrayRegion(noiseArray, 0, noiseLen, nativeNoiseArray);

    delete [] nativeLightArray;
    delete [] nativeBlurArray;
    delete [] nativeNoiseArray;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeLBNSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_QualityChecker_nativeLBNSetProperty
        (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_QualityChecker]->nativeLBNSetProperty");
    if (gQualityLBN == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeLBNSetProperty engine has not been created");
        return;
    }
    gQualityLBN->set(static_cast<seeta::QualityOfLBN::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeLBNGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_QualityChecker_nativeLBNGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_QualityChecker]->nativeLBNGetProperty");
    if (gQualityLBN == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeLBNGetProperty engine has not been created");
        return 0;
    }
    return gQualityLBN->get(static_cast<seeta::QualityOfLBN::Property>(property));
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopLBNChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopLBNChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopLBNChecker");
    if (gQualityLBN == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopLBNChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityLBN;
    gQualityLBN = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativePoseCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativePoseCheck
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativePoseCheck");
    seeta::QualityOfPose *pPose = new seeta::QualityOfPose();
    if (pPose == nullptr) {
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativePoseCheck get image data failed");
        // delete native object
        delete pPose;
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativePoseCheck get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        // delete native object
        delete pPose;
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativePoseCheck get points failed");
        // release imageData.data
        free(nativeImageData.data);
        // delete native object
        delete pPose;
        return nullptr;
    }

    seeta::QualityResult result;
    try {
        result = pPose->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;
    // delete native object
    delete pPose;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartPoseExChecker
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartPoseExChecker
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_QualityChecker]->nativeStartPoseExChecker");
    if (gQualityPoseEx != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartPoseExChecker engine has been created");
        return EXIT_FAILURE;
    }
    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_MaskDetector]->nativeStartPoseExChecker modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (result == EXIT_SUCCESS) {
        gQualityPoseEx = new seeta::QualityOfPoseEx(modelSetting);
    }
    result = gQualityPoseEx != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativePoseExCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativePoseExCheck
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativeStartPoseExChecker");
    if (gQualityPoseEx == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartPoseExChecker engine has not been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeStartPoseExChecker get image data failed");
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativeStartPoseExChecker get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeStartPoseExChecker get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    seeta::QualityResult result;
    try {
        result = gQualityPoseEx->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativePoseExSetProperty
 * Signature: (IF)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_QualityChecker_nativePoseExSetProperty
        (JNIEnv *env, jobject obj, jint property, jfloat value) {
//    LOGD("face--- [native_QualityChecker]->nativePoseExSetProperty");
    if (gQualityPoseEx == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativePoseExSetProperty engine has not been created");
        return;
    }
    gQualityPoseEx->set(static_cast<seeta::QualityOfPoseEx::PROPERTY>(property), value);
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativePoseExGetProperty
 * Signature: (I)F
 */
extern "C" JNIEXPORT jfloat JNICALL Java_com_seetaface_v6_QualityChecker_nativePoseExGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_QualityChecker]->nativePoseExGetProperty");
    if (gQualityPoseEx == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativePoseExGetProperty engine has not been created");
        return 0;
    }
    return gQualityPoseEx->get(static_cast<seeta::QualityOfPoseEx::PROPERTY>(property));
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopPoseExChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopPoseExChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopPoseExChecker");
    if (gQualityPoseEx == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopPoseExChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityPoseEx;
    gQualityPoseEx = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartResolutionChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartResolutionChecker__
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStartResolutionChecker");
    if (gQualityResolution != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartResolutionChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityResolution = new seeta::QualityOfResolution();
    int result = gQualityResolution != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint) result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartResolutionChecker
 * Signature: (FF)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartResolutionChecker__FF
        (JNIEnv *env, jobject obj, jfloat low, jfloat high) {
//    LOGD("face--- [native_QualityChecker]->nativeStartResolutionChecker");
    if (gQualityResolution != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartResolutionChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityResolution = new seeta::QualityOfResolution(low, high);
    int result = gQualityResolution != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint) result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeResolutionCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativeResolutionCheck
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativeResolutionCheck");
    if (gQualityResolution == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeResolutionCheck engine has been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeResolutionCheck get image data failed");
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativeResolutionCheck get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeResolutionCheck get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    seeta::QualityResult result;
    try {
        result = gQualityResolution->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopResolutionChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopResolutionChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopResolutionChecker");
    if (gQualityResolution == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopResolutionChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityResolution;
    gQualityResolution = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartIntegrityChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartIntegrityChecker__
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStartIntegrityChecker");
    if (gQualityIntegrity != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartIntegrityChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityIntegrity = new seeta::QualityOfIntegrity();
    int result = gQualityIntegrity != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStartIntegrityChecker
 * Signature: (FF)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStartIntegrityChecker__FF
        (JNIEnv *env, jobject obj, jfloat low, jfloat high) {
//    LOGD("face--- [native_QualityChecker]->nativeStartIntegrityChecker");
    if (gQualityIntegrity != nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStartIntegrityChecker engine has been created");
        return EXIT_FAILURE;
    }
    gQualityIntegrity = new seeta::QualityOfIntegrity(low, high);
    int result = gQualityIntegrity != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeIntegrityCheck
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;I)Lcom/seetaface/v6/QualityResult;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_QualityChecker_nativeIntegrityCheck
        (JNIEnv *env, jobject obj, jobject image,
         jobject rect, jobjectArray points, jint featureSize) {
//    LOGD("face--- [native_QualityChecker]->nativeIntegrityCheck");
    if (gQualityIntegrity == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeIntegrityCheck engine has been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_QualityChecker]->nativeIntegrityCheck get image data failed");
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_QualityChecker]->nativeIntegrityCheck get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_QualityChecker]->nativeIntegrityCheck get points failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    seeta::QualityResult result;
    try {
        result = gQualityIntegrity->check(nativeImageData, nativeFaceRect, nativePointFs, featureSize);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    jobject resultObj = gu::ObjectMapping::createQualityResultObject(env, result);
    return resultObj;
}

/*
 * Class:     com_seetaface_v6_QualityChecker
 * Method:    nativeStopIntegrityChecker
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_QualityChecker_nativeStopIntegrityChecker
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_QualityChecker]->nativeStopIntegrityChecker");
    if (gQualityIntegrity == nullptr) {
//        LOGW("face--- [native_QualityChecker]->nativeStopIntegrityChecker engine has not been created");
        return EXIT_FAILURE;
    }

    delete gQualityIntegrity;
    gQualityIntegrity = nullptr;

    return (jint) EXIT_SUCCESS;
}