#ifndef _PIC_DRAW
#define _PIC_DRAW

#include<opencv2/opencv.hpp>
#include <potracelib.h>
#include "binary_pic_processing.h"

IplImage* drawHistogram(CvHistogram*, int); //����ֱ��ͼ
IplImage* showVectorImg(CvSize, potrace_state_t*); //����ʸ����������»���ͼ��
ImageSplitArray* showSplitVectorImg(CvSize, potrace_state_t*); //����ʸ����������»���ͼ�񣬽�ÿ��������߷ֱ�ŵ�һ��ͼƬ��
void drawStatedLineImg(SegmentsArray*, IplImage*, int, CvScalar); //�Ӻϲ��߶μ����л���ָ��״̬���߶�ͼ��
void drawEleImg(SegmentsArray*, LineSetArray*, IplImage*); //�Ӻϲ��߶μ����л��Ʒǻ�ѧ��ͼ��
void mergeLinesAndImgCon(SegmentsArray *, int, int *, ImageCon *); //��n��ʸ���߶�����ͨ��ͼ��ϲ�
void drawEleImg(SegmentsArray *, CvPoint2D32f *, IplImage *); //�����ڽӾ������ͼ��

#endif