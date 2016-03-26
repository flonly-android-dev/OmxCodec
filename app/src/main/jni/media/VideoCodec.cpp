//
// Created by Flonly on 3/9/16.
//
#include <media/openmax/OMX_IVCommon.h>
#include <media/stagefright/MediaDefs.h>
#include <binder/ProcessState.h>
#include <utils/Errors.h>
#include "utils/mlog.h"
#include "VideoCodec.h"

VideoCodec::VideoCodec() {
    mVideoInfo.width = 640;
    mVideoInfo.height = 480;
    mVideoInfo.fps = 30;
    mVideoInfo.bitrate = 800 * 1024;
    mVideoInfo.iframe_interval = 3;
    mVideoInfo.color_format = OMX_COLOR_FormatYUV420SemiPlanar;
    mVideoSource = new AVSource(mVideoInfo);
}

VideoCodec::~VideoCodec() {
    //mVideoSource->clear();
    mVideoEncoder->stop();
    //delete mVideoSource;
    mClient.disconnect();
}

int VideoCodec::init() {
    //android::ProcessState::self()->startThreadPool();
    status_t status;
    status = mClient.connect();
    LOGD("mClient.connect() status[%d]", status);

    setupEncMeta();
    bool createEncoder = true;
    mVideoEncoder = OMXCodec::Create(mClient.interface(), mEncMeta, createEncoder,
                                     mVideoSource, NULL, 0, NULL);
    if (mVideoEncoder == NULL) {
        LOGE("create mVideoEncoder failed");
    } else {
        LOGI("create mVideoEncoder success");
    }

//    status = mVideoEncoder->start();
//    if (status != OK) {
//        LOGE("mVideoEncoder->start() failed , ret = 0x%08x", status);
//    }
    //mMp4Write = new MPEG4Writer("/sdcard/omx.mp4");
//    status = mMp4Write->addSource(mVideoEncoder);
//    if(status != OK){
//        LOGE("mMp4Write->addSource failed");
//    }
//
//    status = mMp4Write->start();
//    if(status != OK){
//        LOGE("mMp4Write->start() failed , ret = 0x%08x",status);
//    }
}

void VideoCodec::test() {
    //return;
    status_t status;
    LOGD("VideoCodec::%s", __FUNCTION__);
    int64_t start = systemTime();
//    encode_video();
//    return;
    while (!mMp4Write->reachedEOS()) {
        usleep(100000);
        //LOGD("wait for end");
    }
    status = mMp4Write->stop();
    int64_t end = systemTime();
    if (status != OK && status != ERROR_END_OF_STREAM) {
        LOGE("record failed: %d\n", status);
        return;
    }
    LOGI("encoIding speed is: %.2f fps\n", (300 * 1E9) / (end - start));
    //encode_video();
//    vep.module = this;
//    vep.type = THREAD_TYPE_VIDEO_ENCODE;
//    _thread_encode_video.Start("video_encoder");
//    _thread_encode_video.AddTask(thread_main, &vep);

}

void *VideoCodec::thread_main(void *param) {
    LOGD("VideoCodec::%s", __FUNCTION__);
    ThreadParam *pParam = (ThreadParam *) param;
    switch (pParam->type) {
        case THREAD_TYPE_VIDEO_ENCODE:
            pParam->module->encode_video();
            break;
        default:
            break;
    }
    LOGE("VideoCodec::Run thread type = %d\n", pParam->type);
    return NULL;
}

void VideoCodec::encode_video() {
    LOGD("VideoCodec::%s", __FUNCTION__);
    uint32_t framecount = 0;
    for (; ;) {
        MediaBuffer *mVideoBuffer;
        MediaSource::ReadOptions options;
        LOGD("try read frame");
        status_t err = mVideoEncoder->read(&mVideoBuffer, &options);
        LOGD("encode read ret = 0x%08x", err);
        if (err == OK) {
            if (mVideoBuffer->range_length() > 0) {
                // If video frame availabe, render it to mNativeWindow
//                sp<MetaData> metaData = mVideoBuffer->meta_data();
//                int64_t timeUs = 0;
//                metaData->findInt64(kKeyTime, &timeUs)
//                native_window_set_buffers_timestamp(mNativeWindow.get(), timeUs * 1000);
//                err = mNativeWindow->queueBuffer(mNativeWindow.get(),
//                                                 mVideoBuffer->graphicBuffer().get());
//                if (err == 0) {
//                    metaData->setInt32(kKeyRendered, 1);
//                }
                framecount++;
                LOGD("encode frame success, framecount=%d", framecount);
            }
            if(framecount > 300){
                break;
            }
            mVideoBuffer->release();
        }
//        if (mMp4Write->reachedEOS()) {
//            LOGD("VideoCodec EOF");
//            break;
//        }
    }
}

status_t VideoCodec::setupEncMeta() {
    mEncMeta = new MetaData;
    mEncMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
    mEncMeta->setInt32(kKeyWidth, mVideoInfo.width);
    mEncMeta->setInt32(kKeyHeight, mVideoInfo.height);
    mEncMeta->setInt32(kKeyFrameRate, mVideoInfo.fps);
    mEncMeta->setInt32(kKeyBitRate, mVideoInfo.bitrate);
    mEncMeta->setInt32(kKeyIFramesInterval, mVideoInfo.iframe_interval);
    mEncMeta->setInt32(kKeyStride, mVideoInfo.width);
    mEncMeta->setInt32(kKeySliceHeight, mVideoInfo.height);
    mEncMeta->setInt32(kKeyColorFormat, OMX_COLOR_FormatYUV420SemiPlanar);
    return OK;
}

sp<MetaData> VideoCodec::getEncMeta() {
    return mEncMeta;
}

bool VideoCodec::reachedEOS() {
    return mVideoSource->reachedEOS();
}
