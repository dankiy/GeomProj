# GeomProj

## Функционал

### Ввод

+ Строка, задающая функцию в явном или неявном (для кривых не выше второго порядка) виде
+ Проверка корректности вводимых значений 

### Вывод

+ Уравнение в каноническом виде
+ Инварианты
+ Тип кривой или поверхности
+ История

### Сцена

+ 2D
  + Построение графика функции
  + Приближение

+ ?3D
  + Построение графика поверхности или кривой
  + Приближение 
  + Поворот 
  
### История

+ Сохранение предыдущих запросов
+ Удаление предыдущих запросов

### Backend

+ Преобразование строки в АСД (абстрактное синтаксическое дерево) по алгоритму сортировочной станции
+ Приведение уравнения к каноническому виду
+ Инварианты


## Алгоритм

### 1. Пользователь вводит строку

### 2. На BE строка преобразутеся в АСД

### 3. BE возвращает FE:

  + Область определения
  + Размер сцены по-умолчанию > особенно необходимо для периодических функций
  + Тип кривой > для нас пока 1-го или 2-го 
  + Необходимую точность вычисления > актуально для сложных функций
  
### 4. FE:

  + Считает количество пикселей единичного отрезка для данного размера сцены
  + Отрисовывает координатные оси и разметку с учетом размера сцены
  + Вызывает цикл отрисовки на области определения 
    + Число вызовов зависит от типа кривой
    + Шаг вычисляется в зависимости от размера сцены и необходимой точности
    + На каждом шаге цикла вызывается функция на BE, которой в качестве аргумента передается значение x
  
### 5. При изменении размера сцены пункт 4 повторяется

##
