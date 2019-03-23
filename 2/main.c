#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int arr[100];
	int len;
} vector;

/*
compute the greatest common divisor of two numbers
input: two integers
output: their gcd
*/
int greatest_common_divisor(int x, int y) {
	int divided = x > y ? x : y;
	int divisor = x < y ? x : y;
	while (divisor != 0) {
		int remainder = divided % divisor;
		divided = divisor;
		divisor = remainder;
	}
	return divided;
}

/*
read a list of integer numbers of a specified length
input:
output: resulted vector
*/
vector read_vector() {
	vector result;
	result.len = 0;
	int n;
	printf("give the length of the vector: ");
	scanf("%d", &n);
	
	while (result.len < n) {
		printf("give a number:");
		scanf("%d", result.arr + result.len++);
	}
	return result;
}

/*
calculate the start and end of the sequence with the maximum sum of a vector
input: a vector and two indices for the beginning and ending of the required vector
output: modifies the two indices
*/
void find_largest_sum_seq(vector v, int *start, int *end) {
	int sum = 0;
	int max = 0x80000000;
	int new_sum = 1;
	for (int i = 0; i < v.len; i++) {
		sum += v.arr[i];
		if (sum > max) {
			max = sum;
			*end = i;
			if (new_sum) {
				*start = i;
				new_sum = 0;
			}
		}
		if (sum < 0) {
			sum = 0;
			new_sum = 1;
		}
	}
}

/*
print the menu options
input:
output: the menu options
*/
void print_menu() {
	printf("0: exit\n");
	printf("1: relative primes to n, smaller than n\n");
	printf("2: find the longest subsequence in a vector with the maximum sum\n");
	printf("3: find the largest exponent of p in the decomposition of n\n");
}

/*
finds the exponent a number p can be raised to until it doesn't divide a number n
input: the number which will must be divied and the number for which to check the exponent
output: the required exponent
*/
int exponent_from_decomposition(int n, int p) {
	int exp = 0, div = p;
	while (n % div == 0) {
		div *= p;
		exp++;
	}
	return exp;
}

/*
checks if a number is prime or not
input: the number that will be checked
output: 1 if it is not prime, 0 if it is prime
*/
int is_not_prime(int p) {
	int divisors = 0;
	for (int i = 1; i < p; i++)
		if (p % i == 0) divisors++;
	return divisors == 2;
}

int main() {
	int cmd = -1;
	while (cmd != 0){
		print_menu();
		
		do {
			printf("enter a valid command (0-3): ");
			scanf("%d", &cmd);
		} while (cmd < 0 || cmd > 3);

		switch (cmd) {
			case 1: {
				int n;
				do {
					printf("enter a non-zero natural number: ");
					scanf("%d", &n);
				} while (n < 0);
				printf("numbers smaller than %d and relatively prime to it: ", n);
				for (int i = 0; i < n; i++)
					if (greatest_common_divisor(i, n) == 1)
						printf("%d ", i);
					break;
				}
				case 2: {
					vector numbers = read_vector();
					int start = 0, end = 0;
					find_largest_sum_seq(numbers, &start, &end);
					printf("sequence with the maximum sum is: ");
					for (int i = start; i <= end; i++)
						printf("%d ", numbers.arr[i]);
					break;
				}
				case 3: {
					int n, p;
					do {
						printf("enter a non-zero natural number: ");
						scanf("%d", &n);
					} while (n < 0);
					do {
						printf("enter a prime number: ");
						scanf("%d", &p);
					} while (is_not_prime(p));
					int exp = exponent_from_decomposition(n, p);
					printf("the exponent of %d in the decomposition of %d is %d \n", p, n, exp);
				}
			}
			printf("\n\n");
		}
		return 0;
	}