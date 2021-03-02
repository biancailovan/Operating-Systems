#include <stdio.h>
#include <stdlib.h>


#define SUCCESS 				0
#define ERROR_READ 				1
/*Functia insereaza vectorul v2 de lungime n2 in vectorul v1 de lungime n2 si capacitate c1
pe pozitia pos. Functia va returna 0 in caz de succes si -1 in cazul in care
capacitatea nu este suficient de mare

Pt exemplul din laborator:
Size of the first array: 
6
Array1:
1 2 3 4 5 6
Size of the second array: 
2
Array2:
10 20
1 2 1 10 20 4 5 6 
*/

int readArray(int *n1, int *v1, int *n2, int *v2)
{
	int i, size1, size2;

	printf("Size of the first array: \n");
	if (scanf("%d",&size1) != 1){ 
		printf("Error reading size1!\n");
		return ERROR_READ;
	}

	printf("Array1:\n");
	for(i=1;i<=size1;i++){
		if(scanf("%d",&v1[i]) != 1){ 
			printf("Error reading element %d!\n",i);
			return ERROR_READ;
		}
	}


	printf("Size of the second array: \n");
	if (scanf("%d",&size2) != 1){ 
		printf("Error reading size1!\n");
		return ERROR_READ;
	}

	printf("Array2:\n");
	for(i=1;i<=size2;i++){
		if(scanf("%d",&v2[i]) != 1){ 
			printf("Error reading element %d!\n",i);
			return ERROR_READ;
		}
	}

	*n1 = size1;
	*n2 = size2;
	return SUCCESS;
}

int insert(int *v1, int n1, int c1, int *v2, int n2, int pos)
{
	for(int i=n1+n2;i>pos+n2;i--){
		v1[i]=v1[i-pos+1];
	}

	for(int i=pos;i<=pos+n2;i++){
		v1[i]=v2[i-pos];
	}

		return SUCCESS;

		if(n1+n2>=c1){
			return -1;
		}
}



int main()
{
	int v1[150], v2[150], n1, n2, res;
	res = readArray(&n1, v1, &n2, v2);

	if (res != 0) {
		return res;
	}

	insert(v1, n1, 8, v2, n2, 3);
	
	for(int i=1;i<=n1+n2;i++)
		printf("%d ", v1[i]);

	return 0;
}
