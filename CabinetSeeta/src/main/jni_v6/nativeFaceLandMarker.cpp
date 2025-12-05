//
// Created by gu on 2021/6/22 0022.
//
#include "com_seetaface_v6_FaceLandMarker.h"

#include <android/log.h>
#include "FaceLandmarker.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::FaceLandmarker *gFaceLandmarker;

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("seetaface6--- [native_landmarker]->nativeCreateEngine");

    if (gFaceLandmarker != nullptr) {
//        LOGW("seetaface6--- [native_landmarker]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("seetaface6--- [native_landmarker]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }

    if (result == EXIT_SUCCESS) {
        gFaceLandmarker = new seeta::FaceLandmarker(modelSetting);
    }
    result = gFaceLandmarker != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("seetaface6--- [native_landmarker]->nativeDestroyEngine");
    if (gFaceLandmarker == nullptr) {
//        LOGW("seetaface6--- [native_landmarker]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gFaceLandmarker;
    gFaceLandmarker = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeGetPointsLength
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeGetPointsLength
        (JNIEnv *env, jobject obj) {
//    LOGD("seetaface6--- [native_landmarker]->nativeGetPointsLength");
    if (gFaceLandmarker == nullptr) {
//        LOGW("seetaface6--- [native_landmarker]->nativeGetPointsLength engine has not been created");
        return 0;
    }
    return gFaceLandmarker->number();
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeGetFacePoints
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;)[Lcom/seetaface/v6/SeetaPointF;
 */
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeGetFacePoints__Lcom_seetaface_v6_SeetaImageData_2Lcom_seetaface_v6_SeetaRect_2
        (JNIEnv *env, jobject obj, jobject image, jobject rect) {
//    LOGD("seetaface6--- [native_landmarker]->nativeGetFacePoints");
    if (gFaceLandmarker == nullptr) {
//        LOGW("seetaface6--- [native_landmarker]->nativeGetFacePoints engine has not been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("seetaface6--- [native_landmarker]->nativeGetFacePoints get image data failed");
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("seetaface6--- [native_landmarker]->nativeGetFacePoints get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    std::vector<SeetaPointF> points;
    try {
        points = gFaceLandmarker->mark(nativeImageData, nativeFaceRect);
    } catch (std::exception& e) {
    }

    // release imageData.data
    free(nativeImageData.data);

    jobjectArray jFacePointArray = nullptr;
    if (!points.empty()) {
        jFacePointArray = gu::ObjectMapping::createPointFArrayObject(env, points);
    }

    return jFacePointArray;
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeGetFacePoints
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointF;)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeGetFacePoints__Lcom_seetaface_v6_SeetaImageData_2Lcom_seetaface_v6_SeetaRect_2_3Lcom_seetaface_v6_SeetaPointF_2
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray pointsArray) {
//    LOGD("face--- [native_landmarker]->nativeGetFacePoints");
    if (gFaceLandmarker == nullptr) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePoints engine has not been created");
        return;
    }

    if (pointsArray == nullptr || env->GetArrayLength(pointsArray) == 0) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePoints points array is empty");
        return;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePoints get image data failed");
        return;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePoints get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return;
    }

    int length = env->GetArrayLength(pointsArray);
    SeetaPointF *seetaPointF = new SeetaPointF[length];
    try {
        gFaceLandmarker->mark(nativeImageData, nativeFaceRect, seetaPointF);
    } catch (std::exception& e) {
    }

    // release imageData.data
    free(nativeImageData.data);

    gu::ObjectMapping::setPointFArrayObject(env, seetaPointF, length, pointsArray);

    delete [] seetaPointF;
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeGetFacePointsWithMask
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;)[Lcom/seetaface/v6/SeetaPointsWithMask;
 */
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeGetFacePointsWithMask__Lcom_seetaface_v6_SeetaImageData_2Lcom_seetaface_v6_SeetaRect_2
        (JNIEnv *env, jobject obj, jobject image, jobject rect) {
//    LOGD("face--- [native_landmarker]->nativeGetFacePointsWithMask");
    if (gFaceLandmarker == nullptr) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePointsWithMask engine has not been created");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePointsWithMask get image data failed");
        return nullptr;
    }

    SeetaRect nativeFaceRect{};
    if (!gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect)) {
//        LOGW("face--- [native_landmarker]->nativeGetFacePointsWithMask get rect failed");
        // release imageData.data
        free(nativeImageData.data);
        return nullptr;
    }

    std::vector<seeta::v6::FaceLandmarker::PointWithMask> points;
    try {
        points = gFaceLandmarker->mark_v2(nativeImageData, nativeFaceRect);
    } catch (std::exception& e) {
    }

    // release imageData.data
    free(nativeImageData.data);

    jobjectArray jFacePointArray = nullptr;
    if (!points.empty()) {
        jFacePointArray = gu::ObjectMapping::createPointFWithMaskArrayObject(env, points);
    }
    return jFacePointArray;
}

/*
 * Class:     com_seetaface_v6_FaceLandMarker
 * Method:    nativeGetFacePointsWithMask
 * Signature: (Lcom/seetaface/v6/SeetaImageData;Lcom/seetaface/v6/SeetaRect;[Lcom/seetaface/v6/SeetaPointsWithMask;)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceLandMarker_nativeGetFacePointsWithMask__Lcom_seetaface_v6_SeetaImageData_2Lcom_seetaface_v6_SeetaRect_2_3Lcom_seetaface_v6_SeetaPointsWithMask_2
        (JNIEnv *env, jobject obj, jobject image,
                jobject rect, jobjectArray pointsArray) {
//    LOGD("face--- [native_landmarker]->nativeGetFacePointsWithMask");
    if (gFaceLandmarker == nullptr) {
        return;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData);

    SeetaRect nativeFaceRect{};
    gu::ObjectMapping::getSeetaRect(env, rect, &nativeFaceRect);

    int length = env->GetArrayLength(pointsArray);
    SeetaPointF *seetaPointF = new SeetaPointF[length];
    int32_t *maskArray = new int32_t[length];
    try {
        gFaceLandmarker->mark(nativeImageData, nativeFaceRect, seetaPointF, maskArray);
    } catch (std::exception& e) {
    }

    // release imageData.data
    free(nativeImageData.data);

    gu::ObjectMapping::setPointFWithMaskArrayObject(env, seetaPointF, maskArray, length, pointsArray);

    delete [] seetaPointF;
    delete [] maskArray;
}
