//
// Created by Flonly on 3/23/16.
//

#ifndef OMXCODEC_AUDIOCODEC_H
#define OMXCODEC_AUDIOCODEC_H


#include <media/stagefright/OMXClient.h>
#include "PcmSource.h"
#include "SineSource.h"
#include "utils/mlog.h"

class AudioCodec :public MediaSource {
public:
    /* only suport aac yet */
    enum MimeType { MIME_TYPE_AAC = 0};
    virtual status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options);
    virtual sp<MetaData> getFormat() { return mCodec->getFormat(); }
    virtual status_t start(MetaData *params) {
        LOGD("AudioCodec starting");
        status_t err = UNKNOWN_ERROR;
        if((err = mCodec->start(params)) != OK) {
            LOGE("AudioCodec start error, err = %d",err);
        }
        return err;
    }
    virtual status_t stop() { return mCodec->stop(); }
    bool reachedEOS();

public:
    AudioCodec(sp<PcmSource> source, MimeType mimeType = MIME_TYPE_AAC);
    status_t init();
    sp<MediaSource> getCodec(){ return mCodec;}
    ~AudioCodec(){}
private:
    status_t setupEncMeta();
private:
    sp<MetaData> mEncMeta;
    OMXClient mClient;
    sp<MediaSource> mCodec;
    sp<PcmSource> mSource;
    MimeType  mMimeType;
    bool mIsEncoder;
    int64_t mFrameCount;
};


#endif //OMXCODEC_AUDIOCODEC_H
