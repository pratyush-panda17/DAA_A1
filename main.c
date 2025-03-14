#include <stdio.h>
#include <stdlib.h>  
#include <time.h>
#include "structs.h"
#include "plot_staircase.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))

int compareByXDescending(const void *a, const void *b) { //comparison function for two dimensional points. (x coordinate is given priority)
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;

    // First compare by x in descending order
    if (p2->x != p1->x) {
        return p2->x - p1->x;  // Descending order for x
    }
    
    // If x is the same, compare by y in descending order
    return p2->y - p1->y;  // Descending order for y
}

// Function to create a new tree node with a single tuple
Node* createNode(Point point, int layer_index) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    node->height = 1;
    node->y_val = point.y;
    node->x_val = point.x;
    node->array_size = 1;  
    node->layer_index = layer_index;

    // Allocate memory for one tuple
    node->layer = (Point*)malloc(sizeof(Point));
    if (!node->layer) {
        printf("Memory allocation failed for array\n");
        free(node);
        return NULL;
    }

    node->layer[0] = point;  // Store the single tuple

    node->left = NULL;
    node->right = NULL;
    return node;
}

int getHeight(struct Node *n){ 
    if(n==NULL)
        return 0;
    return n->height;
}

int getBalanceFactor(struct Node *n){
    if(n==NULL){
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

struct Node* rightRotate(struct Node* y){
    struct Node* x = y->left;
    struct Node* T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    y->height = MAX(getHeight(y->right), getHeight(y->left)) + 1;
    x->height = MAX(getHeight(x->right), getHeight(x->left)) + 1;
    
 
    return x;
}

int getInorderPredecessor(Node* root,int y){ //returns the largest y value strictly smaller than the input 
    if (root==NULL){
        return -1;
    }
    if(root->y_val>=y){
        if (root->left==NULL){
            return -1;
        }
        return getInorderPredecessor(root->left,y);
    }
    return MAX(root->y_val, getInorderPredecessor(root->right,y));
}

Node* nodeToInsertIn(Node* root,int y, Point point){ //when inserting in a layer, it may be possible that there are multiple nodes                                                 
    if (root==NULL){                                   // with the same y- value, in such a case the largest x value is chosen
        return NULL;
    }
    if (root->y_val == y){
        Node* root2 =  nodeToInsertIn(root->right,y,point);
        if (root2==NULL){
            return root;
        }else if (root->x_val > root2->x_val){
            return root;
        }else{
            return root2;
        }
    }
    
    if (root->y_val>y){
        return nodeToInsertIn(root->left,y,point);
    }else{
        return nodeToInsertIn(root->right,y,point);
    }
}

void insertInLayer(Node* root,int y,Point point){ //updates the head node of the layer and inserts value in the layer array.
    Node* dest = nodeToInsertIn(root,y,point);
    Point *new_array = (Point*)realloc(dest->layer, (dest->array_size + 1) * sizeof(Point));
    dest->layer = new_array;
    dest->layer[dest->array_size] = point;
    dest->array_size++;
    dest->y_val = point.y;
    dest->x_val = point.x;
}

Node* insertInTree(Node* root,Point point, int layer_index){ // in the case that a given point does not belong to any layer,
    if (root==NULL){    
        return createNode(point,layer_index);                   //a new node is added to represent the new layer.
    }
    if (root->y_val<point.y){
        root->right = insertInTree(root->right,point,layer_index);
    }
    else if(root->y_val>=point.y){
        root->left = insertInTree(root->left,point,layer_index);
    }
    root->height = 1 + MAX(getHeight(root->left), getHeight(root->right));
    int bf = getBalanceFactor(root);
    if(bf>1){ //this is because a new node is only added to the tree if it has the smallest y_value, hence only right rotations are required       
        return rightRotate(root);
        }
    return root;
}

Point* generateRandomPoints(int n, int k) { //helper function for testing
    // Allocate memory for n points
    Point* points = (Point*)malloc(n * sizeof(Point));
    if (!points) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Seed random number generator
    srand(time(NULL));
    
    // Generate random points
    for(int i = 0; i < n; i++) {
        points[i].x = rand() % (k + 1);  // Random x in [0, k]
        points[i].y = rand() % (k + 1);  // Random y in [0, k]
    }

    return points;
}

void printArray(Point arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("(%d,%d)", arr[i].x,arr[i].y);
        if (i < size - 1) {
            printf(", "); // Add a comma between elements
        }
    }
    printf("]\n");
}

void printTree(Node* root){ 
    if (root==NULL){
        return;
    }
    printTree(root->right);
    printArray(root->layer,root->array_size);
    printTree(root->left);
}

void printTreeGraphic(Node* root, int space) { //helper function for visualizing the tree
    if (root == NULL) return;

    space += 5;  // Increase distance between levels

    // Print right subtree first
    printTreeGraphic(root->right, space);

    // Print current node
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("(%d,%d)\n", root->x_val,root->y_val);

    // Print left subtree
    printTreeGraphic(root->left, space);
}


void getStaircases(Node* root,FILE* staircaseFile){ //function to create the staircase file used for plotting
    if (root==NULL){
        return;
    } 
  
    // Write staircase path

    getStaircases(root->right,staircaseFile);
    for (int i = 0; i < root->array_size - 1; i++) {
        fprintf(staircaseFile, "%d %d\n", root->layer[i].x, root->layer[i].y); 
        fprintf(staircaseFile, "%d %d\n", root->layer[i + 1].x, root->layer[i + 1].y);
    }
    fprintf(staircaseFile, "%d %d\n", root->layer[root->array_size - 1].x, root->layer[root->array_size - 1].y);
    fprintf(staircaseFile, "\n");
    getStaircases(root->left,staircaseFile);
    return;
}

void makeOutputFile(Node* root,FILE* outputFile){ //prints the output in the required format
    if(root==NULL){
        return;
    }
    makeOutputFile(root->right,outputFile);
    for (int i = 0; i < root->array_size; i++) {
        fprintf(outputFile, "%d %d\n", root->layer[i].x, root->layer[i].y); 
    }
    fprintf(outputFile, "\n");
    makeOutputFile(root->left,outputFile);
    return;
}



  
int main(){
    int n;
    scanf("%d", &n);
    Point* points = (Point*)malloc(n * sizeof(Point));

    FILE *pointsFile = fopen("points.txt", "w");

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }

    for (int i = 0; i < n; i++) { //creating a points file for plotting
        fprintf(pointsFile,"%d %d\n", points[i].x, points[i].y);
    }



    qsort(points, n, sizeof(Point), compareByXDescending); //first sort the list of points in descending order
    
    Node* root = NULL;
    int layer_index = 2;

    root  = insertInTree(root,points[0],1); // the first point will always be maximal
    for(int i =1;i<n;i++){
        if (points[i].x==points[i-1].x && points[i].y==points[i-1].y){
            continue;
        }
        int pred = getInorderPredecessor(root,points[i].y); 
        if (pred==-1){ //no inorderpredecessor implies it has the smallest y-value.
            root = insertInTree(root,points[i],layer_index);
            layer_index++;
        }
        else{ 
            insertInLayer(root,pred,points[i]);
        }
    }
    FILE *staircaseFile = fopen("staircase.txt", "w");


    getStaircases(root,staircaseFile);
    fclose(pointsFile);
    fclose(staircaseFile);
    plotStaircase(pointsFile,staircaseFile);
    makeOutputFile(root,stdout);
    return 0;
}