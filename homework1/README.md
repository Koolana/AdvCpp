# AdvCpp
# Homework 1
Нужно реализовать класс Process, согласно интерфейсу.
Класс представляет собой процесс операционной системы
Linux.

При создании экземпляра класса создается дочерний
процесс, при уничтожении переменной все ресурсы
освобождаются, процесс завершается.

Класс позволяет отправлять данные на стандартный поток
ввода процесса и читать данные с его стандартного
потока вывода.

Все исключительные ситуации должны быть корректно
описаны и передавать управление вызывающему коду
