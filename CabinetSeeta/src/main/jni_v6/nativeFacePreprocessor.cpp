//
// Created by gu on 2021/6/22 0022.
//
#include "com_seetaface_v6_FaceImagePreprocessor.h"

#include <android/log.h>
#include <android/bitmap.h>
#include "Struct.h"
#include "ObjectMapping.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "Seeta", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "Seeta", __VA_ARGS__)

#define YUV_FORMAT_NV21 0
#define YUV_FORMAT_NV12 1
#define YUV_FORMAT_420_888 2

void convertNV21ToBGR(const unsigned char* nv21Data, int width, int height, unsigned char* out) {
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            unsigned char yValue = nv21Data[width * j + i];
            int index = i % 2 == 0 ? i : i - 1;
            unsigned char vValue = nv21Data[width * height + width * (j / 2) + index];
            unsigned char uValue = nv21Data[width * height + width * (j / 2) + index + 1];

            double r = yValue + (1.370705 * (vValue - 128));
            double g = yValue - (0.698001 * (vValue - 128)) - (0.337633 * (uValue - 128));
            double b = yValue + (1.732446 * (uValue - 128));

            r = r < 0 ? 0 : ( r > 255 ? 255 : r);
            g = g < 0 ? 0 : ( g > 255 ? 255 : g);
            b = b < 0 ? 0 : ( b > 255 ? 255 : b);

            out[width * j * 3 + i * 3 + 0] = (unsigned char) b;
            out[width * j * 3 + i * 3 + 1] = (unsigned char) g;
            out[width * j * 3 + i * 3 + 2] = (unsigned char) r;
        }
    }
}

void convertNV12ToBGR(const unsigned char* nv12Data, int width, int height, unsigned char* out) {
    int i, j, index;
    unsigned char yValue, vValue, uValue;
    double r, g, b;
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            yValue = nv12Data[width * j + i];
            index = i % 2 == 0 ? i : i - 1;
            uValue = nv12Data[width * height + width * (j / 2) + index];
            vValue = nv12Data[width * height + width * (j / 2) + index + 1];

            r = yValue + (1.370705 * (vValue - 128));
            g = yValue - (0.698001 * (vValue - 128)) - (0.337633 * (uValue - 128));
            b = yValue + (1.732446 * (uValue - 128));

            r = r < 0 ? 0 : ( r > 255 ? 255 : r);
            g = g < 0 ? 0 : ( g > 255 ? 255 : g);
            b = b < 0 ? 0 : ( b > 255 ? 255 : b);

            out[width * j * 3 + i * 3 + 0] = (unsigned char) b;
            out[width * j * 3 + i * 3 + 1] = (unsigned char) g;
            out[width * j * 3 + i * 3 + 2] = (unsigned char) r;
        }
    }
}

void convertI420ToBGR(const unsigned char* yuvData, int width, int height, unsigned char* out) {
    int i, j, index;
    unsigned char yValue, vValue, uValue;
    double r, g, b;
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            yValue = yuvData[width * j + i];
            index = (width / 2) * (j / 2) + i / 2;
            uValue = yuvData[width * height + index];
            vValue = yuvData[width * height * 5 / 4 + index];

            r = yValue + (1.370705 * (vValue - 128));
            g = yValue - (0.698001 * (vValue - 128)) - (0.337633 * (uValue - 128));
            b = yValue + (1.732446 * (uValue - 128));

            r = r < 0 ? 0 : ( r > 255 ? 255 : r);
            g = g < 0 ? 0 : ( g > 255 ? 255 : g);
            b = b < 0 ? 0 : ( b > 255 ? 255 : b);

            out[width * j * 3 + i * 3 + 0] = (unsigned char) b;
            out[width * j * 3 + i * 3 + 1] = (unsigned char) g;
            out[width * j * 3 + i * 3 + 2] = (unsigned char) r;
        }
    }
}

void convertRGB565ToBGR(const unsigned char* rgbData, int width, int height, unsigned char* bgrData) {
    int pixelLength = width * height;
    unsigned char r, g, b;
    unsigned char pixel[2];
    for(int i = 0; i < pixelLength; i++) {
        pixel[0] = rgbData[i * 2 + 0];
        pixel[1] = rgbData[i * 2 + 1];
        r = pixel[1] & 0xf8;
        g = (pixel[1] << 5) | ((pixel[0] & 0xe0) >> 3);
        b = pixel[0] << 3;
        bgrData[i * 3 + 0] = b | ((b & 0x38) >> 3); // b
        bgrData[i * 3 + 1] = g | ((g & 0x0c) >> 2); // g
        bgrData[i * 3 + 2] = r | ((r & 0x38) >> 3); // r
    }
}

void convertARGBToBGR(const unsigned char* argbData, int width, int height, unsigned char* bgrData) {
    int pixelLength = width * height;
    for(int i = 0; i < pixelLength; i++) {
        bgrData[i * 3 + 0] = argbData[i * 4 + 2]; // b
        bgrData[i * 3 + 1] = argbData[i * 4 + 1]; // g
        bgrData[i * 3 + 2] = argbData[i * 4 + 0]; // r
    }
}

void convertYUVToBGR(const unsigned char* yData, const unsigned char* uData, const unsigned char* vData,
                                   int width, int height, unsigned char* out) {
    int i, j, index;
    unsigned char yValue, vValue, uValue;
    double r, g, b;
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            yValue = yData[j * width + i];
            index = (width / 2) * (j / 2) + i / 2;
            uValue = uData[index];
            vValue = vData[index];

            r = yValue + (1.370705 * (vValue - 128));
            g = yValue - (0.698001 * (vValue - 128)) - (0.337633 * (uValue - 128));
            b = yValue + (1.732446 * (uValue - 128));

            r = r < 0 ? 0 : ( r > 255 ? 255 : r);
            g = g < 0 ? 0 : ( g > 255 ? 255 : g);
            b = b < 0 ? 0 : ( b > 255 ? 255 : b);

            out[width * j * 3 + i * 3 + 0] = (unsigned char) b;
            out[width * j * 3 + i * 3 + 1] = (unsigned char) g;
            out[width * j * 3 + i * 3 + 2] = (unsigned char) r;
        }
    }
}

/*
 * Class:     com_seetaface_v6_FaceImagePreprocessor
 * Method:    processBitmap
 * Signature: (Landroid/graphics/Bitmap;)Lcom/seetaface/v6/SeetaImageData;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_FaceImagePreprocessor_processBitmap
        (JNIEnv *env, jobject obj, jobject bitmap) {
    AndroidBitmapInfo bitmapInfo;
    if (AndroidBitmap_getInfo(env, bitmap, &bitmapInfo) != ANDROID_BITMAP_RESULT_SUCCESS) {
//        LOGW("face--- [native_preprocessor]->processBitmap Get bitmap info failed!");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    nativeImageData.width = bitmapInfo.width;
    nativeImageData.height = bitmapInfo.height;
    nativeImageData.channels = 3;
    int dataLength = nativeImageData.width * nativeImageData.height * nativeImageData.channels;
    nativeImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    if (nativeImageData.data == nullptr) {
//        LOGW("face--- [native_preprocessor]->processBitmap Alloc seeta image data failed!");
        return nullptr;
    }

    void* buffer;
    jboolean converted = JNI_TRUE;
    AndroidBitmap_lockPixels(env, bitmap, &buffer);
    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        convertARGBToBGR(static_cast<unsigned char *>(buffer), nativeImageData.width, nativeImageData.height, nativeImageData.data);
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        convertRGB565ToBGR(static_cast<unsigned char *>(buffer), nativeImageData.width, nativeImageData.height, nativeImageData.data);
    } else {
        converted = JNI_FALSE;
    }
    AndroidBitmap_unlockPixels(env, bitmap);

    jobject jFaceImageObj = nullptr;
    if (converted) {
        jFaceImageObj = gu::ObjectMapping::createFaceImageDataObject(env, nativeImageData, dataLength);
    }

    // release imageData.data
    free(nativeImageData.data);

    return jFaceImageObj;
}

/*
 * Class:     com_seetaface_v6_FaceImagePreprocessor
 * Method:    processYUVData
 * Signature: ([BIII)Lcom/seetaface/v6/SeetaImageData;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_FaceImagePreprocessor_processYUVData
        (JNIEnv *env, jobject obj, jbyteArray image, jint width, jint height, jint format) {
    jboolean isCopy;
    unsigned char* buffer = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(image, &isCopy));
//    LOGD("face--- [native_preprocessor]->processYUVData isCopy:%d", isCopy);
    if (buffer == nullptr) {
//        LOGW("face--- [native_preprocessor]->processYUVData Image buffer is nullptr!");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    nativeImageData.width = width;
    nativeImageData.height = height;
    nativeImageData.channels = 3;
    int dataLength = width * height * nativeImageData.channels;
    nativeImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    if (nativeImageData.data == nullptr) {
//        LOGW("face--- [native_preprocessor]->processYUVData Alloc seeta image data failed!");
        return nullptr;
    }

    jboolean converted = JNI_TRUE;
    if (format == YUV_FORMAT_NV12) { // nv12
        convertNV12ToBGR(buffer, nativeImageData.width, nativeImageData.height, nativeImageData.data);
    } else if (format == YUV_FORMAT_NV21) { // nv21
        convertNV21ToBGR(buffer, nativeImageData.width, nativeImageData.height, nativeImageData.data);
    } else if (format == YUV_FORMAT_420_888) { // nv21
        convertI420ToBGR(buffer, nativeImageData.width, nativeImageData.height, nativeImageData.data);
    } else {
        converted = JNI_FALSE;
//        LOGW("face--- [native_preprocessor]->processYUVData Unsupport format");
    }
    env->ReleaseByteArrayElements(image, (jbyte *)buffer, 0);

    jobject jFaceImageObj = nullptr;
    if (converted) {
        jFaceImageObj = gu::ObjectMapping::createFaceImageDataObject(env, nativeImageData, dataLength);
    }

    return jFaceImageObj;
}

/*
 * Class:     com_seetaface_v6_FaceImagePreprocessor
 * Method:    processSplitYUVData
 * Signature: ([B[B[BII)Lcom/seetaface/v6/SeetaImageData;
 */
extern "C" JNIEXPORT jobject JNICALL Java_com_seetaface_v6_FaceImagePreprocessor_processSplitYUVData
        (JNIEnv *env, jobject obj,
                jbyteArray yArray, jbyteArray uArray, jbyteArray vArray,
                jint width, jint height) {
    jboolean isCopy;
    auto* yBuffer = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(yArray, &isCopy));
    auto* uBuffer = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(uArray, &isCopy));
    auto* vBuffer = reinterpret_cast<unsigned char *>(env->GetByteArrayElements(vArray, &isCopy));
    if (yBuffer == nullptr || uBuffer == nullptr || vBuffer == nullptr) {
//        LOGW("face--- [native_engine]->processYUVData Image buffer is nullptr!");
        return nullptr;
    }

    SeetaImageData nativeImageData{};
    nativeImageData.width = width;
    nativeImageData.height = height;
    nativeImageData.channels = 3;
    int dataLength = width * height * nativeImageData.channels;
    nativeImageData.data = static_cast<unsigned char *>(malloc(dataLength));
    if (nativeImageData.data == nullptr) {
//        LOGW("face--- [native_engine]->processYUVData Alloc seeta image data failed!");
        return nullptr;
    }

    // convert I420 to BGR
    convertYUVToBGR(yBuffer, uBuffer, vBuffer, nativeImageData.width, nativeImageData.height, nativeImageData.data);

    // release byte array
    env->ReleaseByteArrayElements(yArray, (jbyte *)yBuffer, 0);
    env->ReleaseByteArrayElements(uArray, (jbyte *)uBuffer, 0);
    env->ReleaseByteArrayElements(vArray, (jbyte *)vBuffer, 0);

    jobject jFaceImageObj = gu::ObjectMapping::createFaceImageDataObject(env, nativeImageData, dataLength);

    free(nativeImageData.data);

    return jFaceImageObj;
}