#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>

using namespace std;

class Node
{
public:
    int count; // Значение
    char symbol; // Буква
    Node* left; // Левый потомок
    Node* right; // Правый потомок

    Node(char symbol, int count)
    {
        this->symbol = symbol; // Сохранить символ для текущего узла
        this->count = count; // Сохранить значение для текущего узла
    }
    Node(Node* left, Node* right)
    {
        symbol = '0'; // Символ для текущего узла - 0, так как буква отсутсвует
        this->left = left; // Сохранить левый потомок для текущего узла
        this->right = right; // Сохранить правый потомок для текущего узла
        count = this->left->count + this->right->count; // В качестве count сохранить сумму значений потомков left и right
    }
};

void BuildTable(Node* root, vector<bool>& code, map<char, vector<bool>>& table) // Функция построения таблицы
{
    if (root->left) // Если левый потомок текущего узла не нулевой
    {
        code.push_back(0); // Добавить 0 в конец списка code
        BuildTable(root->left, code, table); // Вызвать функцию BuildTable для левого потомка текущего узла
    }

    if (root->right) // Если правый потомок текущего узла не нулевой
    {
        code.push_back(1); // Добавить 1 в конец списка code
        BuildTable(root->right, code, table); // Вызвать функцию BuildTable для правого потомка текущего узла
    }

    if (root->symbol != '0') // Если символ текущего узла не 0
        table[root->symbol] = code; // Текущему символу назначим полученный код
    if (code.size()) // Если вектор code не пуст
        code.pop_back(); // Удалить элемент из конца
}

bool SortNode(const Node* x, const Node* y)
{
    return x->count < y->count;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int a = 0, c;
    string text; // Создаем переменную для хранения строки под названием text
    ifstream in("input.txt"); // Окрываем файл input.txt для чтения
    getline(in, text); // Считать строку из файла
    in.close(); // Закрываем файл
    map<char, int> symbols; // Создаем контрейнер map для символов нашей строки
    for (int i = 0; i < text.length(); i++) // Пока не достигнут конец строки text
        symbols[text[i]]++; // Заполняем контейнер символами
    list<Node*> trees; // Создаем список узлов
    map<char, int>::iterator it; // Создаем итератор для контейнера map
    for (it = symbols.begin(); it != symbols.end(); it++) // Пока мы не достигли конца map
    {
        Node* newNode = new Node(it->first, it->second); // Создаем новый узел
        trees.push_back(newNode); // И записываем его в конец списка
    }
    while (trees.size() != 1) // Если размер списка не равен 1
    {
        trees.sort(SortNode); // Локальная сортировка
        Node* xNode = trees.front(); // Инициализируем xNode как первый элемент trees
        trees.pop_front(); // Удалим элемент из начала списка trees
        Node* yNode = trees.front(); // Инициализируем yNode как первый элемент trees
        trees.pop_front(); // Удалим элемент из начала списка trees
        Node* parent = new Node(xNode, yNode); // Инициализируем parent с помощью конструктора, подавая значения xNode и yNode в качестве предков
        trees.push_back(parent); // Запишем parent в конец списка trees
    }
    Node* root = trees.front(); // Инициализируем главный узел root как первый элемент списка trees
    vector<bool> code; // Создадим вектор для кодов
    map<char, vector<bool> > table; // Создаем контейнер map для хранения символа и его кода
    BuildTable(root, code, table); // Вызовем метод построения таблицы
    for (it = symbols.begin(); it != symbols.end(); it++) // Пока мы не прошлись по всему контейнеру map
    {
        cout << it->first << " - "; // Напечатать текущий элемент и "-"
        for (int j = 0; j < table[it->first].size(); j++) // Пока мы не напечатали полностью код для текущего символа 
            cout << table[it->first][j]; // Вывести на экран текущую цифру кода
        cout << endl; // Переход на новую строку
    }
    ofstream outext("output.txt"); // Открываем для записи файл output.txt
    for (int i = 0; i < text.length(); i++) // Пока мы не достигли конца строки text
        for (int j = 0; j < table[text[i]].size(); j++) // Пока мы не напечатали полностью код текущего символа
        {
            outext << table[text[i]][j]; // Печатать текщую цифру кода в файл output.txt
        }
    outext.close(); // Закрыть файл для вывода
    return 0;
}
