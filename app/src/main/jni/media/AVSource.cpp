//
// Created by Flonly on 3/9/16.
//

#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaDefs.h>
#include <media/openmax/OMX_IVCommon.h>
#include <utils/mlog.h>
#include "AVSource.h"
#include "media/tools.h"

AVSource::AVSource(struct VideoInfo vi) {
    mVideoInfo = vi;

    mFormat = new MetaData;
    //mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
    mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_RAW);
    mFormat->setInt32(kKeyWidth, mVideoInfo.width);
    mFormat->setInt32(kKeyHeight, mVideoInfo.height);
    mFormat->setInt32(kKeyColorFormat, mVideoInfo.color_format);

    mYuvSize = mVideoInfo.width * mVideoInfo.height * 3 / 2;
    LOGD("mYuvSize = %d",mYuvSize);
    mYuv420Blue = new char[mYuvSize];
    mYuv420Red  = new char[mYuvSize];
    mGroup.add_buffer(new MediaBuffer(mYuvSize));
    mFrameCount = 0;
    createYuvData();

    mReachEOS = false;
}

AVSource::~AVSource() {
    //delete mYuv420;
}

status_t AVSource::read(MediaBuffer **buffer, const MediaSource::ReadOptions *options) {
    int videoLen = 8;//seconds
    if(mFrameCount >= mVideoInfo.fps * videoLen){
        LOGD("AVSource ERROR_END_OF_STREAM");
        mReachEOS = true;
        sleep(10);
        return ERROR_END_OF_STREAM;
    }
    LOGD("AVSource :%s : %d",__FUNCTION__,__LINE__);
    status_t ret = mGroup.acquire_buffer(buffer,false);
    LOGD("AVSource :%s : %d",__FUNCTION__,__LINE__);
    if (ret != OK) {
        LOGE("acquire_buffer failed");
        return ret;
    }

#if 0
    // iterate through solid planes of color.
    static unsigned char x = 0x60;
    memset((*buffer)->data(), x, mSize);
    x = x >= 0xa0 ? 0x60 : x + 1;
#endif

    updateSouce((char*)(*buffer)->data());
    (*buffer)->set_range(0, mYuvSize);
    (*buffer)->meta_data()->clear();
    //(*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, packet.flags & AV_PKT_FLAG_KEY);
    (*buffer)->meta_data()->setInt64(kKeyTime, (int64_t)mFrameCount * 1000000 / mVideoInfo.fps);

    LOGI(" == AVSource read , frame_count = %d",mFrameCount);
    return OK;
}

void AVSource::createYuvData(){

    uint32_t iWH = mVideoInfo.width * mVideoInfo.height;
    char *rgb24 = new char[iWH*3];
    //for red
    for(int i=0; i < iWH; i++){
//        rgb24[i*3] = random()%255;
//        rgb24[i*3+1] = random()%255;
//        rgb24[i*3+2] = random()%255;
        if(i<iWH/2){
            rgb24[i*3] = 0;
        }else{
            rgb24[i*3] = 255;
        }
//        rgb24[i*3+1] = 0;
//        rgb24[i*3+2] = 0;
        rgb24[i*3+1] = random()%255;
        rgb24[i*3+2] = random()%255;
    }

    static struct timeval tv, ctv;
    gettimeofday(&tv,NULL);
    uint32_t time1 = tv.tv_sec * 1000000 + tv.tv_usec;
    RGB24ToYUV420(mVideoInfo.width,mVideoInfo.height,(BYTE*)rgb24,(BYTE*)mYuv420Red);
    gettimeofday(&tv,NULL);
    uint32_t time2 = tv.tv_sec * 1000000 + tv.tv_usec;
    LOGD("time cost for rgb2yuv420 = %dus",time2-time1);

    for(int i=0; i < iWH; i++){
        rgb24[i*3] = random()%255;
        rgb24[i*3+1] = random()%255;
//        rgb24[i*3] = 0;
//        rgb24[i*3+1] = 0;
        if(i<iWH/2){
            rgb24[i*3+2] = 255;
        }else{
            rgb24[i*3+2] = 0;
        }

    }
    RGB24ToYUV420(mVideoInfo.width,mVideoInfo.height,(BYTE*)rgb24,(BYTE*)mYuv420Blue);

}

void AVSource::updateSouce(char *dst) {
    LOGD("enter AVSource::updateSouce");
    //TODO update yuvdata;
    static struct timeval tv, ctv;
    static uint32_t maxtime = 0;
    static uint32_t avgtime = 0;
//    if (mFrameCount == 0) {
//        gettimeofday(&tv, NULL);
//    } else {
//        gettimeofday(&ctv, NULL);
//        int32_t time_wait = (ctv.tv_sec * 1000000 + ctv.tv_usec) - (tv.tv_sec * 1000000 + tv.tv_usec) -
//                             (1000000 / mVideoInfo.fps);
//        avgtime = (time_wait + 1000000 / mVideoInfo.fps)*0.2 + avgtime*0.8;
//        if(time_wait + 1000000 / mVideoInfo.fps > maxtime){
//            maxtime = time_wait + 1000000 / mVideoInfo.fps;
//            //LOGD("time cost between updateSouce avgtime=%d, maxtime= %d",avgtime, maxtime);
//        }
//        //LOGD("time cost between updateSouce = %d", time_wait + 1000000 / mVideoInfo.fps);
//        if (time_wait > 0) {
//            LOGD("sleep time %d",time_wait);
//            usleep(time_wait);
//        } else {
//            //LOGE("encode cost too much time with more %dus",time_wait);
//        }
//        tv = ctv;
//    }
    LOGD("============ updateSouce ============ ");
    int count = (mFrameCount + 1) % (mVideoInfo.fps);
    //if(count < mVideoInfo.fps/2){
    if(mFrameCount%2==1){
        //LOGD("time cost between updateSouce avgtime=%d, maxtime= %d",avgtime, maxtime);
        memcpy(dst,mYuv420Red,mYuvSize);
    }else{
        memcpy(dst,mYuv420Blue,mYuvSize);
    }
    mFrameCount++;
}

