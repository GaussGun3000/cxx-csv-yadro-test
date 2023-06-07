# Тестовое задание по обработке CSV - файлов 
## Задание
Входные данные - CSV-файл, представляющий собой таблицу. Первая строка - буквенные наименования столбцов, первый столбец - числовые наименования строк. В ячейках содержится или целое число, или формула формата <ячейка1><оператор><ячейка2>.

Выходные данные - таблица из рациональных чисел (могут получится в результате деления)

## Сборка: 
Используется CMake. Можно использовать файлы конфигурации из репозитория - CMakeLists.txt и сгенерированные файлы CMake в cmake-build-debug. Нужен компилятор MinGW 6 (g++). Путь до компилятора указать в CMakeLists.txt (в строчке **set(CMAKE_CXX_COMPILER "D:/MinGW2/mingw64/bin/g++.exe")** заменить путь на нужный). Чтобы сгенерировать файлы CMake самостоятельно, можно использовать в консоли команду **cmake -G "MinGW Makefiles" .** в папке проекта. Файл CMakeLists.txt с путём к компилятору обязателен. После этого в папке cmake-build-debug (или при отдельной генерации - в папке с файлом *Makefile*) запустить команду **mingw32-make**. После этого можно запустить в билд-папке приложение командой **.\yadro_csv_test.exe [filename] [warnings]** где "filename" - путь к файлу, "warnings" (true/false) - вывод предупреждений в консоль (опционально, если не указывать - предупреждения выключены). Можно также запустить код с самодельными тестами из файла test.cpp (приложение test.exe будет собрано в той же папке).
