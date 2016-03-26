//
// Created by Flonly on 3/9/16.
//

#ifndef ANDROIDOMX_VIDEOCODEC_H
#define ANDROIDOMX_VIDEOCODEC_H

#include <media/mediarecorder.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/OMXCodec.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MPEG4Writer.h>
#include <utils/Errors.h>

#include "AVSource.h"
#include "thread/thread.h"

using namespace android;
class VideoCodec{
    enum ThreadType {
        UNKNOWN_TREAD_TYPE = -1,
        THREAD_TYPE_VIDEO_ENCODE = 1,
        THREAD_TYPE_AUDIO_ENCODE,
        THREAD_TYPE_COUNT
    };
    typedef struct
    {
        ThreadType type;
        VideoCodec* module;
    }ThreadParam;

    ThreadParam vep;

public:
    VideoCodec();
    int init();
    void test();
    status_t setupEncMeta();
    sp<MetaData> getEncMeta();
    sp<MediaSource> getCodec(){ return  mVideoEncoder;}
    ~VideoCodec();
    bool reachedEOS();

protected:
    static void* thread_main(void* param);
    sp<MetaData> mEncMeta;
    void encode_video();
private:
    OMXClient mClient;
    //sp<ANativeWindow> mNativeWindow;
    sp<MediaSource> mVideoEncoder;
    sp<MPEG4Writer> mMp4Write;

    sp<AVSource> mVideoSource;
    CPThread _thread_encode_video;
    struct VideoInfo mVideoInfo;
};

#endif //ANDROIDOMX_VIDEOCODEC_H
