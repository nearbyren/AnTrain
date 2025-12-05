//
// Created by Administrator on 2021/7/15 0015.
//
#include "com_seetaface_v6_GenderPredictor.h"
#include <android/log.h>
#include "GenderPredictor.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

static seeta::GenderPredictor *gGenderPredictor = nullptr;

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeCreateEngine
 * Signature: (Ljava/lang/String;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativeCreateEngine
        (JNIEnv *env, jobject obj, jstring model) {
//    LOGD("face--- [native_GenderPredictor]->nativeCreateEngine");
    if (gGenderPredictor != nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeCreateEngine engine has been created");
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;
    seeta::ModelSetting modelSetting(seeta::ModelSetting::AUTO, 0);
    if (model != nullptr) {
        const char *modelFile = env->GetStringUTFChars(model, 0);
//        LOGD("face--- [native_GenderPredictor]->nativeCreateEngine modelFile:%s", modelFile);
        if (modelFile != nullptr) {
            modelSetting.append(modelFile);
            env->ReleaseStringUTFChars(model, modelFile);
            result = EXIT_SUCCESS;
        }
    }
    if (result == EXIT_SUCCESS) {
        gGenderPredictor = new seeta::GenderPredictor(modelSetting);
    }
    result = gGenderPredictor != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
    return (jint)result;
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeDestroyEngine
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativeDestroyEngine
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_GenderPredictor]->nativeDestroyEngine");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeDestroyEngine engine has not been created");
        return EXIT_FAILURE;
    }

    delete gGenderPredictor;
    gGenderPredictor = nullptr;

    return (jint) EXIT_SUCCESS;
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeGetCropFaceWidth
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativeGetCropFaceWidth
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_GenderPredictor]->nativeGetCropFaceWidth");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeGetCropFaceWidth engine has not been created");
        return 0;
    }
    return gGenderPredictor->GetCropFaceWidth();
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeGetCropFaceHeight
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativeGetCropFaceHeight
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_GenderPredictor]->nativeGetCropFaceHeight");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeGetCropFaceHeight engine has not been created");
        return 0;
    }
    return gGenderPredictor->GetCropFaceHeight();
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeGetCropFaceChannels
 * Signature: ()I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativeGetCropFaceChannels
        (JNIEnv *env, jobject obj) {
//    LOGD("face--- [native_GenderPredictor]->nativeGetCropFaceChannels");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeGetCropFaceChannels engine has not been created");
        return 0;
    }
    return gGenderPredictor->GetCropFaceChannels();
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeCropFace
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;Lcom/seetaface/v6/SeetaImageData;)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_com_seetaface_v6_GenderPredictor_nativeCropFace
        (JNIEnv *env, jobject obj, jobject image,
                jobjectArray points, jobject coppedImage) {
//    LOGD("face--- [native_GenderPredictor]->nativeCropFace");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeCropFace engine has not been created");
        return JNI_FALSE;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_GenderPredictor]->nativeCropFace get image data failed");
        return JNI_FALSE;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_GenderPredictor]->nativeCropFace get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return JNI_FALSE;
    }

    SeetaImageData nativeCroppedImageData{};
    nativeCroppedImageData.width = gGenderPredictor->GetCropFaceWidth();
    nativeCroppedImageData.height = gGenderPredictor->GetCropFaceHeight();
    nativeCroppedImageData.channels = gGenderPredictor->GetCropFaceChannels();
    int dataLength = nativeCroppedImageData.width * nativeCroppedImageData.height * nativeCroppedImageData.channels;
    nativeCroppedImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    bool result;
    try {
        result = gGenderPredictor->CropFace(nativeImageData, nativePointFs, nativeCroppedImageData);
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
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativePredictGender
 * Signature: (Lcom/seetaface/v6/SeetaImageData;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativePredictGender
        (JNIEnv *env, jobject obj, jobject image) {
//    LOGD("face--- [native_GenderPredictor]->nativePredictGender");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativePredictGender engine has not been created");
        return -1;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_GenderPredictor]->nativeCropFace get image data failed");
        return -1;
    }

    seeta::GenderPredictor::GENDER gender;
    try {
        gGenderPredictor->PredictGender(nativeImageData, gender);
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_GenderPredictor]->nativePredictGender gender:%d", gender);

    // release native image data
    free(nativeImageData.data);

    return gender;
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativePredictGenderWithCrop
 * Signature: (Lcom/seetaface/v6/SeetaImageData;[Lcom/seetaface/v6/SeetaPointF;)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_seetaface_v6_GenderPredictor_nativePredictGenderWithCrop
        (JNIEnv *env, jobject obj, jobject image,
                jobjectArray points) {
//    LOGD("face--- [native_GenderPredictor]->nativePredictGenderWithCrop");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativePredictGenderWithCrop engine has not been created");
        return -1;
    }

    SeetaImageData nativeImageData{};
    if (!gu::ObjectMapping::getSeetaImageData(env, image, &nativeImageData)) {
//        LOGW("face--- [native_GenderPredictor]->nativePredictGenderWithCrop get image data failed");
        return -1;
    }

    int pointFLength = env->GetArrayLength(points);
    SeetaPointF* nativePointFs = new SeetaPointF[pointFLength];
    if (!gu::ObjectMapping::getSeetaPointFArray(env, points, nativePointFs, pointFLength)) {
//        LOGW("face--- [native_AgePredictor]->nativePredictGenderWithCrop get pointFs failed");
        // release native image data
        free(nativeImageData.data);
        return -1;
    }

    seeta::GenderPredictor::GENDER gender;
    try {
        gGenderPredictor->PredictGenderWithCrop(nativeImageData, nativePointFs, gender);
    } catch (std::exception& e) {
    }
//    LOGD("face--- [native_GenderPredictor]->nativePredictGenderWithCrop gender:%d", gender);

    // release native image data
    free(nativeImageData.data);
    // delete native array
    delete [] nativePointFs;

    return gender;
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeSetProperty
 * Signature: (ID)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_seetaface_v6_GenderPredictor_nativeSetProperty
        (JNIEnv *env, jobject obj, jint property, jdouble value) {
//    LOGD("face--- [native_GenderPredictor]->nativeSetProperty");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeSetProperty engine has not been created");
        return;
    }
    gGenderPredictor->set(static_cast<seeta::GenderPredictor::Property>(property), value);
}

/*
 * Class:     com_seetaface_v6_GenderPredictor
 * Method:    nativeGetProperty
 * Signature: (I)D
 */
extern "C" JNIEXPORT jdouble JNICALL Java_com_seetaface_v6_GenderPredictor_nativeGetProperty
        (JNIEnv *env, jobject obj, jint property) {
//    LOGD("face--- [native_GenderPredictor]->nativeGetProperty");
    if (gGenderPredictor == nullptr) {
//        LOGW("face--- [native_GenderPredictor]->nativeGetProperty engine has not been created");
        return 0;
    }
    return gGenderPredictor->get(static_cast<seeta::GenderPredictor::Property>(property));
}