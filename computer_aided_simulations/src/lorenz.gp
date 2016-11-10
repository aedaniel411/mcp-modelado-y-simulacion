#!/usr/bin/gnuplot

do for [ii=1:50000:10] { splot 'lor.dat' every ::1::ii w l ls 1, 'lor.dat' every ::ii::ii w p ls 1 }


