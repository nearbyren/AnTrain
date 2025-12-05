//
// Created by gu on 2021/7/15 0015.
//
#include "com_seetaface_v6_AgePredictor.h"
#include <android/log.h>
#include "AgePredictor.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::AgePredictor *gAgePredictor = nullptr;

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_AgePredictor]->nativeCreateEngine");
    if (gAgePredictor != nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_AgePredictor]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }

    if (result == EXIT_SUCCESS) {
        gAgePredictor = new seeta::AgePredictor(modelSetting);
    }

    result = gAgePredictor != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_AgePredictor]->nativeDestroyEngine");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gAgePredictor;
    gAgePredictor = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeGetCropFaceWidth
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativeGetCropFaceWidth
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_AgePredictor]->nativeGetCropFaceWidth");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeGetCropFaceWidth engine has not been created");
        return 0;
    }
    return gAgePredictor->GetCropFaceWidth();
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeGetCropFaceHeight
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativeGetCropFaceHeight
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_AgePredictor]->nativeGetCropFaceHeight");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeGetCropFaceHeight engine has not been created");
        return 0;
    }
    return gAgePredictor->GetCropFaceHeight();
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeGetCropFaceChannels
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativeGetCropFaceChannels
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_AgePredictor]->nativeGetCropFaceChannels");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeGetCropFaceChannels engine has not been created");
        return 0;
    }
    return gAgePredictor->GetCropFaceChannels();
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeCropFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;Lcom/seetaface/v6/SeetaImageData;)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_AgePredictor_nativeCropFace
        (JNIEnv *env, jobject obj, jobject image,
                jobjectArray points, jobject coppedImage) {
//    LOGD("face--- [native_AgePredictor]->nativeCropFace");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeCropFace engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_AgePredictor]->nativeCropFace get image data failed");
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_AgePredictor]->nativeCropFace get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    SeetaImageData nativeCroppedImageData{};
    nativeCroppedImageData.width = gAgePredictor->GetCropFaceWidth();
    nativeCroppedImageData.height = gAgePredictor->GetCropFaceHeight();
    nativeCroppedImageData.channels = gAgePredictor->GetCropFaceChannels();
    int dataLength = nativeCroppedImageData.width * nativeCroppedImageData.height * nativeCroppedImageData.channels;
    nativeCroppedImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    bool result;
    try {
        result = gAgePredictor->CropFace(nativeImageData, nativePointFs, nativeCroppedImageData);
    } catch (std::exception& e) {
    }

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    gu::ObjectMapping::setFaceImageDataObject(env, nativeCroppedImageData, dataLength, coppedImage);

    // release native image data
    free(nativeCroppedImageData.data);

    return result;
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativePredictAge
 * Signature: (Lcom/seetaface/v6/SeetaImageData;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativePredictAge
        (JNIEnv *env, jobject obj, jobject croppedImage) {
//    LOGD("face--- [native_AgePredictor]->nativePredictAge");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativePredictAge engine has not been created");
        return 0;
    }

    SeetaImageData nativeCroppedImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, croppedImage, &nativeCroppedImageData)) {
//        LOGW("face--- [native_AgePredictor]->nativePredictAge get image data failed");
        return 0;
    }

    int age = 0;
    try {
        gAgePredictor->PredictAge(nativeCroppedImageData, age);
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_AgePredictor]->nativePredictAge age:%d", age);

    // release native image data
    free(nativeCroppedImageData.data);

    return age;
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativePredictAgeWithCrop
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_AgePredictor_nativePredictAgeWithCrop
        (JNIEnv *env, jobject obj, jobject image, jobjectArray points) {
//    LOGD("face--- [native_AgePredictor]->nativePredictAgeWithCrop");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativePredictAgeWithCrop engine has not been created");
        return 0;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_AgePredictor]->nativePredictAgeWithCrop get image data failed");
        return 0;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_AgePredictor]->nativePredictAgeWithCrop get pointFs failed");
        return 0;
    }

    int age = 0;
    try {
        gAgePredictor->PredictAgeWithCrop(nativeImageData, nativePointFs, age);
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_AgePredictor]->nativePredictAgeWithCrop age:%d", age);

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    return age;
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_AgePredictor_nativeSetProperty
    (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_AgePredictor]->nativeSetProperty");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeSetProperty engine has not been created");
        return;
    }
    gAgePredictor->set(static_cast<seeta::AgePredictor::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_AgePredictor
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_AgePredictor_nativeGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_AgePredictor]->nativeGetProperty");
    if (gAgePredictor == nullptr) {
//        LOGW("face--- [native_AgePredictor]->nativeGetProperty engine has not been created");
        return 0;
    }
    return gAgePredictor->get(static_cast<seeta::AgePredictor::Property>(property));
}
