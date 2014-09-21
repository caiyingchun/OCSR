#include "pic_binarization.h"

/* ��ȡ�Ҷ�ͼ��һάֱ��ͼ
 * grayImageData �Ҷ�ͼ������
 * hist_size ֱ��ͼ�о���������Ŀ
 * ranges �Ҷȼ��ķ�Χ�б�
 */
CvHistogram* getHistogram(IplImage* grayImageData, int hist_size, float** ranges)
{
    //����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    //����Ҷ�ͼ���һάֱ��ͼ
    cvCalcHist(&grayImageData,gray_hist,0,0);
    //��һ��ֱ��ͼ
    //cvNormalizeHist(gray_hist,1.0);
	return gray_hist;
}

/* ����Զ���ȡ��ֵ����ֵ
 * grayImageData �Ҷ�ͼ������
 * gray_hist ֱ��ͼ
 */
int otsu(IplImage* grayImageData, CvHistogram* gray_hist)
{
	int threshold = 0; //�Ҷ�ͼ��ֵ����ֵ
	double total_pixels_num = 0; //�Ҷ�ͼ����������
	double total_gray_level = 0; //�Ҷ�ͼ���ܻҶ�ֵ
	double low_pixels_num = 0; //����ֵС�ڵ�����ֵ����������
	double low_gray_level = 0; //����ֵС�ڵ�����ֵ���ܻҶ�ֵ
	double low_average_gray_level = 0; //����ֵС�ڵ�����ֵ��ƽ���Ҷ�ֵ
	double high_pixels_num = 0; //����ֵ������ֵ����������
	double high_gray_level = 0; //����ֵ������ֵ���ܻҶ�ֵ
	double high_average_gray_level = 0; //����ֵ������ֵ��ƽ���Ҷ�ֵ
	double max_variance = 0; //�����䷽��
	double temp_variance = 0; //ÿ�α�������䷽��
	int i = 0, j = 0;
	int hist_size = gray_hist->mat.dim[0].size; //ֱ��ͼ�о���������Ŀ
	for(i=0; i<hist_size; i++)
	{
		total_pixels_num += gray_hist->mat.data.fl[i];
		total_gray_level += i*gray_hist->mat.data.fl[i];
	}
	for(i=0; i<hist_size; i++)
	{
		low_pixels_num += gray_hist->mat.data.fl[i];
		low_gray_level += i*gray_hist->mat.data.fl[i];
		high_pixels_num = total_pixels_num - low_pixels_num;
		high_gray_level = total_gray_level - low_gray_level;
		if(low_pixels_num > 0)
			low_average_gray_level = low_gray_level / low_pixels_num;
		else
			low_average_gray_level = 0;
		if(high_pixels_num > 0)
			high_average_gray_level = high_gray_level / high_pixels_num;
		else
			high_average_gray_level = 0;
		temp_variance = low_pixels_num * high_pixels_num * (low_average_gray_level - high_average_gray_level) * (low_average_gray_level - high_average_gray_level);
		if(max_variance <= temp_variance)
		{
			max_variance = temp_variance;
			threshold = i;
		}
	}
	return threshold;
}

/* �Ҷ�ͼ��ֵ��
 * grayImageData �Ҷ�ͼ������
 * threshold ��ֵ����ֵ
 */
void grayImgBinarization(IplImage* grayImageData, int threshold)
{
	char* imgData = grayImageData->imageData;
	for(int i=0; i<grayImageData->height; i++)
	{
		for(int j=0; j<grayImageData->width; j++)
		{
			if((unsigned char)imgData[i*grayImageData->widthStep+j] <= threshold)
			{
				//(*foregroundCounts)++;
				imgData[i*grayImageData->widthStep+j] = (char)255;
			}
			else
				imgData[i*grayImageData->widthStep+j] = 0;
		}
	}
}

/* ͼ��ڰ׷�ת
 * grayImageData �Ҷ�ͼ������
 * threshold ��ֵ����ֵ
 */
void reverseImgColor(IplImage* imageData)
{
	char* imgData = imageData->imageData;
	for(int i=0; i<imageData->height; i++)
	{
		for(int j=0; j<imageData->width; j++)
		{
			if((unsigned char)imgData[i*imageData->widthStep+j] == 255)
			{
				imgData[i*imageData->widthStep+j] = 0;
			}
			else
				imgData[i*imageData->widthStep+j] = (char)255;
		}
	}
}
