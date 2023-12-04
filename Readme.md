Тестировалось на Windows через Visual Studio, для параллелизации использовался PPL

Весь код в quicksort.cpp

`void quicksort_seq(int* a)` - послед.реализация

`void quicksort_par(int* a)` - паралл.реализация

`int main()` - тестирование скорости

Результат (в секундах, среднее по 5 запускам):
```
Sequential: 8.3456
Parallel: 2.6744
```
