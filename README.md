# MLAT
— технология, позволяющая определять местоположение самолётов или иных воздушных судов (ВС) без использования дополнительного к имеющемуся оборудованию на ВС.
## Зависимости
Для С++ присутствует единственная зависимость GTest - фрэймворк для юнит-тестов. В случае отсутствия данной зависимости библиотеки и тестовое приложение соберется, а unit-тесты будут пропущены. Для Python, который используется для визуализации данных требуется matplotlib. 
## Сборка
```bash
sudo apt-get install libgmock-dev
sudo apt-get install libgtest-dev
pip3 install matplotlib
mkdir build
cd build
> conan_paths.cmake
cmake ..
```
При сборке под windows, необходимо установить менеджер зависимостей conan версии 1.56.0
```
pip3 install matplotlib
mkdir build && cd build
conan install ..
cmake ..
```
## Запуск
Для запуска необходимо запустить исполняемый файл simulator и выполнить run-plotter.sh, которому при необходимости выдать права на исполнение с помощью команды 
```bash
chmod +x run-plotter.sh
```
При запуске под windows необходимо запустить simulator.exe и выполнить run-plotter.cmd

## Описание модели.
Пусть самолет имеет координаты (*x, y, z*), а *i*-aя вышка имеет координаты (*x<sub>i</sub>, y<sub>i</sub>, z<sub>i</sub>*). Всего пусть задано *n > 3* вышек, тогда есть 

$$ 
  C_n^k = \frac{n(n-1)}{2} 
$$

способов выбрать две из них. Обозначим, расстояние от самолета до *i*-ой вышки как *d<sub>i</sub>*. Расстояние от судна до *j*-ой вышки, соответственно, как 
*d<sub>j</sub>*. Соответсвующие расстояния выражаются по формулам

$$
d_i = \sqrt{(x-x_i)^2 + (y-y_i)^2 + (z-z_i)^2}, \\ 
d_j = \sqrt{(x-x_j)^2 + (y-y_j)^2 + (z-z_j)^2}.\\
$$

Зададим функцию *d*<sub>*ij*</sub>(*x, y, z*), как расстояние между вышками *i* и *j*, тогда

$$
d_{ij}(x,y,z) = |d_i - d_j| = \left | \sqrt{(x-x_i)^2 + (y-y_i)^2 + (z-z_i)^2} - \sqrt{(x-x_j)^2 + (y-y_j)^2 + (z-z_j)^2} \right |.
$$

С другой стороны, нетрудно получить, что

$$
d_i = ct_i,
$$

где *t<sub>i<sub>* является временем, за которое сигнал доходит от самолета, до вышки, тогда обозначим 

$$
  d_{ij} = c|t_i - t_j| = c\Delta t_{ij}, 
$$

где *c* - скорость распространения сигнала, в нашем случае это скорость света, а разница времен есть ни что иное, как Time Difference of Arrival (TDOA), эта информация является известной нам, так как её мы получаем с вышек.
## Постановка задачи
Используя, описанную модель вычислить координаты самолета при известных Time Of Arrival (TOA).
## Решение
Первоочередно перейдем от TOA, к TDOA. Далее, ясно, что

$$
d_{ij}(x,y,z) = d_{ij} \Leftrightarrow f_{ij}(x,y,z) \equiv d_{ij}(x,y,z) - d_{ij} = 0,
$$

перебрав все возможные комбинации *i* и *j*, получим систему функций *f(x, y, z) = (f<sub>11</sub>, ..., f<sub>n(n-1)</sub>)*. Нетрудно догадаться, что задача состоит в поиске таких параметров *(x, y, z)*, что *f(x, y, z) = 0*. Построенная система уравнений является переопределенной. Она содержит всего 3 неизвестных, а вышек, как минимум 4, то есть минимум 6 уравнений. 

Применим метод наименьших квадратов (МНК):
  
$$
S(x,y,z) = ||f(x,y,z)||_2^2 = ∑_{i,j} f_{ij}^2(x,y,z).
$$

Будем искать такие решения, которые будут минимизировать написанную сумму, для этого применим алгоритм Гаусса-Ньютона, обозначив за *x* вектор решения, получим:

$$
x^{(k+1)} = x^{(k)} - (J_f^TJ_f)^{-1}J_f^T \cdot x^{(k)},
$$

где *J<sub>f<sub>* - матрица Якоби для системы *f*. А произведение матриц в вычитаемом есть псевдообратная матрица к матрице Якоби, которую можно вычислить с помощью QR разложения.

![image](https://user-images.githubusercontent.com/48962685/236623043-0d819765-4db5-4434-837c-a63fc0a9261c.png)

## Литература
1. [Вычисление сингулярного разложения матриц. Афанасьева А. А.](https://core.ac.uk/download/pdf/336869551.pdf)
2. [Gauss–Newton algorithm, wikipedia.](https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm)
3. [Moore–Penrose inverse, wikipedia, chapter Construction.](https://en.wikipedia.org/wiki/Moore%E2%80%93Penrose_inverse)
4. [SVD, wikipedia.](https://en.wikipedia.org/wiki/Singular_value_decomposition)
5. [A New Approach to Linear Filtering and Prediction Problems](http://www.cs.unc.edu/~welch/kalman/media/pdf/Kalman1960.pdf)
6. [Построение параметров траектории с использованием фильтра Калмана с шагом коррекции по всем измерениям в РЛС дальнего обнаружения. Бородавкин Л. В.](https://www.researchgate.net/publication/313343865_POSTROENIE_PARAMETROV_TRAEKTORII_S_ISPOLZOVANIEM_FILTRA_KALMANA_S_SAGOM_KORREKCII_PO_VSEM_IZMERENIAM_V_RLS_DALNEGO_OBNARUZENIA_THE_KALMAN_FILTER_TRACK_PARAMETER_ESTIMATION_WITH_UPDATE_STEP_BY_ALL_ACCU)
