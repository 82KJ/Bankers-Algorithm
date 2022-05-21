#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int request[NUMBER_OF_RESOURCES];

// �� �迭 ��,�� ���� �� �Լ�
int comp_arr(int *x, int *y) {
	int x_bigger_than_y = 0;
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		if (x[i] > y[i]) x_bigger_than_y += 1;
	}

	// x�� y���� ������ return 1
	if (x_bigger_than_y == 0) return 1;
	else return 0;
}

// safety checking �Լ�
int is_safe() {
	// 1. Work = Available && Finish = false
	int work[NUMBER_OF_RESOURCES];
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		work[i] = available[i];
	}
	int finish[NUMBER_OF_CUSTOMERS] = {0,0,0,0,0};
	int process_order[NUMBER_OF_CUSTOMERS] = { 0,0,0,0,0 };
	int order = 0;

	// 2. ��� process�� �ݺ������� Ž��
	while (1) {
		int sum = 0; // (process ���� && need<= work)�� ���μ����� �� üŷ

		for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
			// ����, ���� ���μ����� ���� ������� �ʾҰ�, need�� work���� ������
			// �ش� ���μ����� �����Ű�� resource�� �ݳ��Ѵ�
			if (finish[i] == 0 && comp_arr(need[i], work) == 1) {
				for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
					work[j] += allocation[i][j];
				}
				finish[i] = 1;
				process_order[order++] = i; // process ���� ������ ����ش�
				sum += 1;
			}
		}

		if (sum == 0) { 
			int flag = 0;
			
			// ��� process�� ����Ǿ����� Ȯ��
			for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
				if (finish[i] == 0) {
					flag = 1;
					break;
				}
			}

			if (flag == 0) {// ��� process�� ���� --> safe
				printf("Process Execution Order : ");
				for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
					printf("P%d ", process_order[i]);
				}
				printf("\n");
				return 0;
			}
			else return -1; // ������� ���� process�� ���� --> unsafe
		}
	}
}

int release_resources(int process_num) {

	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		available[i] += request[i];
		allocation[process_num][i] -= request[i];
		need[process_num][i] += request[i];
	}
}


int request_resources(int process_num) {

	// ���� ó�� 1. request�� available���� ���� ���
	if (comp_arr(request, available) == 0) {
		printf("Request is Bigger than Available\n");
		return -1;
	}

	// ���� ó�� 2. request�� process�� need���� ���� ���
	if (comp_arr(request, need[process_num]) == 0) {
		printf("Request is Bigger than Need\n");
		return -1;
	}

	// available -= request, allocation += request, need -= request
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		available[i] -= request[i];
		allocation[process_num][i] += request[i];
		need[process_num][i] -= request[i];
	}

	// ���� safe�ϴٸ�, return 0
	if (is_safe() == 0) {
		release_resources(process_num);
		return 0;
	}
	else {	// unsafe�ϴٸ�, return -1
		release_resources(process_num);
		return -1;
	}

}


int main() {

	// 1. maximum �Է�
	FILE* max_txt = fopen("max.txt", "r");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
			fscanf(max_txt, "%d ", &maximum[i][j]);
		}
	}
	fclose(max_txt);

	// 2. allocation �Է�
	FILE* allocation_txt = fopen("allocation.txt", "r");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
			fscanf(allocation_txt, "%d ", &allocation[i][j]);
			
			// 3. need �ʱ�ȭ
			need[i][j] = maximum[i][j] - allocation[i][j];
		}
	}
	fclose(allocation_txt);

	// 4. available�� user �Է�
	printf("Available Resources (3) : ");
	scanf("%d %d %d", &available[0], &available[1], &available[2]);
	printf("<If you want to end, press Q>\n");
	
	// 5. Request�� ������ ���� ������ ����� ����Ѵ�
	char yn;
	int process_num;
	while (1) {
		printf("\n==========================================\n");
		printf("\nIs There a Requested Process ? [Y/N/Q] ");
		scanf(" %c", &yn);

		if (yn == 'Y') {
			printf("Enter the Process number and resources : ");
			scanf("%d %d %d %d", &process_num, &request[0], &request[1], &request[2]);
			
			if (request_resources(process_num) == 0) {
				printf("All processes have been successfully terminated\n");
			}
			else {
				printf("==> Deadlock\n");
			}
		}
		else if(yn == 'N'){
			if (is_safe() == 0) {
				printf("==> All processes have been successfully terminated\n");
			}
			else {
				printf("==> Deadlock\n");
			}

		}
		else if (yn == 'Q') {
			break;
		}

		else {
			printf("You entered it incorrectly\n");
		}
	}
	return 0;
}