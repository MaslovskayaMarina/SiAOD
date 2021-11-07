#include <iostream>
#include <string>
#include <fstream>
#include <time.h> // Для замера времени выполнения команд
using namespace std;

class B_search {
public:
    string data;
    int key; // ключ
    string s_key;
    static int i;
    string ALLkeyS = "";
    B_search* left = NULL, * right = NULL; // Указатели на потомков
    B_search* createNode(int key, string data, string s_key) { // Создать узел
        B_search* node = new B_search;
        node->key = key;
        node->data = data;
        node->s_key = s_key;
        node->left = 0;
        node->right = 0;
        return node;
    }
    void insertBinSearchTree(B_search*& T, string s_key, string data, int key) { // Построение дерева
        int this_key = key;
        if (this_key == 0)
            for (int i = 0; i < s_key.length(); i++)
                if (s_key[i] != '.')
                    this_key = this_key + (int)s_key[i] - 48;
        if (!T) {
            T = createNode(this_key, data, s_key);
            cout << "Создан узел с данными: " << T->key << " " << T->data << endl;
            ALLkeyS = ALLkeyS + ' ' + to_string(T->key);
        }
        else {
            if (this_key == T->key)
                this_key++;
            if (this_key < T->key)
                insertBinSearchTree(T->left, s_key, data, this_key);
            else if (this_key > T->key)
                insertBinSearchTree(T->right, s_key, data, this_key);
        }
    }

    B_search* searchNode(B_search* T, int key) { // Поиск ключа в дереве
        if (!T) {
            cout << "Искомый узел не найден!" << endl;
            return nullptr;
        }  
        if (T) {
            if (key == T->key) {
                cout << "Был найден ключ= " << T->key << " Значение= " << T->data << endl;
                return T;
            }
            if (key > T->key) {
                return searchNode(T->right, key);
            }
            if (key < T->key) {
                return searchNode(T->left, key);
            }
        }
                  
    }
 
    void printTree(B_search* T, int level) {
        if (T) {
            printTree(T->right, level + 1);
            for (int i = 0; i < level; i++) cout << "                    ";
            cout << T->data << T->key << endl;
            printTree(T->left, level + 1);
        }
        else return;
    }

    B_search* del(B_search* T) { //Находим самый левый узел
        if (T->left == NULL) return T; //При нахождении самого левого узла вернуть его указатель
        return del(T->left); // Вызвать рекурсию от левого потомка
    }
  
    B_search* deleteNode(int key, B_search* &T) { // Удаление узла
        if (!T) return T; // Возврат T, если T nullptr
        if (key < T->key) T->left = deleteNode(key, T->left); //Вызов рекурсии от левого потомка
        else if (key > T->key) T->right = deleteNode(key, T->right); //Вызов рекурсии от правого потомка
        else if (T->left and T->right) {
            cout << "Был удален узел со значением " << T->key << " " << T -> data << endl;
            T->key = del(T->right)->key; // Новое значение key
            T->data = del(T->right)->data; // Новое значения data
            T->right = deleteNode(T->key, T->right); // Новый указатель на правого потомка
        }
        else {        
            if (T->left) T = T->left;
            else if (T->right) T = T->right;          
            else T = nullptr;
        }      
        return T;
    }
};

class File {
public:
    void updatefile(B_search* T, string keyS) {
        B_search B;
        B_search* node;
        ofstream ff;
        int key;
        ff.open("BinFile.bin", ios::binary | ios::in);
        int s = 0;
        cout << keyS << endl;
        for (int i = 1; i < keyS.length(); i++) {
            key = 0;
            int d = 10;
            while (keyS[i] != ' ' and i < keyS.length()) {
                cout << keyS[i] << ' ';
                if (i < keyS.length() - 1)
                    if (keyS[i - 1] == ' ' and keyS[i + 1] == ' ')
                        d = 1;
                key = d*(key + keyS[i] - '0');
                i++;
                d = 1;
            }           
            node = B.searchNode(T, key);
            if (node) {
                cout << node->s_key << ' ' << node->data << endl;
                ff << node->s_key << ' ' << node->data << endl;
            }
        }
        ff.close();     
    }
    void createfile() { // Создание обычного текстового файла
        ofstream fout("TestFile.txt"); // Открываем файл для записи
        fout << "24.09.2001 Марина" << endl; // Записываем данные...
        fout << "11.01.2009 Анатолий" << endl;
        fout << "10.08.1995 Геннадий" << endl;
        fout << "27.02.2002 Степан" << endl;
        fout << "11.10.2007 Юлия" << endl;
        fout << "15.09.1999 Григорий" << endl;
        fout << "17.06.2006 Валентин" << endl;
        fout << "17.07.1962 Инна" << endl;
        fout << "25.12.1993 Виктор" << endl;
        fout << "01.02.2004 Елена" << endl;
        fout << "01.02.2005 Валентина" << endl;
        fout.close(); // Закрываем текстовый файл
        ifstream fin("TestFile.txt"); // Открываем только что созданный файл для чтения
        ofstream ffout; // Объявляем новый файл для записи
        ffout.open("BinFile.bin", ios::binary | ios::out | ios::trunc); //Открываем бинарный файл для записи
        ffout.clear(); // Чистим бинарный файл
        ffout.close(); // Закрываем бинарный файл
        ofstream in("BinFile.bin", ios::binary | ios::in); // Снова открываем бинарный файл для записи
        char buff[25]; // Создаем строку, в которой будем хранить считываемые значения из текстового файла
        while (fin.getline(buff, 25)) {
            in << buff << endl;
        }
        in.close();
        fin.close();
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    B_search B; 
    B_search* root = NULL;
    File f;
    // Создание бинарного файла
    f.createfile();
    ifstream ffout("BinFile.bin", ios::binary | ios::out);
    char BIGDATA[25];
    int count = 0;
    string keyS ="";
    cout << "Создан бинарный файл со следующим содержанием:" << endl;
    while (ffout.getline(BIGDATA, 25)) {
        string key;
        string data;
        int k = 0;
        bool f = true;
        for (int i = 0; i < 24; i++) {   
            if ((int)BIGDATA[i] == 0) f = false;
            if (BIGDATA[i] == ' ') {
                k++;
                i++;
            }
            if (f and k % 2 == 0) key = key + BIGDATA[i];
            else if (f and (int)BIGDATA[i] >= -64 and (int)BIGDATA[i] <= -1) data = data + BIGDATA[i];
        }
        cout << key << ' ' << data << endl;
        B.insertBinSearchTree(root, key, data, 0);
        count++;
    }
    cout << "-----ВЫВОД БДП НА ЭКРАН-----" << endl;
    B.printTree(root, 0);
    cout << "-----ПОИСК-----" << endl;
    B.searchNode(root, 18);
    cout << "\n-----УДАЛЕНИЕ УЗЛА-----" << "\n";
    B.deleteNode(14, root);
    cout << "\n-----ВЫВОД БДП НА ЭКРАН-----" << endl;
    B.printTree(root, 0);
    cout << "\n-----УДАЛЕНИЕ УЗЛА-----" << "\n";
    B.deleteNode(18, root);
    cout << "\n-----ВЫВОД БДП НА ЭКРАН-----" << endl;
    B.printTree(root, 0);
    fstream clear_file("BinFile.bin", ios::binary | ios::out);
    clear_file.close();
    f.updatefile(root, B.ALLkeyS);
}
