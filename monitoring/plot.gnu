#! /usr/bin/gnuplot -persist

set title "Задержка отклика службы" font ",16"
set xlabel "Время"
set ylabel "Задержка, μs"

# Отображение

set boxwidth 0.7                    # толщина столбцов
set style fill solid border -1      # заливка столбцов

# Выставляем режим вывода в файл, формат PNG

set term png size 1000,400
set output "tmp/plot.png"           # указываем путь и имя файла
set grid                            # выводим грид

# Параметры по «X»

set auto x
set xdata time                      # выставляем, что данные по «X» это время
set timefmt "%Y-%m-%d %H:%M:%S"     # формат времени

# Параметры по «Y»

set yrange [0:]                     # начинаем от 0

# Рисуем график

#plot "tmp/plot.dat" using 1:4 with boxes

plot for [a in 'ERROR'] sprintf("<(grep -v '%s$' tmp/plot.dat)", a) u 1:4 w boxes title "no errors" lt rgb "#00BB00", \
     for [b in 'END'] sprintf("<(grep -v '%s$' tmp/plot.dat)", b) u 1:4 w boxes title "with errors" lt rgb "#AA0000"