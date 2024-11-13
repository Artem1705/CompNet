#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
using namespace std;

int main()
{
    ofstream request;
    ifstream response;
    int size_1 = 0, size_2 = 0;
    string str = "", name = "";
    int weight, tall;

    cout << "What's your name: ";
    cin >> name;

    while (true)
    {
        cout << "Vvedite ves: ";
        cin >>weight;
        cout << "Vvedite rost: ";
        cin >> tall;


        // Записываем имя и массив оценок в файл-запрос
        request.open("C:/input.txt", ios::app);
        request << name << endl;
        request << weight << endl;
        request << tall << endl;
        request << endl;
        request.close();

        // Ожидание ответа от сервера с проверкой новых записей
        this_thread::sleep_for(chrono::milliseconds(2000));
        bool flag = false;

        while (!flag)
        {
            response.open("C:/output.txt");
            response.seekg(0, ios::end);
            size_2 = response.tellg();

            if (size_1 != size_2)
            {
                string mes;
                response.seekg(size_1, ios::beg);
                // Читаем все новые строки до конца файла
                while (getline(response, str)) {
                    mes = str;
                };
                cout << mes << endl;
                size_1 = size_2;
                flag = true;  // Новая запись найдена и выведена
            }
            response.close();
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
}
