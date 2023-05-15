#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t mutex;

int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
void* customer_routine(void* customer_num);

int main(int argc, char *argv[]) {
    if (argc != NUMBER_OF_RESOURCES + 1) {
        printf("Incorrect number of arguments\n");
        return -1;
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = atoi(argv[i+1]);
    }

    pthread_t customers[NUMBER_OF_CUSTOMERS];
    pthread_mutex_init(&mutex, NULL);

    // Initialize the maximum resources each customer can request
    srand(time(0));
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % available[j] + 1;
            need[i][j] = maximum[i][j];
        }
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_create(&customers[i], NULL, &customer_routine, (void*)(long)i);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

void* customer_routine(void* customer_num) {
    int customer = (int)(long)customer_num;
    int request[NUMBER_OF_RESOURCES];
    int last_request_status = -1; // Flag to store the last request status

    while (1) {
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            request[i] = rand() % (need[customer][i] + 1);
        }

        pthread_mutex_lock(&mutex);

        printf("Customer %d requests resources: ", customer);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            printf("%d ", request[i]);
        }
        printf("\n");

        int request_status = request_resources(customer, request);
        if (request_status != last_request_status) {
            if (request_status == 0) {
                printf("Customer %d's request is granted\n", customer);
                release_resources(customer, request);
            } else {
                printf("Customer %d's request is denied due to potential unsafe state\n", customer);
            }

            last_request_status = request_status;
        }

        pthread_mutex_unlock(&mutex);

        sleep(rand() % 5 + 1);
    }

    return NULL;
}


int request_resources(int customer_num, int request[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            return -1;
        }
        if (request[i] > available[i]) {
            return -1;
        }
    }

    // Temporarily allocate resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -=        request[i];
    }

    // Check if the state is safe
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];
    memcpy(work, available, sizeof(work));
    memset(finish, 0, sizeof(finish));

    while (1) {
        int found = 0;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (finish[i] == 0) {
                int j;
                for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == NUMBER_OF_RESOURCES) {
                    for (int k = 0; k < NUMBER_OF_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            break;
        }
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        if (finish[i] == 0) {
            // Not safe. Revert the resources
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                available[j] += request[j];
                allocation[customer_num][j] -= request[j];
                need[customer_num][j] += request[j];
            }
            return -1;
        }
    }

    return 0;
}

void release_resources(int customer_num, int release[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }
}

