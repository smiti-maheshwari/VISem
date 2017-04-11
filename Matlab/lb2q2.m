X=imread('/home/placements2017/VISem/IVP/LAB/Image.jpg');

R = X;
R(:,:,2:3) = 0;
subplot(321);
imshow(R);
title('Pure Red');

G = X;
G(:,:,[1 3]) = 0;
subplot(322);
imshow(G);
title('Pure Green');

B = X;
B(:,:,1:2) = 0;
subplot(323);
imshow(B);
title('Pure Blue');

rb=X;
rb(:,:,3)=0;
subplot(324);
imshow(rb);
title('Mix Red and Blue');

rg=X;
rg(:,:,2)=0;
subplot(325);
imshow(rg);
title('Mix Red and Green');

gb=X;
gb(:,:,1)=0;
subplot(326);
imshow(gb);
title('Mix green and blue');