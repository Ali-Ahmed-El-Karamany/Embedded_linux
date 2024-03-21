#include<stdio.h>
#include"lib/myMath.h"




int main()
{
	int x = 100;
	int y = 10;
	
	printf("Addition Result: %d \n", add(x, y));
	printf("Subtraction Result: %d \n", sub(x, y));
	printf("Multiplication Result: %d \n", multiply(x, y));
	printf("Division Result: %f \n", divide(x, y));
	printf("Modulus Result: %d \n", modulus(x, y));
}

