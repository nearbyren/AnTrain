//
// Created by gu on 2021/6/18 0018.
//
#include "com_seetaface_v6_FaceDetector.h"
#include <android/log.h>
#include <android/bitmap.h>
#include "FaceDetector.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::FaceDetector *gFaceDetector;

/*
 * Class:     com_seetaface_v6_FaceDetector
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceDetector_nativeCreateEngine
  (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_detector]->nativeCreateEngine");
    if (gFaceDetector != nullptr) {
//        LOGW("face--- [native_detector]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_detector]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }

    if (result == EXIT_SUCCESS) {
        gFaceDetector = new seeta::FaceDetector(modelSetting);
    }
    result = gFaceDetector != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_FaceDetector
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceDetector_nativeDestroyEngine
  (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_detector]->nativeDestroyEngine");
    if (gFaceDetector == nullptr) {
//        LOGW("face--- [native_detector]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gFaceDetector;
    gFaceDetector = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_FaceDetector
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceDetector_nativeSetProperty
  (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_detector]->nativeSetProperty property:%d, value:%lf", property, value);
    if (gFaceDetector == nullptr) {
//        LOGW("face--- [native_detector]->nativeSetProperty engine has not been created");
        return;
    }
    gFaceDetector->set(static_cast<seeta::FaceDetector::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_FaceDetector
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_FaceDetector_nativeGetProperty
  (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_detector]->nativeGetProperty property:%d", property);
    if (gFaceDetector == nullptr) {
//        LOGW("seetaface6--- [native_detector]->nativeGetProperty engine has not been created");
        return 0;
    }
    return gFaceDetector->get(static_cast<seeta::FaceDetector::Property>(property));
}

/*
 * Class:     com_seetaface_v6_FaceDetector
 * Method:    nativeDetectFaces
 * Signature: (Lcom/seetaface/v6/SeetaImageData;)[Lcom/seetaface/v6/SeetaFaceInfo;
 */
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_seetaface_v6_FaceDetector_nativeDetectFaces
  (JNIEnv *env, jobject obj, jobject image) {
//    LOGD("seetaface6--- [native_detector]->nativeDetectFaces");
    if (gFaceDetector == nullptr) {
//        LOGW("seetaface6--- [native_detector]->nativeDetectFaces Have you initialized the engine?");
        return nullptr;
    }

    // create native struct
    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("seetaface6--- [native_AgePredictor]->nativeDetectFaces get image data failed");
        return JNI_FALSE;
    }

    SeetaFaceInfoArray faces;
    try {
        faces = gFaceDetector->detect(nativeImageData);
    } catch (std::exception& e) {
    }

    // release imageData.data
    free(nativeImageData.data);

    jobjectArray jFaceInfoArray = nullptr;
    if (faces.size > 0) {
//        LOGD("seetaface6--- [native_detector]->nativeDetectFaces Detect %d faces", faces.size);
        jFaceInfoArray = gu::ObjectMapping::createFaceInfoObjectArray(env, faces);
    } else {
//        LOGD("seetaface6--- [native_detector]->nativeDetectFaces Detect no faces");
    }

    return jFaceInfoArray;
}