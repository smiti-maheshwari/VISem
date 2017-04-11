I = imread('eight.tif');

CC = bwconncomp(I,4)
CC = bwconncomp(I,8)