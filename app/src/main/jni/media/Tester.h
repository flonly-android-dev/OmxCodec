//
// Created by Flonly on 3/23/16.
//

#ifndef OMXCODEC_TESTER_H
#define OMXCODEC_TESTER_H


#include "VideoCodec.h"
#include "AudioCodec.h"

class Tester {
public:
    Tester();
    void testMp4();
    void testAAC();
    ~Tester();
private:
    VideoCodec *avcEncoder;
    AudioCodec *aacEncoder;
};


#endif //OMXCODEC_TESTER_H
