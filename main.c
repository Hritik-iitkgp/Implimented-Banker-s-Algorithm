#include <stdio.h>

// The maximum number of processes and resources
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

//  The available, maximum, allocated, and need arrays
int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocated[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
// number of process and number of resources for input
int num_processes, num_resources;

// Function to compute the need matrix
void need_A() {
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      need[i][j] = maximum[i][j] - allocated[i][j];
    }
  }
}

// Function to check if a request can be assigned or not by assigning the
// requested resources and if it is safe sequence return 1 otherwise return 0
int assign(int process, int request[]) {
  // one more array for avaiable , allocated and need
  int available_r[MAX_RESOURCES];
  int allocated_r[MAX_PROCESSES][MAX_RESOURCES];
  int need_r[MAX_PROCESSES][MAX_RESOURCES];

  // Initialize the arrays temparory such that we can assign allocate the
  // requested resources
  for (int i = 0; i < num_resources; i++) {
    available_r[i] = available[i] - request[i];
  }
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      allocated_r[i][j] = allocated[i][j];
      need_r[i][j] = need[i][j];
    }
  }
  // let allocate the requested resources
  for (int i = 0; i < num_resources; i++) {
    allocated_r[process][i] += request[i];
    need_r[process][i] -= request[i];
    available_r[i] -= request[i];
  }

  // Define temporary arrays for the remaining and complete array
  int remaining[MAX_RESOURCES];
  int complete[MAX_PROCESSES];

  // Initialize the remaining and complete vectors as available resource and
  // complete as zero
  for (int i = 0; i < num_resources; i++) {
    remaining[i] = available_r[i];
  }
  for (int i = 0; i < num_processes; i++) {
    complete[i] = 0;
  }

  // Find a safe sequence
  int count = 0;
  while (count < num_processes) {
    int found = 0;
    for (int i = 0; i < num_processes; i++) {
      if (complete[i] == 0) {
        int j;
        for (j = 0; j < num_resources; j++) {
          if (need_r[i][j] > remaining[j])
            break;
        }
        if (j == num_resources) {
          for (int k = 0; k < num_resources; k++) {
            remaining[k] += allocated_r[i][k];
          }
          complete[i] = 1;
          found = 1;
          count++;
        }
      }
    }
    if (found == 0)
      return 0; // No safe sequence found
  }
  return 1; // Safe sequence found
}

// Function to request resources and it will print weather request grantes or
// not if granted then update the available  , allocated , max need matrix
void request_r(int process, int request[]) {
  // Check if the request can be granted
  if (assign(process, request)) {
    // Allocate the requested resources
    for (int i = 0; i < num_resources; i++) {
      allocated[process][i] += request[i];
      need[process][i] -= request[i];
      available[i] -= request[i];
    }
    printf("Request granted.\n");
  } else {
    printf("Request denied.\n");
  }
}

int main() {
  // Read in the number of processes and resources
  printf("Enter the number of processes: ");
  scanf("%d", &num_processes);
  printf("Enter the number of resources: ");
  scanf("%d", &num_resources);

  // Read in the available matrix
  printf("Enter the available matrix:\n");
  for (int i = 0; i < num_resources; i++) {
    scanf("%d", &available[i]);
  }

  // Read in the maximum matrix
  printf("Enter the maximum matrix:\n");
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      scanf("%d", &maximum[i][j]);
    }
  }

  // Read in the allocated matrix
  printf("Enter the allocated matrix:\n");
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      scanf("%d", &allocated[i][j]);
    }
  }

  // compute the need matrix
  need_A();

  // Read in requests and grant them if possible
  while (1) {
    // Read in the process number and request vector
    int process;
    int request[MAX_RESOURCES];
    printf("Enter the process number and request vector (or -1 to quit): ");
    scanf("%d", &process);
    if (process == -1)
      break;
    for (int i = 0; i < num_resources; i++) {
      scanf("%d", &request[i]);
    }

    // Request the resources
    request_r(process, request);

    // Print the current state
    printf("Current state:\n");
    printf("Available:\n");
    for (int i = 0; i < num_resources; i++) {
      printf("%d ", available[i]);
    }
    printf("\n");

    printf("Maximum:\n");
    for (int i = 0; i < num_processes; i++) {
      for (int j = 0; j < num_resources; j++) {
        printf("%d ", maximum[i][j]);
      }
      printf("\n");
    }

    printf("Allocated:\n");
    for (int i = 0; i < num_processes; i++) {
      for (int j = 0; j < num_resources; j++) {
        printf("%d ", allocated[i][j]);
      }
      printf("\n");
    }

    printf("Need:\n");
    for (int i = 0; i < num_processes; i++) {
      for (int j = 0; j < num_resources; j++) {
        printf("%d ", need[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}
