clear all; close all; clc;

%%
WIDTH = 320;
HEIGHT = 240;

%%
% fileID = fopen('img_data.bin');
% A = fread(fileID, [76806,1], 'uint8');

if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

s = serial("COM3", 'BaudRate', 1000000, 'Timeout', 10, 'Terminator', 'LF');
s.InputBufferSize = 76806;
fopen(s);
disp('Waiting for Data:')
data = fscanf(s);
disp('Sucssesfull')
fclose(s)

A = unicode2native(data, 'US-ASCII');

%%
img = A(6:end-1);
img_r = reshape(img, [WIDTH,HEIGHT]);

%%
% YUV 422
% 
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
      %O(row, col, :) = uint8(YUV);
    end
end

%%
im = (RGB(:,:,1) + RGB(:,:,2) + RGB(:,:,3));
GIM  = cast(im,'uint8');
Shot = imread('shot.bmp');

figure;
subplot(2,2,1), imshow(im)
subplot(2,2,2), imshow(GIM)
subplot(2,2,3), imshow(img_r)
subplot(2,2,4), imshow(Shot)



%% Above methods VS Russian Shot
% im2=dlmread('im.txt',im);
% GIM2=dlmread('GIM.txt',GIM);
% img_r2=dlmread('img_r.txt',img_r);
% 
% figure;
% subplot(2,2,1), imshow(im2)
% subplot(2,2,2), imshow(GIM2)
% subplot(2,2,3), imshow(img_r2)
% subplot(2,2,4), imshow(Shot)












  