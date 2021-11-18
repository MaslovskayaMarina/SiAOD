#include <iostream>
#include <vector>

using namespace std;

typedef struct graph {
	int E = 0, V = 0; // Количество ребер и вершин
	vector<vector<int>> g; // Связи в графе
};

typedef graph* Graph;

void printGraph(Graph Gr) { // Функция отображения графа
	cout << "\n---ОТОБРАЖЕНИЕ ГРАФА В ПАМЯТИ---\n\n";
	for (int i = 1; i != Gr->g.size(); i++) { // Пока основной список вершин не пройден
		for (int y = 0; y != Gr->g[i].size(); y++) { // Выводить подчиненные элементы каждой из вершин
			if (Gr->g[i][y] != 0) cout << "Вершина " << i << " соединена с вершиной " << y << "\n";
		}
	}
}

void expV(Graph Gr) {
	int ver, k = 0;
	cout << "Степень какой вершины нужно определить?\n";
	cin >> ver;
	if (ver > Gr->g.size()) {
		cout << "Такой вершины нет в графе!\n";
		return;
	}
	else {
		for (int i = 1; i < Gr->g.size(); i++) { // Пока основной список вершин не пройден
			for (int y = 1; y < Gr->g[i].size(); y++) { // Выводить подчиненные элементы каждой из вершин
				if ((i == ver || y == ver) && Gr->g[i][y] != 0) k++;
			}
		}
		cout << "Степень вершины " << ver << " равна " << k << "\n";
	}
}

Graph GRAPHInit(int V) { // Инициализация графа из V вершин
	Graph gr = new graph; // Создаем новый граф
	gr->V = V; // Записываем количество вершин
	vector<vector<int>> gt(V + 1); // Создаем список, который будет содержать связи графа
	int NeOr, Or; 
	cout << "Количество неориентированных ребер = ";
	cin >> NeOr; // Записываем количество неориентированных ребер
	cout << "Количество ориентированных ребер = ";
	cin >> Or; // Записываем количество ориентированных ребер
	gr->E = Or + 2 * NeOr; // Сохраняем количество ребер
	for (int k = Or + 2 * NeOr; k > 0; k--) { // Заполнение связей в графе
		int v1, v2, distance;
		cin >> v1 >> v2 >> distance; // Считываем 2 вершины, соединенные ребром и дистанцию
		if (distance > gt[v1].size()) {
			gt[v1].resize(distance + 4);
		}
		auto iter = gt[v1].cbegin();
		gt[v1].insert(iter + v2, distance); // Создаем связь между ними с помощью списка
	}
	gr->g = gt; // Записываем созданный список связей в список связей графа
	return gr; // Возвращаем созданный граф
}

void dijkstra(Graph Gr) {
	int v1, v2;
	cout << "\nВведите вершину, от которой нужно построить путь\n";
	cin >> v1;
	cout << "\nВведите вершину, к которой нужно построить путь\n";
	cin >> v2;
	bool visited[10000];
	int dist[10000], P[10000], y = 0;
	P[0] = v1;
	for (int i = 1; i < Gr->V + 1; i++) { // Запись веса каждой вершины
		dist[i] = Gr->g[v1][i];
		if (dist[i] == 0 and i != v1) {
			dist[i] = INT_MAX;
		}
		visited[i] = false;
	}
	dist[v1] = 0; // Назначение веса изначальной вершине - 0
	int index, k;
	for (int i = 1; i < Gr->V + 1; i++) {
		int min = INT_MAX;
		for (int j = 1; j < 10000; j++)
		{
			if (!visited[j] && dist[j] < min && dist[j] != 0)
			{
				min = dist[j];
				index = j;		
			}
		}
		k = index;
		visited[k] = true;
		for (int j = 1; j < 10000; j++) {
			if (!visited[j] && Gr->g[k][j] != INT_MAX && dist[k] != INT_MAX && (dist[k] + Gr->g[k][j] < dist[j]) && Gr->g[k][j]!=0)
			{
				dist[j] = dist[k] + Gr->g[k][j];
				if (j == v2) {
					y++;
					P[y] = k;
				}
			}
		}		
	}
	cout << v1 << " -> " << v2 << " Дистанция = " << dist[v2] << "\n";
	cout << "Путь: ";
	for (int i = 0; i < Gr->V; i++) {
		if (P[i] > 0) cout << P[i] << " -> ";
	}
	cout << v2 << "\n";
}

int main() {
	setlocale(LC_ALL, "Russian");
	Graph Gr = new graph;
	int n;
	cout << "Количество вершин = ";
	cin >> n;
	Gr = GRAPHInit(n);
	while (true) {
		int t;
		cout << "\n-----МЕНЮ-----\n";
		cout << "1 - Вывод графа на экран\n";
		cout << "2 - Опеределить степень вершины\n";
		cout << "3 - Вывод кратчайшего пути от одной вершины к другой с помощью метода Дейкстры\n";
		cout << "0 - Выход из программы\n";
		cin >> t;
		if (t == 0) break;
		else if (t == 1) {
			printGraph(Gr);
		}
		else if (t == 2) {
			expV(Gr);
		}
		else if (t == 3) {
			dijkstra(Gr);
		}
	}
}
