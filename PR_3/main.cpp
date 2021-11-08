#include<iostream>
#include<stack>
using namespace std;

struct Node // Структура узла *
{
    char data; // Содержимое узла
    Node* left, * right; // Указатели на потомков
};

bool isOperator(char c) // Является ли оператором*
{
    if (c == '+' || c == '-' || c == '*' || c == '/') return true;
    else return false;
}

Node* newNode(char data) // Создание нового узла*
{
    Node* node = new Node;
    node->left = node->right = NULL; // Создание потомков
    node->data = data; // Информационная часть
    return node;
};

Node* constructTree(char postfix[])// *
{
    stack<Node*> st;
    Node* x, * y, * z;
    for (int i = 0; i < strlen(postfix); i++)
    {
        if (!isOperator(postfix[i])) // Если текущий элемент не является оператором - вставляем в стек
        {
            x = newNode(postfix[i]); // Создаем новый узел
            st.push(x); // Вызываем метод push() для нового узла
        }
        else // Если текущий элемент является оператором - вставляем два узла и делаем их детьми
        {
            x = newNode(postfix[i]); // Создаем новый узел
            y = st.top(); // Вызваем метод top() и сохраняем результат в y
            st.pop(); // Вызываем метод pop()
            z = st.top(); // Вызываем метод top() и сохраняем результат в z
            st.pop(); // Вызываем метод pop()
            x->right = y; // Правому потомку x задаем значение y
            x->left = z; // Левому потомку x задаем значение z
            st.push(x); // Вызываем метод push() для x
        }
    }
    x = st.top(); // Сохраняем результат выполнения метода top() в x
    st.pop(); // Вызываем метод pop()
    return x; 
}

int priority(char x) //*
{
    if ((x == '*') || (x == '/')) return 2;
    if ((x == '+') || (x == '-')) return 1;
    if ((x == '(') || (x == ')')) return 0;
}

class stack1 // *
{
    int top;
    char body[100];
public:
    stack1() {
        top = NULL;
    }
    bool empty() {
        return top == NULL;
    }
    char get_top_element() {
        return body[top];
    }
    int top_prior() {
        return priority(body[top]);
    }
    void push(char x)
    {
        top++;
        body[top] = x;
    }
    char pop()
    {
        top--;
        return body[top + 1];
    }
};

string convertToPostfix(char *data) { // Функция конвертирования строки в постфиксную форму*
    stack1 s; // Создаем стек s
    int k = 0;
    char NewData[100];
    int len = int(strlen(data)); // len - длина строки data
    for (int i = 0; i <= len - 1; i++)
    {
        if (data[i] == '(') s.push(data[i]); // Если текущий элемент = ( то вызываем для него метод push
        else if ((data[i] == '+') || (data[i] == '-') || (data[i] == '/') || (data[i] == '*')) // Иначе если текущий элемент - оператор
        {
            while ((!s.empty()) && (s.top_prior() > priority(data[i]))) // Пока стек не пустой и значение метода top_prior() больше чем значение метода priority()
            {
                k++; // Инкремент k
                NewData[k] = s.pop(); // k-му элементу массива символов NewData присвоить значение метода pop()
            }
            s.push(data[i]); // Вызов метода push для текущего элемента
        }
        else if (data[i] == ')') // Если текущий элемент = ) 
        {
            while ((!s.empty()) && (s.get_top_element() != '(')) // Пока стек не пуст и вызов метода get_top_element() не вернёт (
            {
                k++; // Инкремент k
                NewData[k] = s.pop(); // k-му элементу массива символов NewData присвоить значение метода pop()
            }
            s.pop(); // Вызов метоа pop()
        }
        else
        {
            k++; // Инкремент k
            NewData[k] = data[i]; // k-му элементу массива символов NewData присвоить значение i-го элемента строки символов data
        }
    }
    while (!s.empty()) // Пока стек не пуст
    {
        k++; // Инкремент k
        NewData[k] = s.pop(); // k-му элементу массива символов NewData присвоить значение метода pop()
    }
    string NewDataInString; // Создадим строку символов
    for (int i = 1; i <= k; i++) // Конвертируем полученный массив символов в строку
        NewDataInString = NewDataInString + NewData[i];
    return NewDataInString; // Вернём строку символов
}

bool isEmpty(Node* T) { //*
    if (T) return false;
    else return true;
}

void printBinTree(Node* T, int level, int L) { // *Функция вывода дерева на экран
    if (!isEmpty(T->right)) { // Правый потомок не пустой
        printBinTree(T->right, level + 1, L); // Вызов функции для правого потомка 
        for (int i = 1; i <= level * L; i++) { // Печать пробелов в зависимости от уровня, на котором находится текущий элемент
            cout << ' '; 
        }
        cout << T->data << endl; // Вывод значения data текущего элемента в дереве
    }
    if (!isEmpty(T->left)) { // Левый потомок не пустой
        printBinTree(T->left, level + 1, L); // Вызов функции для левого потомка 
    }
    else { // Левый потомок - пуст
        for (int i = 1; i <= level * L; i++) { // Печать пробелов в зависимости от уровня, на котором находится текущий элемент
            cout << ' ';
        }
        cout << T->data << endl; // Вывод значения data текущего элемента в дереве
    }
}

int nodes(Node* T, int level, int i, int k) { //* Функция подсчёта количества узлов на заданном уровне
    if (i == level) k++; // Если текущий уровень элемента в дереве совпадает с тем, который мы ищем добавить - инкремент k
    if (!isEmpty(T->right)) { // Если правый потомок не пустой
        k += nodes(T->right, level, i + 1, 0); // Прибавить к k результат выполнения фукнции для правого потомка

    }
    if (!isEmpty(T->left)) { // Если левый потомок не пустой

        k += nodes(T->left, level, i + 1, 0); // Прибавить к k результат выполнения фукнции для левого потомка
    }
    return k; // Вернуть значение - количество узлов на занном уровне
}

void infix(Node* T) // Функция вывода на экран выражения в префиксной форме*
{
    if (T) // Текущий элемент не NULL
    {
        cout << T->data; // Вывести на экран значение data текущего элемента
        infix(T->left); // Вызов функции для левого потомка
        infix(T->right); // Вызов функции для правого потока
    }
}

double leftproblem(Node* T) { // *Функция подсчёта значения выражения в левом поддереве
    if (T->left != NULL and T->right != NULL) { // Правый и левый потомок текущего элемента не пустые
            if (T->data == '+') return leftproblem(T->left) + leftproblem(T->right); // Если значение data текущего элемента = + то вернуть значение вызова функции для левого потомка + вызова левого потомка
            if (T->data == '-') return leftproblem(T->left) - leftproblem(T->right); // Если значение data текущего элемента = - то вернуть значение вызова функции для левого потомка - вызова левого потомка
            if (T->data == '*') return leftproblem(T->left) * leftproblem(T->right); // Если значение data текущего элемента = * то вернуть значение вызова функции для левого потомка * вызов левого потомка
            if (T->data == '/') return leftproblem(T->left) / leftproblem(T->right); // Если значение data текущего элемента = / то вернуть значение вызова функции для левого потомка / вызов левого потомка
            }
    return (T->data)-48; // Вернуть значение data в виде цифры
}

int main()
{   // Вариант 14. Бинарное древо выражения. Студент: Масловская Марина ИНБО-01-20
    setlocale(LC_ALL, "Russian");
 //   char str[] = "4*(1+2)+(2*2)";
 /*   char str[] = "a/(b-(c*d))";
    cout << "Введено выражение: " << str << endl;
    string postfix = convertToPostfix(str); // Конвертируем в постфиксную форму, чтобы было проще построить бинарное дерево выражений
    cout << postfix;
    char char_array[8]; 
    for (int i = 0; i < 7; i++)
        char_array[i] = postfix[i]; // Конвертируем колченную строку постфиксной формы string и массив char
        */
    char char_array[] = "ab+c/d-e*";
    Node* root = constructTree(char_array); // Вызываем фукцию построения дерева
    while (true) { // Зацикленное меню
        int p;
        cout << "-----МЕНЮ-----" << endl;
        cout << "1 - Вывод дерева на экран" << endl;
        cout << "2 - Подсчёт количества узлов на заданном уровне" << endl;
        cout << "3 - Вычислить значение выражения в левом поддереве" << endl;
        cout << "4 - Вывод префиксной формы левого поддерева" << endl;
        cout << "0 - Выход из программы" << endl;
        cin >> p;
        if (p == 0) break;
        if (p == 1) {
            cout << "-----ВЫВОД ДЕРЕВА-----" << endl;
            printBinTree(root, 0, 1);
        }
        if (p == 2) {
            cout << "-----ПОДСЧЁТ КОЛИЧЕСТВА УЗЛОВ НА ЗАДАННОМ УРОВНЕ-----" << endl;
            int level;
            cin >> level;
            cout << "Количество узлов = " << nodes(root, level, 0, 0) << endl;
        }
        if (p == 3) {
            cout << "-----ВЫЧИСЛЕНИЕ ЗНАЧЕНИЯ ВЫРАЖЕНИЯ В ЛЕВОМ ПОДДЕРЕВЕ-----" << endl;
            cout << "Результат = " << leftproblem(root->left) << endl;
        }
        if (p == 4) {
            cout << "-----ВЫВОД ПРЕФИКСНОЙ ФОРМЫ ЛЕВОГО ПОДДЕРЕВА-----" << endl;
            cout << "Префиксная форма выражения: ";
            infix(root->left);
            cout << endl;
        }
    }
    return 0;
}
