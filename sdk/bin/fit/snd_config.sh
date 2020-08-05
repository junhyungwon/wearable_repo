#!/bin/sh

amixer cset numid=32 off
amixer cset numid=90 off
amixer cset numid=83 off
amixer cset numid=84 off
amixer cset numid=86 off
amixer cset numid=92 off
amixer cset numid=89 off
amixer cset numid=87 on
amixer cset numid=93 on

amixer cset numid=97 0
amixer cset numid=94 0
amixer cset numid=32 on
amixer cset numid=31 80%

# playback
amixer cset numid=65 on # DAC_L1 is HPLOUT
amixer cset numid=59 on # DAC_L1 is HPROUT
#amixer cset numid=101 2
amixer cset numid=101 0
#amixer cset numid=99 2
amixer cset numid=99 1
amixer cset numid=34 5
amixer cset numid=35 2
amixer cset numid=22 off
amixer cset numid=1 90%
