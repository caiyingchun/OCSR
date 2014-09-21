#ifndef _RECOM_CHEM_STRUCT
#define _RECOM_CHEM_STRUCT

#include<opencv2/opencv.hpp>
#include "binary_pic_processing.h"
#include "math_cal.h"
#include "recognize_bonds.h"

/* ��ѧ���߶εĶ˵����������˵�ΪpointArray[i][j]����ÿ����������һ����������
 * i ��i���߶�
 * j ��i���߶εĵ�j���˵�
 * next ��һ���˵�����
 */
typedef struct _LinePointsIndex
{
	int i;
	int j;
	_LinePointsIndex *next;
}LinePointsIndex;

/* LinePointsIndex������ɵ�����
 * i ��i����ͨ�������洢�ַ���ͨ��-1��ʾ����̼ԭ��
 * point_set LinePointsIndex����
 * count LinePointsIndex�����нڵ������
 * next ��һ��LinePointsIndex����
 */
typedef struct _RecombinedPoints
{
	int i;
	LinePointsIndex *point_set;
	int count;
	_RecombinedPoints *next;
}RecombinedPoints;

/* �洢��ѧ�ṹʽ��ÿ���ڵ�
 * node ÿ���ڵ������
 * chars ÿ���ڵ��ʾ�Ļ�ѧԪ�ط���
 */
typedef struct _ChemStruNodes
{
	CvPoint2D32f node;
	char *chars;
}ChemStruNodes;

int setBondSetRPI(SegmentsArray *, ImageSplitArray *, ChemStruNodes **, int ***); //���ɻ�ѧ�ṹʽ���ڽӾ���
void mergeSpOneChar(SegmentsArray *, ImageSplitArray *); //�ϲ�����Ļ�ѧԪ�ط��ţ�Br��Si��Cl��������ɵ�ԭ�ӣ�
void mergeHorVChemChars(ImageSplitArray *); //�ϲ����Ż����ŵĻ�ѧ����
char *getJMEStr(ChemStruNodes *, int **, int); //�����ڽӾ���ͽڵ���������JME��ʽ���ַ���

#endif