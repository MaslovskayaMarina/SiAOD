#include <iostream>
#include <string>
#include <iomanip>
#include <time.h> 
#include <fstream>
using namespace std;

class B_search {
public:
    int data; // Ссылка на строку в файле
    int key; // Ключ
    int rotate = 0;
    int count = 0;
    string ALLkeyS = "";
    B_search* left = NULL, * right = NULL; // Указатели на потомков
    B_search* splay(B_search* T, int key, int ct) { // Метод подъема ключа в корень
        if (!T || T->key == key) // Если root = NULL или ключ является корнем
            return T; // Вернуть текущий узел
        if (T->key > key) {
            if (ct > 0) count++;// Если ключ может лежать в левом поддереве
            if (!T->left) {// Ключа нет в дереве
                if (ct > 0) count++;
                return T; // Вернуть текущий узел
            }
            if (T->left->key > key) { 
                if (ct > 0) count++;// Ключ может лежать в левом-левом поддереве 
                T->left->left = splay(T->left->left, key, ct); // Рекурсивно поднимем ключ
                T = rightRotate(T); // Выполним правый поворот
            }
            else if (T->left->key < key) {
                if (ct > 0) count++;// Может лежать в левом-правом поддереве
                T->left->right = splay(T->left->right, key, ct); // Рекурсивно поднимем ключ
                if (T->left->right) {
                    if (ct > 0) count++;// Если полученный узел не пустой
                    T->left = leftRotate(T->left); // Выполним левый поворот
                }
            }
            return (T->left == NULL) ? T : rightRotate(T); // Выполним правый разворот для корня
        }
        else {// Ключ может находиться в правом поддереве
            if (T->right == NULL) { 
                if (ct > 0) count++;// Если искомого ключа нет в дереве
                return T; // Возвращаем текущий узел
            }
            if (T->right->key > key) {
                if (ct > 0) count++;// Ключ может находиться в правом-правом поддереве
                T->right->left = splay(T->right->left, key, ct); // Рекурсивно поднимем ключ
                if (T->right->left != NULL) { // Если полученный узел не пустой
                    if (ct > 0) count++;
                    T->right = rightRotate(T->right); // Выполним правый поворот
                }
            }
            else if (T->right->key < key) { 
                if (ct > 0) count++;// Ключ может находиться в правом-левом поддереве
                T->right->right = splay(T->right->right, key, ct); // Рекурсивно поднимем ключ
                T = leftRotate(T); // Выполним левый поворот
            }
            return (T->right == NULL) ? T : leftRotate(T); // Выполним левый разворот для корня
        }
    }
    B_search* rightRotate(B_search* T)
    {
        B_search* node = T->left; // Сохраним в узле node левое поддерево T
        T->left = node->right; // В левом поддереве T сохраним правое поддерево node
        node->right = T; // В правом поддереве node сохраним T
        rotate++; // Используем счётчик поворотов
        return node; // Возвращаем узел node
    }
    B_search* leftRotate(B_search* T)
    {
        B_search* node = T->right; // Сохраним в узле node правое поддерево T
        T->right = node->left; // В правом поддереве T сохраним левое поддерево node
        node->left = T; // В левом поддереве node сохраним T
        rotate++; // Используем счётчик поворотов
        return node; // Возвращаем узел node
    }
    B_search* createNode(int key, int data) { // Создать узел
        B_search* node = new B_search; // Новый узел
        node->key = key; // Ключ
        node->data = data; // Ссылка на строку в файле
        node->left = 0; // Значение левого поддерева
        node->right = 0; // Значение правого поддерева 
        return node; // Вернуть созданный узел
    }

    B_search* insertBinSearchTree(B_search* T, int data, int key) { // Метод вставки ключа в дерево
        int this_key = key; // Актуальное значение key
        char BIGDATA[25]; // Строка, в которую будем считывать строки из файла
        if (this_key == 0) { // Если ключ пока что 0
            ifstream ffout("BinFile.bin", ios::binary | ios::out); // Открываем файл           
            int i = 0; // Инициализируем i (для подсчёта строк)
            while (data != i) { // Пока не дойдем до нужной строки в файле
                ffout.getline(BIGDATA, 25); // Будем считывать каждую строку и сохранять в BIGDATA
                i++; // Инкремент i            
            }
            int h = 1;
            for (int y = 0; BIGDATA[y] != ' '; y++) { // Узнаем значение ключа
                if (y < 24) if (BIGDATA[y + 1] != ' ') {
                    h = 10;
                }
                    this_key = h*(this_key + (int)BIGDATA[y] - '0');
                    h = 1;
            }
            ffout.close(); // Закрываем файл
        }
        if (!T) {
            cout << "Был вставлен ключ " << this_key << endl; // Выводим данные созданного узла
            ALLkeyS = ALLkeyS + ' ' + to_string(this_key); // Записываем в строку со всеми ключами наш текущий ключ
            // В будущем ALLkeyS понадобится для синхронизации файла
            return createNode(this_key, data); // Если узел пустой, вызываем метод создания узла
        }
        else {
            while (understudy(T, this_key) != 0) this_key++; // Проверяем, нет ли ключа-дублера
            T = splay(T, this_key, 0); // Вызываем метод подъема ключа в корень для T
            B_search* node = createNode(this_key, data); // Создаем новый узел node
            if (this_key < T->key) { // Текущий ключ меньше ключа найденного узла
                node->right = T; // В правое поддерево node сохраняем T
                node->left = T->left; // В левое поддерево node сохраняем левое поддерево T
                T->left = nullptr; // Левое поддерево T делаем пустым
                cout << "Был вставлен ключ " << this_key << endl; // Выводим данные созданного узла
                ALLkeyS = ALLkeyS + ' ' + to_string(this_key); // Записываем ключ в массив ключей
            }
            else { // Текущий ключ больше ключа найденного узла
                node->left = T; // В левое поддерево node записываем T
                node->right = T->right; // В правое поддерево node записываем правое поддерево T
                T->right = nullptr; // Правое поддерево T делаем пустым
                cout << "Был вставлен ключ " << this_key << endl; // Выводим данные созданного узла
                ALLkeyS = ALLkeyS + ' ' + to_string(this_key); // Записываем ключ в массив ключей
            }
            return node; // Возвращаем узел node
        }
    }

    B_search* searchNode(B_search* T, int key) // Метод поиска узла в дереве и возврата ссылки на него
    {
        return splay(T, key, 1); // Вызываем для текущего узла и ключа метод splay
    }

    int search(B_search* T, int key) { // Поиск ключа в дереве и возврат ссылки на данные в файле
        return splay(T, key, count)->data; // Вызываем для текущего узла и ключа метод splay и из результата берем значение data
    }

    void printTree(B_search* T, int level) { // Метод вывод дерева на экран
        if (T) { // Если узел не пуст
            printTree(T->right, level + 1); // Вызываем метод вывода дерева для правого поддерева
            for (int i = 0; i < level; i++) cout << "   "; // Соблюдаем расстояние
            cout << T->key << endl;     // Выводим полученные данные на экран
            printTree(T->left, level + 1); // Вызыываем метод вывода дерева для левого поддерева
        }
        else return; // Выход
    }

    B_search* del(B_search* T) { //Находим самый левый узел
        if (T->left == NULL) return T; //При нахождении самого левого узла вернуть его указатель
        return del(T->left); // Вызвать рекурсию от левого потомка
    }

    B_search* deleteNode(int key, B_search*& T) { // Удаление ключа
        if (!T) return T; // Возврат T, если T nullptr
        if (key < T->key) T->left = deleteNode(key, T->left); //Вызов рекурсии от левого потомка
        else if (key > T->key) T->right = deleteNode(key, T->right); //Вызов рекурсии от правого потомка
        else if (T->left and T->right) { // Оба потомка не пустые
            ifstream ffout("BinFile.bin", ios::binary | ios::out); // Открываем файле
            char BIGDATA[25];
            int x = 0;                                             // Чтобы получить данные,
            while (T->data != x) {                                 // которые удаляются
                ffout.getline(BIGDATA, 25);
                x++;
            }
            cout << "Был удален узел со значением " << BIGDATA << endl; // Вывод того, что мы удаляем
            T->key = del(T->right)->key; // Новое значение key
            T->data = del(T->right)->data; // Новое значения data
            T->right = deleteNode(T->key, T->right); // Новый указатель на правого потомка
        }
        else {
            if (T->left) T = T->left; // Если левый потомок не пуст, то заменить текущий узел левым потомком
            else if (T->right) T = T->right; // Если правый потомок не пуст, то заменить текущий узел правый потомком        
            else T = nullptr; // Иначе вернуть нулевой указатель
        }
        return T;
    }

    int understudy(B_search* T, int key) { // Поиск ключа в дереве и возврат ссылки на данные в файле
        if (!T) // Если узел пустой
            return 0; // Вернуть ноль
        if (T) { // Если узел не пустой
            if (key == T->key) // Если ключ совпадает с искомым
                return T->data; // Вернуть ссылку
            if (key > T->key) // Если ключ больше искомого
                return understudy(T->right, key); // Вызов метода поиска для правого поддерева
            if (key < T->key) // Если ключ меньше искомого
                return understudy(T->left, key); // Вызов метода поиска для левого поддерева
        }
    }
};

class File {
public:
    void updatefile(B_search* T, string keyS) { // Метод синхронизации дерева с файлом
        B_search B; // Инициализируем класс B_search
        B_search* node; // Создаем объект класса B_search
        string _data[255];
        for (int j = 0; j < 255; j++)
            _data[j] = "0";
        int key;
        int s = 0;
        for (int i = 1; i < keyS.length(); i++) { // Считываем из ранее созданной
            key = 0;                              // строки со всеми ключами каждый ключ
            int d = 10;
            while (keyS[i] != ' ' and i < keyS.length()) {
                if (i < keyS.length() - 1)
                    if (keyS[i - 1] == ' ' and keyS[i + 1] == ' ')
                        d = 1;
                key = d * (key + keyS[i] - '0');
                i++;
                d = 1;
            }
            T = B.searchNode(T, key); // Ищем узел с найденным ключом
            if (T) { // Если узел найден
                ifstream ffout("BinFile.bin", ios::binary | ios::out);
                char BIGDATA[25];
                int x = 0;
                while (T->data != x) {
                    ffout.getline(BIGDATA, 25);
                    x++;
                }
                _data[x] = BIGDATA; // Сохраняем его содержимое в массив строк
                ffout.close();
            }
        }
        // Очищаем файл
        fstream clear_file("BinFile.bin", ios::binary | ios::out);
        clear_file.close();
        // Перезаписываем файл используя созданный массив строк
        ofstream fin("BinFile.bin", ios::binary | ios::in);
        for (int j = 0; j < 255; j++)
            if (_data[j] != "0") fin << _data[j] << endl;
        fin.close(); // Закрываем файл
    }

    void createfile() { // Создание обычного текстового файла
        ofstream fout("TestFile.txt"); // Открываем файл для записи
        fout << "2 2+4 6" << endl; // Записываем данные...
        fout << "1 1+0 1" << endl;
        fout << "10 5+5 10" << endl;
        fout << "27 6+6 12" << endl;
        fout << "11 8*8 64" << endl;
        fout << "9 4+4 8" << endl;
        fout << "9 5*5 25" << endl;
        fout << "23 2*3 6" << endl;
        fout << "66 1*1 1" << endl;
        fout << "77 5+4 9" << endl;
        fout << "2 5+6 11" << endl;
        for (int j = 12; j <= 500; j++)
            fout << "6 5+6 11" << endl;
            fout << "999 5+6 11" << endl;
        fout.close(); // Закрываем текстовый файл
        ifstream fin("TestFile.txt"); // Открываем только что созданный файл для чтения
        ofstream ffout; // Объявляем новый файл для записи
        ffout.open("BinFile.bin", ios::binary | ios::out | ios::trunc); //Открываем бинарный файл
        ffout.clear(); // Чистим бинарный файл
        ffout.close(); // Закрываем бинарный файл
        ofstream in("BinFile.bin", ios::binary | ios::in); // Снова открываем бинарный файл для записи
        char buff[25]; // Создаем строку, в которой будем хранить считываемые значения из текстового файла
        while (fin.getline(buff, 25))
            in << buff << endl;
        in.close(); // Закрываем все файлы
        fin.close();
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    B_search B;
    B_search* root = NULL;
    B_search* node;
    File f;
    // Создание бинарного файла
    f.createfile();
    ifstream ffout("BinFile.bin", ios::binary | ios::out);
    char BIGDATA[25];
    int i = 0;
    cout << sizeof(B_search);
    while (ffout.getline(BIGDATA, 25)) {
        i++;
        root = B.insertBinSearchTree(root, i, 0);
        //cout << "\n-----ВЫВОД СДП НА ЭКРАН-----\n";
        //B.printTree(root, 0);
    }
    cout << endl << "В качестве ключа используется сумма цифр. При совпадении ключа значение увеличивается на 1" << endl;
    cout << endl << "Среднее число выполненных поворотов= " << (double)B.rotate / i << endl;
    //cout << "\n-----ВЫВОД СДП НА ЭКРАН-----\n";
    //B.printTree(root, 0);
    cout << "\n-----ПОИСК-----\n";
    cout << "Ищем узел с ключом 654" << endl;
    clock_t start, end;
    start = clock();
    node = B.searchNode(root, -24);
    cout << B.count;
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    cout << sizeof(B_search);
    int v = B.search(root, 787);
    root = node;
    if (root) {
        ifstream ff("BinFile.bin", ios::binary | ios::out);
        char _BIGDATA[25];
        int x = 0;
        while (root->data != x) {
            ff.getline(_BIGDATA, 25);
            x++;
        }
        ff.close();
        cout << "Был найден ключ= " << root->key << " Данные: " << _BIGDATA << endl;
    }
    cout << "Поиск ключа 12 в дереве вернул ссылку на " << v << " строку файла" << endl;
    //cout << "\n-----ВЫВОД СДП НА ЭКРАН-----\n";
    //B.printTree(root, 0);
    //cout << "\n-----ВЫВОД СДП НА ЭКРАН-----\n";
    //B.printTree(root, 0);
    cout << "\n-----УДАЛЕНИЕ УЗЛА-----" << "\n";
    B.deleteNode(12, root);
    cout << "\n-----ВЫВОД СДП НА ЭКРАН-----\n";
    //B.printTree(root, 0);
    //f.updatefile(root, B.ALLkeyS);
    
}
