/*
Read two sets from keyboard. First two numbers provided
will be the number of elements within each set. Next,
you will provide the elements from each set.
Compute card(intersection(set1, set2)) and card(union(set1, set2)) 
for each set, and, after that, 
card(intersection(set1, set2)) / card(union(set1, set2))
card(x) is the number of elements within set x
*/

#include <stdio.h>

#define SUCCESS 				0
#define ERROR_READ 				1

int read_sets(int *set1, int *set2, int *size1, int *size2)
{
	int i, sz1, sz2;
	*size1 = 0;
	*size2 = 0;
	
	if (scanf("%d", &sz1) != 1) {
		printf("Error reading size 1!\n");
		return ERROR_READ;
	}

	if (scanf("%d", &sz2) != 1) {
		printf("Error reading size 2!\n");
		return ERROR_READ;
	}
	for (i = 1; i <= sz1; i++) {
		if (scanf("%d", &set1[i]) != 1) {
			printf("Error reading element %d from set 1\n", i);
			return ERROR_READ;
		}
	}
	for (i = 1; i <= sz2; i++) {
		if (scanf("%d", &set1[i]) != 1) {
			printf("Error reading element %d from set 2\n", i);
			return ERROR_READ;
		}	
	}
	*size1 = sz1;
	*size2 = sz2;
	return SUCCESS;
}

float compute(int *set1, int *set2, int size1, int size2)
{
	int i = 0, j = 0, common = 0;
	while (i < size1 && j < size2) {
		if (set1[i] == set2[j]) {
			i++;
			common++;
		} else if (set1[i] < set2[j]) {
			i++;
		} else {
			j++;
		}
	}
	return 1.0 * common / (size1 + size2);
}

int main(int argc, char **argv)
{
	int set1[150], set2[150], size1, size2, res;
	res = read_sets(set1, set2, &size1, &size2);
	
	if (res != SUCCESS) {
		return res;
	}

	printf("size1: %d\n", size1);
	for(int i=1;i<=size1;i++){
		printf("%d", set1[i]);
	}

	printf("size2: %d\n", size2);
	for(int i=1;i<=size2;i++){
		printf("%d", set2[i]);
	}

	
	printf("result is %.2f!\n", compute(set1, set2, size1, size2));
	return SUCCESS;
}