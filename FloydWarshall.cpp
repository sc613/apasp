#include <iostream>

using namespace std;

int main(){
    int V, E;
    cin >> V >> E;
    int **d = new int*[V];
    for (int i = 0; i < V; i++)
        d[i] = new int[V];
    for (int i = 0; i < V; i++)
    for (int j = 0; j < V; j++)
        d[i][j] = -1;
    int v1, v2;
    for(int i = 0; i < E; i++){
        cin >> v1 >> v2;
        d[v1][v2] = 1;
        d[v2][v1] = 1;
    }
    
    int tmp;
    for (int i = 0; i < V; i++)
    for (int j = 0; j < V; j++)
    for (int k = 0; k < V; k++){
        if (d[j][i] == -1 || d[i][k] == -1) tmp = -1;
        else tmp = d[j][i] + d[i][k];
        
        if (d[j][k] == -1) d[j][k] = tmp;
        else if (tmp != -1 && d[j][k] > tmp) d[j][k] = tmp;        
    }

    for (int i = 0; i < V; i++){
        for (int j = 0; j < V; j++){
            printf("%d ", d[i][j]);
        }
        printf("\n");
    }


    return 0;
}
