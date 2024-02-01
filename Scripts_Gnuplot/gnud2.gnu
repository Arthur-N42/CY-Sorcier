set term png
set output 'images/d2.png'
set terminal pngcairo size 1000,600

set style fill solid
set boxwidth 0.5
set ytics
set xtics

set xlabel 'DISTANCE'
set ylabel 'NOM CONDUCTEUR'
set title 'DISTANCE/CONDUCTEUR - option -d2'

set datafile separator ';'
set yrange[0:*] reverse
unset key

set offsets 0,0,0.5,0.5
plot 'temp/data_d2.txt' using (0.5*$2):0:(0.5*$2):(0.5/2.):ytic(strcol(1)) with boxxy title '' linecolor rgb 'blue'