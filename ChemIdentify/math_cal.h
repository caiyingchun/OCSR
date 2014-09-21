#ifndef _MATH_CAL
#define _MATH_CAL

#define M_PI 3.14159265358979323846
#define NATIVE_MACHINE_WORD 32
#define ANGLE_THRESHOLD 20
#define DISTANCE_THRESHOLD 2.5
#define KMAX_THRESHOLD 8
#define KMIN_THRESHOLD 0.2
#define DVALUE_THRESHOLD 2.0

#include<opencv2/opencv.hpp>
#include "binary_pic_processing.h"

extern double AVGBONDLEN;

CvPoint calBezierCurvePoint(CvPoint2D32f*, double); //����һ��3�α����������е�ĳ��������
double cal2PointDistance(CvPoint2D32f*, CvPoint2D32f*); //��������֮��ľ���
float calAngleBetweenLines(CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*); //������ֱ��֮��ļнǣ���ΧΪ[0, ��/2]
bool judgeContainStatus(CvPoint2D32f*, CvPoint2D32f*, int, int, int, int); //�����߶ζ˵��ж��߶��Ƿ���һ�����ο���
void generalLinearEqu(CvPoint2D32f *, CvPoint2D32f *, double *); //��֪������ֱ�߷��̵�һ��ʽ(Ax+By+C=0)
double pointToLine(CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*); //����㵽ֱ�ߵľ���
bool mergeLineJudge(CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*, float, float); //�ж������߶��Ƿ��ܺϲ���һ�����߶Σ���ƽ���ཻ��
CvPoint2D32f* mergeLine(CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*, CvPoint2D32f*); //�ϲ������߶�
CvPoint2D32f* findFarthestDots(SegmentsArray*); //Ѱ��SegmentsArray��ӵ�����x����Сx�������㣬ӵ�����y����Сy�������㣻�����ؾ���Զ����������
CvPoint2D32f* findFarthestDotsFromSet(CvPoint2D32f *, int); //Ѱ�ҵ㼯������֮�����Զ��������
void findPossibleCharLines(ImageCon*, SegmentsArray*, IndexArray*); //Ѱ������ͨ�������ı��νӽ��Ļ�ѧ���߶Σ���Ϊ���ܵĻ�ѧ�����߶Σ�
void mergeBondAndUnkown(SegmentsArray*); //��ʶ��Ϊ��ѧ�����߶���δ֪�߶ν��кϲ�
void calAvgBondLen(SegmentsArray*); //���㻯ѧ���߶ε�ƽ������

#endif