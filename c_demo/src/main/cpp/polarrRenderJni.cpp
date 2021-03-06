//
// Created by Colin PRO on 2017/12/12.
//

#include <jni.h>
#include <time.h>
#include <inttypes.h>
#include <android/log.h>
#include <string.h>
#include <malloc.h>


#include "polarrRender.h"

#define LOG_TAG "POLARR_JNI"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

PolarrRender *polarrRender;

unsigned char *as_unsigned_char_array(JNIEnv *env, jbyteArray array) {
    int len = env->GetArrayLength(array);
    unsigned char *buf = new unsigned char[len];
    env->GetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte *>(buf));

    return buf;
}

jbyteArray as_byte_array(JNIEnv *env, unsigned char *buf, int len) {
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte *>(buf));
    return array;
}

long long currentTimeInMilliseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

extern "C"
JNIEXPORT void JNICALL
Java_co_polarr_render_PolarrRenderJni_init(JNIEnv *env, jclass type,
                                           jint width, jint height, jint stride, jint scanline,
                                           jboolean needEgl) {
    polarrRender = new PolarrRender;

    long during;
    long long startTime;

    startTime = currentTimeInMilliseconds();

    polarrRender->init(needEgl);
    polarrRender->setYUVsize(width, height, stride, scanline, INPUT_YUV_TYPE_NV21);

    during = currentTimeInMilliseconds() - startTime;

    ALOGD("Init sdk:%"
                  PRId64
                  "ms", during);


}

extern "C"
JNIEXPORT void JNICALL
Java_co_polarr_render_PolarrRenderJni_release(JNIEnv *env, jclass type) {
    delete polarrRender;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_co_polarr_render_PolarrRenderJni_updateYUVData(JNIEnv *env, jclass type,
                                                    jbyteArray yuvArr) {
    int len = env->GetArrayLength(yuvArr);
    unsigned char *yuvBytes = as_unsigned_char_array(env, yuvArr);

//    int size = len * 2 / 3;
//    u_char *yBytes = (u_char *) malloc(sizeof(u_char) * size);
//    u_char *uvBytes = (u_char *) malloc(sizeof(u_char) * size / 2);
//    memcpy(yBytes, yuvBytes, sizeof(u_char) * size);
//    memcpy(uvBytes, yuvBytes + size, sizeof(u_char) * size / 2);

    long during;
    long long startTime;

    startTime = currentTimeInMilliseconds();

    polarrRender->initFilter(F_COMMON_5);

    during = currentTimeInMilliseconds() - startTime;

    ALOGD("Init filter & size:%"
                  PRId64
                  "ms", during);

    startTime = currentTimeInMilliseconds();

    polarrRender->applyFilter(yuvBytes, INPUT_YUV_TYPE_NV21);
    during = currentTimeInMilliseconds() - startTime;
    ALOGD("APPLY FILTER:%"
                  PRId64
                  "ms", during);


//    memcpy(yuvBytes, yBytes, sizeof(u_char) * size);
//    memcpy(yuvBytes + size, uvBytes, sizeof(u_char) * size / 2);
//
//
//    delete yBytes;
//    delete uvBytes;

    return as_byte_array(env, yuvBytes, len);
}
