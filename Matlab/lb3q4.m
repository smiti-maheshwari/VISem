clc;
clear all;


I = imread('faceimages.jpg');


x1 = 106;
y1 = 83;

x2 = 180;
y2 = 79;

dist = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
disp (dist);