#!/bin/sh
for d in 2 ; do
        for n in 150 200 250 300 450 500 550 600 1000; do
		qrsh -nostdin -N split$d$n  GETAROOT=/home/ko/local/geta3 ./split-fusion NTCIR $d 20 $n >& /home/ko/getasearch/log/log$n$d.txt &
        done
done

