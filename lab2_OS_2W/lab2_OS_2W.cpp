#include <iostream>
#include <Windows.h>
#include <string>

bool FileMapping() {
    std::string fileName, mapName, data;
    HANDLE file, mapFile;
    LPVOID fileMappingAddress;

    std::cout << "Введите путь к файлу: ";
    std::getline(std::cin, fileName);

    file = CreateFile(fileName.data(), GENERIC_WRITE | GENERIC_READ, NULL, nullptr, CREATE_ALWAYS, NULL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка при создании файла, код: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Введите название отображения: ";
    std::getline(std::cin, mapName);

    mapFile = CreateFileMapping(file, nullptr, PAGE_READWRITE, NULL, 128, mapName.data());
    if (!mapFile) {
        std::cout << "Ошибка про создании отображения, код: " << GetLastError();
        CloseHandle(file);
        return false;
    }

    fileMappingAddress = MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, NULL, NULL, NULL);
    if (!fileMappingAddress) {
        std::cout << "Ошибка отображения файла, код: " << GetLastError();
        CloseHandle(file);
        CloseHandle(mapFile);
        return false;
    }

    std::cout << "Файл успешно спроецирован" << std::endl
        << "Введите данные: ";
    std::getline(std::cin, data);
    memcpy(fileMappingAddress, data.c_str(), data.size());
    std::cout << "Данные успешно записаны" << std::endl;
    return true;
}


int main()
{
    bool result;
    system("chcp 1251");
    system("cls");
    result = FileMapping();
    if (result) {
        std::cout << "Теперь можно использовать программу-читатель.\nВНИМАНИЕ! Если вы закроете текущую программу, то программа-читатель не будет работать." << std::endl
            << "Если вы продолжите, программа закроется, убедитесь, что вы закончили работу с программой-читателем";
        system("pause");
    }
}

