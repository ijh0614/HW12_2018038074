/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);//배열을 초기화해주는 함수
int freeArray(int *a);//동적할당 해제
void printArray(int *a);//현재 배열 출력

int selectionSort(int *a);//선택 정렬 구현
int insertionSort(int *a);//삽입 정렬 구현
int bubbleSort(int *a);//버블 정렬
int shellSort(int *a);//셸정렬
/* recursive function으로 구현 */
int quickSort(int *a, int n);//퀵 정렬


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);//인자로 main에 있는 해시 테이블 배열을 직접 전달함

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);//해시 테이블과 slot에 저장된 찾고 싶은 값을 입력한다.


int main()
{
	char command;//사용자가 선택하는 기능을 입력받는 변수
	int *array = NULL;//무작위로 값을 입력받을 배열
	int *hashtable = NULL;//해쉬 테이블의 배열
	int key = -1;
	int index = -1;

	srand(time(NULL));

	do{
		printf("-------[2018038074]----------------------[ImJongHoon]----------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);//array변수의 주소를 보냄.
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);//정렬 상태 출력
			hashing(array, &hashtable);//해시 함수 수행. 인자로 해시 테이블 직접 전달.
			printArray(hashtable);//해시 테이블 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);//slot에 저장된 찾고 싶은 값 입력
			printArray(hashtable);//해시 테이블을 출력하고
			index = search(hashtable, key);//찾고 싶은 값이 해시 테이블의 어디 index에 저장되었는지 반환
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)//인자로 이중 포인터 즉, array의 주소를 받음
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {//아직 배열이 할당되지 않았을 경우
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);//동적할당한 주소를 temp에 저장 후
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;//이미 배열이 할당되어있는 경우

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;//같은 값이 나올 수 있음

	return 0;
}

int freeArray(int *a)//배열 free해줌
{
	if(a != NULL)
		free(a);
	return 0;
}

void printArray(int *a)
{
	if (a == NULL) {//initialize 해주기 전이라면
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)//최소 너비 2를 주고 빈자리에 0을 채우도록 함.
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);//최소 너비 5
	printf("\n");
}


int selectionSort(int *a)/*최소 값을 찾아 앞으로 순서를 바꾸는 정렬방식*/
{
	int min;//최솟값 저장
	int minindex;//최솟값 찾은 곳 인덱스
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);//정렬 전

	for (i = 0; i < MAX_ARRAY_SIZE; i++)//처음부터 시작
	{
		minindex = i;//최솟값 저장할 배열의 인덱스
		min = a[i];//정렬 안 된 부분의 가장 첫번째값을 일단 최소값에 넣어놓고
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j])//그 이후 값들과 비교해서 더 작은 값이 나오면
			{
				min = a[j];
				minindex = j;//값 저장
			}
		}
		a[minindex] = a[i];
		a[i] = min;//값을 바꿔줌
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);//정렬 후
	return 0;
}

int insertionSort(int *a)/*정렬된 부분집합에 정렬되지 않은 부분집합에서 값을 뽑아 삽입하는 형식*/
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);//정렬 전 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)//a[0]은 정렬된 부분집합으로 생각하고 시작함. 그래서 i=1부터 시작
	{
		t = a[i];//정렬되지 않은 부분집합에서 값 하나 뽑아서 저장
		j = i;//정렬된 부분집합에서 탐색할 index 시작 j에 저장
		while (a[j-1] > t && j > 0)//j-1부터 정렬된 부분집합. 하나씩 비교하기. j가 끝까지 돌거나 t보다 작은 값이 나올 때까지.
		{
			a[j] = a[j-1];//오른쪽으로 하나 옮기고
			j--;//왼쪽으로 이동 탐색.
		}
		a[j] = t;//반복문 탈출시 해당위치에 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);//정렬 후 출력

	return 0;
}

int bubbleSort(int *a)/*최대 값을 맨 뒤로 보내주는 정렬*/
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);//정렬 전

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j])//앞의 배열의 값이 크면
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;//바꿔줌
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);//정렬 후

	return 0;
}

int shellSort(int *a)//삽입 정렬의 문제점을 보완하기 위한 정렬
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);//정렬 전

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)//h만큼의 간격. 한번 반복문을 돌 때마다 간격이 1/2로 줄어들고 간격이 0이 되면 반복문 종료
	{
		for (i = 0; i < h; i++)//하나씩 증가해서 간격만큼 증가 가능
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)//간격으로 구분한 영역 다음 영역으로 이동
			{
				v = a[j];//인덱스에 저장된 값 임시 저장
				k = j;//인덱스 저장
				while (k > h-1 && a[k-h] > v)//다음 인덱스로 이동할 수 없을 때까지 또는 바꿀 필요가 없을 때 탈출
				{
					a[k] = a[k-h];
					k -= h;//값을 바꾸거나
				}
				a[k] = v;//그대로 유지
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);//정렬 후

	return 0;
}

int quickSort(int *a, int n)//기준 값을 중심으로 왼쪽 부분집합과 오른쪽 부분집합으로 나누어 정렬
{//인자로 배열의 크기를 받음
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];//기준 값 pivot 피봇
		i = -1;//배열의 왼쪽을 가르킴
		j = n - 1;//배열의 오른쪽을 가르킴

		while(1)
		{
			while(a[++i] < v);//기준 값보다 큰 값이 나올 때까지 이동
			while(a[--j] > v);//기중 값보다 작은 값이 나올 때까지 이동

			if (i >= j) break;//서로 지나쳤을 경우 더 바꿔줄 필요가 없으므로 탈출
			t = a[i];
			a[i] = a[j];
			a[j] = t;//지나친 게 아닐경우 서로 값을 바꿔줌
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;//피봇을 정렬시킨 장소로 값 바꿔주기

		quickSort(a, i);//왼쪽
		quickSort(a+i+1, n-i-1);//오른쪽 나누어서 진행
	}


	return 0;
}

int hashCode(int key) {//제산 해시 함수. 나머지를 key값으로 저장.
   return key % MAX_HASH_TABLE_SIZE;
}

int hashing(int *a, int **ht)//해시 함수
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		//역참조로 main의 헤쉬 테이블에 주소 전달
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;//해쉬 테이블에 슬롯(slot) 초기화(buket에 slot하나)

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];//index가 i에 있는 값을 key로 저장
		hashcode = hashCode(key);//제산 해시 함수
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)//slot에 아무런 값도 저장되어있지 않으면
		{
			hashtable[hashcode] = key;//평범하게 저장
		} else 	{//slot에 값이 있으면, 즉 충돌이 발생하면

			index = hashcode;

			while(hashtable[index] != -1)//선형 조사법을 사용한다.
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;//빈 slot이 나올 때까지 반복하고
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;//빈 index를 찾으면 저장한다.
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);//제산 해시 함수에 넣어주었을 때 나온 값

	if(ht[index] == key)//오버 플로우가 발생하지 않아서 그 자리에 저장되어 있던 경우
		return index;//그냥 index 반환

	//오버 플로우가 발생한 경우
	while(ht[++index] != key)
	{
		//선형 조사법으로 key를 찾을 때까지 이동
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}