//
// Created by Flonly on 3/23/16.
//

#include <utils/mlog.h>
#include <media/stagefright/OMXCodec.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include "AudioCodec.h"
#include "SineSource.h"

AudioCodec::AudioCodec(sp<PcmSource> source, AudioCodec::MimeType mimeType) {
    mSource = source;
    mMimeType = mimeType;
    mCodec = NULL;
    mIsEncoder = true;
    mFrameCount = 0;
}

status_t AudioCodec::init() {
    status_t status = setupEncMeta();
    if (status != OK) {
        LOGE(" AudioCodec::setupEncMeta() failed ");
        return status;
    }
    status = mClient.connect();
    LOGD("mClient.connect() status[%d]", status);
    mCodec = OMXCodec::Create(mClient.interface(), mEncMeta, mIsEncoder,
                              mSource, NULL, 0, NULL);
    if (mCodec == NULL) {
        LOGE("create AudioCodec failed");
    } else {
        LOGI("create AudioCodec success");
    }

    LOGI("AudioCodec::init success");
    return OK;
}

status_t AudioCodec::setupEncMeta() {
    if (mMimeType != MIME_TYPE_AAC) {
        return BAD_VALUE;
    }
    mEncMeta = new MetaData;
    mEncMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC);
    mEncMeta->setInt32(kKeyChannelCount, mSource->getChannelCount());
    mEncMeta->setInt32(kKeySampleRate, mSource->getSampleRate());
    mEncMeta->setInt32(kKeyMaxInputSize, mSource->getMaxBufferSize());
    mEncMeta->setInt32(kKeyBitRate, 160000);
    return OK;
}

bool AudioCodec::reachedEOS() {
    return mSource->reachedEOS();
}

status_t AudioCodec::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options
) {

    status_t err = OK;
    int32_t int32_value = 0;
    void * pointer_value = NULL;
//    err = mCodec->read(buffer,options);
//    mFrameCount++;
//    if ((*buffer)->meta_data()->findInt32(kKeyIsCodecConfig, &int32_value)) {
//        //copy->meta_data()->setInt64(kKeyIsCodecConfig,int32_value);
//        LOGD("kKeyIsCodecConfig, framecount = %d",mFrameCount);
//    }
//    return err;

    int64_t timestampUs = 0;
    static int64_t lastTimestampUs = 0;
    int32_t isCodecConfig = 0;
    do {
        err = mCodec->read(buffer, options);
        mFrameCount++;
        if (err != OK) {
            (*buffer)->release();
            (*buffer) = NULL;
            LOGD("goes here err=%d", err);
            return err;
        }
        (*buffer)->meta_data()->findInt64(kKeyTime, &timestampUs);
        if (timestampUs <= lastTimestampUs) {
            LOGD("timestampUs <= 0");
            timestampUs = lastTimestampUs + 20;
//            MediaBuffer *copy = new MediaBuffer((*buffer)->range_length());
//            memcpy(copy->data(), (uint8_t *)(*buffer)->data() + (*buffer)->range_offset(),
//                   (*buffer)->range_length());
//            copy->set_range(0, (*buffer)->range_length());
//            copy->meta_data()->clear();
//            copy->meta_data()->setInt64(kKeyTime,timestampUs);

//            if ((*buffer)->meta_data()->findInt32(kKeyIsUnreadable, &int32_value)) {
//                copy->meta_data()->setInt64(kKeyIsUnreadable,int32_value);
//            }
//            if ((*buffer)->meta_data()->findInt32(kKeyIsSyncFrame, &int32_value)) {
//                copy->meta_data()->setInt64(kKeyIsSyncFrame,int32_value);
//            }
//            if ((*buffer)->meta_data()->findInt32(kKeyIsCodecConfig, &int32_value)) {
//                copy->meta_data()->setInt64(kKeyIsCodecConfig,int32_value);
//            }
//            if ((*buffer)->meta_data()->findPointer(kKeyPlatformPrivate, &pointer_value)) {
//                copy->meta_data()->setPointer(kKeyIsCodecConfig,pointer_value);
//            }
//            if ((*buffer)->meta_data()->findPointer(kKeyBufferID, &pointer_value)) {
//                copy->meta_data()->setPointer(kKeyBufferID,pointer_value);
//            }
//            (*buffer)->release();
//            (*buffer) = copy;
             if((*buffer)->meta_data()->findInt32(kKeyIsCodecConfig, &isCodecConfig) && isCodecConfig){
                 LOGD(" 11 timestampUs = %d, isCodecConfig = %d", timestampUs, isCodecConfig);
                 return err;
             } else{
                 (*buffer)->release();
                 (*buffer) = NULL;
                 LOGD(" 22 timestampUs = %d, isCodecConfig = %d", timestampUs, isCodecConfig);
                 return  ERROR_END_OF_STREAM;
             }
        }
        //lastTimestampUs = timestampUs;
    } while ((timestampUs <= 0) && !isCodecConfig);
    //} while (1);

//    int32_t isCodecConfig =0;
//    if((*buffer)->meta_data()->findInt32(kKeyIsCodecConfig, &isCodecConfig)){
//        LOGD("isCodecConfig = 1");
//    }
//    MediaBuffer *copy = new MediaBuffer((*buffer)->range_length());
//    memcpy(copy->data(), (uint8_t *)(*buffer)->data() + (*buffer)->range_offset(),
//           (*buffer)->range_length());
//    copy->set_range(0, (*buffer)->range_length());
//    copy->meta_data();
//    sp<MetaData> meta_data = new MetaData(*(*buffer)->meta_data().get());
//    buffer->release();
//    buffer = NULL;

    LOGD("AudioCodec read frame count = %d", mFrameCount);
    return err;
}
