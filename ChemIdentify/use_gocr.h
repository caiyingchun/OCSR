#ifndef _USE_GOCR
#define _USE_GOCR

#include <opencv2/opencv.hpp>

#ifdef __cplusplus
extern "C" {
#endif
	#include <pgm2asc.h>
	#include <gocr.h>
#ifdef __cplusplus
}
#endif

#define SP_CHAR_WHITELIST "BCFHKNOPRSahir23456789+" //ԭʼͼ����ͨ��ָ���ܲ�����ʶ��������ͨ��ָ�����ĵ��ַ�����tesseractʶ��
#define CHAR_WHITELIST "BCFHKNOPRSahir23456789" //ʸ����СƬ����Ͽ��ܲ�����ʶ������ʸ����СƬ�������Ҫ��������������ʶ�����Ľ����ͬ
#define CON_CHAR_WHITELIST "BFHKNPRh" //����߶�����ͨ����ܲ�����ʶ����
#define WIDTH_CHAR "BCFHKNOPRSa" //�߿��С��2�ķ���

/* �洢ʶ������ַ������飬�����е�ÿ��Ԫ�ش���ʶ�����ÿһ���ַ���
 * count ������Ԫ������
 * charArray �ַ�������
 * text ʶ����������ַ��ŵ�һ��
 */
typedef struct _RecogChar
{
	int count;
	char** charArray;
	char* text;
}RecogChar;

void recognizeCharImageGOCR(IplImage*, bool, RecogChar*); //ʶ��ͼ���е��ַ�
void print_output(job_t*); //���ʶ������ַ�

#endif