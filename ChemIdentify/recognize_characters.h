#ifndef _RECOG_CHAR
#define _RECOG_CHAR

void calSpHeightRange(ImageSplitArray*, float*); //��ѧ������ͨ��ĸ߶�������(ȡ�߿�ȴ���1����ͨ��߶ȵ�ǰ70%�������Ǹ߶ȵ�ƽ��ֵ���ٳ���1.5Ϊ���ޡ�ȡ�߿�ȴ���1����ͨ��߶ȵĺ�30%�������Ǹ߶ȵ�ƽ��ֵ���ٳ���0.5Ϊ����)
void updateSpHeightRange(ImageSplitArray*, float*); //��ʶ�����ѧ���ź���»�ѧ���Ÿ߶ȵ�������
void regSpChars(ImageSplitArray*, float*); //��gocr��ocrad��tesseract��ͬʶ����ͨ���е��ַ�
void regVectorSpChars(ImageSplitArray*, float*, IplImage*); //��gocr��ocrad��tesseract��ͬʶ��ͼ��ʸ����������С�߶�����ɵĻ�ѧԪ��ͼ��
void regConCombineLine(ImageSplitArray *, SegmentsArray *, int, float *); //ʶ��δ֪��ͨ���뻯ѧ���߶ε����
void addImgSpToCharSp(ImageSplitArray *, ImageSplitArray *); //����ѧԪ�ط�����ͨ��ȡ���������洢

#endif