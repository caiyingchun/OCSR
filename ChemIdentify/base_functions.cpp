#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "base_functions.h"

/* int��������鲢����
 * origArr �����������
 * begin �����������ʼλ��
 * end ��������Ľ���λ��
 * temp ��ʱ�洢ÿ�εݹ�ʱ�ͺϲ����������������
 */
void mergeSortInt(int* origArr, int begin, int end, int* temp)
{
	int i=begin, j=0, k=0, mid=0;
	if(begin < end)
	{
		mid = (begin+end)/2;
		j = mid+1;
		mergeSortInt(origArr, begin, mid, temp); //�������
		mergeSortInt(origArr, mid+1, end, temp); //�ұ�����
		//�ϲ��������ߵ�����
		while(i<=mid && j<=end)
		{
			if(origArr[i]>origArr[j])
				temp[k++] = origArr[i++];
			else
				temp[k++] = origArr[j++];
		}
		while(i<=mid)
			temp[k++] = origArr[i++];
		while(j<=end)
			temp[k++] = origArr[j++];
		for(i=0; i<k; i++)
			origArr[begin+i] = temp[i];
	}
}

/* double��������鲢����
 * origArr �����������
 * begin �����������ʼλ��
 * end ��������Ľ���λ��
 * temp ��ʱ�洢ÿ�εݹ�ʱ�ͺϲ����������������
 */
void mergeSortDouble(double* origArr, int begin, int end, double* temp)
{
	int i=begin, j=0, k=0, mid=0;
	if(begin < end)
	{
		mid = (begin+end)/2;
		j = mid+1;
		mergeSortDouble(origArr, begin, mid, temp); //�������
		mergeSortDouble(origArr, mid+1, end, temp); //�ұ�����
		//�ϲ��������ߵ�����
		while(i<=mid && j<=end)
		{
			if(origArr[i]>origArr[j])
				temp[k++] = origArr[i++];
			else
				temp[k++] = origArr[j++];
		}
		while(i<=mid)
			temp[k++] = origArr[i++];
		while(j<=end)
			temp[k++] = origArr[j++];
		for(i=0; i<k; i++)
			origArr[begin+i] = temp[i];
	}
}

/* �ַ����滻����
 * src Դ�ַ���
 * old_str ���滻�ľ��ַ���
 * new_str �����滻�����ַ���
 */
char* strreplace(char *src, char *old_str, char *new_str)
{  
    char *find_postion = strstr(src, old_str);  
      
    int old_str_length = strlen(old_str);
	int new_str_length = strlen(new_str);
    int src_length = strlen(src);  
  
    if (find_postion)  
    {  
        int new_src_str_length = src_length + new_str_length - old_str_length; //�滻����ַ�������
  
        char *tmp_str = (char *)malloc((new_src_str_length + 1) * sizeof(char));
  
        strncpy(tmp_str, src, find_postion - src);
  
        // ��new_str�ŵ�tmp_str������  
        strncpy(tmp_str+(find_postion-src), new_str, new_str_length);
  
        // ��old_str������ַ����ŵ�tmp_str������  
        strcpy(tmp_str+(find_postion-src)+new_str_length, find_postion+old_str_length);
  
        tmp_str[new_src_str_length] = '\0';
		src = strdup(tmp_str);
		free(tmp_str);
		src = strreplace(src, old_str, new_str);
    }
	return src;
}

/* �ַ������Ӻ���
 * dest Ŀ���ַ���
 * src Դ�ַ���
 */
char *myStrCat(char *dest, const char *src)
{
	int dest_len = strlen(dest);
	int src_len = strlen(src);
	char *temp = (char*)malloc(sizeof(char)*(dest_len+src_len+1));
	strcpy(temp, dest);
	strcat(temp, src);
	dest = temp;
	return dest;
}

/* ������ת���ַ���
 * number ��ת��������
 * ����ת������ַ���
 */
char *myItoA(int number)
{
	int count = 1;
	int temp = number;
	char *str = NULL;
	while(temp /= 10)
		count++;
	if(number >= 0)
		str = (char*)malloc(sizeof(char)*(count+1));
	else
		str = (char*)malloc(sizeof(char)*(count+2));
	return itoa(number,str,10);
}

/* ��floatת���ַ�����float������λС����
 * number ��ת���ĸ�����
 * ����ת������ַ���
 */
char *myFtoA(float number)
{
	int num_int = (int)number;
	int count = 1;
	char *str = NULL;
	while(num_int /= 10)
		count++;
	if(number >= 0)
		str = (char*)malloc(sizeof(char)*(count+4));
	else
		str = (char*)malloc(sizeof(char)*(count+5));
	sprintf(str, "%.2f", number);
	return str;
}