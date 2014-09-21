#include "binary_pic_processing.h"
#include <math.h>
#include "math_cal.h"
#include "base_functions.h"
#include "pic_draw.h"

/* ʹ������ͼ�Զ�ֵͼ��ϸ��
 * binaryImageData ��ֵͼ������
 */
void thinImage(IplImage* binaryImageData)
{
						/*  �����ɰ�:			*/
					    /*  ��    ��    ��    ��   */
	static int masks[] = { 0200, 0002, 0040, 0010 }; //����ǰ���0��ʾ8������
	static unsigned char _delete[512] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	int width, height; //ͼ��Ŀ�Ⱥ͸߶ȣ���λ�����أ�
	int x, y;
	int loop = 0; //whileѭ���Ĵ���
	int count = 1; //ÿ�α���ͼ��ɾ���ĵ㣨��һ�α���û��ɾ����ʱ��Ϊϸ����ɣ�
	unsigned char* pre_line_neighbor_map; //��ǰ����������һ��������ÿ�����8��������һ��8������ȻҪ��9λ�洢������һ�е�8�������ֻ��Ҫ�õ����ĵ�7λ�����Կ�����8λ��unsigned char�洢��
	int pixel_neighbor_map; //��ǰ��͵�ǰ�����һ�����8�����ڱ���ѭ����Ϊ��ǰ�㣬���´�ѭ����Ϊ��ߵ㣩
	
	width = binaryImageData->width;
	height = binaryImageData->height;
	pre_line_neighbor_map = (unsigned char*) malloc (width*sizeof(unsigned char));
	pre_line_neighbor_map[width-1] = 0;
	while(count)
	{
		/* ���ټ��㵱ǰ���ص��8����ķ��� */
		/* ��ͼ���������£�*/
		/*	a b c d  */
		/*	e f g h  */
		/*	i j k l  */
		/*	m n o p  */
		/* ��k��8������Էֽ�����²��֣�*/
		/* fgh   fg@ | @@h | @@@
		   jkl = jk@ | @@l | @@@ 
		   nop   no@ | @@@ | @@p */
		/* ��һ����Ϊj��8��������һλ&0666���ڶ�����Ϊg��8��������һ��&0110����������ֱ��ȡk�����½ǵ� */
		
		loop++;
		count = 0;
		for(int i=0; i<4; i++){ //���������ĸ�������ɰ�����ɾ��һ��
			pixel_neighbor_map = binaryImageData->imageData[0] != 0; //ͼ��ĵ�һ�����ص�
			//��ͼ���һ���������һ��ȫ�����أ������ǵõ���Щ�����ص�8����Ĵ���
			for(x=0; x<width-1; x++)
				pre_line_neighbor_map[x] = pixel_neighbor_map = ((pixel_neighbor_map<<1)&0006)|(binaryImageData->imageData[x+1]!=0);
		
			for(y=0; y<height-1; y++)
			{
				//���ÿһ�еĵ�һ�������߼�����8����
				pixel_neighbor_map = ((pre_line_neighbor_map[0]<<2)&0110)|(binaryImageData->imageData[(y+1)*binaryImageData->widthStep]!=0);
				for(x=0; x<width-1; x++)
				{
					pixel_neighbor_map = ((pixel_neighbor_map<<1)&0666)|((pre_line_neighbor_map[x]<<3)&0110)|(binaryImageData->imageData[(y+1)*binaryImageData->widthStep+x+1]!=0);
					pre_line_neighbor_map[x] = pixel_neighbor_map;
					if((pixel_neighbor_map&masks[i])==0 && _delete[pixel_neighbor_map])
					{
						count++;
						binaryImageData->imageData[y*binaryImageData->widthStep+x] = 0;
					}
				}
				/* �����ұ�Ե������ */
				pixel_neighbor_map = (pixel_neighbor_map<<1)&0666;
				if((pixel_neighbor_map&masks[i])==0 && _delete[pixel_neighbor_map]){
					count++;
					binaryImageData->imageData[y*binaryImageData->widthStep+width-1] = 0;
				}
			}
			/* ����ױߵ����� */
			//��õױߵ�һ�������߼�����8����
			pixel_neighbor_map = (pre_line_neighbor_map[0]<<2)&0110;
			for(x=0; x<width; x++){
				pixel_neighbor_map = ((pixel_neighbor_map<<1)&0666)|((pre_line_neighbor_map[x]<<3)&0110);
				if((pixel_neighbor_map&masks[i])==0 && _delete[pixel_neighbor_map]){
					count++;
					binaryImageData->imageData[(height-1)*binaryImageData->widthStep+x] = 0;
				}
			}
		}
		printf ("ThinImage: pass %d, %d pixels deleted\n", loop, count);
		//(*foregroundCounts)-=count;
	}
	free(pre_line_neighbor_map);
}

/* �ӵ�(i,j)��ʼ����ͨ����ҷ���(i��j�еĵ�)
 * image ͼ������
 * ijs ��(i,j)8�����еķ���������к��е�����
 * i ��ͨ���еĵ�������
 * j ��ͨ���еĵ�������
 */
void findConnection(IplImage* image, PointsArray *ijs, int i, int j)
{
	/*****���ijs�ڴ治���������鳤��+64*****/
	if(ijs->count+7 >= ijs->length)
	{
		ijs->length += 64;
		int (*tempIjs)[2];
		tempIjs = (int(*)[2])malloc(sizeof(int[2]) * ijs->length);
		for(int m=0; m<ijs->count; m++){
			tempIjs[m][0] = ijs->pointArray[m][0];
			tempIjs[m][1] = ijs->pointArray[m][1];
		}
		free(ijs->pointArray);
		ijs->pointArray = tempIjs;
	}

	int k_start = ijs->count;
	int k_end;
	if(i>0 && j>0 && image->imageData[(i-1)*image->widthStep+j-1] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i-1;
		ijs->pointArray[ijs->count][1] = j-1;
		image->imageData[(i-1)*image->widthStep+j-1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(i>0 && image->imageData[(i-1)*image->widthStep+j] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i-1;
		ijs->pointArray[ijs->count][1] = j;
		image->imageData[(i-1)*image->widthStep+j] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(i>0 && j<image->width-1 && image->imageData[(i-1)*image->widthStep+j+1] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i-1;
		ijs->pointArray[ijs->count][1] = j+1;
		image->imageData[(i-1)*image->widthStep+j+1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(j>0 && image->imageData[i*image->widthStep+j-1] == (char)255) //��
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i;
		ijs->pointArray[ijs->count][1] = j-1;
		image->imageData[i*image->widthStep+j-1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(j<image->width-1 && image->imageData[i*image->widthStep+j+1] == (char)255) //��
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i;
		ijs->pointArray[ijs->count][1] = j+1;
		image->imageData[i*image->widthStep+j+1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(i<image->height-1 && j>0 && image->imageData[(i+1)*image->widthStep+j-1] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i+1;
		ijs->pointArray[ijs->count][1] = j-1;
		image->imageData[(i+1)*image->widthStep+j-1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(i<image->height-1 && image->imageData[(i+1)*image->widthStep+j] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i+1;
		ijs->pointArray[ijs->count][1] = j;
		image->imageData[(i+1)*image->widthStep+j] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	if(i<image->height-1 && j<image->width-1 && image->imageData[(i+1)*image->widthStep+j+1] == (char)255) //����
	{
		//ijs=(int(*)[2]) realloc (ijs, (*k+1)*sizeof(int[2]));
		ijs->pointArray[ijs->count][0] = i+1;
		ijs->pointArray[ijs->count][1] = j+1;
		image->imageData[(i+1)*image->widthStep+j+1] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		(ijs->count)++;
	}
	k_end = ijs->count;
	for(int ki=k_start; ki<k_end; ki++)
		findConnection(image, ijs, ijs->pointArray[ki][0], ijs->pointArray[ki][1]);
}

/* ����ͨ��ָ�ͼ��
 * image ͼ������
 * i Ѱ�ҵ�һ����Ϊ0�ĵ�ʱѰ����ʼ�������У�Ѱ�ҷ���������ң��������£�
 * j Ѱ�ҵ�һ����Ϊ0�ĵ�ʱѰ����ʼ�������У�Ѱ�ҷ���������ң��������£�
 * imgSpArr ���������ͨ������
 */
void splitImage(IplImage* image, int i, int j, ImageSplitArray* imgSpArr)
{
	int ii, ji;
	for(ii=i; ii<image->height; ii++)
	{
		for(ji=j; ji<image->width; ji++)
		{
			if(image->imageData[ii*image->widthStep+ji] != 0)
				break;
		}
		j=0;
		if(ji < image->width && image->imageData[ii*image->widthStep+ji] != 0)
			break;
	}
	if(ii<image->height && ji<image->width)
	{
		PointsArray poiArr = {64, 0, (int(*)[2]) malloc (64*sizeof(int[2])), image->height, image->width, 0, 0}; //��(i,j)8�����еķ���������к���
		PointsArray* ijs = &poiArr;
		ijs->pointArray[ijs->count][0] = ii;
		ijs->pointArray[ijs->count][1] = ji;
		image->imageData[ii*image->widthStep+ji] = 0;
		ijs->minI > ijs->pointArray[ijs->count][0] ? ijs->minI = ijs->pointArray[ijs->count][0] : ijs->minI = ijs->minI;
		ijs->minJ > ijs->pointArray[ijs->count][1] ? ijs->minJ = ijs->pointArray[ijs->count][1] : ijs->minJ = ijs->minJ;
		ijs->maxI < ijs->pointArray[ijs->count][0] ? ijs->maxI = ijs->pointArray[ijs->count][0] : ijs->maxI = ijs->maxI;
		ijs->maxJ < ijs->pointArray[ijs->count][1] ? ijs->maxJ = ijs->pointArray[ijs->count][1] : ijs->maxJ = ijs->maxJ;
		++(ijs->count);
		findConnection(image, ijs, ii, ji);

		/*****���imgSpArr�ڴ治�����򽫽ṹ�����鳤��+10*****/
		if(imgSpArr->count >= imgSpArr->length)
		{
			imgSpArr->length += 10;
			ImageCon* tempImgCons = (ImageCon*)malloc(sizeof(ImageCon) * imgSpArr->length);
			for(int m=0; m<imgSpArr->count; m++)
			{
				ImageCon tempImgCon = {cvCreateImage(cvGetSize(imgSpArr->imgCons[m].img), imgSpArr->imgCons[m].img->depth, imgSpArr->imgCons[m].img->nChannels), imgSpArr->imgCons[m].minI, imgSpArr->imgCons[m].minJ, imgSpArr->imgCons[m].maxI, imgSpArr->imgCons[m].maxJ, imgSpArr->imgCons[m].aspectRatio, 0, 0};
				tempImgCons[m] = tempImgCon;
				cvCopy(imgSpArr->imgCons[m].img, tempImgCons[m].img, NULL);
				cvReleaseImage(&(imgSpArr->imgCons[m].img));
			}
			free(imgSpArr->imgCons);
			imgSpArr->imgCons = tempImgCons;
		}
		//imagesSplit[imagesSplit->count] = cvCreateImage(cvSize(image->width,image->height), image->depth, image->nChannels);
	    ImageCon imgCon = {cvCreateImage(cvSize(ijs->maxJ-ijs->minJ+1, ijs->maxI-ijs->minI+1), image->depth, image->nChannels), ijs->minI, ijs->minJ, ijs->maxI, ijs->maxJ, 0.0f, 0, 0};
		imgSpArr->imgCons[imgSpArr->count] = imgCon;
		cvZero(imgSpArr->imgCons[imgSpArr->count].img);
		imgSpArr->imgCons[imgSpArr->count].aspectRatio = (float)(ijs->maxI-ijs->minI+1) / (float)(ijs->maxJ-ijs->minJ+1);
		for(int m=0; m<ijs->count; m++)
			imgSpArr->imgCons[imgSpArr->count].img->imageData[(ijs->pointArray[m][0]-ijs->minI)*imgSpArr->imgCons[imgSpArr->count].img->widthStep+(ijs->pointArray[m][1]-ijs->minJ)] = (char)255;
		(imgSpArr->count)++;
		free(ijs->pointArray);
		//(*foregroundCounts)-=ijs->count;
		splitImage(image, ii, ji, imgSpArr);
	}
}

/* ��ͼ���б�ʶ���ĳ��characterTag�Ĳ���ȥ��
 * img ��ͨ��ָ�ǰ��ͼ������
 * imgSpArr ���������ͨ������
 */
void removeCharsImage(IplImage* img, ImageSplitArray* imgSpArr, int tag)
{
	int i=0, m=0, n=0;
	IplImage* spImg;
	for(i=0; i<imgSpArr->count; i++)
	{
		if(imgSpArr->imgCons[i].characterTag == tag)
		{
			spImg = imgSpArr->imgCons[i].img;
			for(m=0; m<spImg->height; m++)
			{
				for(n=0; n<spImg->width; n++)
				{
					if(spImg->imageData[m*spImg->widthStep+n] == (char)255)
						img->imageData[(imgSpArr->imgCons[i].minI+m)*img->widthStep+(imgSpArr->imgCons[i].minJ+n)] = 0;
				}
			}
		}
	}
}

/* ȥ��ֱ�߶μ��������ʶ����Ļ�ѧ����ͼ����߶�
 * ԭ��Ϊ���ҳ�����ͨ����ο��ڵ��߶Σ�Ȼ���߶λ�������������ͨ����Ӧ�㲻ֵͬ�ĵ�����ռ�����߶ε������İٷֱȣ�С��һ����ֵ���жϸ��߶����ڸ���ͨ��
 * lineSet ʸ��������ϲ����ֱ�߶μ���
 * lineSetArray ʸ��������ӹյ�Ͽ����ܺϲ��ɳ��߶ε��߶μ��ϵ�����
 * imgSpArr ͼ����ͨ������
 */
void removeCharsVector(SegmentsArray *lineSet, LineSetArray *lineSetArray, ImageSplitArray *imgSpArr)
{
	int i=0, j=0, k=0, m=0, n=0, sumTotal=0, sumRemain=0;
	IplImage *spImg, *tempImg;
	CvPoint pre_point, now_point;
	for(i=0; i<imgSpArr->count; i++)
	{
		if(imgSpArr->imgCons[i].characterTag == 1)
		{
			spImg = imgSpArr->imgCons[i].img;
			tempImg = cvCreateImage(cvGetSize(spImg), spImg->depth, spImg->nChannels);
			for(j=0; j<lineSet->count; j++)
			{
				if(lineSet->flag[j] == 0 && judgeContainStatus(&(lineSet->pointArray[j][0]), &(lineSet->pointArray[j][1]), imgSpArr->imgCons[i].minI, imgSpArr->imgCons[i].maxI, imgSpArr->imgCons[i].minJ, imgSpArr->imgCons[i].maxJ))
				{
					cvZero(tempImg);
					sumTotal=sumRemain=0;
					for(k=0; k<lineSetArray->lineSet[j].count; k++)
					{
						pre_point.x = (int)lineSetArray->lineSet[j].pointArray[k][0].x - imgSpArr->imgCons[i].minJ;
						pre_point.y = (int)lineSetArray->lineSet[j].pointArray[k][0].y - imgSpArr->imgCons[i].minI;
						now_point.x = (int)lineSetArray->lineSet[j].pointArray[k][1].x - imgSpArr->imgCons[i].minJ;
						now_point.y = (int)lineSetArray->lineSet[j].pointArray[k][1].y - imgSpArr->imgCons[i].minI;
						cvLine(tempImg,pre_point,now_point,cvScalar(255),1,8,0);
					}
					for(m=0; m<tempImg->height; m++)
					{
						for(n=0; n<tempImg->width; n++)
						{
							if(tempImg->imageData[m*tempImg->widthStep+n]){
								sumTotal++;
								if(!spImg->imageData[m*tempImg->widthStep+n])
									sumRemain++;
							}
						}
					}
					if((double)sumRemain/(double)sumTotal < 0.6 || sumRemain < 6)
						lineSet->flag[j] = 2;
				}
			}
		}
	}
}

/* ��ֵͼ��ʸ����(�÷�����ʸ����ͼ�����������������)
 * binaryImageData ��ֵͼ������
 */
potrace_state_t* binaryToVector(IplImage* binaryImageData)
{
	/**********����ֵ�Ҷ�ͼת����λͼ��ʾ**********/
	int dy = (binaryImageData->width % NATIVE_MACHINE_WORD == 0) ? binaryImageData->width / NATIVE_MACHINE_WORD : binaryImageData->width / NATIVE_MACHINE_WORD + 1; //ͼ��ÿ��������dy�������ֱ�ʾ
	int map_size = dy * binaryImageData->height; //ͼ���ܻ�������
	potrace_word one_pixel_to_bit; //ÿ��������0,1��ʾ��0��ʾ������1��ʾǰ����
	int i,j;
	potrace_bitmap_t bit_img = {binaryImageData->width, binaryImageData->height, dy, (potrace_word*) malloc (map_size * sizeof(potrace_word))};
	for(i=0; i<map_size; i++)
	{
		bit_img.map[i] = 0;
		for(j=0; j<32; j++)
		{
			one_pixel_to_bit = ((i%dy)*NATIVE_MACHINE_WORD+j) <= binaryImageData->widthStep ? (binaryImageData->imageData[i/dy*binaryImageData->widthStep+(i%dy)*NATIVE_MACHINE_WORD+j] != 0) : 0;
			bit_img.map[i] |= (one_pixel_to_bit<<(NATIVE_MACHINE_WORD-1-j));
		}
		//printf("%x\n", bit_img.map[i]);
	}
	/**********����ֵ�Ҷ�ͼת����λͼ��ʾ**********/

	potrace_param_t* potrace_param = potrace_param_default();
	potrace_param->alphamax = 0.0;
	potrace_state_t* potrace_state = potrace_trace(potrace_param, &bit_img);
	potrace_param_free(potrace_param);
	free(bit_img.map);
	return potrace_state;
}

/* ȥ��ʸ��������������ı��������ߣ�ͨ��������ֵȥ����
 * potrace_state ʸ����������
 * threshold �����������м��������Ƶ㵽�����������յ��ֱ�ߵľ�����ֵ
 */
void removeCurvingBezierByDistance(potrace_state_t* potrace_state, double threshold)
{
	int i, j;
	double d1, d2;
	potrace_path_t* pNext = potrace_state->plist;
	CvPoint2D32f bezier_segment[4];
	while(pNext != NULL)
	{
		potrace_curve_t* curve = &(pNext->curve);
		for(i=0; i<curve->n; i++) //�����պ������е�ÿ��Ƭ��
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}
				else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				for(j=1; j<4; j++)
				{
					bezier_segment[j].x = (float)curve->c[i][j-1].x;
					bezier_segment[j].y = (float)curve->c[i][j-1].y;
				}
				//������Ƭ�����м��������Ƶ㵽���������յ��ֱ�ߵľ���
				d1 = pointToLine(&(bezier_segment[1]), &(bezier_segment[0]), &(bezier_segment[3]));
				d2 = pointToLine(&(bezier_segment[2]), &(bezier_segment[0]), &(bezier_segment[3]));
				if(d1>=threshold || d2 >=threshold) //ȥ�����벻С����ֵ�ı���������Ƭ��
					curve->tag[i] = POTRACE_TOO_BENT;
			}
		}
		pNext = pNext->next;
	}
}

/* ȥ��ʸ��������������ı��������ߣ�ͨ���Ƕ���ֵȥ����
 * potrace_state ʸ����������
 * threshold ����������(���,���Ƶ�1)����l1��(�յ�,���Ƶ�2)����l2�ļнǵ���ֵ
 */
void removeCurvingBezierByAngle(potrace_state_t* potrace_state, float threshold)
{
	int i, j;
	float theta; //l1��l2�ļнǦȣ���ΧΪ[0, ��/2]
	potrace_path_t* pNext = potrace_state->plist;
	CvPoint2D32f bezier_segment[4];
	while(pNext != NULL)
	{
		potrace_curve_t* curve = &(pNext->curve);
		for(i=0; i<curve->n; i++) //�����պ������е�ÿ��Ƭ��
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}
				else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				for(j=1; j<4; j++)
				{
					bezier_segment[j].x = (float)curve->c[i][j-1].x;
					bezier_segment[j].y = (float)curve->c[i][j-1].y;
				}
				theta = calAngleBetweenLines(&(bezier_segment[0]), &(bezier_segment[1]), &(bezier_segment[2]), &(bezier_segment[3]));
				if(theta>=threshold) //ȥ���нǲ�С����ֵ�ı���������Ƭ��
					curve->tag[i] = POTRACE_TOO_BENT;
			}
		}
		pNext = pNext->next;
	}
}

/* ȥ��ʸ��������г���С����ֵ��Ƭ��
 * potrace_state ʸ����������
 * threshold ������ֵ
 */
void removeVectorSmallSegments(potrace_state_t* potrace_state, double threshold)
{
	int i;
	potrace_path_t* pNext = potrace_state->plist;
	CvPoint2D32f bezier_segment[3];
	double length; //Ƭ�γ���
	while(pNext != NULL)
	{
		potrace_curve_t* curve = &(pNext->curve);
		for(i=0; i<curve->n; i++) //�����պ������е�ÿ��Ƭ��
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}
				else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				bezier_segment[1].x = (float)curve->c[i][2].x;
				bezier_segment[1].y = (float)curve->c[i][2].y;
				length = cal2PointDistance(&(bezier_segment[0]), &(bezier_segment[1]));
				if(length <= threshold) //ȥ��ʸ��������г���С����ֵ��Ƭ��
					curve->tag[i] = POTRACE_TOO_SHORT;
			}
			else if(curve->tag[i] == POTRACE_CORNER) //����ǹս���
			{
				if(i==0) //�պ����ߵ�һ��Ƭ�ε���ʼ�������һ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[curve->n-1][2].x;
					bezier_segment[0].y = (float)curve->c[curve->n-1][2].y;
				}
				else //����һ��Ƭ�ε���ʼ����ǰһ��Ƭ�ε��յ�
				{
					bezier_segment[0].x = (float)curve->c[i-1][2].x;
					bezier_segment[0].y = (float)curve->c[i-1][2].y;
				}
				bezier_segment[1].x = (float)curve->c[i][1].x;
				bezier_segment[1].y = (float)curve->c[i][1].y;
				bezier_segment[2].x = (float)curve->c[i][2].x;
				bezier_segment[2].y = (float)curve->c[i][2].y;
				length = cal2PointDistance(&(bezier_segment[0]), &(bezier_segment[1])) + cal2PointDistance(&(bezier_segment[1]), &(bezier_segment[2]));
				if(length <= threshold) //ȥ��ʸ��������г���С����ֵ��Ƭ��
					curve->tag[i] = POTRACE_TOO_SHORT;
			}
		}
		pNext = pNext->next;
	}
}

/* ȥ������������
 * potrace_state ʸ����������
 */
void removeBezierSegments(potrace_state_t* potrace_state)
{
	int i;
	potrace_path_t* pNext = potrace_state->plist;
	while(pNext != NULL)
	{
		potrace_curve_t* curve = &(pNext->curve);
		for(i=0; i<curve->n; i++) //�����պ������е�ÿ��Ƭ��
		{
			if(curve->tag[i] == POTRACE_CURVETO) //����Ǳ���������Ƭ��
				curve->tag[i] = POTRACE_NODRAW_BEZIER;
		}
		pNext = pNext->next;
	}
}


/* ��ʸ��������еĹս��ߴӹյ㴦�Ͽ������Ⱥϲ�ƽ���������߶�
 * potrace_state ʸ����������
 * lineSegments �սǵ�Ͽ����ϲ�����߶�����
 */
void breakCornerAndRecombine(potrace_state_t* potrace_state, SegmentsArray* lineSegments)
{
	int i;
	potrace_path_t* pNext = potrace_state->plist;
	while(pNext != NULL)
	{
		potrace_curve_t* curve = &(pNext->curve);
		/*****ΪlineSegments����curve->n�����鳤��*****/
		lineSegments->length = lineSegments->count+2*curve->n;
		CvPoint2D32f (*tempPointArray)[2];
		char* tempFlag;
		tempPointArray = (CvPoint2D32f(*)[2])malloc(sizeof(CvPoint2D32f[2]) * lineSegments->length);
		tempFlag = (char*)malloc(sizeof(char) * lineSegments->length);
		for(int m=0; m<lineSegments->count; m++)
		{
			tempPointArray[m][0] = lineSegments->pointArray[m][0];
			tempPointArray[m][1] = lineSegments->pointArray[m][1];
			tempFlag[m] = lineSegments->flag[m];
		}
		free(lineSegments->pointArray);
		free(lineSegments->flag);
		lineSegments->pointArray = tempPointArray;
		lineSegments->flag = tempFlag;

		for(i=0; i<curve->n; i++) //�����պ������е�ÿ��Ƭ��
		{
			if(curve->tag[i] == POTRACE_CORNER) //����ǹս���Ƭ��
			{
				lineSegments->flag[lineSegments->count] = 0;
				//ÿ��ѭ���洢���ǵ�ǰ�ս��ߵĵڶ���ֱ��Ƭ�κ���һ���ս��ߵĵ�һ��ֱ��Ƭ��
				lineSegments->pointArray[lineSegments->count][0].x = (float)curve->c[i][1].x;
				lineSegments->pointArray[lineSegments->count][0].y = (float)curve->c[i][1].y;
				lineSegments->pointArray[lineSegments->count][1].x = (float)curve->c[i][2].x;
				lineSegments->pointArray[lineSegments->count][1].y = (float)curve->c[i][2].y;
				(lineSegments->count)++;

				lineSegments->flag[lineSegments->count] = 0;
				lineSegments->pointArray[lineSegments->count][0].x = (float)curve->c[i][2].x;
				lineSegments->pointArray[lineSegments->count][0].y = (float)curve->c[i][2].y;
				if(i == curve->n-1) //���һ������Ƭ�ε���һ���ս����ǵ�һ���ս���
				{
					lineSegments->pointArray[lineSegments->count][1].x = (float)curve->c[0][1].x;
					lineSegments->pointArray[lineSegments->count][1].y = (float)curve->c[0][1].y;
				}
				else
				{
					lineSegments->pointArray[lineSegments->count][1].x = (float)curve->c[i+1][1].x;
					lineSegments->pointArray[lineSegments->count][1].y = (float)curve->c[i+1][1].y;
				}
				(lineSegments->count)++;

				//�жϸô�ѭ���洢������ֱ��Ƭ���Ƿ��ܹ���һ����ֱ��
				if(mergeLineJudge(&(lineSegments->pointArray[lineSegments->count-2][0]), &(lineSegments->pointArray[lineSegments->count-2][1]), &(lineSegments->pointArray[lineSegments->count-1][0]), &(lineSegments->pointArray[lineSegments->count-1][1]), (float)(M_PI/ANGLE_THRESHOLD), (float)DISTANCE_THRESHOLD))
				{
					CvPoint2D32f* temp = mergeLine(&(lineSegments->pointArray[lineSegments->count-2][0]), &(lineSegments->pointArray[lineSegments->count-2][1]), &(lineSegments->pointArray[lineSegments->count-1][0]), &(lineSegments->pointArray[lineSegments->count-1][1]));
					lineSegments->pointArray[lineSegments->count-2][0] = temp[0];
					lineSegments->pointArray[lineSegments->count-2][1] = temp[1];
					lineSegments->flag[lineSegments->count-1] = 1;
				}
			}
		}
		pNext = pNext->next;
	}
}

/* Ѱ���ܺϲ��ɳ��߶ε��߶μ���
 * lineSegments �����߶ε�����
 * lineSetArray lineSegments����
 * <return>�߶μ��Ϲ��ɵ�����</return>
 */
LineSetArray* groupLineSegments(SegmentsArray* lineSegments, LineSetArray* lineSetArray)
{
	/*****Ѱ���ܺϲ��ɳ��߶ε��߶μ���*****/
	for(int i=0; i<lineSegments->count; i++)
	{
		if(lineSegments->flag[i] == 0)
		{
			int k=0; //kΪ��ǰ���ڴ����tempLineSet���߶ε�����
			SegmentsArray tempLineSet = {5, 0, (CvPoint2D32f(*)[2])malloc(5*sizeof(CvPoint2D32f[2])), (char*)malloc(5*sizeof(char))};
			tempLineSet.pointArray[tempLineSet.count][0] = lineSegments->pointArray[i][0];
			tempLineSet.pointArray[tempLineSet.count][1] = lineSegments->pointArray[i][1];
			(tempLineSet.count)++;
			lineSegments->flag[i] = 1;
			while(k<tempLineSet.count) //���tempLineSet�л���δ������߶�
			{
				for(int j=i+1; j<lineSegments->count; j++)
				{
					if(lineSegments->flag[j] == 0)
					{
						if(mergeLineJudge(&(tempLineSet.pointArray[k][0]), &(tempLineSet.pointArray[k][1]), &(lineSegments->pointArray[j][0]), &(lineSegments->pointArray[j][1]), (float)(M_PI/ANGLE_THRESHOLD), (float)DISTANCE_THRESHOLD))
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

							tempLineSet.pointArray[tempLineSet.count][0] = lineSegments->pointArray[j][0];
							tempLineSet.pointArray[tempLineSet.count][1] = lineSegments->pointArray[j][1];
							(tempLineSet.count)++;
							lineSegments->flag[j] = 1;
						}
					}
				}
				k++;
			}
			lineSetArray->lineSet[lineSetArray->count] = tempLineSet;
			(lineSetArray->count)++;
			/*****���lineSetArray�ڴ治���������鳤��+10*****/
			if(lineSetArray->count >= lineSetArray->length)
			{
				lineSetArray->length += 10;
				SegmentsArray* tempSegmentsArray = (SegmentsArray*)malloc(lineSetArray->length * sizeof(SegmentsArray));
				for(int m=0; m<lineSetArray->count; m++)
				{
					tempSegmentsArray[m] = lineSetArray->lineSet[m];
				}
				free(lineSetArray->lineSet);
				lineSetArray->lineSet = tempSegmentsArray;
			}
		}
	}
	return lineSetArray;
}

/* ��LineSetArray�е�ÿ��lineSet������һ���߶θ������е������߶Σ����Ժϲ�����߶���Ѱ���Ƿ��п��Ժϲ���
 * lineSetArray groupLineSegments�����ķ��ؽ��
 * mergedLine �ܸ�������lineSet��ֱ�߼���
 */
void replaceLineSet(LineSetArray* lineSetArray, SegmentsArray* mergedLine)
{
	for(int i=0; i<lineSetArray->count; i++)
	{
		CvPoint2D32f* tempDots = findFarthestDots(&(lineSetArray->lineSet[i]));
		mergedLine->pointArray[i][0] = tempDots[0];
		mergedLine->pointArray[i][1] = tempDots[1];
		mergedLine->flag[i] = 0;
	}
}

/* ��ʸ�������ת����ֱ�߶μ���
 * potrace_state ʸ����������
 * lineSetArray ʸ��������ӹյ�Ͽ����ܺϲ��ɳ��߶ε��߶μ��ϵ�����
 * mergedLine ����ֱ�߶μ���
 */
void vector2LineSet(potrace_state_t* potrace_state, LineSetArray* lineSetArray, SegmentsArray* mergedLine)
{
	SegmentsArray lineSegments = {0, 0, (CvPoint2D32f(*)[2])malloc(sizeof(CvPoint2D32f[2])), (char*)malloc(sizeof(char))};
	breakCornerAndRecombine(potrace_state, &lineSegments);
	/*IplImage* bond_img = cvCreateImage(cvSize(500, 500), 8, 3);
	cvZero(bond_img);
	drawStatedLineImg(&lineSegments, bond_img, 0, CV_RGB(255,255,0));
	cvNamedWindow("ddd", 1);
    cvShowImage("ddd",bond_img);*/
	groupLineSegments(&lineSegments, lineSetArray);
	mergedLine->length = lineSetArray->count;
	mergedLine->count = lineSetArray->count;
	mergedLine->pointArray = (CvPoint2D32f(*)[2])realloc(mergedLine->pointArray, mergedLine->length*sizeof(CvPoint2D32f[2]));
	mergedLine->flag = (char*)realloc(mergedLine->flag, mergedLine->length*sizeof(char));
	replaceLineSet(lineSetArray, mergedLine);

	//�Ժϲ�����߶���Ѱ���Ƿ��п��Ժϲ���
	/*int preMergedLineCount = mergedLine->count + 1;
	while(preMergedLineCount > mergedLine->count)
	{
		preMergedLineCount = mergedLine->count;
		lineSetArray->length = 10;
		lineSetArray->count = 0;
		lineSetArray->lineSet = (SegmentsArray*)malloc(10*sizeof(SegmentsArray));
		groupLineSegments(mergedLine, lineSetArray);
		mergedLine->length = lineSetArray->count;
		mergedLine->count = lineSetArray->count;
		mergedLine->pointArray = (CvPoint2D32f(*)[2])realloc(mergedLine->pointArray, mergedLine->length*sizeof(CvPoint2D32f[2]));
		mergedLine->flag = (bool*)realloc(mergedLine->flag, mergedLine->length*sizeof(bool));
		replaceLineSet(lineSetArray, mergedLine);
	}*/
}

/* ���㻯ѧ�����뻯ѧ�����߶γ��ȵ�������ֵ(ȡ�߶γ��ȴӴ�С�����ǰ70%�������ǳ��ȵ�ƽ��ֵ���ٳ���0.7Ϊ��ֵ)
 * lineSet �߶μ���
 */
double calCBThreshold(SegmentsArray* lineSet)
{
	double* lineLen = (double*)malloc(lineSet->count * sizeof(double));
	int k = 0, k_bak = 0;
	int i = 0;
	double* temp;
	double sum = 0;
	for(i=0; i<lineSet->count; i++)
	{
		lineLen[k++] = cal2PointDistance(&(lineSet->pointArray[i][0]), &(lineSet->pointArray[i][1]));
	}
	temp = (double*)malloc(k * sizeof(double));
	mergeSortDouble(lineLen, 0, k-1, temp);
	k_bak = k;
	k = k * 0.7;
	for(i=0; i<k; i++)
		sum += lineLen[i];
	free(lineLen);
	return (sum / k) * 0.7;
}

/* ȥ���߶μ����г���С����ֵ���߶�
 * lineSet �߶μ���
 * threshold ������ֵ(���ֵΪ0�����Զ�������ֵ)
 * <return>�����ǻ�ѧ�����߶�����</return>
 */
int removeLineSetSmallSegments(SegmentsArray* lineSet, double threshold)
{
	int i, bondCounts = 0;
	double d;
	if(threshold == 0.0)
		threshold = calCBThreshold(lineSet);
	for(i=0; i<lineSet->count; i++)
	{
		d = cal2PointDistance(&(lineSet->pointArray[i][0]), &(lineSet->pointArray[i][1]));
		if(d > threshold)
		{
			lineSet->flag[i] = 3;
			bondCounts++;
		}
	}
	return bondCounts;
}