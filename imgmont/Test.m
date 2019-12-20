clear all; close all; clc;

WIDTH = 320;
HEIGHT = 240;

img_r2=dlmread('img_r.txt');
im2=dlmread('im.txt');
GIM2=dlmread('GIM.txt');
Shot = imread('shot.bmp');

img_r2  = cast(img_r2,'uint8');

GIM2  = cast(GIM2,'uint8');

for i = 1:WIDTH
    for j = 1:HEIGHT
%      U Y V Y
       UYVY(i,j) = bin2dec(num2str([bitget(img_r2(i,j), 6:-1:5,'uint8'),bitget(img_r2(i,j), 2:-1:1,'uint8'),bitget(img_r2(i,j), 8:-1:7,'uint8'),bitget(img_r2(i,j), 4:-1:3,'uint8')]));

%      V Y U Y
       VYUY(i,j) = bin2dec(num2str([bitget(img_r2(i,j), 6:-1:5,'uint8'),bitget(img_r2(i,j), 2:-1:1,'uint8'),bitget(img_r2(i,j), 4:-1:3,'uint8'),bitget(img_r2(i,j), 8:-1:7,'uint8')]));

%      Y U Y V                         
       YUYV(i,j) = bin2dec(num2str([bitget(img_r2(i,j), 8:-1:7,'uint8'),bitget(img_r2(i,j), 4:-1:3,'uint8'),bitget(img_r2(i,j), 6:-1:5,'uint8'),bitget(img_r2(i,j), 2:-1:1,'uint8')]));

%      Y V Y U                         
       YVYU(i,j) = bin2dec(num2str([bitget(img_r2(i,j), 8:-1:7,'uint8'),bitget(img_r2(i,j), 4:-1:3,'uint8'),bitget(img_r2(i,j), 2:-1:1,'uint8'),bitget(img_r2(i,j), 6:-1:5,'uint8')]));

    end
end

UYVY  = cast(UYVY,'uint8');
VYUY  = cast(VYUY,'uint8');
YUYV  = cast(YUYV,'uint8');
YVYU  = cast(YVYU,'uint8');

figure;
subplot(2,2,1), imshow(UYVY), title('UYVY')
subplot(2,2,2), imshow(VYUY), title('VYUY')
subplot(2,2,3), imshow(YUYV), title('YUYV')
subplot(2,2,4), imshow(YVYU), title('YVYU')
 
figure;
subplot(2,2,1), imshow(img_r2),  title('RAW')
subplot(2,2,2), imshow(im2/255), title('Pos RGB')
subplot(2,2,3), imshow(GIM2),    title('Pos RGB 8unit')
subplot(2,2,4), imshow(Shot),    title('RUS')
