#include <math.h>
#include "pic_draw.h"
#include "math_cal.h"

/* ����ֱ��ͼ
 * gray_hist �Ҷ�ͼֱ��ͼ
 * hist_size ֱ��ͼ�о���������Ŀ
 */
IplImage* drawHistogram(CvHistogram* gray_hist, int hist_size)
{
	int hist_img_height = 300;
	int scale = 2;
    //����һ��һάֱ��ͼ�ġ�ͼ����������Ϊ�Ҷȼ���*scale����������Ϊ���ظ���
    IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_img_height),8,3);
    cvZero(hist_image);
    //ͳ��ֱ��ͼ�е����ֱ����
    float max_value = 0;
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
    
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��
    for(int i=0;i<hist_size;i++)
    {
        float bin_val = (float)cvGetReal1D(gray_hist->bins, i); //����i�ĸ���
        int intensity = cvRound(bin_val/max_value*hist_img_height);  //Ҫ���Ƶĸ߶�
        cvRectangle(hist_image, cvPoint(i*scale,hist_img_height), cvPoint((i+1)*scale,hist_img_height-intensity), CV_RGB(255,255,255));
    }
	return hist_image;
}

/* ����һ���պ�����
 * image �����߻����ڸ�ͼ����
 * potrace_curve �����Ƶ�����
 * precision ���߱��ָ�ķ���
 * thickness ���Ƶ����ߵĴ�ϸ
 */
void drawCurve(IplImage* image, potrace_curve_t* curve, int precision, int thickness)
{
	double t; //���������߲��������в�����ֵ
	int i, j;
	CvPoint pre_point, now_point;
	CvPoint2D32f bezier_segment[4];
	for(i=0; i<curve->n; i++) //�������Ʊպ������е�ÿ��Ƭ��
	{
		if(i%2==0)
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				for(j=1; j<4; j++)
				{
					bezier_segment[j].x = (float)curve->c[i][j-1].x;
					bezier_segment[j].y = (float)curve->c[i][j-1].y;
				}
				pre_point = calBezierCurvePoint(bezier_segment, 0);
				for(j=1; j<=precision; j++)
				{
					t = (double)j / (double)precision;
					now_point = calBezierCurvePoint(bezier_segment, t);
					cvLine(image,pre_point,now_point,CV_RGB(255,0,255),thickness,CV_AA,0); //ż�������������÷�ɫ
					pre_point = now_point;
				}
			}else if(curve->tag[i] == POTRACE_CORNER) //����ǹս���
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					pre_point.x = (int)curve->c[curve->n-1][2].x;
					pre_point.y = (int)curve->c[curve->n-1][2].y;
				}else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					pre_point.x = (int)curve->c[i-1][2].x;
					pre_point.y = (int)curve->c[i-1][2].y;
				}
				now_point.x = (int)curve->c[i][1].x;
				now_point.y = (int)curve->c[i][1].y;
				cvLine(image,pre_point,now_point,CV_RGB(255,255,255),thickness,CV_AA,0); //ż���ս����ð�ɫ

				pre_point = now_point;
				now_point.x = (int)curve->c[i][2].x;
				now_point.y = (int)curve->c[i][2].y;
				cvLine(image,pre_point,now_point,CV_RGB(255,255,255),thickness,CV_AA,0); //ż���ս����ð�ɫ
			}
		}else
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				for(j=1; j<4; j++)
				{
					bezier_segment[j].x = (float)curve->c[i][j-1].x;
					bezier_segment[j].y = (float)curve->c[i][j-1].y;
				}
				pre_point = calBezierCurvePoint(bezier_segment, 0);
				for(int i=1; i<=precision; i++)
				{
					t = (double)i / (double)precision;
					now_point = calBezierCurvePoint(bezier_segment, t);
					cvLine(image,pre_point,now_point,CV_RGB(0,255,255),thickness,CV_AA,0); //����������������ǳ��ɫ
					pre_point = now_point;
				}
			}else if(curve->tag[i] == POTRACE_CORNER) //����ǹս���
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					pre_point.x = (int)curve->c[curve->n-1][2].x;
					pre_point.y = (int)curve->c[curve->n-1][2].y;
				}else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					pre_point.x = (int)curve->c[i-1][2].x;
					pre_point.y = (int)curve->c[i-1][2].y;
				}
				now_point.x = (int)curve->c[i][1].x;
				now_point.y = (int)curve->c[i][1].y;
				cvLine(image,pre_point,now_point,CV_RGB(255,255,0),thickness,CV_AA,0); //�����ս����û�ɫ

				pre_point = now_point;
				now_point.x = (int)curve->c[i][2].x;
				now_point.y = (int)curve->c[i][2].y;
				cvLine(image,pre_point,now_point,CV_RGB(255,255,0),thickness,CV_AA,0); //�����ս����û�ɫ
			}
		}
	}
}

/* ����ʸ����������»���ͼ��
 * imgSize Ҫ���Ƶ�ͼ���С
 * potrace_state ʸ����������
 */
IplImage* showVectorImg(CvSize imgSize, potrace_state_t* potrace_state)
{
	potrace_path_t* pNext = potrace_state->plist;
	IplImage* image = cvCreateImage(imgSize,8,3);
	cvZero(image);
	while(pNext != NULL)
	{
		drawCurve(image, &(pNext->curve), 50, 1);
		pNext = pNext->next;
	}
	return image;
}

/* ����ʸ����������»���ͼ�񣬽�ÿ��������߷ֱ�ŵ�һ��ͼƬ��
 * imgSize Ҫ���Ƶ�ͼ���С
 * potrace_state ʸ����������
 */
ImageSplitArray* showSplitVectorImg(CvSize imgSize, potrace_state_t* potrace_state)
{
	ImageSplitArray imgSpArrObj = {10, 0, (ImageCon*)malloc(10*sizeof(ImageCon))};
	ImageSplitArray* imgSpArr = &imgSpArrObj;
	potrace_path_t* pNext = potrace_state->plist;
	while(pNext != NULL)
	{
		/*****���imgSpArr�ڴ治�����򽫽ṹ�����鳤��+10*****/
		if(imgSpArr->count >= imgSpArr->length)
		{
			imgSpArr->length += 10;
			ImageCon* tempImgCons;
			tempImgCons = (ImageCon*)malloc(sizeof(ImageCon) * imgSpArr->length);
			for(int m=0; m<imgSpArr->count; m++)
			{
				ImageCon tempImgCon = {cvCreateImage(cvGetSize(imgSpArr->imgCons[m].img), imgSpArr->imgCons[m].img->depth, imgSpArr->imgCons[m].img->nChannels), 0, 0, 0, 0};
				tempImgCons[m] = tempImgCon;
				cvCopy(imgSpArr->imgCons[m].img, tempImgCons[m].img, NULL);
				tempImgCons[m].minI = imgSpArr->imgCons[m].minI;
				tempImgCons[m].minJ = imgSpArr->imgCons[m].minJ;
				tempImgCons[m].maxI = imgSpArr->imgCons[m].maxI;
				tempImgCons[m].maxJ = imgSpArr->imgCons[m].maxJ;
				cvReleaseImage(&(imgSpArr->imgCons[m].img));
			}
			free(imgSpArr->imgCons);
			imgSpArr->imgCons = tempImgCons;
		}

		ImageCon imgCon = {cvCreateImage(imgSize, 8, 3), 0, 0, 0, 0};
		cvZero(imgCon.img);
		drawCurve(imgCon.img, &(pNext->curve), 50, 2);
		imgSpArr->imgCons[imgSpArr->count].img = imgCon.img;
		(imgSpArr->count)++;
		pNext = pNext->next;
	}
	return imgSpArr;
}

/* �Ӻϲ��߶μ����л���ָ��״̬���߶�ͼ��
 * linSet �߶μ���
 * img ָ��״̬���߶�ͼ��
 * flag ��ǰ�߶ε�״̬��ʶ����
 * color ���Ƶ��߶���ɫ
 */
void drawStatedLineImg(SegmentsArray* lineSet, IplImage* img, int flag, CvScalar color)
{
	CvPoint pre_point, now_point;
	//double l;
	for(int i=0; i<lineSet->count; i++)
	{
		//l = cal2PointDistance(&(lineSet->pointArray[i][0]), &(lineSet->pointArray[i][1]));
		if(lineSet->flag[i] == flag)
		{
			pre_point.x = (int)lineSet->pointArray[i][0].x;
			pre_point.y = (int)lineSet->pointArray[i][0].y;
			now_point.x = (int)lineSet->pointArray[i][1].x;
			now_point.y = (int)lineSet->pointArray[i][1].y;
			//if(i%2==0)
			cvLine(img,pre_point,now_point,color,1,8,0);
			//else
				//cvLine(bond_img,pre_point,now_point,cvScalar(255),1,CV_AA,0);
		}
	}
}

/* �Ӻϲ��߶μ����л���δ֪�߶�ͼ��
 * linSet �߶μ���
 * lineSetArray δ�ϲ�ǰ���߶μ�������
 * ele_img δ֪�߶�ͼ��
 */
void drawEleImg(SegmentsArray* lineSet, LineSetArray* lineSetArray, IplImage* ele_img)
{
	CvPoint pre_point, now_point;
	for(int i=0; i<lineSet->count; i++)
	{
		if(lineSet->flag[i] == 0)
		{
			for(int j=0; j<lineSetArray->lineSet[i].count; j++)
			{
				pre_point.x = (int)lineSetArray->lineSet[i].pointArray[j][0].x;
				pre_point.y = (int)lineSetArray->lineSet[i].pointArray[j][0].y;
				now_point.x = (int)lineSetArray->lineSet[i].pointArray[j][1].x;
				now_point.y = (int)lineSetArray->lineSet[i].pointArray[j][1].y;
				//if(i%2==0)
				cvLine(ele_img,pre_point,now_point,cvScalar(255),1,8,0);
				//else
					//cvLine(ele_img,pre_point,now_point,cvScalar(255),1,CV_AA,0);
			}
		}
	}
}

/* �����ڽӾ������ͼ��
 * bondSet ��ѧ������
 * nodes �ڽӾ�����ÿ�д���ĵ�����
 * img ������ɵ�ͼ��
 */
void drawEleImg(SegmentsArray *bondSet, CvPoint2D32f *nodes, IplImage *img)
{
	CvPoint pre_point, now_point;
	for(int i=0; i<bondSet->count; i++)
	{
		if(bondSet->flag[i] != 1)
		{
			pre_point.x = (int)nodes[bondSet->recombinePIndex[i][0]].x;
			pre_point.y = (int)nodes[bondSet->recombinePIndex[i][0]].y;
			now_point.x = (int)nodes[bondSet->recombinePIndex[i][1]].x;
			now_point.y = (int)nodes[bondSet->recombinePIndex[i][1]].y;
			cvLine(img,pre_point,now_point,cvScalar(255),1,8,0);
		}
	}
}