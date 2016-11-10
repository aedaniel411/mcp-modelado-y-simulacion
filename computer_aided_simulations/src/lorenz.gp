#!/usr/bin/gnuplot

set title "Lorenz Atractor \n Animated 3d"

do for [ii=1:50000:10] { 
    splot 'lor.dat' every ::1::ii w l ls 1 title 'Historico', \
    'lor.dat' every ::ii::ii w p ls 1 title 'Punto' 
}
