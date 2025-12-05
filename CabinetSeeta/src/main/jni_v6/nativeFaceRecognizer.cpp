//
// Created by gu on 2021/6/18 0018.
//
#include "com_seetaface_v6_FaceRecognizer.h"

#include <android/log.h>
#include "FaceRecognizer.h"
#include "ObjectMapping.h"

#define GJ_DEBUG

#ifdef GJ_DEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#else
#define LOGD(...)
#endif
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::FaceRecognizer *gFaceRecognizer;

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCreateEngine
  (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("seetaface6--- [native_recognizer]->createEngine");

    if (gFaceRecognizer != nullptr) {
//        LOGW("seetaface6--- [native_recognizer]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("seetaface6--- [native_recognizer]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }

    if (result == EXIT_SUCCESS) {
        gFaceRecognizer = new seeta::FaceRecognizer(modelSetting);
    }
    result = gFaceRecognizer != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeDestroyEngine(JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeDestroyEngine");

    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_landmarker]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gFaceRecognizer;
    gFaceRecognizer = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceWidth
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceWidth
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceWidth");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceWidth engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceWidth();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceHeight
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceHeight
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceHeight");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceHeight engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceHeight();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceChannels
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceChannels
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceChannels");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceChannels engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceChannels();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCropFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;Lcom/seetaface/v6/SeetaImageData;)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCropFace__Lcom_seetaface_v6_SeetaImageData_2_3Lcom_seetaface_v6_SeetaPointF_2Lcom_seetaface_v6_SeetaImageData_2
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jobjectArray pointFArray, jobject croppedImageDataObj) {
//    LOGD("face--- [native_recognizer]->nativeCropFace");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFace engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeCropFace get image data failed");
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(pointFArray);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, pointFArray, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_recognizer]->nativeCropFace get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    SeetaImageData nativeCroppedImageData{};
    nativeCroppedImageData.width = gFaceRecognizer->GetCropFaceWidthV2();
    nativeCroppedImageData.height = gFaceRecognizer->GetCropFaceHeightV2();
    nativeCroppedImageData.channels = gFaceRecognizer->GetCropFaceChannelsV2();
    int dataLength = nativeCroppedImageData.width * nativeCroppedImageData.height * nativeCroppedImageData.channels;
    nativeCroppedImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    bool result = JNI_FALSE;
    try {
        result = gFaceRecognizer->CropFace(nativeImageData, nativePointFs, nativeCroppedImageData);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    gu::ObjectMapping::setFaceImageDataObject(env, nativeCroppedImageData, dataLength, croppedImageDataObj);

    // release native image data
    free(nativeCroppedImageData.data);

    return result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCropFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;)Lcom/seetaface/v6/SeetaImageData;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCropFace__Lcom_seetaface_v6_SeetaImageData_2_3Lcom_seetaface_v6_SeetaPointF_2
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jobjectArray pointFArray) {
//    LOGD("face--- [native_recognizer]->nativeCropFace");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFace engine has not been created");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeCropFace get image data failed");
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(pointFArray);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, pointFArray, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_recognizer]->nativeCropFace get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return nullptr;
    }

    seeta::ImageData croppedImageData;
    try {
        croppedImageData = gFaceRecognizer->CropFace(nativeImageData, nativePointFs);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    if (croppedImageData.width == 0 || croppedImageData.height == 0
        || croppedImageData.data == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFace crop failed");
        return nullptr;
    }
    int dataLength = croppedImageData.count();
    jobject imageData = gu::ObjectMapping::createFaceImageDataObject(env, croppedImageData, dataLength);
    return imageData;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceWidthV2
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceWidthV2
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceWidthV2");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceWidthV2 engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceWidthV2();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceHeightV2
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceHeightV2
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceHeightV2");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceHeightV2 engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceHeightV2();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetCropFaceChannelsV2
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetCropFaceChannelsV2
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetCropFaceChannelsV2");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetCropFaceChannelsV2 engine has not been created");
        return JNI_FALSE;
    }
    return gFaceRecognizer->GetCropFaceChannelsV2();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCropFaceV2
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;Lcom/seetaface/v6/SeetaImageData;)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCropFaceV2__Lcom_seetaface_v6_SeetaImageData_2_3Lcom_seetaface_v6_SeetaPointF_2Lcom_seetaface_v6_SeetaImageData_2
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jobjectArray pointFArray, jobject croppedImageDataObj) {
//    LOGD("face--- [native_recognizer]->nativeCropFaceV2");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 get image data failed");
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(pointFArray);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, pointFArray, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    SeetaImageData nativeCroppedImageData{};
    nativeCroppedImageData.width = gFaceRecognizer->GetCropFaceWidthV2();
    nativeCroppedImageData.height = gFaceRecognizer->GetCropFaceHeightV2();
    nativeCroppedImageData.channels = gFaceRecognizer->GetCropFaceChannelsV2();
    int dataLength = nativeCroppedImageData.width * nativeCroppedImageData.height * nativeCroppedImageData.channels;
    nativeCroppedImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    bool result;
    try {
        result = gFaceRecognizer->CropFaceV2(nativeImageData, nativePointFs, nativeCroppedImageData);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    gu::ObjectMapping::setFaceImageDataObject(env, nativeCroppedImageData, dataLength, croppedImageDataObj);

    // release native image data
    free(nativeCroppedImageData.data);

    return result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCropFaceV2
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;)Lcom/seetaface/v6/SeetaImageData;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCropFaceV2__Lcom_seetaface_v6_SeetaImageData_2_3Lcom_seetaface_v6_SeetaPointF_2
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jobjectArray pointFArray) {
//    LOGD("face--- [native_recognizer]->nativeCropFaceV2");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 engine has not been created");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 get image data failed");
        return nullptr;
    }

    int pointFLength = env->GetArrayLength(pointFArray);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, pointFArray, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return nullptr;
    }

    seeta::ImageData croppedImageData;
    try {
        croppedImageData = gFaceRecognizer->CropFaceV2(nativeImageData, nativePointFs);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    if (croppedImageData.width == 0 || croppedImageData.height == 0
        || croppedImageData.data == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCropFaceV2 crop failed");
        return nullptr;
    }
    int dataLength = croppedImageData.count();
    jobject imageData = gu::ObjectMapping::createFaceImageDataObject(env, croppedImageData, dataLength);
    return imageData;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetExtractFeatureSize
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetExtractFeatureSize
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_recognizer]->nativeGetExtractFeatureSize");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeGetExtractFeatureSize engine has not been created");
        return 0;
    }
    return gFaceRecognizer->GetExtractFeatureSize();
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeExtractFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;[F)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeExtractFace
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jobjectArray pointFArray, jfloatArray featureArray) {
//    LOGD("face--- [native_recognizer]->nativeExtractCropFace");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeExtractCropFace engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeExtractCropFace get image data failed");
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(pointFArray);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, pointFArray, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_recognizer]->nativeExtractCropFace get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    int nativeFeatureLength = gFaceRecognizer->GetExtractFeatureSize();
    float *nativeFeatures = new float[nativeFeatureLength];

    bool result = JNI_FALSE;
    try {
        result = gFaceRecognizer->Extract(nativeImageData, nativePointFs, nativeFeatures);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);

    if (result) {
        env->SetFloatArrayRegion(featureArray, 0, nativeFeatureLength, nativeFeatures);
    }

    delete [] nativeFeatures;

    return result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeExtractCropFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[F)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeExtractCropFace
        (JNIEnv *env, jobject obj, jobject imageDataObj,
                jfloatArray featureArray) {
//    LOGD("face--- [native_recognizer]->nativeExtractCropFace");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeExtractCropFace engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, imageDataObj, &nativeImageData)) {
//        LOGW("face--- [native_recognizer]->nativeExtractCropFace get image data failed");
        return JNI_FALSE;
    }

    int nativeFeatureLength = gFaceRecognizer->GetExtractFeatureSize();
    float *nativeFeatures = new float[nativeFeatureLength];

    bool result = JNI_FALSE;
    try {
        result = gFaceRecognizer->ExtractCroppedFace(nativeImageData, nativeFeatures);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);

    if (result) {
        env->SetFloatArrayRegion(featureArray, 0, nativeFeatureLength, nativeFeatures);
    }

    delete [] nativeFeatures;

    return result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeCompareFace
 * Signature: ([F[F)F
 */
extern "C" JNIEXPORT jfloat JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeCompareFace
        (JNIEnv *env, jobject obj, jfloatArray featureArray1, jfloatArray featureArray2) {
//    LOGD("face--- [native_recognizer]->nativeCompareFace");
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->nativeCompareFace engine has not been created");
        return 0;
    }
    jboolean isCopy;
    float *features1 = env->GetFloatArrayElements(featureArray1, &isCopy);
    float *features2 = env->GetFloatArrayElements(featureArray2, &isCopy);

    float result = 0;
    try {
        result = gFaceRecognizer->CalculateSimilarity(features1, features2);
    } catch (std::exception& e) {
    }

    env->ReleaseFloatArrayElements(featureArray1, features1, 0);
    env->ReleaseFloatArrayElements(featureArray2, features2, 0);

    return result;
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeSetProperty
        (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_recognizer]->setProperty property:%d, value:%lf", property, value);
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->setProperty engine has not been created");
        return;
    }
    gFaceRecognizer->set(static_cast<seeta::FaceRecognizer::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_FaceRecognizer
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_FaceRecognizer_nativeGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_recognizer]->getProperty property:%d", property);
    if (gFaceRecognizer == nullptr) {
//        LOGW("face--- [native_recognizer]->getProperty engine has not been created");
        return 0;
    }
    return gFaceRecognizer->get(static_cast<seeta::FaceRecognizer::Property>(property));
}
