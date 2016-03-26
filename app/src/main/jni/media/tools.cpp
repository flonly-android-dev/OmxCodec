//
// Created by Flonly on 3/10/16.
//

#include <stddef.h>
#include "tools.h"

#ifndef max
#define max(a,b) (((a) < (b))?(b) :(a))
#endif

#ifndef min
#define min(a,b)  (((a) > (b))?(b) :(a))
#endif

//to nv21
void RGB24ToYUV420(int Width, int Height, BYTE* RgbBuffer, BYTE* YuvBuffer)
{
    BYTE* yuvBuf=YuvBuffer;
    int nWidth=Width;
    int nHeight=Height;

    int i, j;
    BYTE*bufY = yuvBuf;
    BYTE*bufU = yuvBuf + nWidth * nHeight;
    //BYTE*bufV = bufU + (nWidth* nHeight* 1/4);
    BYTE*bufV = bufU + 1 ;
    //BYTE*bufUV = yuvBuf + nWidth * nHeight;
    BYTE*Y=bufY;
    BYTE*U=bufU;
    BYTE*V=bufV;
    BYTE*bufRGB;
    unsigned char y, u, v, r, g, b,testu,testv;
    unsigned char* pRGBBuffer = RgbBuffer;
    if (NULL == pRGBBuffer)
    {
        return ;
    }
    for (j = 0; j<nHeight;j++)
    {
        bufRGB = RgbBuffer + nWidth * (nHeight - 1-j) * 3 ;
        for (i = 0;i<nWidth;i++)
        {
            int pos = nWidth * i + j;
            r= *(bufRGB++);
            g = *(bufRGB++);
            b = *(bufRGB++);
            y =(unsigned char)(( 66 * r + 129 * g +  25 * b + 128) >>8) + 16;//16
            v = (unsigned char)((-38 * r -  74 * g + 112 * b + 128) >>8) +128  ; //128
            u = (unsigned char)((112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;
            *(bufY++)=max(0,min(y, 255 ));


            if (j%2==0&&i%2 ==0)
            {
                if (u>255)
                {
                    u=255;
                }
                if (u<0)
                {
                    u = 0;
                }
                *(bufU) =u;
                bufU +=2;
            }
            else
            {
                if (i%2==0)
                {
                    if (v>255)
                    {
                        v = 255;
                    }
                    if (v<0)
                    {
                        v = 0;
                    }
                    *(bufV) =v;
                    bufV +=2;
                }
            }
        }
    }
}