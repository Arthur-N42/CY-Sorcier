set term png
set output 'images/d1.png'
set terminal pngcairo size 700,600

set style fill solid
set boxwidth 0.5
set ytics
set xtics

set xlabel 'TRAJETS'
set ylabel 'NOM CONDUCTEUR'
set title 'NB TRAJETS/CONDUCTEUR - option -d1'

set datafile separator ';'
set yrange[0:*] reverse
unset key

set offsets 0,0,0.5,0.5
plot 'temp/data_d1.txt' using (0.5*$2):0:(0.5*$2):(0.5/2.):ytic(strcol(1)) with boxxy title '' linecolor rgb 'blue'