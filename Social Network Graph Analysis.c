#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// Structure to represent an adjacency list node
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Structure to represent an adjacency list
typedef struct {
    Node* head;
} AdjList;

// Structure to represent a graph
typedef struct {
    int numVertices;
    AdjList* array;
} Graph;

// Function to create a new graph with given number of vertices
Graph* createGraph(int numVertices) {
    if (numVertices <= 0 || numVertices > MAX_VERTICES) {
        printf("Invalid number of vertices. Must be between 1 and %d.\n", MAX_VERTICES);
        return NULL;
    }
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->array = (AdjList*)malloc(numVertices * sizeof(AdjList));

    for (int i = 0; i < numVertices; ++i) {
        graph->array[i].head = NULL;
    }

    return graph;
}

// Function to add an edge to an undirected graph
void addEdge(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        printf("Invalid input: vertex numbers must be between 0 and %d\n", graph->numVertices - 1);
        return;
    }

    // Add an edge from src to dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since the graph is undirected, add an edge from dest to src as well
    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = src;
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Function to print the adjacency list representation of the graph
void printGraph(Graph* graph) {
    printf("Adjacency list of the graph:\n");
    for (int i = 0; i < graph->numVertices; ++i) {
        Node* temp = graph->array[i].head;
        printf("Vertex %d -> ", i);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Utility function for DFS
void DFSUtil(Graph* graph, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);

    Node* temp = graph->array[v].head;
    while (temp) {
        int connectedVertex = temp->vertex;
        if (!visited[connectedVertex]) {
            DFSUtil(graph, connectedVertex, visited);
        }
        temp = temp->next;
    }
}

// Function to perform DFS traversal
void DFS(Graph* graph, int startVertex) {
    if (startVertex < 0 || startVertex >= graph->numVertices) {
        printf("Invalid start vertex.\n");
        return;
    }

    bool* visited = (bool*)malloc(graph->numVertices * sizeof(bool));
    for (int i = 0; i < graph->numVertices; ++i) {
        visited[i] = false;
    }

    DFSUtil(graph, startVertex, visited);

    free(visited);
}

// Utility function for BFS
void BFS(Graph* graph, int startVertex) {
    if (startVertex < 0 || startVertex >= graph->numVertices) {
        printf("Invalid start vertex.\n");
        return;
    }
    bool* visited = (bool*)malloc(graph->numVertices * sizeof(bool));
    for (int i = 0; i < graph->numVertices; ++i) {
        visited[i] = false;
    }

    int* queue = (int*)malloc(graph->numVertices * sizeof(int));
    int front = 0;
    int rear = 0;

    visited[startVertex] = true;
    queue[rear++] = startVertex;

    while (front < rear) {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);

        Node* temp = graph->array[currentVertex].head;
        while (temp) {
            int adjVertex = temp->vertex;
            if (!visited[adjVertex]) {
                visited[adjVertex] = true;
                queue[rear++] = adjVertex;
            }
            temp = temp->next;
        }
    }

    free(visited);
    free(queue);
}

// Function to find and print connected components
void findConnectedComponents(Graph* graph) {
    bool* visited = (bool*)malloc(graph->numVertices * sizeof(bool));
    for (int v = 0; v < graph->numVertices; v++) {
        visited[v] = false;
    }

    for (int v = 0; v < graph->numVertices; v++) {
        if (!visited[v]) {
            DFSUtil(graph, v, visited);
            printf("\n");
        }
    }

    free(visited);
}

// Function to calculate degree centrality of vertices
void degreeCentrality(Graph* graph) {
    int* degree = (int*)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; ++i) {
        degree[i] = 0;
        Node* temp = graph->array[i].head;
        while (temp) {
            degree[i]++;
            temp = temp->next;
        }
        printf("Degree centrality of vertex %d: %d\n", i, degree[i]);
    }
    free(degree);
}

int main() {
    int numVertices, numEdges;
    printf("Enter the number of vertices in the graph: ");
    scanf("%d", &numVertices);

    Graph* graph = createGraph(numVertices);
    if (!graph) return 1;

    printf("Enter the number of edges in the graph: ");
    scanf("%d", &numEdges);

    if (numEdges < 0) {
        printf("Invalid number of edges. Must be a non-negative integer.\n");
        return 1;
    }

    printf("Enter %d edges as pairs of vertices (src dest):\n", numEdges);
    for (int i = 0; i < numEdges; ++i) {
        int src, dest;
        scanf("%d %d", &src, &dest);
        addEdge(graph, src, dest);
    }

    // Print the adjacency list representation of the graph
    printGraph(graph);

    // Perform DFS traversal
    printf("DFS traversal starting from vertex 0:\n");
    DFS(graph, 0);
    printf("\n");

    // Perform BFS traversal
    printf("BFS traversal starting from vertex 0:\n");
    BFS(graph, 0);
    printf("\n");

    // Find connected components
    printf("Connected components of the graph:\n");
    findConnectedComponents(graph);

    // Calculate degree centrality of vertices
    printf("Degree centrality of vertices:\n");
    degreeCentrality(graph);

    // Memory cleanup
    for (int i = 0; i < graph->numVertices; ++i) {
        Node* temp = graph->array[i].head;
        while (temp) {
            Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(graph->array);
    free(graph);

    return 0;
}
