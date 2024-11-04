#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Функция для определения размера файла
size_t ByteOfFile(const string path) {
    ifstream file(path.c_str());
    size_t s = 0;
    while (!file.eof()) {
        file.get();
        s++;
    }
    file.close();
    s--;
    return s;
}

// Структура для хранения данных клиента
struct ClientData {
    string name;
    string age;
    string height;
    string weight;
};

// Функция для обработки данных клиента
ClientData processData(const ClientData& clientData) {
    ClientData processedData = clientData;
    // Здесь можно добавить логику обработки данных
    // Например, рассчитать BMI (но в этом случае вам придется
    // преобразовать строки в числа):
    // processedData.weight = processedData.weight / (processedData.height * processedData.height);
    return processedData;
}

// Функция клиента
void client(const string& inputFile, const string& outputFile) {
    ofstream outFile(inputFile);
    ifstream inFile(outputFile);

    // Ввод данных клиента
    ClientData clientData;
    cout << "Введите имя: ";
    cin >> clientData.name;
    cout << "Введите возраст: ";
    cin >> clientData.age;
    cout << "Введите рост (в метрах): ";
    cin >> clientData.height;
    cout << "Введите вес (в кг): ";
    cin >> clientData.weight;

    // Запись данных в файл
    outFile << clientData.name << endl;
    outFile << clientData.age << endl;
    outFile << clientData.height << endl;
    outFile << clientData.weight << endl;
    outFile.close(); // Закрыть файл, чтобы сервер мог его прочитать

    // Ждать изменений в файле outputFile
    size_t lastFileSize = ByteOfFile(outputFile);
    while (true) {
        size_t currentFileSize = ByteOfFile(outputFile);
        if (currentFileSize != lastFileSize) {
            // Файл изменен, читать данные
            ClientData processedData;
            getline(inFile, processedData.name);
            getline(inFile, processedData.age);
            getline(inFile, processedData.height);
            getline(inFile, processedData.weight);

            // Вывод результата
            cout << "Обработанные данные:" << endl;
            cout << "Имя: " << processedData.name << endl;
            cout << "Возраст: " << processedData.age << endl;
            cout << "Рост: " << processedData.height << endl;
            cout << "Вес: " << processedData.weight << endl;
            lastFileSize = currentFileSize;
        }
        // Пауза для избежания перегрузки процессора
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Функция сервера
void server(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    // Ждать изменений в файле inputFile
    size_t lastFileSize = ByteOfFile(inputFile);
    while (true) {
        size_t currentFileSize = ByteOfFile(inputFile);
        if (currentFileSize != lastFileSize) {
            // Файл изменен, читать данные
            ClientData clientData;
            getline(inFile, clientData.name);
            getline(inFile, clientData.age);
            getline(inFile, clientData.height);
            getline(inFile, clientData.weight);

            // Обработка данных
            ClientData processedData = processData(clientData);

            // Запись результата в файл
            outFile << processedData.name << endl;
            outFile << processedData.age << endl;
            outFile << processedData.height << endl;
            outFile << processedData.weight << endl;
            outFile.close(); // Закрыть файл, чтобы клиент мог его прочитать

            // Вывод результата в консоль
            cout << "Сервер получил данные:" << endl;
            cout << "Имя: " << processedData.name << endl;
            cout << "Возраст: " << processedData.age << endl;
            cout << "Рост: " << processedData.height << endl;
            cout << "Вес: " << processedData.weight << endl;
            lastFileSize = currentFileSize;
        }
        // Пауза для избежания перегрузки процессора
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    string inputFile = "C:\\input.txt";
    string outputFile = "C:\\output.txt";

    // Создание потоков для клиента и сервера
    thread clientThread(client, inputFile, outputFile);
    thread serverThread(server, inputFile, outputFile);

    // Ожидание завершения работы потоков
    clientThread.join();
    serverThread.join();

    return 0;
}
