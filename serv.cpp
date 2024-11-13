#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

using namespace std;

int main()
{
    ifstream request;
    ofstream response;
    int size_1 = 0, size_2 = 0, weight, tall;
    string name;

    request.open("C:/input.txt");
    request.seekg(0, ios::end);
    size_2 = request.tellg();
    size_1 = size_2;
    request.close();
    cout << "Server started" << endl;
    while (true)
    {
        request.open("C:/input.txt");
        request.seekg(0, ios::end);
        size_2 = request.tellg();

        if (size_1 != size_2)
        {
            request.seekg(size_1, ios::beg);
            getline(request, name);
            cout << "working with : " << name << endl;// Получаем имя клиента
            request >> weight;
            request >> tall;
            bool hasLowGrade = false;
            cout << endl;
            if (weight >= tall / 3)
                hasLowGrade = true;
            response.open("C:/output.txt", ios::app);
            cout << "end" << endl;
            if (hasLowGrade) {
                response << "Norm weight" << endl;
            }
            else {
                response << "No norm weight" << endl;
            }
            response.close();

            size_1 = size_2;
        }

        request.close();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
