//
// Created by Flonly on 3/23/16.
//

#include <utils/mlog.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaDefs.h>
#include <math.h>
#include "PcmSource.h"

PcmSource::PcmSource(int framesize, int bitPerSample, int channel, int sampleRate) {
    mFrameSize = framesize;
    mBitPerSample = bitPerSample;
    mChannelCount = channel;
    mSampleRate = sampleRate;
    mFrameCount = 0;
    mReachEOS = false;

    mFormat = new MetaData;
    mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
    mFormat->setInt32(kKeyChannelCount, mChannelCount);
    mFormat->setInt32(kKeySampleRate, sampleRate);
    mFormat->setInt32(kKeyMaxInputSize, MAX_BUFFER_SIZE);

    mBufferGroup.add_buffer(new MediaBuffer(MAX_BUFFER_SIZE));
}

int PcmSource::updateSouce(char *dst, int maxsize) {
    int data_size = 4096; /**/
    //TODO copy pcm data to dst;
    return  data_size;
}

status_t PcmSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    mFrameCount++;
    //TODO for test
    //usleep(10*1000);
    int time = 8;//seconds
    if(mFrameCount >= time*mSampleRate/mFrameSize) {
        LOGD("PcmSource ERROR_END_OF_STREAM");
        //mReachEOS = true;
        //sleep(10);
        //*buffer = NULL;
        return ERROR_END_OF_STREAM;
    }


    status_t ret = mBufferGroup.acquire_buffer(buffer,false);
    //LOGD("PcmSource :%s : %d",__FUNCTION__,__LINE__);
    if (ret != OK) {
        LOGE("acquire_buffer failed");
        return ret;
    }

    int16_t *ptr = (int16_t *)(*buffer)->data();

    const double k = kFrequency / mSampleRate * (2.0 * M_PI);

    double x = mFrameCount * mFrameSize * k;
    for (size_t i = 0; i < mFrameSize; ++i) {
        int16_t amplitude = (int16_t)(32767.0 * sin(x));

        *ptr++ = amplitude;
        if (mChannelCount == 2) {
            *ptr++ = amplitude;
        }
        x += k;
    }
//    int size;
//    size = updateSouce((char*)(*buffer)->data(),MAX_BUFFER_SIZE);
//    (*buffer)->set_range(0, size);
    (*buffer)->set_range(0, mFrameSize*4);
    (*buffer)->meta_data()->clear();
    //(*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, packet.flags & AV_PKT_FLAG_KEY);
    (*buffer)->meta_data()->setInt64(kKeyTime, (int64_t)mFrameCount * 1000000 * mFrameSize / mSampleRate);
    LOGD(" == PcmSource read OK frameCount=%d",mFrameCount);
    return OK;
}
