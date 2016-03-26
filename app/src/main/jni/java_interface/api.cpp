//
// Created by flonly on 2016/3/5.
//

#include <string.h>
#include <jni.h>

#include "media/VideoCodec.h"
#include "media/Tester.h"

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
//com.example.flonly.androidomx
//Java_com_example_hellojni_HelloJni_stringFromJNI

JavaVM* gJavaVM;
extern "C" {

JNIEXPORT void JNICALL
Java_com_example_flonly_omxcodec_MainActivity_testomx(JNIEnv *env,
                                                        jobject thiz) {

    env->GetJavaVM(&gJavaVM);
    //OmxMaster om;
    //om.list_component();
//    H264Encoder encoder;
//    encoder.init();
//    encoder.test();
//    VideoCodec  vc;
//    vc.init();
//    vc.test();

    Tester t;
    t.testMp4();
    //t.testAAC();
}
}