#ifndef _RECOG_BOND
#define _RECOG_BOND

#include "binary_pic_processing.h"

void regWedgeBond(SegmentsArray *, IplImage *, SegmentsArray *); //ʶ��ʵШ�μ�
void regDottedWedgeBond(ImageSplitArray *, float, SegmentsArray *); //ʶ����Ш�μ�����ϸ��ͼ������ͨ���͸ߵ����ֵС�ڻ�ѧ������ͨ��߶ȵ���Сֵ����ͨ���ٽ�����Щ��ͨ��Ϊһ�飬���������Ƭ����������2��ÿ����ͨ������ĵ�������һ���߶Σ�����Ϊ����Ш�μ���
void regDTBond(SegmentsArray *, SegmentsArray *); //ʶ��˫��������
void addBondTobondSet(SegmentsArray *, CvPoint2D32f, CvPoint2D32f, char); //������ȷʶ��Ļ�ѧ���߶μ��뻯ѧ��������

#endif