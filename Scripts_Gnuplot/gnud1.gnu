set term png
set output 'images/d1.png'
set xtics rotate
set title 'NB ROUTES option -d1'
set ylabel 'NOM CONDUCTEUR'
set style fill solid
set boxwidth 0.5
set datafile separator ';'
set yrange[0:*]
plot 'temp/data_d1.txt' using 0:2:xtic(1) with boxes title '' linecolor rgb 'blue'