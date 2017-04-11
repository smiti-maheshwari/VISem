function [x, freq] = drawhist(img)

freq = zeros(512);
x = zeros(512);

[w, h] = size(img);

for i = 1:w
    for j = 1:h
        freq(img(i, j)) = freq(img(i, j)) + 1;    
    end
end

for i = 1: 512
    x(i) = i;
end


%imhist(img);
