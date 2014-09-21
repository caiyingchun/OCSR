#include <math.h>
#include "math_cal.h"

double AVGBONDLEN = 0.0;

/* ����һ��3�α����������е�ĳ��������
 * control_points ���������ߵĿ��Ƶ�����(4�����Ƶ�)
 * t ���������߲��������в�����ֵ
 * 3�α��������ߵĲ�������ΪP0*(1-t)^3+3*P1*(1-t)^2*t+3*P2*(1-t)*t^2+P3*t^3  t��[0,1]
 */
CvPoint calBezierCurvePoint(CvPoint2D32f* control_points, double t)
{
	CvPoint p;
	p.x = (int)(control_points[0].x*pow(1-t, 3) + 3*control_points[1].x*pow(1-t, 2)*t + 3*control_points[2].x*(1-t)*pow(t, 2) + control_points[3].x*pow(t, 3));
	p.y = (int)(control_points[0].y*pow(1-t, 3) + 3*control_points[1].y*pow(1-t, 2)*t + 3*control_points[2].y*(1-t)*pow(t, 2) + control_points[3].y*pow(t, 3));
	return p;
}

/* ��������֮��ľ���
 * pointA ��A
 * pointB ��B
 */
double cal2PointDistance(CvPoint2D32f* pointA, CvPoint2D32f* pointB)
{
	return sqrt(pow((pointA->x - pointB->x), 2) + pow((pointA->y - pointB->y), 2));
}

/* ������ֱ��֮��ļнǣ���ΧΪ[0, ��/2]
 * A1, B1 ֱ��l1�ϵĵ�
 * A2, B2 ֱ��l2�ϵĵ�
 */
float calAngleBetweenLines(CvPoint2D32f* A1, CvPoint2D32f* B1, CvPoint2D32f* A2, CvPoint2D32f* B2)
{
	float k1, k2; //l1��l2��б��
	float theta; //l1��l2�ļнǦȣ���ΧΪ[0, ��/2]
	if(B1->x - A1->x != 0.0 && B2->x - A2->x != 0)
	{
		k1 = (B1->y - A1->y) / (B1->x - A1->x);
		k2 = (B2->y - A2->y) / (B2->x - A2->x);
		if(k1 * k2 == -1)
			theta = (float)(M_PI / 2);
		else
		{
			theta = atan(fabs((k1-k2) / (1+k1*k2)));
		}
	}
	else if(B1->x - A1->x == 0.0 && B2->x - A2->x == 0) //��y��ƽ��
	{
		theta = 0.0;
	}
	else if(B1->x - A1->x == 0.0)
	{
		k2 = fabs((B2->y - A2->y) / (B2->x - A2->x));
		theta = (float)(M_PI / 2 - atan(k2));
	}
	else if(B2->x - A2->x == 0.0)
	{
		k1 = fabs((B1->y - A1->y) / (B1->x - A1->x));
		theta = (float)(M_PI / 2 - atan(k1));
	}
	return theta;
}

/* �����߶ζ˵��ж��߶��Ƿ���һ�����ο���
 * A, B �߶�l�Ķ˵�
 * minI, maxI, minJ, maxJ ���ο���С����У���С�����
 * <returns>��������Ϊbool�����Ϊtrue��ʾ�߶��ھ��ο��ڣ����Ϊfalse��ʾ�߶β��ھ��ο���</returns>
 */
bool judgeContainStatus(CvPoint2D32f *A, CvPoint2D32f *B, int minI, int maxI, int minJ, int maxJ)
{
    minI = minI-1;
	maxI = maxI+1;
	minJ = minJ-1;
	maxJ = maxJ+1;
    if(A->x>=minJ && A->x<=maxJ && A->y>=minI && A->y<=maxI && B->x>=minJ && B->x<=maxJ && B->y>=minI && B->y<=maxI)
		return true;
	else
		return false;
}

/* ���������߶���Χ������ж������߶��Ƿ��ཻ
 * A, B �߶�l1�Ķ˵�
 * C, D �߶�l2�Ķ˵�
 * <returns>��������Ϊbool�����Ϊtrue��ʾ�����߶ο����ཻ�����Ϊfalse��ʾ�����߶β��ཻ</returns>
 */
bool judgeAboutCrossStatus(CvPoint2D32f A, CvPoint2D32f B, CvPoint2D32f C, CvPoint2D32f D)
{
    bool returnResult = true;
    //�ж�l1��XY�������ֵ
    float maxX1, minX1, maxY1, minY1;
	maxX1 = minX1 = A.x;
    maxY1 = minY1 = A.y;
    if (A.x < B.x)
        maxX1 = B.x;
    else
        minX1 = B.x;
    if (A.y < B.y)
        maxY1 = B.y;
    else
        minY1 = B.y;

	//�ж�l2��XY�������ֵ
    float maxX2, minX2, maxY2, minY2;
    maxX2 = minX2 = C.x;
    maxY2 = minY2 = C.y;
    if (C.x < D.x)
        maxX2 = D.x;
    else
        minX2 = D.x;
	if (C.y < D.y)
        maxY2 = D.y;
    else
        minY2 = D.y;
    //�Ƚ���ֵ��С
    if ((minX1 > maxX2) || (maxX1 < minX2) || (minY1 > maxY2) || (maxY1 < minY2))
    {
        returnResult = false;
    }
    return returnResult;
}

/* ���C������ֱ��AB��߻����ұ�
 * A, B һ���߶ε������˵�
 * C ��һ���߶ε�����һ���˵�
 * ���أ�0���ߡ�1��ߡ�-1�ұ�
 */
int leftRight(CvPoint2D32f A, CvPoint2D32f B, CvPoint2D32f C)  
{  
	float t;
	A.x -= C.x; B.x -= C.x;
	A.y -= C.y; B.y -= C.y;
	t = A.x*B.y-A.y*B.x;
	return t==0.0f ? 0 : t>0?1:-1;
}

/* �ж������߶��Ƿ��ཻ
 * A, B �߶�l1�Ķ˵�
 * C, D �߶�l2�Ķ˵�
 * <returns>��������Ϊbool�����Ϊtrue��ʾ�����߶��ཻ�����Ϊfalse��ʾ�����߶β��ཻ</returns>
 */
bool judge2LinesRelation(CvPoint2D32f A, CvPoint2D32f B, CvPoint2D32f C, CvPoint2D32f D)
{
    bool returnResult = true;
	int f1, f2, f3, f4;
    returnResult = judgeAboutCrossStatus(A, B, C, D);
    if (returnResult)//�����ж������߶ο����ཻ
    {
		f1 = leftRight(A, B, C);
		f2 = leftRight(A, B, D);
		f3 = leftRight(C, D, A);
		f4 = leftRight(C, D, B);
        if(((f1 ^ f2) == -2 || f1 == 0 || f2 == 0) && ((f3 ^ f4) == -2 || f3 == 0 || f4 == 0)) //1^-1 = -2 �߶�CD��ֱ��AB�ཻ���߶�AB��ֱ��CD�ཻ=>�߶�CD���߶�AB�ཻ
			returnResult = true;
		else
			returnResult = false;
    }
    return returnResult;
}

/* ��֪������ֱ�߷��̵�һ��ʽ(Ax+By+C=0)
 * M, N ֱ���ϵ�����
 * co һ��ʽ������ϵ������
 */
void generalLinearEqu(CvPoint2D32f *M, CvPoint2D32f *N, double *co)
{
	//��ֱ�߷���һ��ʽAx+By+C=0(��ֱ������ʽ��Ϊһ��ʽ)
	co[0] = (double)N->y - (double)M->y; //A
	co[1] = (double)M->x - (double)N->x; //B
	co[2] = (double)N->x * (double)M->y - (double)M->x * (double)N->y; //C
}


/* ����㵽ֱ�ߵľ���
 * P ��
 * M, N ֱ���ϵ�����
 */
double pointToLine(CvPoint2D32f* P, CvPoint2D32f* M, CvPoint2D32f* N)
{
	double co[3];
	//��ֱ�߷���һ��ʽAx+By+C=0(��ֱ������ʽ��Ϊһ��ʽ)
	generalLinearEqu(M, N, co);
	return fabs(co[0] * (double)P->x + co[1] * (double)P->y + co[2]) / sqrt(co[0] * co[0] + co[1] * co[1]);
}

/* �ж������߶��Ƿ��ܺϲ���һ�����߶Σ���ƽ���ཻ��
 * A, B �߶�m1�Ķ˵�
 * C, D �߶�m2�Ķ˵�
 * angleThreshold �ж�m1��m2�Ƿ�ƽ�е���ֵ(�н���ֵ)
 * distanceThreshold m1�������˵㵽m2�ľ�����ֵ
 */
bool mergeLineJudge(CvPoint2D32f* A, CvPoint2D32f* B, CvPoint2D32f* C, CvPoint2D32f* D, float angleThreshold, float distanceThreshold)
{
	double d1, d2, d3, d4, l1, l2, l3, l4, f, lm1, lm2;
	float theta = calAngleBetweenLines(A, B, C, D);
	int count = 0;
	double co[3]; //ֱ�߷���һ��ʽ������ϵ��
	CvPoint2D32f pArray[4];

	d1 = pointToLine(A, C, D);
	d2 = pointToLine(B, C, D);
	d3 = pointToLine(C, A, B);
	d4 = pointToLine(D, A, B);
	l1 = cal2PointDistance(A, C);
	l2 = cal2PointDistance(A, D);
	l3 = cal2PointDistance(B, C);
	l4 = cal2PointDistance(B, D);
	if(theta < angleThreshold)
	{
		if(l1 < distanceThreshold || l2 < distanceThreshold || l3 < distanceThreshold || l4 < distanceThreshold)
		{
			return true;
		}
		else if(d1 < distanceThreshold || d2 < distanceThreshold || d3 < distanceThreshold || d4 < distanceThreshold)
		{
			if(judge2LinesRelation(*A, *B, *C, *D))
				return true;
			//float m1MaxX = A->x, m1MinX = A->x; //�߶�m1�����˵�������������Сֵ
			//float m1MaxY = A->y, m1MinY = A->y; //�߶�m1�����˵��������������Сֵ
			//float m2MaxX = C->x, m2MinX = C->x; //�߶�m2�����˵�������������Сֵ
			//float m2MaxY = C->y, m2MinY = C->y; //�߶�m2�����˵��������������Сֵ
			//if(A->x<B->x)
			//	m1MaxX = B->x;
			//else
			//	m1MinX = B->x;
			//if(A->y<B->y)
			//	m1MaxY = B->y;
			//else
			//	m1MinY = B->y;
			//if(C->x<D->x)
			//	m2MaxX = D->x;
			//else
			//	m2MinX = D->x;
			//if(C->y<D->y)
			//	m2MaxY = D->y;
			//else
			//	m2MinY = D->y;
			///*if((m1MaxX - m1MinX) < (m1MaxY - m1MinY))
			//{
			//	if(m1MaxY>=m2MinY&&m1MinY<=m2MaxY)
			//		return true;
			//}
			//else
			//{
			//	if(m1MaxX>=m2MinX&&m1MinX<=m2MaxX)
			//		return true;
			//}*/
			//if((m1MaxX>=m2MinX&&m1MinX<=m2MaxX) && (m1MaxY>=m2MinY&&m1MinY<=m2MaxY)) //�߶���Ӿ����ཻ
			//	return true;
			
			//�߶�m2��ֱ��m1�ϵ�ͶӰ
			//generalLinearEqu(A, B, co); //��m1��ֱ�߷���һ��ʽ
			//pArray[0] = *A;
			//pArray[1] = *B;
			////�����߶�m2�������˵���ֱ��m1�ϵ�ͶӰ��
			//pArray[2].x = (float)((co[1]*co[1]*C->x-co[0]*co[1]*C->y-co[0]*co[2])/(co[0]*co[0]+co[1]*co[1]));
			//pArray[2].y = (float)((co[0]*co[0]*C->y-co[0]*co[1]*C->x-co[1]*co[2])/(co[0]*co[0]+co[1]*co[1]));
			//pArray[3].x = (float)((co[1]*co[1]*D->x-co[0]*co[1]*D->y-co[0]*co[2])/(co[0]*co[0]+co[1]*co[1]));
			//pArray[3].y = (float)((co[0]*co[0]*D->y-co[0]*co[1]*D->x-co[1]*co[2])/(co[0]*co[0]+co[1]*co[1]));
			//f = findFarthestDotsFromSet(pArray, 4);
			//lm1 = cal2PointDistance(A, B);
			//lm2 = cal2PointDistance(&(pArray[2]), &(pArray[3]));
			//if(f <= (lm1+lm2))
			//	return true;
		}
	}
	//else
	//{
	//	if((d1 < distanceThreshold/3 && d2 < distanceThreshold/3) || (d3 < distanceThreshold/3 && d4 < distanceThreshold/3))
	//	{
	//		float m1MaxX = A->x, m1MinX = A->x; //�߶�m1�����˵�������������Сֵ
	//		float m1MaxY = A->y, m1MinY = A->y; //�߶�m1�����˵��������������Сֵ
	//		float m2MaxX = C->x, m2MinX = C->x; //�߶�m2�����˵�������������Сֵ
	//		float m2MaxY = C->y, m2MinY = C->y; //�߶�m2�����˵��������������Сֵ
	//		if(A->x<B->x)
	//			m1MaxX = B->x;
	//		else
	//			m1MinX = B->x;
	//		if(A->y<B->y)
	//			m1MaxY = B->y;
	//		else
	//			m1MinY = B->y;
	//		if(C->x<D->x)
	//			m2MaxX = D->x;
	//		else
	//			m2MinX = D->x;
	//		if(C->y<D->y)
	//			m2MaxY = D->y;
	//		else
	//			m2MinY = D->y;
	//		if((m1MaxX>m2MinX&&m1MinX<m2MaxX) && (m1MaxY>m2MinY&&m1MinY<m2MaxY)) //�߶���Ӿ����ཻ
	//			return true;
	//		else if((l1 < distanceThreshold && l2 < distanceThreshold) || (l3 < distanceThreshold && l4 < distanceThreshold))
	//			return true;
	//	}
	//}
	return false;
}

/* �ϲ������߶�
 * A, B �߶�l1�Ķ˵�
 * C, D �߶�l2�Ķ˵�
 */
CvPoint2D32f* mergeLine(CvPoint2D32f* A, CvPoint2D32f* B, CvPoint2D32f* C, CvPoint2D32f* D)
{
	double d1, d2;
	CvPoint2D32f* newPoints = (CvPoint2D32f*)malloc(2*sizeof(CvPoint2D32f));
	newPoints[0] = *A;
	newPoints[1] = *B;
	d1 = cal2PointDistance(A, B);
	d2 = cal2PointDistance(A, C);
	if(d1 < d2)
	{
		d1 = d2;
		newPoints[0] = *A;
		newPoints[1] = *C;
	}
	d2 = cal2PointDistance(A, D);
	if(d1 < d2)
	{
		d1 = d2;
		newPoints[0] = *A;
		newPoints[1] = *D;
	}
	d2 = cal2PointDistance(B, C);
	if(d1 < d2)
	{
		d1 = d2;
		newPoints[0] = *B;
		newPoints[1] = *C;
	}
	d2 = cal2PointDistance(B, D);
	if(d1 < d2)
	{
		d1 = d2;
		newPoints[0] = *B;
		newPoints[1] = *D;
	}
	d2 = cal2PointDistance(C, D);
	if(d1 < d2)
	{
		d1 = d2;
		newPoints[0] = *C;
		newPoints[1] = *D;
	}
	return newPoints;
}

/* Ѱ��SegmentsArray��ӵ�����x����Сx�������㣬ӵ�����y����Сy�������㣻�����ؾ���Զ����������
 * lineSegments �߶�����
 */
CvPoint2D32f* findFarthestDots(SegmentsArray* lineSegments)
{
	float minX, maxX, minY, maxY;
	CvPoint2D32f* minmaxXEndDots = (CvPoint2D32f*)malloc(2*sizeof(CvPoint2D32f));
	CvPoint2D32f* minmaxYEndDots = (CvPoint2D32f*)malloc(2*sizeof(CvPoint2D32f));
	minX = maxX = lineSegments->pointArray[0][0].x;
	minY = maxY = lineSegments->pointArray[0][0].y;
	minmaxXEndDots[0] = lineSegments->pointArray[0][0];
	minmaxXEndDots[1] = lineSegments->pointArray[0][0];
	minmaxYEndDots[0] = lineSegments->pointArray[0][0];
	minmaxYEndDots[1] = lineSegments->pointArray[0][0];
	for(int i=0; i<lineSegments->count; i++)
	{
		for(int j=0; j<2; j++)
		{
			if(minX > lineSegments->pointArray[i][j].x)
			{
				minX = lineSegments->pointArray[i][j].x;
				minmaxXEndDots[0] = lineSegments->pointArray[i][j];
			}
			if(maxX < lineSegments->pointArray[i][j].x)
			{
				maxX = lineSegments->pointArray[i][j].x;
				minmaxXEndDots[1] = lineSegments->pointArray[i][j];
			}

			if(minY > lineSegments->pointArray[i][j].y)
			{
				minY = lineSegments->pointArray[i][j].y;
				minmaxYEndDots[0] = lineSegments->pointArray[i][j];
			}
			if(maxY < lineSegments->pointArray[i][j].y)
			{
				maxY = lineSegments->pointArray[i][j].y;
				minmaxYEndDots[1] = lineSegments->pointArray[i][j];
			}
		}
	}
	double d1 = cal2PointDistance(&(minmaxXEndDots[0]), &(minmaxXEndDots[1]));
	double d2 = cal2PointDistance(&(minmaxYEndDots[0]), &(minmaxYEndDots[1]));
	if(d1 > d2)
	{
		free(minmaxYEndDots);
		return minmaxXEndDots;
	}
	else
	{
		free(minmaxXEndDots);
		return minmaxYEndDots;
	}
}

/* Ѱ�ҵ㼯������֮�����Զ��������
 * pointArray ������
 * pointCount �������е������
 */
CvPoint2D32f* findFarthestDotsFromSet(CvPoint2D32f *pointArray, int pointCount)
{
	float minX, maxX, minY, maxY;
	CvPoint2D32f* minmaxXEndDots = (CvPoint2D32f*)malloc(2*sizeof(CvPoint2D32f));
	CvPoint2D32f* minmaxYEndDots = (CvPoint2D32f*)malloc(2*sizeof(CvPoint2D32f));
	minX = maxX = pointArray[0].x;
	minY = maxY = pointArray[0].y;
	minmaxXEndDots[0] = pointArray[0];
	minmaxXEndDots[1] = pointArray[0];
	minmaxYEndDots[0] = pointArray[0];
	minmaxYEndDots[1] = pointArray[0];
	for(int i=0; i<pointCount; i++)
	{
		if(minX > pointArray[i].x)
		{
			minX = pointArray[i].x;
			minmaxXEndDots[0] = pointArray[i];
		}
		if(maxX < pointArray[i].x)
		{
			maxX = pointArray[i].x;
			minmaxXEndDots[1] = pointArray[i];
		}

		if(minY > pointArray[i].y)
		{
			minY = pointArray[i].y;
			minmaxYEndDots[0] = pointArray[i];
		}
		if(maxY < pointArray[i].y)
		{
			maxY = pointArray[i].y;
			minmaxYEndDots[1] = pointArray[i];
		}
	}
	double d1 = cal2PointDistance(&(minmaxXEndDots[0]), &(minmaxXEndDots[1]));
	double d2 = cal2PointDistance(&(minmaxYEndDots[0]), &(minmaxYEndDots[1]));
	if(d1 > d2)
	{
		free(minmaxYEndDots);
		return minmaxXEndDots;
	}
	else
	{
		free(minmaxXEndDots);
		return minmaxYEndDots;
	}
}

/* Ѱ������ͨ�������ı��νӽ��Ļ�ѧ���߶Σ���Ϊ���ܵĻ�ѧ�����߶Σ�
 * imgCon �洢��ͨ��ͼ��Ľṹ��
 * lineSet �߶μ���
 * indexArr ����ͨ�������ı��νӽ��Ļ�ѧ���߶���lineSet�е���������ṹ��
 */
void findPossibleCharLines(ImageCon* imgCon, SegmentsArray* lineSet, IndexArray* indexArr)
{
	CvPoint2D32f A, B;
	int j = 0;
	int minI=imgCon->minI, maxI=imgCon->maxI, minJ=imgCon->minJ, maxJ=imgCon->maxJ;
	//����ͨ����Χ�ټ�һȦ����ֹ�����ؿ�Ȼ�߶ȵ��µ���Ӿ��ε�����һ����Ϊ��
	minI -= 2;
	maxI += 2;
	minJ -= 2;
	maxJ += 2;
	for(int i=0; i<lineSet->count; i++)
	{
		if(lineSet->flag[i] == 3) //�����ǻ�ѧ�����߶�
		{
			//��ͨ��Խ��ߵĶ˵�
			A.x = (float)minJ;
			A.y = (float)minI;
			B.x = (float)maxJ;
			B.y = (float)maxI;

			//��ѧ���߶�������ı����ĸ��ߴ�������һ������
			if(judgeAboutCrossStatus(A, B, lineSet->pointArray[i][0], lineSet->pointArray[i][1])) //�߶�����ı�������ͨ������ı��Σ��ȼ�����ͨ��Խ��ߵ�����ı��Σ������ཻ
			{
				//��ͨ�򶥱ߵĶ˵�
				A.x = (float)minJ;
				A.y = (float)minI;
				B.x = (float)maxJ;
				B.y = (float)minI;
				if(judge2LinesRelation(A, B, lineSet->pointArray[i][0], lineSet->pointArray[i][1]))
				{
					indexArr->nearLineIndex[j++] = i;
				}
				else
				{
					//��ͨ��ױߵĶ˵�
					A.x = (float)minJ;
					A.y = (float)maxI;
					B.x = (float)maxJ;
					B.y = (float)maxI;
					if(judge2LinesRelation(A, B, lineSet->pointArray[i][0], lineSet->pointArray[i][1]))
					{
						indexArr->nearLineIndex[j++] = i;
					}
					else
					{
						//��ͨ����ߵĶ˵�
						A.x = (float)minJ;
						A.y = (float)minI;
						B.x = (float)minJ;
						B.y = (float)maxI;
						if(judge2LinesRelation(A, B, lineSet->pointArray[i][0], lineSet->pointArray[i][1]))
						{
							indexArr->nearLineIndex[j++] = i;
						}
						else
						{
							//��ͨ���ұߵĶ˵�
							A.x = (float)maxJ;
							A.y = (float)minI;
							B.x = (float)maxJ;
							B.y = (float)maxI;
							if(judge2LinesRelation(A, B, lineSet->pointArray[i][0], lineSet->pointArray[i][1]))
							{
								indexArr->nearLineIndex[j++] = i;
							}
						}
					}
				}
			}
		}
	}
	indexArr->count = j;
}

/* ��ʶ��Ϊ��ѧ�����߶���δ֪�߶ν��кϲ�
 * ��ÿ����ѧ���߶Σ�������˵㸽����Ƭ�Σ����߶ξͼ��뼯�ϣ����߶��жϼн�С����ֵ�������Լ�����ʣ���߶��жϣ�ֱ����������Ԫ��
 * lineSet �߶μ���
 */
void mergeBondAndUnkown(SegmentsArray* lineSet)
{
	int i, j;
	double d1, d2, l1, l2, l3, l4, f;
	float theta, maxX, minX, maxY, minY, maxUnknowX, minUnknowX, maxUnknowY, minUnknowY;
	bool mergeFlag = false; //δ֪�߶��Ƿ��뻯ѧ���߶κϲ��ı�־λ
	CvPoint2D32f pArray[4];
	CvPoint2D32f *farestPoint;
	double co[3]; //ֱ�߷���һ��ʽ������ϵ��

	for(i=0; i<lineSet->count; i++)
	{
		if(lineSet->flag[i] == 3)
		{
			int k=0; //kΪ��ǰ���ڴ����tempLineSet���߶ε�����
			SegmentsArray tempLineSet = {5, 0, (CvPoint2D32f(*)[2])malloc(5*sizeof(CvPoint2D32f[2])), (char*)malloc(5*sizeof(char))};
			tempLineSet.pointArray[tempLineSet.count][0] = lineSet->pointArray[i][0];
			tempLineSet.pointArray[tempLineSet.count][1] = lineSet->pointArray[i][1];
			(tempLineSet.count)++;
			lineSet->flag[i] = 1;
			//����tempLineSet.pointArray[0]��������������Сx�������Сy
			maxX = minX = tempLineSet.pointArray[0][0].x;
			maxY = minY = tempLineSet.pointArray[0][0].y;
			if(tempLineSet.pointArray[0][0].x > tempLineSet.pointArray[0][1].x)
				minX = tempLineSet.pointArray[0][1].x;
			else
				maxX = tempLineSet.pointArray[0][1].x;
			if(tempLineSet.pointArray[0][0].y > tempLineSet.pointArray[0][1].y)
				minY = tempLineSet.pointArray[0][1].y;
			else
				maxY = tempLineSet.pointArray[0][1].y;


			while(k<tempLineSet.count) //���tempLineSet�л���δ������߶�
			{
				for(j=0; j<lineSet->count; j++)
				{
					if(lineSet->flag[j] == 0) //δ֪�߶�
					{
						d1 = pointToLine(&(lineSet->pointArray[j][0]), &(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]));
						d2 = pointToLine(&(lineSet->pointArray[j][1]), &(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]));
						if(d1 <= DISTANCE_THRESHOLD && d2 <= DISTANCE_THRESHOLD) //δ֪�߶������㵽��ѧ���ľ����
						{
							//����lineSet->pointArray[j]��������������Сx�������Сy
							maxUnknowX = minUnknowX = lineSet->pointArray[j][0].x;
							maxUnknowY = minUnknowY = lineSet->pointArray[j][0].y;
							if(lineSet->pointArray[j][0].x > lineSet->pointArray[j][1].x)
								minUnknowX = lineSet->pointArray[j][1].x;
							else
								maxUnknowX = lineSet->pointArray[j][1].x;
							if(lineSet->pointArray[j][0].y > lineSet->pointArray[j][1].y)
								minUnknowY = lineSet->pointArray[j][1].y;
							else
								maxUnknowY = lineSet->pointArray[j][1].y;

							l1 = cal2PointDistance(&(tempLineSet.pointArray[k][0]), &(lineSet->pointArray[j][0]));
							l2 = cal2PointDistance(&(tempLineSet.pointArray[k][0]), &(lineSet->pointArray[j][1]));
							l3 = cal2PointDistance(&(tempLineSet.pointArray[k][1]), &(lineSet->pointArray[j][0]));
							l4 = cal2PointDistance(&(tempLineSet.pointArray[k][1]), &(lineSet->pointArray[j][1]));
							if(l1 <= DISTANCE_THRESHOLD || l2 <= DISTANCE_THRESHOLD || l3 <= DISTANCE_THRESHOLD || l4 <= DISTANCE_THRESHOLD) //ĳ�����߶ζ˵�����
								mergeFlag = true;
							else if((maxX-minX) > (maxY-minY))
							{
								if(maxUnknowX >= minX && minUnknowX <= maxX) //δ֪�߶��ڻ�ѧ���߶��ڲ�
									mergeFlag = true;
							}
							else
							{
								if(maxUnknowY >= minY && minUnknowY <= maxY) //δ֪�߶��ڻ�ѧ���߶��ڲ�
									mergeFlag = true;
							}
						}
					}
					else if(lineSet->flag[j] == 3) //�жϻ�ѧ���߶��Ƿ���tempLineSet.pointArray[0]�߶δ󲿷��غ�
					{
						d1 = pointToLine(&(lineSet->pointArray[j][0]), &(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]));
						d2 = pointToLine(&(lineSet->pointArray[j][1]), &(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]));
						if(d1 <= DISTANCE_THRESHOLD && d2 <= DISTANCE_THRESHOLD) //��ѧ���߶������㵽��ѧ���ľ��붼�ܽ�
						{
							generalLinearEqu(&(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]), co); //�󾭹���ѧ���߶ε������˵��ֱ�߷���һ��ʽ
							pArray[0] = tempLineSet.pointArray[0][0];
							pArray[1] = tempLineSet.pointArray[0][1];
							//�����߶ε������˵���ֱ���ϵ�ͶӰ��
							pArray[2].x = (float)((co[1]*co[1]*(lineSet->pointArray[j][0].x)-co[0]*co[1]*(lineSet->pointArray[j][0].y)-co[0]*co[2])/(co[0]*co[0]+co[1]*co[1]));
							pArray[2].y = (float)((co[0]*co[0]*(lineSet->pointArray[j][0].y)-co[0]*co[1]*(lineSet->pointArray[j][0].x)-co[1]*co[2])/(co[0]*co[0]+co[1]*co[1]));
							pArray[3].x = (float)((co[1]*co[1]*(lineSet->pointArray[j][1].x)-co[0]*co[1]*(lineSet->pointArray[j][1].y)-co[0]*co[2])/(co[0]*co[0]+co[1]*co[1]));
							pArray[3].y = (float)((co[0]*co[0]*(lineSet->pointArray[j][1].y)-co[0]*co[1]*(lineSet->pointArray[j][1].x)-co[1]*co[2])/(co[0]*co[0]+co[1]*co[1]));
							farestPoint = findFarthestDotsFromSet(pArray, 4);
							f = cal2PointDistance(&(farestPoint[0]), &(farestPoint[1]));
							l1 = cal2PointDistance(&(pArray[0]), &(pArray[1]));
							l2 = cal2PointDistance(&(pArray[2]), &(pArray[3]));
							if((l1 + l2 - f) > 0.75*l1 || (l1 + l2 - f) > 0.75*l2)
								mergeFlag = true;
						}
					}
					if(mergeFlag == true)
					{
						/*****���tempLineSet�ڴ治���������鳤��+5*****/
						if(tempLineSet.count >= tempLineSet.length)
						{
							tempLineSet.length += 5;
							CvPoint2D32f (*tempPointArray)[2];
							char* tempFlag;
							tempPointArray = (CvPoint2D32f(*)[2])malloc(sizeof(CvPoint2D32f[2]) * tempLineSet.length);
							tempFlag = (char*)malloc(sizeof(char) * tempLineSet.length);
							for(int m=0; m<tempLineSet.count; m++)
							{
								tempPointArray[m][0] = tempLineSet.pointArray[m][0];
								tempPointArray[m][1] = tempLineSet.pointArray[m][1];
								tempFlag[m] = tempLineSet.flag[m];
							}
							free(tempLineSet.pointArray);
							free(tempLineSet.flag);
							tempLineSet.pointArray = tempPointArray;
							tempLineSet.flag = tempFlag;
						}
						tempLineSet.pointArray[tempLineSet.count][0] = lineSet->pointArray[j][0];
						tempLineSet.pointArray[tempLineSet.count][1] = lineSet->pointArray[j][1];
						(tempLineSet.count)++;
						lineSet->flag[j] = 1;
						mergeFlag = false;
					}
					//else if(lineSet->flag[j] == 3) //��ѧ���߶�
					//{
					//	l1 = cal2PointDistance(&(tempLineSet.pointArray[k][0]), &(lineSet->pointArray[j][0]));
					//	l2 = cal2PointDistance(&(tempLineSet.pointArray[k][0]), &(lineSet->pointArray[j][1]));
					//	l3 = cal2PointDistance(&(tempLineSet.pointArray[k][1]), &(lineSet->pointArray[j][0]));
					//	l4 = cal2PointDistance(&(tempLineSet.pointArray[k][1]), &(lineSet->pointArray[j][1]));
					//	if(l1 <= DISTANCE_THRESHOLD || l2 <= DISTANCE_THRESHOLD || l3 <= DISTANCE_THRESHOLD || l4 <= DISTANCE_THRESHOLD) //ĳ�����߶ζ˵�����
					//	{
					//		theta = calAngleBetweenLines(&(tempLineSet.pointArray[0][0]), &(tempLineSet.pointArray[0][1]), &(lineSet->pointArray[j][0]), &(lineSet->pointArray[j][1]));
					//		if(theta <= (float)(2*M_PI/ANGLE_THRESHOLD))
					//		{
					//			/*****���tempLineSet�ڴ治���������鳤��+5*****/
					//			if(tempLineSet.count >= tempLineSet.length)
					//			{
					//				tempLineSet.length += 5;
					//				CvPoint2D32f (*tempPointArray)[2];
					//				char* tempFlag;
					//				tempPointArray = (CvPoint2D32f(*)[2])malloc(sizeof(CvPoint2D32f[2]) * tempLineSet.length);
					//				tempFlag = (char*)malloc(sizeof(char) * tempLineSet.length);
					//				for(int m=0; m<tempLineSet.count; m++)
					//				{
					//					tempPointArray[m][0] = tempLineSet.pointArray[m][0];
					//					tempPointArray[m][1] = tempLineSet.pointArray[m][1];
					//					tempFlag[m] = tempLineSet.flag[m];
					//				}
					//				free(tempLineSet.pointArray);
					//				free(tempLineSet.flag);
					//				tempLineSet.pointArray = tempPointArray;
					//				tempLineSet.flag = tempFlag;
					//			}
					//			tempLineSet.pointArray[tempLineSet.count][0] = lineSet->pointArray[j][0];
					//			tempLineSet.pointArray[tempLineSet.count][1] = lineSet->pointArray[j][1];
					//			(tempLineSet.count)++;
					//			lineSet->flag[j] = 1;
					//		}
					//	}
					//}
				}
				k++;
			}
			CvPoint2D32f* minmaxXEndDots = findFarthestDots(&tempLineSet);
			lineSet->pointArray[i][0] = minmaxXEndDots[0];
			lineSet->pointArray[i][1] = minmaxXEndDots[1];
			lineSet->flag[i] = 3;
			free(tempLineSet.pointArray);
			free(minmaxXEndDots);
		}
	}
}

/* ���㻯ѧ���߶ε�ƽ������
 * lineSet �߶μ���
 */
void calAvgBondLen(SegmentsArray* lineSet)
{
	double d, sumD = 0;
	int count = 0;
	for(int i=0; i<lineSet->count; i++)
	{
		if(lineSet->flag[i] > 3 && lineSet->flag[i] < 9)
		{
			d = cal2PointDistance(&(lineSet->pointArray[i][0]), &(lineSet->pointArray[i][1]));
			sumD += d;
			count++;
		}
	}
	AVGBONDLEN = sumD/count;
}

///* �ж������߶��Ƿ��ཻ
// * A, B �߶�l1�Ķ˵�
// * C, D �߶�l2�Ķ˵�
// * <returns>��������Ϊbool�����Ϊtrue��ʾ�����߶��ཻ�����Ϊfalse��ʾ�����߶β��ཻ</returns>
// */
//bool judge2LinesRelation(CvPoint2D32f A, CvPoint2D32f B, CvPoint2D32f C, CvPoint2D32f D)
//{
//    bool returnResult = true;
//    returnResult = judgeAboutCrossStatus(A, B, C, D);
//    if (returnResult)//�����ж������߶ο����ཻ
//    {
//        float BAx, BAy, BCx, BCy, BDx, BDy, BABCk, BABDk;
//        BAx = A.x - B.x;
//        BAy = A.y - B.y;
//        BCx = C.x - B.x;
//        BCy = C.y - B.y;
//        BDx = D.x - B.x;
//        BDy = D.y - B.y;
//		BABCk = BAx * BCy - BAy * BCx;
//        BABDk = BAx * BDy - BAy * BDx;
//        if (((BABCk > 0) && (BABDk > 0)) || ((BABCk < 0) && (BABDk < 0)))
//        {
//            returnResult = false;
//        }
//        else if (((BABCk > 0) && (BABDk < 0)) || ((BABCk < 0) && (BABDk > 0)))
//        {
//            float BCBDk;
//            BCBDk = BCx * BDy - BCy * BDx;
//            if (((BABDk > 0) && (BCBDk > 0)) || ((BABDk < 0) && (BCBDk < 0)))
//            {
//                returnResult = true;
//            }
//            else
//            {
//                returnResult = false;
//            }                   
//        }
//        else if ((BABCk == 0)||(BABDk==0))//��C��D��ֱ��AB��
//        {
//            double templine02Coords[3];
//            if (BABCk == 0)//��C��ֱ��AB��
//            {
//                templine02Coords[0] = line02Coords[0];
//                templine02Coords[1] = line02Coords[1];
//                templine02Coords[2] = line02Coords[2];
//            }
//            else//��D��ֱ��AB��
//            {
//                templine02Coords[0] = line02Coords[3];
//                templine02Coords[1] = line02Coords[4];
//                templine02Coords[2] = line02Coords[5];
//            }
//            if (line01Coords[0] == line01Coords[3])//�Ƿ�ֱ������Ƚ�Yֵ
//            {
//                double maxY, minY;
//                maxY = minY = line01Coords[1];
//                if (line01Coords[1] < line01Coords[4])
//                    maxY = line01Coords[4];
//                else
//                    minY = line01Coords[4];
//                if ((templine02Coords[1] >= minY) && (templine02Coords[1] <= maxY))//���߶���
//                    returnResult = true;
//                else
//                    returnResult = false;
//            }
//            else //�Ƚ�Xֵ
//            {
//                double maxX, minX;
//                maxX = minX = line01Coords[0];
//                if (line01Coords[0] < line01Coords[3])
//                    maxX = line01Coords[3];
//                else
//                    minX = line01Coords[3];
//                if ((templine02Coords[0] >= minX) && (templine02Coords[0] <= maxX))//���߶���
//                    returnResult = true;
//                else
//                    returnResult = false;
//            }
//        }
//    }
//    return returnResult;
//}