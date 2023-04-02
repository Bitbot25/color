#include <stdio.h>
#include <stdlib.h>

#define NUM_REGISTERS 4
#define NUM_VARIABLES 7

struct varstat {
  int usage_cnt;
};

struct varstat statistics[NUM_VARIABLES] = {
  {2},
  {1},
  {2},
  {1},
  {2},
  {3},
  {3}
};

// Graph representation
int graph[NUM_VARIABLES][NUM_VARIABLES] = {
    {0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1},
    {1, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 1},
    {0, 1, 0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 0},
};
// Color assignments
int colors[NUM_VARIABLES];

// Find the node with the fewest neighbors
int findMinDegreeNode(int* degrees, int size) {
    int minDegree = NUM_VARIABLES + 1;
    int minNode = -1;
    for (int i = 0; i < size; i++) {
        if (degrees[i] < minDegree && colors[i] == -1) {
            minDegree = degrees[i];
            minNode = i;
        }
    }
    return minNode;
}

// Assign colors to the nodes in the graph
void colorGraphBriggs() {
    int degrees[NUM_VARIABLES];
    for (int i = 0; i < NUM_VARIABLES; i++) {
        degrees[i] = 0;
        colors[i] = -1;
        for (int j = 0; j < NUM_VARIABLES; j++) {
            if (graph[i][j]) {
                degrees[i]++;
            }
        }
    }
    for (int i = 0; i < NUM_VARIABLES; i++) {
        int node = findMinDegreeNode(degrees, NUM_VARIABLES);
        if (node == -1) {
            printf("Error: graph is not colorable\n");
            return;
        }
        int usedColors[NUM_REGISTERS] = {0};
        for (int j = 0; j < NUM_VARIABLES; j++) {
            if (graph[node][j] && colors[j] != -1) {
                usedColors[colors[j]] = 1;
            }
        }
        for (int j = 0; j < NUM_REGISTERS; j++) {
            if (!usedColors[j]) {
                colors[node] = j;
                break;
            }
        }
    }
}

// Simplify the graph by coalescing nodes that can be safely merged
void simplifyGraph(char* modified) {
    int coalesced[NUM_VARIABLES] = {0};
    for (int i = 0; i < NUM_VARIABLES; i++) {
        if (coalesced[i]) continue;
        for (int j = i + 1; j < NUM_VARIABLES; j++) {
            if (coalesced[j]) continue;
            if (!graph[i][j] && colors[i] == colors[j]) {
                // Merge nodes i and j
                coalesced[j] = 1;
                colors[j] = colors[i];
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    if (graph[j][k]) {
                        if (graph[i][k] != 1 && graph[j][k] != 0) {
                          *modified = 1;
                        }
                        graph[i][k] = 1;
                        graph[j][k] = 0;
                    }
                }
            }
        }
    }
}

void colorGraphDSatur() {
  fprintf(stderr, "Error: DSatur is not implemented (line %d, file %s)", __LINE__, __FILE_NAME__);
  exit(1);
}

int costOfColoring() {
  int cost = 0;
  for (int i = 0; i < NUM_VARIABLES; i++) {
    int access_cost = colors[i] == -1 ? 10 : 1;
    int usage_cnt = statistics[i].usage_cnt;
    cost += access_cost * usage_cnt;
  }
  return cost;
}

void printGraph() {
  for (int i = 0; i < NUM_VARIABLES; i++) {
    printf("Variable %d:\n", i);
    for (int j = 0; j < NUM_VARIABLES; j++) {
      if (graph[i][j]) {
        printf("\tInterferes with %d\n", j);
      }
    }
  }
}

int main() {
    for (int j = 0; j < 3; j++) {
      printGraph();
      // colorGraphBriggs();
      colorGraphDSatur();
      int cost = costOfColoring();
      printf("Cost: %d\n", cost);
      printf("Color assignments:\n");
      for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("Variable %d: register %d\n", i, colors[i]);
      }
      char modified = 0;
      simplifyGraph(&modified);
      if (modified) {
        printf("Coalesced.\n");
      } else {
        printf("Not modified.\n");
        break;
      }
    }
    return 0;
}
