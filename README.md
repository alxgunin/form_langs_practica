Реализация задачи №10 из практической работы. Строим НКА, удаляем Eps-переходы, состовляем список вершин, от которых можно дойти до терминальной по слову x^k(если список пуст, то ответ INF). Далее из всех таких вершин запускаем алгоритм BFS, чтобы найти ближайшую стартовую вершину(обозначим ее длину как l). Тогда ответом будет l + k. 

Данная задача покрыта unit-тестами, покрытие кода 95%(total coverage дополнительно считает еще и строки из библиотек для тестирования).