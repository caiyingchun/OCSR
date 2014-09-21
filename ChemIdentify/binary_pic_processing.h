#ifndef _BIN_PIC_PRO
#define _BIN_PIC_PRO

#include<opencv2/opencv.hpp>
#include <potracelib.h>

/* �洢��ͨ��ͼ��Ľṹ��
 * img ��ͨ��ͼ������
 * minI ��ͨ����С��
 * minJ ��ͨ����С��
 * maxI ��ͨ�������
 * maxJ ��ͨ�������
 * aspectRatio ��ͨ��߿��
 * characterTag ��ͨ�����ͱ�ʶ��δ֪0����ѧ����1������2��˫��3������4��Ш�μ�ʵ5��Ш�μ���6������Ȧ7
 * characters ��ͨ��ʶ��ɵ��ַ�
 */
typedef struct _ImageCon
{
	IplImage* img;
	int minI;
	int minJ;
	int maxI;
	int maxJ;
	float aspectRatio;
	int characterTag;
	char* characters;
}ImageCon;

/* �洢��ͨ��ͼ������Ľṹ��
 * length ���鳤��
 * count ��ͨ�������
 * imgCons ImageCon������
 */
typedef struct _ImageSplitArray
{
	int length;
	int count;
	ImageCon* imgCons;
}ImageSplitArray;

/* �洢һ����ͨ�������е�λ�õĽṹ��
 * length ���鳤��
 * count ��ͨ���е������
 * pointArray ��������
 * minI ��ͨ����С��
 * minJ ��ͨ����С��
 * maxI ��ͨ�������
 * maxJ ��ͨ�������
 */
typedef struct _PointsArray
{
	int length;
	int count;
	int (*pointArray)[2];
	int minI;
	int minJ;
	int maxI;
	int maxJ;
}PointsArray;

/* �洢�߶�����Ľṹ��
 * length ���鳤��
 * count �߶�����
 * pointArray �߶ε������˵���ɵ����飨ʵ/��Ш�μ��Ѽ�˴洢��pointArray[i][0]���ֶ˴洢��pointArray[i][1]��
 * recombinePIndex �洢�����Ļ�ѧ�ṹʽ��ÿ����ѧԪ�ص��������recombinePIndex[i][0]��ʾ��i���߶εĵ�0���˵������黯ѧ�ṹʽ�������ĵ�����
 * flag ��ǰ�߶ε�״̬��ʶ����(�ϲ������߶��к����Ϊ�������ڡ���) 0��δ֪��1�������ڣ�2����ѧ���ţ�3����ѧ����4��������5��˫����6��������7��ʵШ�μ���8����Ш�μ�
 */
typedef struct _SegmentsArray
{
	int length;
	int count;
	CvPoint2D32f (*pointArray)[2];
	char* flag;
	int (*recombinePIndex)[2];
}SegmentsArray;

/* �洢�߶μ��ϵ�����
 * length ���鳤��
 * count �߶μ��ϵ�����
 * lineSet �߶μ�������
 */
typedef struct _LineSetArray
{
	int length;
	int count;
	SegmentsArray* lineSet;
}LineSetArray;

/* �洢����ͨ�������ı��νӽ��Ļ�ѧ���߶���lineSet�е�����������
 * count ����������
 * nearLineIndex ��������
 */
typedef struct _IndexArray
{
	int count;
	int* nearLineIndex;
}IndexArray;

void thinImage(IplImage*); //ʹ������ͼ�Զ�ֵͼ��ϸ��
void splitImage(IplImage*, int, int, ImageSplitArray*); //����ͨ��ָ�ͼ��
void removeCharsImage(IplImage*, ImageSplitArray*, int); //��ͼ���б�ʶ���ĳ��characterTag�Ĳ���ȥ��
void removeCharsVector(SegmentsArray *, LineSetArray *, ImageSplitArray *); //ȥ��ֱ�߶μ��������ʶ����Ļ�ѧ����ͼ����߶�
potrace_state_t* binaryToVector(IplImage*); //��ֵͼ��ʸ����
void removeCurvingBezierByDistance(potrace_state_t*, double); //ȥ��ʸ��������������ı��������ߣ�ͨ��������ֵȥ����
void removeCurvingBezierByAngle(potrace_state_t*, float); //ȥ��ʸ��������������ı��������ߣ�ͨ���Ƕ���ֵȥ����
void removeVectorSmallSegments(potrace_state_t*, double); //ȥ��ʸ��������г���С����ֵ��Ƭ��
void removeBezierSegments(potrace_state_t*); //ȥ������������
void breakCornerAndRecombine(potrace_state_t*, SegmentsArray*); //��ʸ��������еĹս��ߴӹյ㴦�Ͽ������Ⱥϲ�ƽ���������߶�
LineSetArray* groupLineSegments(SegmentsArray*); //Ѱ���ܺϲ��ɳ��߶ε��߶μ���
void replaceLineSet(LineSetArray*, SegmentsArray*); //��LineSetArray�е�ÿ��lineSet������һ���߶θ������е������߶�
void vector2LineSet(potrace_state_t*, LineSetArray*, SegmentsArray*); //��LineSetArray�е�ÿ��lineSet������һ���߶θ������е������߶Σ����Ժϲ�����߶���Ѱ���Ƿ��п��Ժϲ���
int removeLineSetSmallSegments(SegmentsArray*, double); //ȥ���߶μ����г���С����ֵ���߶�

#endif