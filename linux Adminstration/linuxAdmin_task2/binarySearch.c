#include<stdio.h>
#define ARRAY_SIZE 5
void BinarySearch(int* arr, int Number);
int arr[ARRAY_SIZE];
int	search;

int main()
{
	printf("please enter your numbers \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		scanf("%d", &arr[i]);
	}
	printf("please enter the value you want to search \n");
	scanf("%d", &search);

	BinarySearch(arr, search);


		


}

void BinarySearch(int* arr, int Number)
{
	int start = 0;
	int end = ARRAY_SIZE-1;
	int mid = (start + end) / 2;
	int result;

	while (Number!=arr[mid])
	{
		if (Number < arr[mid])
		{
			end = mid - 1;
			mid = (start + end) / 2;


		}
		else if (Number > arr[mid])
		{
			start = mid + 1;
			mid = (start + end) / 2;

		}

		if (start == mid && mid == end)
		{
			printf("NOT FOUND");
			
		}
	}
	if (Number == arr[mid])
	{
		printf("YOUR SEARCH RESULT IS FOUND \n");
		result = mid+1;
		printf("%d  your element in index:", mid);

	}
		


}
