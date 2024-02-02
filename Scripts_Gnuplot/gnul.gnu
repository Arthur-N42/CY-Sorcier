set term png
set output 'images/l.png'
set terminal pngcairo size 1000,600

set style fill solid
set boxwidth 0.5
set ytics
set xtics

set ylabel 'DISTANCE'
set xlabel 'ID TRAJET'
set title 'DISTANCE/ID TRAJET - option -l'

set datafile separator ';'
set yrange[0:*]
unset key

plot 'temp/data_l.txt' using 0:($2):(0.5):xtic(strcol(1)) with boxes title '' linecolor rgb 'blue'