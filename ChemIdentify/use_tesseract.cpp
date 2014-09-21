#include "baseapi.h"
#include "pic_binarization.h"
#include "use_gocr.h"
#include "use_tesseract.h"

/* ���ʶ������ַ�
 * job һ��ʶ����ҵ
 * recog_char ʶ������ַ�������
 */
void outputCharsTESSER(char* string, RecogChar* recog_char)
{
	int i = 0, j = 0;
	int linecounter = 1;
	int total_chars = 0;
	if(string && string[0])
	{
		//�ҳ�����
		while(string[i] != '\0')
		{
			if(string[i++] == '\n' && i>1 && string[i-2] != '\n')
				linecounter++;
		}
		if(string[i-1] == '\n')
			linecounter--;
		recog_char->count = linecounter;
		recog_char->charArray = (char**)realloc(recog_char->charArray, sizeof(char*)*recog_char->count);
		
		//�ҳ�ÿ�е��ַ��������������ַ���
		i = 0;
		linecounter = 0;
		while(string[i] != '\0')
		{
			j++;
			if(string[i++] == '\n')
			{
				if(j > 1)
				{
					recog_char->charArray[linecounter] = (char*)malloc(sizeof(char)*j);
					total_chars += j-1;
					linecounter++;
				}
				j = 0;
			}
		}
		if(string[i-1] != '\n')
		{
			recog_char->charArray[linecounter] = (char*)malloc(sizeof(char)*(j+1));
			total_chars += j;
		}
		recog_char->text = (char*)realloc(recog_char->text, sizeof(char)*(total_chars+1));

		//�����ַ���
		i = 0;
		j = 0;
		linecounter = 0;
		total_chars = 0;
		while(string[i] != '\0')
		{
			j++;
			if(string[i] != '\n')
			{
				recog_char->charArray[linecounter][j-1] = string[i];
				recog_char->text[total_chars++] = string[i];
			}
			else
			{
				if(j > 1)
				{
					recog_char->charArray[linecounter][j-1] = '\0';
					linecounter++;
				}
				j = 0;
			}
			i++;
		}
		if(string[i-1] != '\n')
			recog_char->charArray[linecounter][j] = '\0';
		recog_char->text[total_chars] = '\0';
	}
	else
	{
		recog_char->count = 0;
		recog_char->text[total_chars] = '\0';
	}
}

/* ʶ��Ҷ�ͼ�е��ַ�
 * image IplImage���͵�ͼ�����ݣ��Ҷ�ͼ��
 * invert �Ƿ�תͼ����ɫ����true����false
 * recog_char ʶ������ַ�������
 */
void recognizeCharImageTESSER(IplImage* image, bool invert, RecogChar* recog_char)
{
	//tesseract::PageSegMode pagesegmode = tesseract:: PSM_SINGLE_CHAR;
	//if(invert)
	//	reverseImgColor(image);
	tesseract::TessBaseAPI api;
	api.Init(NULL, "eng", tesseract::OEM_DEFAULT);  //��ʼ�����������԰������ļ��壺chi_sim;Ӣ�ģ�eng��Ҳ�����Լ�ѵ�����԰�
	api.SetVariable("tessedit_char_whitelist", CHAR_WHITELIST);
	api.SetImage((unsigned char*)image->imageData,image->width,image->height,1,image->widthStep);
	outputCharsTESSER(api.GetUTF8Text(), recog_char);
	//if(invert)
	//	reverseImgColor(image);
}