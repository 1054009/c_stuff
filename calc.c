#include <stdio.h>

int main(int argc, char** argv)
{
	float firstNumber;
	float secondNumber;
	char operator;

	printf("Enter first number:\n");
	scanf("%f", &firstNumber);

	printf("Enter second number:\n");
	scanf("%f", &secondNumber);

	printf("Enter operator (+, -, /, *):\n");
	scanf(" %c", &operator);

	float answer = 0;

	switch (operator)
	{
		default:
		case '+':
			answer = firstNumber + secondNumber;
			break;

		case '-':
			answer = firstNumber - secondNumber;
			break;

		case '/':
			answer = firstNumber / secondNumber;
			break;

		case '*':
			answer = firstNumber * secondNumber;
			break;
	}

	printf("%f", answer);

	return 0;
}
