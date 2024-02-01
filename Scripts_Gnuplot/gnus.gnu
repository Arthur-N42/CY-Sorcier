set term png
set output 'images/s.png'
set terminal pngcairo size 1400,1000

set style fill solid
set boxwidth 0.5
set ytics
set xtics
set xtics rotate by -310 right

set xlabel 'ID TRAJET'
set ylabel 'DISTANCE'
set title 'DISTANCE (MAX - MIN) PAR TRAJETS - option -s'

set datafile separator ';'
set yrange[0:*]

plot 'temp/data_s.txt' using 0:2:3:xticlabels(1) linecolor rgb '0xFF000000' title '', 'temp/data_s.txt' using 0:3 with filledcurves x1 linecolor rgb '#9bbf84' title 'Distance Max/Min (Km)', 'temp/data_s.txt' using 0:2 with filledcurves x1 linecolor rgb 'white' title '', 'temp/data_s.txt' using 0:5 smooth csplines linecolor '#427043' linewidth 2 title 'Distance Moyenne (Km)'
