//
// Created by Flonly on 3/23/16.
//

#ifndef OMXCODEC_PCMSOURCE_H
#define OMXCODEC_PCMSOURCE_H

#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <utils/Errors.h>

using namespace android;

class PcmSource : public MediaSource {
public:
    PcmSource(int framesize, int bitPerSample, int channel, int sampleRate);
    int updateSouce(char *dst, int maxsize);
    virtual status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options);
    virtual sp<MetaData> getFormat() { return mFormat; }
    virtual status_t start(MetaData *params) { return OK; }
    virtual status_t stop() { return OK; }
    virtual ~PcmSource(){}

    int getSampleRate(){ return  mSampleRate;}
    int getChannelCount() { return mChannelCount;}
    int getMaxBufferSize() { return MAX_BUFFER_SIZE;}
    bool reachedEOS() {return mReachEOS;}
private:
    static const double kFrequency = 500.0;
    enum { MAX_BUFFER_SIZE = 4096};
    MediaBufferGroup mBufferGroup;
    sp<MetaData> mFormat;
    int mFrameSize;
    int mBitPerSample;
    int mChannelCount;
    int mSampleRate;
    int64_t mFrameCount;
    bool mReachEOS;
};


#endif //OMXCODEC_PCMSOURCE_H
