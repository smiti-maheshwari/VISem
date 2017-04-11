RGB=imread('/home/placements2017/VISem/IVP/LAB/Image.jpg');
subplot(221);
imshow(RGB);

subplot(222);
imshow(RGB*1.2);

I=rgb2gray(RGB);
I=im2double(I);
[m n] = size(I);

c = 1;
g =[0.5 0.7 0.9 1 2];
for r = 1:length(g)
    for p = 1 : m
        for q = 1 : n
            I3(p, q) = c * I(p, q).^ g(r);  
        end
    end
end

subplot(223);
imshow(I3);


g =[0.5 0.7 0.9 1 2 3 4 5 6];
for r=1:length(g)
    for p = 1 : m
        for q = 1 : n
            I3(p, q) = c * I(p, q).^ g(r);  
        end
    end
end
subplot(224);
imshow(I3);
