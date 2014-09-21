#include "recombine_chem_struct.h"
#include "base_functions.h"

/* ����LinePointsIndex�ڵ�
 * i ��i���߶�
 * j ��i���߶εĵ�j���˵�
 */
LinePointsIndex *createNewLPNode(int i, int j)
{
	LinePointsIndex *node = (LinePointsIndex*)malloc(sizeof(LinePointsIndex));
	node->i = i;
	node->j = j;
	node->next = NULL;
	return node;
}

/* ��LinePointsIndex�����ͷ����ӽڵ�
 * head ͷ�ڵ�
 * node ��Ҫ��ӵĽڵ�
 */
void addNodeInLinePointsIndex(LinePointsIndex *head, LinePointsIndex *node)
{
	node->next = head->next;
	head->next = node;
}

/* �ϲ�����LinePointsIndex����(��head2�ϲ���head1��)
 * head1 ��һ�������ͷ�ڵ�
 * head2 �ڶ��������ͷ�ڵ�
 */
void mergeTwoLinePointsIndex(LinePointsIndex *head1, LinePointsIndex *head2)
{
	LinePointsIndex *temp = head1;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = head2->next;
}

/* ����RecombinedPoints�ڵ�
 * eleIndex ��ѧԪ����ͨ������(��ֵ���ڵ���0˵������һ����ѧԪ�ط��Žڵ�)
 * i ��i���߶�
 * j ��i���߶εĵ�j���˵�
 */
RecombinedPoints *createNewRPNode(int eleIndex, int i, int j)
{
	RecombinedPoints *node = (RecombinedPoints*)malloc(sizeof(RecombinedPoints));
	node->i = eleIndex;
	LinePointsIndex *lpHead = (LinePointsIndex*)malloc(sizeof(LinePointsIndex));
	lpHead->next = createNewLPNode(i, j);
	node->point_set = lpHead;
	node->count = 1;
	node->next = NULL;
	if(eleIndex >= 0)
		node->count = 2;
	return node;
}

/* ��RecombinedPoints����ͷ����ӽڵ�
 * head ͷ�ڵ�
 * node ��Ҫ��ӵĽڵ�
 */
void addNodeInRecombinedPoints(RecombinedPoints *head, RecombinedPoints *node)
{
	node->next = head->next;
	head->next = node;
}

/* ��RecombinedPoints��һ���ڵ��LinePointsIndex���������LinePointsIndex�ڵ�
 * nodeRP RecombinedPoints�ڵ�
 * nodeLP LinePointsIndex�ڵ�
 */
void addLPNodeInRP(RecombinedPoints *nodeRP, LinePointsIndex *nodeLP)
{
	addNodeInLinePointsIndex(nodeRP->point_set, nodeLP);
	(nodeRP->count)++;
}

/* �ϲ�����RecombinedPoints�ڵ�(�ϲ���node1�ϣ�ɾ��node2)
 * node1 ��һ���ڵ�
 * node2 �ڶ����ڵ�
 * head RecombinedPointsͷ�ڵ�
 * ����node2����һ���ڵ�
 */
RecombinedPoints *mergeTwoRecombinedPointsNode(RecombinedPoints *node1, RecombinedPoints *node2, RecombinedPoints *head)
{
	mergeTwoLinePointsIndex(node1->point_set, node2->point_set);
	node1->count += node2->count;
	RecombinedPoints *node2Pre = head;
	while(node2Pre->next != node2)
		node2Pre = node2Pre->next;
	node2Pre->next = node2->next;
	node2->point_set = NULL;
	node2->next = NULL;
	return node2Pre;
}

/* �ж�ĳ����ѧ�����Ƿ��ܹ�����һ���˵���
 * bondSet��ѧ������
 * i ��i���߶�
 * j ��i���߶εĵ�j���˵�
 * charSpArr ��ѧ������ͨ��
 * k ��k����ѧ����
 */
bool judgePtoC(SegmentsArray *bondSet, int i, int j, ImageSplitArray *charSpArr, int k)
{
	int o = (j+1)%2; //ȡ�õ�i���߶ε���һ���˵�
	double newMinI = charSpArr->imgCons[k].minI-AVGBONDLEN/2,
		   newMaxI = charSpArr->imgCons[k].maxI+AVGBONDLEN/2,
		   newMinJ = charSpArr->imgCons[k].minJ-AVGBONDLEN/2,
		   newMaxJ = charSpArr->imgCons[k].maxJ+AVGBONDLEN/2;
	CvPoint2D32f p1, p2, p3, p4, intersection;
	double iLineCo[3], diag1Co[3], diag2Co[3];
	p1.x = charSpArr->imgCons[k].minJ-1;
	p1.y = charSpArr->imgCons[k].minI-1;
	p2.x = charSpArr->imgCons[k].minJ-1;
	p2.y = charSpArr->imgCons[k].maxI+1;
	p3.x = charSpArr->imgCons[k].maxJ+1;
	p3.y = charSpArr->imgCons[k].minI-1;
	p4.x = charSpArr->imgCons[k].maxJ+1;
	p4.y = charSpArr->imgCons[k].maxI+1;
	if(bondSet->pointArray[i][j].x >= newMinJ && bondSet->pointArray[i][j].x <= newMaxJ && bondSet->pointArray[i][j].y >= newMinI && bondSet->pointArray[i][j].y <= newMaxI) //��i���߶εĵ�j���˵��ڻ�ѧԪ�ط�����ͨ����չ��ķ����˵���˵��뻯ѧԪ�ط��ž���Ͻ�
	{
		//��i���߶�����ֱ���뻯ѧ������ͨ�������Խ��ߵĽ���ֻҪ��һ������ͨ���ڣ��Ҹý��㵽(i,j)�ľ���С�ڸõ㵽(i,o)�ľ���
		generalLinearEqu(&(bondSet->pointArray[i][j]), &(bondSet->pointArray[i][o]), iLineCo);
		generalLinearEqu(&(p1), &(p4), diag1Co);
		//��������ֱ�ߵĽ���(A0*x+B0*y+C0=0��A1*x+B1*y+C1=0�Ľ���x=(B1*C0-B0*C1)/(A1*B0-A0*B1), y=(A0*C1-A1*C0)/(A1*B0-A0*B1))
		double a1b0a0b1 = diag1Co[0]*iLineCo[1]-iLineCo[0]*diag1Co[1],
			   b1c0b0c1 = diag1Co[1]*iLineCo[2]-iLineCo[1]*diag1Co[2],
			   a0c1a1c0 = iLineCo[0]*diag1Co[2]-diag1Co[0]*iLineCo[2];
		if(a1b0a0b1 != 0)
		{
			intersection.x = b1c0b0c1/a1b0a0b1;
			intersection.y = a0c1a1c0/a1b0a0b1;
			if((intersection.x>=p1.x && intersection.x<=p4.x) || (intersection.y>=p1.y && intersection.y<=p4.y))
			{
				if(cal2PointDistance(&(bondSet->pointArray[i][j]), &intersection) < cal2PointDistance(&(bondSet->pointArray[i][o]), &intersection))
					return true;
			}
		}
		generalLinearEqu(&(p2), &(p3), diag2Co);
		a1b0a0b1 = diag2Co[0]*iLineCo[1]-iLineCo[0]*diag2Co[1];
		b1c0b0c1 = diag2Co[1]*iLineCo[2]-iLineCo[1]*diag2Co[2];
		a0c1a1c0 = iLineCo[0]*diag2Co[2]-diag2Co[0]*iLineCo[2];
		if(a1b0a0b1 != 0)
		{
			intersection.x = b1c0b0c1/a1b0a0b1;
			intersection.y = a0c1a1c0/a1b0a0b1;
			if((intersection.x>=p2.x && intersection.x<=p3.x) || (intersection.y>=p3.y && intersection.y<=p2.y))
			{
				if(cal2PointDistance(&(bondSet->pointArray[i][j]), &intersection) < cal2PointDistance(&(bondSet->pointArray[i][o]), &intersection))
					return true;
			}
		}
	}
	return false;
}

/* ����������߶ζ˵�ŵ�һ��
 * head RecombinedPointsͷ�ڵ�
 * bondSet ��ѧ������
 */
void mergeNearPoints(RecombinedPoints *head, SegmentsArray *bondSet)
{
	LinePointsIndex *lpNode = NULL;
	int firstBond = 0;
	int i, j, bondSeti, bondSetj;
	for(i=0; i<bondSet->count; i++)
	{
		if(bondSet->flag[i] == 1)
			firstBond++;
		else
			break;
	}
	head->next = createNewRPNode(-1, firstBond, 0); //-1�����ǻ�ѧԪ����ͨ��ڵ�
	RecombinedPoints *rpNode = createNewRPNode(-1, firstBond, 1);
	addNodeInRecombinedPoints(head, rpNode);
	RecombinedPoints *temp = NULL;
	double tempD;
	for(i=firstBond+1; i<bondSet->count; i++)
	{
		if(bondSet->flag[i] != 1)
		{
			for(j=0; j<2; j++)
			{
				temp = head->next;
				while(temp != NULL)
				{
					bondSeti = temp->point_set->next->i;
					bondSetj = temp->point_set->next->j;
					tempD = cal2PointDistance(&(bondSet->pointArray[bondSeti][bondSetj]), &(bondSet->pointArray[i][j]));
					if(tempD < 1.0) //������ĺϲ�
					{
						lpNode = createNewLPNode(i, j);
						addLPNodeInRP(temp, lpNode);
						break;
					}
					temp = temp->next;
				}
				if(temp == NULL) //δ�ҵ��������������һ��RecombinedPoints�ڵ�
				{
					rpNode = createNewRPNode(-1, i, j);
					addNodeInRecombinedPoints(head, rpNode);
				}
			}
		}
	}
}


/* ���黯ѧ���뻯ѧ����
 * bondSet ��ѧ������
 * charSpArr ��ѧ������ͨ��
 * ����ͷ���
 */
RecombinedPoints *recombineChemStruct(SegmentsArray *bondSet, ImageSplitArray *charSpArr)
{
	int i = 0, j = 0, count = 0, bond1i, bond1j, bond2i, bond2j;
	bool flag;
	RecombinedPoints *head = (RecombinedPoints*)malloc(sizeof(RecombinedPoints)), *temp = NULL, *tempTemp = NULL, *tempTempTemp = NULL;
	LinePointsIndex *lpNode = NULL;
	double d = 0.0, minD = DBL_MAX;
	CvPoint2D32f p;

	//����������߶ζ˵�ŵ�һ��
	mergeNearPoints(head, bondSet);

	//��ÿ����ѧ������ͨ����ӵ�RecombinedPoints������,����ӽ�ȥ��characterTag��Ϊ0
	if(charSpArr->count > 0)
	{
		for(i=0; i<charSpArr->count; i++)
		{
			temp = head->next;
			count = 0;
			while(temp != NULL)
			{
				if(temp->count == 1)
				{
					flag = judgePtoC(bondSet, temp->point_set->next->i, temp->point_set->next->j, charSpArr, i);
					if(flag)
					{
						if(count == 0)
						{
							temp->i = i; //����ѧԪ�ط��ŷ����һ����������ĵ����˵�
							(temp->count)++;
							count++;
							tempTemp = temp;
							charSpArr->imgCons[i].characterTag = 0;
						}
						else
						{
							temp = mergeTwoRecombinedPointsNode(tempTemp, temp, head);
						}
					}
				}
				temp = temp->next;
			}
		}
	}

	//�����˵�ϲ������������С��ĳ����ֵ�ĵ���
	temp = head->next;
	while(temp != NULL)
	{
		if(temp->count == 1)
		{
			minD = DBL_MAX;
			tempTemp = head->next;
			while(tempTemp != NULL)
			{
				if(tempTemp->i == -1 && tempTemp != temp) //������ѧԪ�ط��ŵĵ�
				{
					bond1i = temp->point_set->next->i;
					bond1j = temp->point_set->next->j;
					bond2i = tempTemp->point_set->next->i;
					bond2j = tempTemp->point_set->next->j;
					d = cal2PointDistance(&(bondSet->pointArray[bond1i][bond1j]), &(bondSet->pointArray[bond2i][bond2j]));
					if(minD > d)
					{
						minD = d;
						tempTempTemp = tempTemp;
					}
				}
				tempTemp = tempTemp->next;
			}
			if(minD <= DISTANCE_THRESHOLD*2)
				temp = mergeTwoRecombinedPointsNode(tempTempTemp, temp, head);
		}
		temp = temp->next;
	}

	//δ�����Ļ�ѧ��������������ĺϲ���֮�佨����ϵ
	if(charSpArr->count > 0)
	{
		for(i=0; i<charSpArr->count; i++)
		{
			if(charSpArr->imgCons[i].characterTag == 1) //δ�����Ļ�ѧԪ�ط���
			{
				minD = DBL_MAX;
				temp = head->next;
				while(temp != NULL)
				{
					if(temp->i == -1) //������ѧԪ�ط��ŵĵ�
					{
						p.x = (double)(charSpArr->imgCons[i].minJ + charSpArr->imgCons[i].maxJ) / 2;
						p.y = (double)(charSpArr->imgCons[i].minI + charSpArr->imgCons[i].maxI) / 2;
						bond1i = temp->point_set->next->i;
						bond1j = temp->point_set->next->j;
						d = cal2PointDistance(&(bondSet->pointArray[bond1i][bond1j]), &(p));
						if(minD > d)
						{
							minD = d;
							tempTemp = temp;
						}
					}
					temp = temp->next;
				}
				bond1i = tempTemp->point_set->next->i;
				bond1j = tempTemp->point_set->next->j;
				addBondTobondSet(bondSet, p, bondSet->pointArray[bond1i][bond1j], 4); //��ӵĻ�ѧ���ĵ�0���˵���p
				
				//������ӵĻ�ѧ���ĵ�1���˵����tempTemp�ڵ��е�LinePointsIndex������
				lpNode = createNewLPNode(bondSet->count-1, 1);
				addLPNodeInRP(tempTemp, lpNode);

				tempTempTemp = createNewRPNode(i, bondSet->count-1, 0); //����һ���ڵ���p���ҹ�����i����ѧԪ�ط���
				addNodeInRecombinedPoints(head, tempTempTemp);
			}
		}
	}
	return head;
}

/* ���ɻ�ѧ�ṹʽ���ڽӾ���
 * bondSet ��ѧ������
 * charSpArr ��ѧ������ͨ��
 * adjacencyMatrix �ڽӾ���
 * ���ؽڵ�����
 */
int setBondSetRPI(SegmentsArray *bondSet, ImageSplitArray *charSpArr, ChemStruNodes **nodes, int ***adjacencyMatrix)
{
	RecombinedPoints *head = recombineChemStruct(bondSet, charSpArr);
	RecombinedPoints *temp = head->next;
	LinePointsIndex *lpNode = NULL;
	int count = 0, bond1i, bond1j;
	while(temp != NULL)
	{
		lpNode = temp->point_set->next;
		while(lpNode != NULL)
		{
			bondSet->recombinePIndex[lpNode->i][lpNode->j] = count;
			lpNode = lpNode->next;
		}
		count++;
		temp = temp->next;
	}
	//�����ڽӾ�����ÿ�����������ԭ�Ӻ�������������
	*nodes = (ChemStruNodes *)malloc(sizeof(ChemStruNodes)*count);
	temp = head->next;
	count = 0;
	while(temp != NULL)
	{
		if(temp->i >= 0) //���л�ѧԪ�ط���
		{
			(*nodes)[count].node.x = (float)(charSpArr->imgCons[temp->i].minJ + charSpArr->imgCons[temp->i].maxJ) / 2;
			(*nodes)[count].node.y = (float)(charSpArr->imgCons[temp->i].minI + charSpArr->imgCons[temp->i].maxI) / 2;
			int len = strlen(charSpArr->imgCons[temp->i].characters);
			(*nodes)[count].chars = (char*)malloc(sizeof(char)*(len+1));
			strcpy((*nodes)[count].chars, charSpArr->imgCons[temp->i].characters);
		}
		else //����̼ԭ��
		{
			bond1i = temp->point_set->next->i;
			bond1j = temp->point_set->next->j;
			(*nodes)[count].node = bondSet->pointArray[bond1i][bond1j];
			(*nodes)[count].chars = "C";
		}
		count++;
		temp = temp->next;
	}
	//�����ڽӾ���
	*adjacencyMatrix = (int**)malloc(sizeof(int*)*count);
	for(int i=0; i<count; i++)
	{
		(*adjacencyMatrix)[i] = (int*)malloc(sizeof(int)*(i+1));
		for(int j=0; j<(i+1); j++)
			(*adjacencyMatrix)[i][j] = 0;
	}
	int bigI, smallJ; //bondSet->recombinePIndex[i][0]��bondSet->recombinePIndex[i][1]�е�������Сֵ
	for(int i=0; i<bondSet->count; i++)
	{
		bigI = bondSet->recombinePIndex[i][0];
		smallJ = bondSet->recombinePIndex[i][0];
		if(bondSet->recombinePIndex[i][0] > bondSet->recombinePIndex[i][1])
			smallJ = bondSet->recombinePIndex[i][1];
		else
			bigI = bondSet->recombinePIndex[i][1];
		switch(bondSet->flag[i])
		{
			case 4: //����
				(*adjacencyMatrix)[bigI][smallJ] += 1;
				break;
			case 5: //˫��
				(*adjacencyMatrix)[bigI][smallJ] += 2;
				break;
			case 6: //����
				(*adjacencyMatrix)[bigI][smallJ] = 3;
				break;
			case 7: //ʵШ�μ�
				(*adjacencyMatrix)[bigI][smallJ] = -1;
				break;
			case 8: //��Ш�μ�
				(*adjacencyMatrix)[bigI][smallJ] = -2;
				break;
			default:
				break;
		}
	}
	return count;
}

/* �����ڽӾ���ͽڵ���������JME��ʽ���ַ���
 * nodes ��ѧ�ṹʽ�нڵ�����
 * adjacencyMatrix �ڽӾ���
 * nodes_count ��ѧ�ṹʽ�ڵ�����
 * ����JME��ʽ���ַ���
 */
char *getJMEStr(ChemStruNodes *nodes, int **adjacencyMatrix, int nodes_count)
{
	int bonds_count = 0;
	int len = strlen(nodes[0].chars);
	char *str = "";
	char *temp = NULL;
	bool flag = true;
	for(int i=0; i<nodes_count; i++)
	{
		str = myStrCat(str, " ");
		str = myStrCat(str, nodes[i].chars);
		for(int j=0; j<2; j++)
		{
			str = myStrCat(str, " ");
			if(flag)
			{
				temp = myFtoA(nodes[i].node.x);
				flag = false;
			}
			else
			{
				temp = myFtoA(-(nodes[i].node.y));
				flag = true;
			}
			str = myStrCat(str, temp);
			free(temp);
		}
	}

	for(int i=0; i<nodes_count; i++)
	{
		for(int j=0; j<(i+1); j++)
		{
			if(adjacencyMatrix[i][j] != 0)
			{
				bonds_count++;
				str = myStrCat(str, " ");
				temp = myItoA(i+1);
				str = myStrCat(str, temp);
				free(temp);

				str = myStrCat(str, " ");
				temp = myItoA(j+1);
				str = myStrCat(str, temp);
				free(temp);

				str = myStrCat(str, " ");
				temp = myItoA(adjacencyMatrix[i][j]);
				str = myStrCat(str, temp);
				free(temp);
			}
		}
	}
	char *jme_str = "";
	temp = myItoA(nodes_count);
	jme_str = myStrCat(jme_str, temp);
	free(temp);

	jme_str = myStrCat(jme_str, " ");
	temp = myItoA(bonds_count);
	jme_str = myStrCat(jme_str, temp);
	free(temp);

	return myStrCat(jme_str, str);
}

/* �ϲ�����Ļ�ѧԪ�ط��ţ�Br��Si��Cl��������ɵ�ԭ�ӣ�
 * bondSet ��ѧ������
 * charSpArr ��ѧ������ͨ��
 */
void mergeSpOneChar(SegmentsArray *bondSet, ImageSplitArray *charSpArr)
{
	float k = FLT_MAX; //��ѧ����б��
	float minX, maxX, minY, maxY, avgX, avgY, dValueMinY, dValueMaxY, hDValue; //minX, maxX, minY, maxY:��ѧ���������СX,Y��avgX, avgY����ѧ�������ĵ��X,Y��dValueMinY����ѧ���Ŷ����뻯ѧ�������Ĵ�ֱ����, dValueMaxY����ѧ���ŵײ��뻯ѧ���ײ��Ĵ�ֱ����, hDValue����ѧ�����뻯ѧ����ˮƽ����
	for(int i=0; i<bondSet->count; i++)
	{
		if(i==18)
			int ddd=0;
		if(bondSet->pointArray[i][0].x != bondSet->pointArray[i][1].x)
			k = fabs((bondSet->pointArray[i][0].y - bondSet->pointArray[i][1].y) / (bondSet->pointArray[i][0].x - bondSet->pointArray[i][1].x));
		if(k >= KMAX_THRESHOLD) //����ÿ�����ߣ��ж��������û��B��C��S
		{
			minY = bondSet->pointArray[i][0].y;
			maxY = bondSet->pointArray[i][0].y;
			minX = bondSet->pointArray[i][0].x;
			maxX = bondSet->pointArray[i][0].x;
			if(bondSet->pointArray[i][0].y < bondSet->pointArray[i][1].y)
				maxY = bondSet->pointArray[i][1].y;
			else
				minY = bondSet->pointArray[i][1].y;
			if(bondSet->pointArray[i][0].x < bondSet->pointArray[i][1].x)
				maxX = bondSet->pointArray[i][1].x;
			else
				minX = bondSet->pointArray[i][1].x;
			for(int j=0; j<charSpArr->count; j++)
			{
				int len = strlen(charSpArr->imgCons[j].characters);
				if(charSpArr->imgCons[j].characters[len-1] == 'C')
				{
					dValueMinY = fabs(minY - charSpArr->imgCons[j].minI);
					dValueMaxY = fabs(maxY - charSpArr->imgCons[j].maxI);
					hDValue = fabs(minX - charSpArr->imgCons[j].maxJ);
					if(dValueMinY <= DVALUE_THRESHOLD && dValueMaxY <= DVALUE_THRESHOLD && hDValue <= (charSpArr->imgCons[j].maxJ-charSpArr->imgCons[j].minJ)/2)
					{
						char *temp = (char*)malloc((len+2)*sizeof(char));
						strcpy(temp, charSpArr->imgCons[j].characters);
						temp[len] = 'l';
						temp[len+1] = '\0';
						free(charSpArr->imgCons[j].characters);
						charSpArr->imgCons[j].characters = temp;
						charSpArr->imgCons[j].maxJ = (int)maxX;
						bondSet->flag[i] = 1; //ȥ���û�ѧ��
						break;
					}
				}
				else if(charSpArr->imgCons[j].characters[len-1] == 'B' || charSpArr->imgCons[j].characters[len-1] == 'S')
				{
					dValueMinY = fabs(minY - (charSpArr->imgCons[j].minI + charSpArr->imgCons[j].maxI) / 2);
					dValueMaxY = fabs(maxY - charSpArr->imgCons[j].maxI);
					hDValue = fabs(minX - charSpArr->imgCons[j].minJ);
					if(dValueMinY <= DVALUE_THRESHOLD && dValueMaxY <= DVALUE_THRESHOLD && hDValue <= (charSpArr->imgCons[j].maxJ-charSpArr->imgCons[j].minJ)/2)
					{
						char *temp = (char*)malloc((len+2)*sizeof(char));
						strcpy(temp, charSpArr->imgCons[j].characters);
						if(charSpArr->imgCons[j].characters[len-1] == 'B')
							temp[len] = 'r';
						else
							temp[len] = 'i';
						temp[len+1] = '\0';
						free(charSpArr->imgCons[j].characters);
						charSpArr->imgCons[j].characters = temp;
						charSpArr->imgCons[j].maxJ = (int)maxX;
						bondSet->flag[i] = 1; //ȥ���û�ѧ��
						break;
					}
				}
			}
		}
		else if(k <= KMIN_THRESHOLD) //����ÿ���̺��ߣ����丽��ͬһˮƽ���ϵĻ�ѧ������
		{
			double bondLen = cal2PointDistance(&(bondSet->pointArray[i][0]), &(bondSet->pointArray[i][1]));
			if(bondLen < AVGBONDLEN * 2 / 3)
			{
				avgX = (bondSet->pointArray[i][0].x + bondSet->pointArray[i][1].x) / 2;
				avgY = (bondSet->pointArray[i][0].y + bondSet->pointArray[i][1].y) / 2;
				for(int j=0; j<charSpArr->count; j++)
				{
					int len = strlen(charSpArr->imgCons[j].characters);
					dValueMinY = fabs(avgY - charSpArr->imgCons[j].minI);
					if(avgX > charSpArr->imgCons[j].maxJ) //��ѧ���ڻ�ѧ���ŵ��ұ�
					{
						hDValue = avgX - charSpArr->imgCons[j].maxJ;
						if(bondLen < (charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ) && dValueMinY < (charSpArr->imgCons[j].maxI - charSpArr->imgCons[j].minI)/2 && hDValue < ((charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ)/2+bondLen/2)) //��ѧ������С�ڻ�ѧ���ſ�ȣ���ѧ���뻯ѧ���Ŷ����Ĵ�ֱ����С�ڻ�ѧ���Ÿ߶ȵ�һ�룬��ѧ�������뻯ѧ���ŵı߾��ˮƽ����С�ڣ���ѧ���ſ�ȵ�һ��+��ѧ�����ȵ�һ�룩
						{
							char *temp = (char*)malloc((len+2)*sizeof(char));
							strcpy(temp, charSpArr->imgCons[j].characters);
							temp[len] = '-';
							temp[len+1] = '\0';
							free(charSpArr->imgCons[j].characters);
							charSpArr->imgCons[j].characters = temp;
							bondSet->flag[i] = 1; //ȥ���û�ѧ��
							break;
						}
					}
					else if(avgX < charSpArr->imgCons[j].minJ) //��ѧ���ڻ�ѧ���ŵ����
					{
						hDValue = charSpArr->imgCons[j].minJ - avgX;
						if(bondLen < (charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ) && dValueMinY < (charSpArr->imgCons[j].maxI - charSpArr->imgCons[j].minI)/2 && hDValue < ((charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ)/2+bondLen/2)) //��ѧ������С�ڻ�ѧ���ſ�ȣ���ѧ���뻯ѧ���Ŷ����Ĵ�ֱ����С�ڻ�ѧ���Ÿ߶ȵ�һ�룬��ѧ�������뻯ѧ���ŵı߾��ˮƽ����С�ڣ���ѧ���ſ�ȵ�һ��+��ѧ�����ȵ�һ�룩
						{
							char *temp = (char*)malloc((len+2)*sizeof(char));
							temp = "-";
							strcat(temp, charSpArr->imgCons[j].characters);
							free(charSpArr->imgCons[j].characters);
							charSpArr->imgCons[j].characters = temp;
							bondSet->flag[i] = 1; //ȥ���û�ѧ��
							break;
						}
					}
				}
			}
		}
	}
}

/* �ϲ����Ż����ŵĻ�ѧ����
 * charSpArr ��ѧ������ͨ��
 */
void mergeHorVChemChars(ImageSplitArray *charSpArr)
{
	int i = 0, j = 0, flag = 0;
	int dValueMaxY, dValueMinX, dValueMaxX, hDValue1, hDValue2; //dValueMaxY��������ѧ���ŵײ��Ĵ�ֱ����,dValueMinX��������ѧ������ߵ�ˮƽ����,dValueMaxX��������ѧ�����ұߵ�ˮƽ����, hDValue��������ѧ���ŵ�ˮƽ/��ֱ����
	float maxHalfChar, iHalfChar, jHalfChar; //maxHalfChar�������ַ���һ���ȵ����ֵ
	int leni, lenj;
	//�ϲ����Ż�ѧԪ�ط���
	for(i=0; i<charSpArr->count; i++)
	{
		flag = i;
		if(charSpArr->imgCons[i].characterTag == 1) //��ѧ������ͨ��
		{
			leni = strlen(charSpArr->imgCons[i].characters);
			iHalfChar = (float)(charSpArr->imgCons[i].maxJ - charSpArr->imgCons[i].minJ)/2;
			for(j=i+1; j<charSpArr->count; j++)
			{
				if(charSpArr->imgCons[j].characterTag == 1) //��ѧ������ͨ��
				{
					dValueMaxY = abs(charSpArr->imgCons[i].maxI - charSpArr->imgCons[j].maxI);
					if(dValueMaxY < 3) //��ѧԪ�ط��ŵײ�������һ��ֱ����
					{
						jHalfChar = (float)(charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ)/2;
						if(iHalfChar < jHalfChar)
							maxHalfChar = jHalfChar;
						else
							maxHalfChar = iHalfChar;
						hDValue1 = charSpArr->imgCons[i].maxJ - charSpArr->imgCons[j].minJ;
						hDValue2 = charSpArr->imgCons[i].minJ - charSpArr->imgCons[j].maxJ;
						if(hDValue1 < 0 && hDValue1 > -1*maxHalfChar) //i��j�Ľ����
						{
							lenj = strlen(charSpArr->imgCons[j].characters);
							char *temp = (char*)malloc((leni+lenj+1)*sizeof(char));
							strcpy(temp, charSpArr->imgCons[i].characters);
							strcat(temp, charSpArr->imgCons[j].characters);
							free(charSpArr->imgCons[i].characters);
							charSpArr->imgCons[i].characters = temp;
							charSpArr->imgCons[i].maxJ = charSpArr->imgCons[j].maxJ;
							charSpArr->imgCons[j].characterTag = 0; //ȥ��j��ѧԪ�ط���
							i = flag;
							break;
						}
						else if(hDValue2 > 0 && hDValue2 < maxHalfChar) //i��j�Ľ��ұ�
						{
							lenj = strlen(charSpArr->imgCons[j].characters);
							char *temp = (char*)malloc((leni+lenj+1)*sizeof(char));
							strcpy(temp, charSpArr->imgCons[j].characters);
							strcat(temp, charSpArr->imgCons[i].characters);
							free(charSpArr->imgCons[i].characters);
							charSpArr->imgCons[i].characters = temp;
							charSpArr->imgCons[i].minJ = charSpArr->imgCons[j].minJ;
							charSpArr->imgCons[j].characterTag = 0; //ȥ��j��ѧԪ�ط���
							i = flag;
							break;
						}
					}
				}
			}
		}
	}

	//�ϲ����Ż�ѧԪ�ط���
	for(i=0; i<charSpArr->count; i++)
	{
		flag = i;
		if(charSpArr->imgCons[i].characterTag == 1) //��ѧ������ͨ��
		{
			leni = strlen(charSpArr->imgCons[i].characters);
			iHalfChar = (float)(charSpArr->imgCons[i].maxJ - charSpArr->imgCons[i].minJ)/2;
			for(j=i+1; j<charSpArr->count; j++)
			{
				if(charSpArr->imgCons[j].characterTag == 1) //��ѧ������ͨ��
				{
					dValueMinX = abs(charSpArr->imgCons[i].maxJ - charSpArr->imgCons[j].maxJ);
					dValueMaxX = abs(charSpArr->imgCons[i].minJ - charSpArr->imgCons[j].minJ);
					if(dValueMinX < 3 && dValueMaxX < 3) //��ѧԪ�ط����������߻�������
					{
						jHalfChar = (float)(charSpArr->imgCons[j].maxJ - charSpArr->imgCons[j].minJ)/2;
						if(iHalfChar < jHalfChar)
							maxHalfChar = jHalfChar;
						else
							maxHalfChar = iHalfChar;
						hDValue1 = charSpArr->imgCons[i].maxI - charSpArr->imgCons[j].minI;
						hDValue2 = charSpArr->imgCons[i].minI - charSpArr->imgCons[j].maxI;
						if(hDValue1 < 0 && hDValue1 > -1*maxHalfChar) //i��j�Ľ��ϱ�
						{
							lenj = strlen(charSpArr->imgCons[j].characters);
							char *temp = (char*)malloc((leni+lenj+1)*sizeof(char));
							strcpy(temp, charSpArr->imgCons[i].characters);
							strcat(temp, charSpArr->imgCons[j].characters);
							free(charSpArr->imgCons[i].characters);
							charSpArr->imgCons[i].characters = temp;
							charSpArr->imgCons[i].maxI = charSpArr->imgCons[j].maxI;
							charSpArr->imgCons[j].characterTag = 0; //ȥ��j��ѧԪ�ط���
							i = flag;
							break;
						}
						else if(hDValue2 > 0 && hDValue2 < maxHalfChar) //i��j�Ľ��±�
						{
							lenj = strlen(charSpArr->imgCons[j].characters);
							char *temp = (char*)malloc((leni+lenj+1)*sizeof(char));
							strcpy(temp, charSpArr->imgCons[j].characters);
							strcat(temp, charSpArr->imgCons[i].characters);
							free(charSpArr->imgCons[i].characters);
							charSpArr->imgCons[i].characters = temp;
							charSpArr->imgCons[i].minI = charSpArr->imgCons[j].minI;
							charSpArr->imgCons[j].characterTag = 0; //ȥ��j��ѧԪ�ط���
							i = flag;
							break;
						}
					}
				}
			}
		}
	}
}