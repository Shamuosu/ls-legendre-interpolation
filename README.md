# ls-legendre-interpolation
Интерполяция методом наименьших квадратов на основе полиномов Лежандра.

Программа интерполирует значение в заданном промежутке.

Для корректной работы программы требуется наличие в этой же папке файла **input.txt**, его достаточно создать вручную.
В самом файле должны содержатся пары точек в формате "**x y**", если в файле будут пары точек с одинаковыми **x** и разными **y**, то программа будет предварительно завершена.

После старта программы вам потребуется ввести в консоли максимальную степень полинома. Далее программа запросит **x**, для которого требуется вычислить **y**, и проверит его на нахождение в допустимом диапазоне. Сам диапазон определяется от наименьшего до наибольшего значения **x** в файле **input.txt**.
