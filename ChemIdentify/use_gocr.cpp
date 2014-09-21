#include <string.h>
#include "pic_binarization.h"
#include "use_gocr.h"

/* ���ʶ������ַ�
 * job һ��ʶ����ҵ
 * recog_char ʶ������ַ�������
 */
void outputCharsGOCR(job_t *job, RecogChar* recog_char)
{
	int linecounter = 0;
	const char* line;
	int len = 0;
	char* temp = (char*)malloc(sizeof(char));
	recog_char->text[0] = '\0';

	assert(job);
        
	/* TODO: replace getTextLine-loop(line) by output_text() (libs have to use pipes)
	simplify code 2010-09-26
	*/
	line = getTextLine(&(job->res.linelist), linecounter++);
	while (line)
	{
		/* notice: decode() is shiftet to getTextLine since 0.38 */
		line = getTextLine(&(job->res.linelist), linecounter++);
	}
	recog_char->count = linecounter-1;
	recog_char->charArray = (char**)realloc(recog_char->charArray, sizeof(char*)*recog_char->count);
	
	linecounter = 0;
	line = getTextLine(&(job->res.linelist), linecounter++);

	while (line)
	{
		recog_char->charArray[linecounter-1] = (char*)malloc(sizeof(char)*strlen(line));
		int line_index = 0;
		for(; (*line != '\n' && *line != '\0'); line++) //�����ַ���line��ȥ�����еĻس���
		{
			recog_char->charArray[linecounter-1][line_index] = *((char*)line);
			line_index++;
		}
		recog_char->charArray[linecounter-1][line_index] = '\0';

		temp = (char*)realloc(temp, (strlen(recog_char->text)+1)*sizeof(char));
		strcpy(temp, recog_char->text);
		len = strlen(recog_char->charArray[linecounter-1])+strlen(recog_char->text)+1;
		recog_char->text = (char*)realloc(recog_char->text, len*sizeof(char));
		strcpy(recog_char->text, temp);
		strcat(recog_char->text, recog_char->charArray[linecounter-1]);
		
		line = getTextLine(&(job->res.linelist), linecounter++);
	}
	free(temp);
	free_textlines(&(job->res.linelist));
}

/* ��IplImageת����gocr�е�pix
 * image IplImage���͵�ͼ������
 * p pix���͵�ͼ������
 */
void iplImage2Pix(IplImage* image, pix* p)
{
	p->x = image->widthStep;
	p->y = image->height;
	p->bpp = image->nChannels;
	p->p = (unsigned char*)image->imageData;
}

/* ʶ��ͼ���е��ַ�
 * image IplImage���͵�ͼ������
 * invert �Ƿ�תͼ����ɫ����true����false
 * recog_char ʶ������ַ�������
 */
void recognizeCharImageGOCR(IplImage* image, bool invert, RecogChar* recog_char)
{
	IplImage* image_copy = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopy(image, image_copy, NULL);
	if(invert)
		reverseImgColor(image_copy);
	job_t job1, *job; /* fixme, dont want global variables for lib */
	job = OCR_JOB = &job1;
	job_init(job); /* init cfg and db */ 
	job_init_image(job); /* single image */
	iplImage2Pix(image_copy, &job->src.p);
	pgm2asc(job);
	outputCharsGOCR(job, recog_char);
	cvReleaseImage(&image_copy);
}