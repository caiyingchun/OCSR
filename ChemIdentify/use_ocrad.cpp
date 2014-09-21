#include "use_ocrad.h"

/* ���ʶ������ַ�
 * job һ��ʶ����ҵ
 * recog_char ʶ������ַ�������
 */
void outputCharsOCRAD(OCRAD_Descriptor * const & ocrdes, RecogChar* recog_char)
{
	const int blocks = OCRAD_result_blocks( ocrdes );
	int linecounter = 0;
	const char * line;
	int len = 0;
	char* temp = (char*)malloc(sizeof(char));
	recog_char->text[0] = '\0';

	for( int b = 0; b < blocks; ++b )
	{
		const int lines = OCRAD_result_lines( ocrdes, b );
		linecounter += lines;
	}
	recog_char->count = linecounter;
	recog_char->charArray = (char**)realloc(recog_char->charArray, sizeof(char*)*recog_char->count);
	
	linecounter = 0;
	for( int b = 0; b < blocks; ++b )
	{
		const int lines = OCRAD_result_lines( ocrdes, b );
		for( int l = 0; l < lines; ++l )
		{
			line = OCRAD_result_line( ocrdes, b, l );
			if( line && line[0] )
			{
				recog_char->charArray[linecounter] = (char*)malloc(sizeof(char)*strlen(line));
				int line_index = 0;
				for(; (*line != '\n' && *line != '\0'); line++) //�����ַ���line��ȥ�����еĻس���
				{
					recog_char->charArray[linecounter][line_index] = *((char*)line);
					line_index++;
				}
				recog_char->charArray[linecounter][line_index] = '\0';

				temp = (char*)realloc(temp, (strlen(recog_char->text)+1)*sizeof(char));
				strcpy(temp, recog_char->text);
				len = strlen(recog_char->charArray[linecounter])+strlen(recog_char->text)+1;
				recog_char->text = (char*)realloc(recog_char->text, len*sizeof(char));
				strcpy(recog_char->text, temp);
				strcat(recog_char->text, recog_char->charArray[linecounter]);

				linecounter++;
			}
		}
	}
	free(temp);
}

/* ��IplImage���Ҷ�ͼ��ת����ocrad�е�OCRAD_Pixmap
 * image IplImage���͵�ͼ������
 * op OCRAD_Pixmap���͵�ͼ������
 */
void iplImage2OCRAD_Pixmap(IplImage* image, OCRAD_Pixmap* op)
{
	op->width = image->widthStep;
	op->height = image->height;
	op->mode = OCRAD_greymap;
	op->data = (const unsigned char*)image->imageData;
}

/* ʶ��Ҷ�ͼ�е��ַ�
 * image IplImage���͵�ͼ�����ݣ��Ҷ�ͼ��
 * invert �Ƿ�תͼ��ǰ���ͱ���
 * recog_char ʶ������ַ�������
 */
void recognizeCharImageOCRAD(IplImage* image, bool invert, RecogChar* recog_char)
{
	OCRAD_Pixmap op;
	iplImage2OCRAD_Pixmap(image, &op);
	OCRAD_Descriptor * const ocrdes = OCRAD_open();
	OCRAD_set_image(ocrdes, (const OCRAD_Pixmap* const)&op, invert);
	OCRAD_recognize( ocrdes, false );
	outputCharsOCRAD(ocrdes, recog_char);
	OCRAD_close(ocrdes);
}