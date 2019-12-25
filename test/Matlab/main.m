clear all; close all; clc;

%%
WIDTH = 320;
HEIGHT = 240;

%%

% changed fscanf which return a char type to fread which returns a 8bit data (a byte) 
% this is marked as not recommended, but ... it worked

s = serial("COM3", 'BaudRate', 1000000, 'Timeout', 10, 'Terminator', 'LF');
s.InputBufferSize = 76806;
fopen(s);
disp('Waiting for Data:')
data = fread(s);
disp('Sucssesfull')
fclose(s);

%%
img = data(6:end-1);
img_r = reshape(img, [WIDTH,HEIGHT]);



%%
% YUV 422

% U Y V Y 

% V Y U Y

mode=0;

for i = 1:WIDTH
    for j = 1:HEIGHT
        
        Y(i,j) = bin2dec(num2str([bitget(img_r(i,j), 6:-1:5,'uint8'), bitget(img_r(i,j), 2:-1:1,'uint8')]));
        
        if mode == 1
            
            U(i,j) = bin2dec(num2str(bitget(img_r(i,j), 8:-1:7,'uint8'))); 
            V(i,j) = bin2dec(num2str(bitget(img_r(i,j), 4:-1:3,'uint8')));
        else
            U(i,j) = bin2dec(num2str(bitget(img_r(i,j), 4:-1:3,'uint8'))); 
            V(i,j) = bin2dec(num2str(bitget(img_r(i,j), 8:-1:7,'uint8')));
        end
    end
end

        
%%

CONVERTER = [1      0      1.13983
               1   -0.39465  -0.58060 
               1    2.03211     0    ];
for row = 1:WIDTH
    for col = 1:HEIGHT
      YUV = [Y(row, col)
             U(row, col)
             V(row, col)];
      RGB(row,col,:) = CONVERTER * double(YUV);
    end
end

%%

im = (RGB(:,:,1) + RGB(:,:,2) + RGB(:,:,3));
GIM  = cast(im,'uint8');
Shot = imread('shoot.bmp');

% figure;
% subplot(2,2,1), imshow(img_r/255),  title('RAW')
% subplot(2,2,2), imshow(im/255), title('Pos RGB')
% subplot(2,2,3), imshow(GIM),    title('Pos RGB 8unit')
% subplot(2,2,4), imshow(Shot),   title('RUS')

%for some mystical reason the images are upside down, so...
im = flipud(im);
img_r = flipud(img_r);
GIM = flipud(GIM);

figure;
subplot(2,2,1), imshow(img_r/255),  title('RAW')
subplot(2,2,2), imshow(im/255), title('Pos RGB')
subplot(2,2,3), imshow(GIM),    title('Pos RGB 8unit')
subplot(2,2,4), imshow(Shot),   title('RUS')




  