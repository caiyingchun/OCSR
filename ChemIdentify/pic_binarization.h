#ifndef _PIC_BIN
#define _PIC_BIN

#include<opencv2/opencv.hpp>

CvHistogram* getHistogram(IplImage*, int, float**); //��ȡ�Ҷ�ͼ��һάֱ��ͼ
int otsu(IplImage*, CvHistogram*); //����Զ���ȡ��ֵ����ֵ
void grayImgBinarization(IplImage*, int); //�Ҷ�ͼ��ֵ��
void reverseImgColor(IplImage*); // ͼ��ڰ׷�ת

#endif