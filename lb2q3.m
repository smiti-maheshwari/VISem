img=imread('/home/placements2017/VISem/IVP/LAB/Image.jpg');

[rows,cols,height]= size(img); 

angle=180;

rads=pi*angle/180;  

rowsf=ceil(rows*abs(cos(rads))+cols*abs(sin(rads)));                      
colsf=ceil(rows*abs(sin(rads))+cols*abs(cos(rads)));                     

C=uint8(zeros([rowsf colsf 3 ]));

xo=ceil(rows/2);                                                            
yo=ceil(cols/2);

midx=ceil((size(C,1))/2);
midy=ceil((size(C,2))/2);

for i=1:size(C,1)
    for j=1:size(C,2)                                                       

         x= (i-midx)*cos(rads)+(j-midy)*sin(rads);                                       
         y= -(i-midx)*sin(rads)+(j-midy)*cos(rads);                             
         x=round(x)+xo;
         y=round(y)+yo;

         if (x>=1 && y>=1 && x<=size(img,1) &&  y<=size(img,2) ) 
              C(i,j,:)=img(x,y,:);  
         end

    end
end

imshow(C)