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

%chenged fscanf which return a char type to fread which returns a 8bit data (a byte) 

s = serial("COM3", 'BaudRate', 1000000, 'Timeout', 10, 'Terminator', 'LF');
s.InputBufferSize = 76806;
fopen(s);
disp('Waiting for Data:')
%data = fscanf(s); %original
data = fread(s);   %fread test
disp('Sucssesfull')
fclose(s);

%A = unicode2native(data, 'US-ASCII'); %original
A = data; %fread test

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
Shot = imread('shoo.bmp');

figure;
subplot(2,2,1), imshow(img_r/255),  title('RAW')
subplot(2,2,2), imshow(im/255), title('Pos RGB')
subplot(2,2,3), imshow(GIM),    title('Pos RGB 8unit')
subplot(2,2,4), imshow(Shot),   title('RUS')

%some mystical reason the images are upside down, so...
im = flipud(im);
img_r = flipud(img_r);
GIM = flipud(GIM);

figure;
subplot(2,2,1), imshow(img_r/255),  title('RAW')
subplot(2,2,2), imshow(im/255), title('Pos RGB')
subplot(2,2,3), imshow(GIM),    title('Pos RGB 8unit')
subplot(2,2,4), imshow(Shot),   title('RUS')


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












  