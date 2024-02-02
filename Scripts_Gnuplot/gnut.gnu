set term png
set output 'images/t.png'
set terminal pngcairo size 1400,1000

set style fill solid
set boxwidth 0.5
set ytics
set xtics
set xtics rotate by -320 right

set xlabel 'VILLES'
set ylabel 'NB TRJAETS'
set title 'VILLE NB DE FOIS PARCOURUES - option -t'

set datafile separator ';'
set yrange[0:*]

plot 'temp/data_t.txt' using ($2):xtic(sprintf("%s", strcol(1))) with boxes title 'Trajets' linecolor rgb 'red', '' using ($0+0.4):($3) with boxes linecolor rgb 'green' title 'Ville de départ'