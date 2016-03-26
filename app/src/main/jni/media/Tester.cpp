//
// Created by Flonly on 3/23/16.
//

#include "Tester.h"
#include "AudioCodec.h"
#include "VideoCodec.h"
#include "SineSource.h"
#include <media/stagefright/MPEG4Writer.h>
#include <utils/mlog.h>
#include <media/stagefright/MediaErrors.h>
#include <binder/ProcessState.h>


Tester::Tester() {
    aacEncoder = NULL;
    avcEncoder = NULL;
}
Tester::~Tester() {
//    if(aacEncoder != NULL){
//        delete aacEncoder;
//    }
//    if(avcEncoder != NULL){
//        delete avcEncoder;
//    }
    //delete aacEncoder;
    //delete avcEncoder;
}

void Tester::testMp4() {
    android::ProcessState::self()->startThreadPool();
    status_t status;
    LOGD("Tester::%s", __FUNCTION__);
    int64_t start = systemTime();
    //sp<SineSource> sineSource = new SineSource(44100,2);

    sp<PcmSource> pcmSource = new PcmSource(1024,8,2,44100);
    aacEncoder = new AudioCodec(pcmSource);
    aacEncoder->init();

    avcEncoder = new VideoCodec();
    avcEncoder->init();

    MPEG4Writer *mMp4Write = new MPEG4Writer("/sdcard/omx.mp4");

    //status = mMp4Write->addSource(aacEncoder->getCodec());
    status = mMp4Write->addSource(aacEncoder);
    if(status != OK){
        LOGE("mMp4Write->addSource failed");
    }
    status = mMp4Write->addSource(avcEncoder->getCodec());
    if(status != OK){
        LOGE("mMp4Write->addSource failed");
    }
    //mMp4Write->setMaxFileDuration(5*1000000);
    sp<MetaData> muxMeta = new MetaData;
    muxMeta->setInt32(kKeyRotation,90);
    //muxMeta->setInt32(kKeyRealTimeRecording, false);
    //mMp4Write->setInterleaveDuration(1000*1000);
    status = mMp4Write->start(muxMeta.get());
    if(status != OK){
        LOGE("mMp4Write->start() failed , ret = 0x%08x",status);
    }

    while (!mMp4Write->reachedEOS()) {
        Vector<String16> args;
        //usleep(6*1000000);
        sleep(1);
        //break;
    }
    status = mMp4Write->stop();



    int64_t end = systemTime();
    if (status != OK && status != ERROR_END_OF_STREAM) {
        LOGE("record failed: %d", status);
        return;
    }
    LOGI("encoIding speed is: %.2f fps", (300 * 1E9) / (end - start));

    //delete aacEncoder;
    //delete avcEncoder;
    //delete muxMeta;
}

void Tester::testAAC() {
    android::ProcessState::self()->startThreadPool();
    const char *aacFilePath = "/sdcard/omx.aac";
    status_t status;
    LOGD("Tester::%s", __FUNCTION__);
    int64_t start = systemTime();
    sp<PcmSource> pcmSource = new PcmSource(1024,8,2,44100);
    aacEncoder = new AudioCodec(pcmSource);
    aacEncoder->init();
    sp<MediaSource> encoder = aacEncoder->getCodec();
    encoder->start();

    FILE *fp = fopen(aacFilePath,"w");
    int32_t n = 0;
    status_t err;
    MediaBuffer *buffer;
    while ((err = encoder->read(&buffer)) == OK) {
        if (buffer->range_length() == 0) {
            buffer->release();
            buffer = NULL;
            //LOGE("buffer len size = 0");
            continue;
        }
        LOGI("buffer size = %d",buffer->range_length());
        fwrite(buffer->data()+buffer->range_offset(),buffer->range_length(),1,fp);
        //fwrite(const void * __restrict, size_t, size_t, FILE * __restrict);
        buffer->release();
        buffer = NULL;
//
//        if (++n == 100) {
//            break;
//        }
    }
    if(err != OK && err != ERROR_END_OF_STREAM){
        LOGE("audio encode end failed, err=%d",err);
    }else{
        LOGI("audio encode end success");
    }
    encoder->stop();
}

