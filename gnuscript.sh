#!/usr/bin/gnuplot
set xlabel "P(k)"
set ylabel "values"
set xrange ["0":"15"]
set xtics 1
set yrange ["0":"35"]
set ytics 2
set grid
plot "degrees.dat" 
pause -1
#set terminal postscript portrait enhanced mono dashed lw 1 "Helvetica" 14
#set output "visualization.png"
exit 
