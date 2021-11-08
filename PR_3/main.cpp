#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <time.h> // Для замера времени выполнения команд

using namespace std;

// ячейка хеш-таблицы
struct Cell
{
    // "N/A" символизирует пустоту поля
    string key_ = "N/A";
    string name_ = "N/A";

    // Метод структуры определяющий, пустая ли ячейка
    bool isEmpty() const {
        return key_ == "N/A" && name_ == "N/A";
    }

    // Метод вывода текущей ячейки
    void output() {
        cout << " " << key_ << endl;
        cout << " " << name_ << endl;
        cout << "-------------------------" << endl;
    }
};

// Хэш-функция (Задание 1.1)
int hashIndex(string key, int hashLen)
{
    int sum = 0;
    for (int i = 0; i < key.length(); i++)
        sum += key[i];
    return sum % hashLen;
}

// Класс хэш-таблицы
class HashTable
{
    int LEN_ = 8; // размер хэш-таблицы
    Cell* H_;     // массив, что хранит элементы таблицы
public:
    // Конструктор - в нем происходит инициализация таблицы
    HashTable() {
        H_ = new Cell[LEN_];
    }

    // Деструктор освободит память хэш-таблицы
    ~HashTable() {
        delete[] H_;
    }

    // Метод вставки записи в таблицу (Задание 1.2)
    void add(Cell cell) {
        if ((((double)realElemCount() + 1) / (double)LEN_) >= 0.75) {   // Условие проверяет, будет ли учтён коэффицент заполненности при добавлении новой записи,
                                                                        //  если нет - произойдёт рехэширование
            cout << "-----------REHASHING-----------" << endl;          // Вывод предупреждения о рехэшировании
            rehashTable();                                              // Вызов метода рехэширования
        }

        int index = hashIndex(cell.key_, LEN_);                         // Получаем хэш записи
        while (true) {
            if (index == LEN_)                                          // В случае, если коллизия произошла на последнем элементе хэш-таблицы - поиск свободной ячейки начнется с начала таблицы
                index = 0;
            if (H_[index].isEmpty() == true) {                          // Условие проверяет, занято ли место с этим значением хэша. Если занято - индекс инкрментируется
                H_[index] = cell;                                       // даем значение элементу по найденному индексу

                updateFile();                                           // добавление этой записи в файл
                return;
            }
            else {
                index++;
            }
        }
    }

    // Метод поиска индекса элемента с заданным ключом, нужен для метода удаления
    int find(string key)
    {
        int index = hashIndex(key, LEN_);                      // Находим хэш параметра


        int i = index;                                         // Начинаем поиск нашей записи с индекса, полученного хэш-функцией
        while (true) {
            if (i + 1 == LEN_) {
                i = 0;
            }
            if (H_[i].key_ == key) {                           // Проверяем ключи
                return i;                                      // Возвращаем искомый индекс
            }
            i++;
        }
        return -1;                                             // -1 значит, что мы не нашли такой элемент
    }

    // Задание 1.4: Найти запись с заданным ключом в файле, используя хеш-таблицу.
    void findElement(string key, Cell& buffer)
    {
        fstream in("hashTable.bin", ios::binary | ios::in);    // Открываем поток ввода
        int counter = 0;                                       // Счётчик, определяющий, какая по счёту запись была передана в файл
        int index = hashIndex(key, LEN_);                      // находим его индекс

        // Поскольку метод обновления элементов добавляет записи по мере возрастания ключа, то определить к какой
        // записи сослаться поможет следующий алгоритм: проверяется каждый элемент хэш-таблицы, к счётчику будет прибавлять по единице с каждым не пустым
        // элементом, пока не найдётся искомай элемент и цикл прервётся
        for (int i = 0; i < LEN_; i++) {
            if (!H_[i].isEmpty()) {
                if (H_[i].key_ == key) {
                    break;
                }
                counter++;
            }
        }
        in.seekg(sizeof(buffer) * counter, ios::beg);            // Перемещение к искомой записи для считывания
        in.read((char*)&buffer, sizeof buffer);                // Считывание записи

        cout << buffer.key_ << " " << buffer.name_ << endl;

        in.close();                                            // Закрываем открытый файл
    }

    // Удаление записи из таблицы и файла (Задание 1.3)
    string deleteElem(string key) {
        int index = find(key);                                 // ищем такой элемент в таблице
        if (index == -1) return "error";                       // если не нашли вернуть строку "ошибка"
        string name = H_[index].name_;                         // вытаскиваем значение

        H_[index].key_ = H_[index].name_ = "N/A";              // "обнуляем" элемент
        updateFile();
        return name;
    }

    // Вспомогательный метод: позволяет узнать, сколько записей присутствует в хэш-таблцице
    int realElemCount() {
        int counter = 0;
        for (int i = 0; i < LEN_; i++)
            if (!H_[i].isEmpty()) {
                counter++;
            }
        return counter;
    }

    // Метод вывода хэш-таблицы в консоль (пустые ячейки хэш-таблицы не учитываются) (Задание 5)
    void output() {
        cout << "\n-------------------------\n--------Hashtable--------\n-------------------------\n\n";

        for (int i = 0; i < LEN_; i++)            // Цикл перебирает все элементы хэш-таблицы и проверяет каждый на существование там записи
            if (!H_[i].isEmpty()) {               // В случае, если по текущему индексу запись существует - она будет выведена в консоль
                cout << " i: " << i << endl;
                H_[i].output();
            }
        cout << endl << endl;
    }

    // Метод рехэширования (Задание 1.5)
    void rehashTable() {
        Cell* oldH_ = H_;                         // Создаётся копия предыдущей хэш-таблицы
        H_ = new Cell[LEN_ * 2];                  // Полю-таблице присваивается новый массив с длинной, в два раза превышающей старую длину
        size_t oldLEN_ = LEN_;                    // Создаётся копия предущего размера хэш-таблицы (для перебора старых элементов)
        LEN_ *= 2;                                // Поле с значением длины хэш-таблицы умножается в два раза

        for (int i = 0; i < oldLEN_; i++) {       // Перебор старых элементов таблицы и добавление их в новую хэш-таблицу
            if (!oldH_[i].isEmpty()) {
                add(oldH_[i]);
            }
        }
        updateFile();                             // После рехэширования следует обновить файл с записями
    }

    // Метод для отладки - позволяет узнать текущий размер таблицы (нужен для проверки метода рехэширования)
    int getLen() {
        return LEN_;
    }

    // Метод вствки записи в бинарный файл
    void writeToFile(Cell& toCell) {
        ofstream ffout;

        ffout.open("hashTable.bin", ios::binary | ios::out);

        if (!ffout)
        {
            cout << "file not open";
            return;
        }

        ffout.write((char*)&toCell, sizeof(toCell));
        ffout.close();
    }

    // Метод, обновляющий файл с записями. Переносит все записи текущей хэш-таблицы в файл, удаляя предущее содержимое
    void updateFile() {
        ofstream ffout;

        ffout.open("hashTable.bin", ios::binary | ios::out | ios::trunc);

        if (!ffout)
        {
            cout << "file not open";
            return;
        }

        for (int i = 0; i < LEN_; i++) {
            if (!H_[i].isEmpty()) {
                ffout.write((char*)&H_[i], sizeof(Cell));
                ffout.clear();
            }
        }
        ffout.close();
    }

    // Метод, читающий записи из файла и вставляющий их в хэш-таблицу (Задание 1.2)
    void readTheFile(Cell& Y)
    {

        fstream in("hashTable.bin", ios::binary | ios::in); // Открываем поток ввода

        in.read((char*)&Y, sizeof Y); //Считываем информацию в объект Y
        while (!in.eof())
        {
            add(Y);
            in.read((char*)&Y, sizeof Y);
        }
        in.close(); //Закрываем открытый файл

    }
};

int main()
{
    clock_t start, end;
    HashTable ht;  // Создаём объект хэш-таблицы
    //-----------------------Тест хэш-таблицы-----------------------//
    cout << sizeof(ht);
    start = clock();
    ht.add({ "05.06.2002", "Margarita" });
    ht.add({ "27.02.2002", "Stepan" });    // Эта запись является коллизией с предыдущей записью (Задание 2)
    ht.add({ "20.08.2002", "Max" });
    ht.add({ "14.02.2000", "Cupid" });
    ht.add({ "30.12.1999", "Gleb" });
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    ht.output(); // Вывод всех элементов хэш-таблицы
    ht.add({ "09.04.2014", "Victoria" }); // Изначальный размер таблицы был 8, следовательно при добавлении этого элемента
                                          // коэффицент заполнения станет больше 0.75 и произойдёт рехеширование (Задание 3)
    ht.output(); // Вывод всех элементов хэш-таблицы
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    start = clock();
    ht.deleteElem("30.12.1999"); // Удалим запись с ключом 30.12.1999
    ht.deleteElem("20.08.2002");
    ht.deleteElem("05.06.2002");
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    ht.output(); // Снова выведем всю таблицу чтобы удостовериться, что запись была удалёна
    //-----------------------Тест записи/чтения-----------------------//
    HashTable ht2;
    Cell cell, cell2, cell3, cell4;
    ht2.readTheFile(cell); // Читаем записи из файла, который был заполнен предыдущей хэш-таблицей и заполням ими новую хэш-таблицу
    ht2.output();
    ht2.findElement("14.02.2000", cell2);
    ht2.findElement("09.04.2014", cell3);
    ht2.findElement("27.02.2002", cell4);
    //-----------------------Заполнение большим количеством записей (Задание 4)-----------------------//
    HashTable ht3;
    ht3.add({ "05.06.2002", "Margarita" });
    ht3.add({ "27.02.2012", "Stepan" });
    ht3.add({ "20.08.2002", "Max" });
    ht3.add({ "14.02.2000", "Cupid" });
    ht3.add({ "30.12.1999", "Gleb" });
    ht3.add({ "09.04.2014", "Victoria" });
    ht3.add({ "22.04.1996", "Timur" });
    ht3.add({ "23.04.1996", "Bekmurat" });
    ht3.add({ "09.10.1996", "Akhmed" });
    ht3.add({ "14.11.2004", "Alexandr" });
    ht3.add({ "03.07.2004", "Maria" });
    ht3.add({ "10.07.2002", "Olga" });
    ht3.add({ "31.11.2002", "Marina" });
    ht3.add({ "01.01.2000", "Musya" });
    ht3.add({ "18.08.1998", "Kesha" });
    ht3.add({ "25.12.1999", "Daria" });
    ht3.add({ "13.06.1999", "Sasha" });
    ht3.add({ "18.03.1992", "Gennadiy" });
    ht3.add({ "21.03.1987", "Slava" });
    ht3.add({ "09.11.2007", "Leonid" });
  //  for(int j = 30; j < 1000; j++)
  //      ht3.add({ "09.11.2007", "Leonid" });
    //ht3.output();
    Cell cell5;
    //clock_t start, end;
    // Тест чтения начального элемента
    start = clock();
    ht3.findElement("23.04.1996", cell5);
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
//     Тест чтения конечного элемента
    start = clock();
    ht3.findElement("09.10.1996", cell5);
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
 //    Тест чтения среднего элемента
    start = clock();
    ht3.findElement("27.02.2012", cell5);
    end = clock();
    printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    cin.get();
 //   end = clock();
 //   printf("The read method was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC)); // 0.0010 секунд
    return 0;
}
