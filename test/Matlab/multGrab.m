clear all; 
close all; 
clc;

WIDTH = 320;
HEIGHT = 240;

if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

s = serial("COM3", 'BaudRate', 2000000, 'Timeout', 30, 'Terminator', 'LF'); 
s.InputBufferSize = 76806;                                                 
fopen(s);                                                                   

for i = 1:6
    data = fread(s);
    
    img = data(6:end-1);                                                       
    img_r = reshape(img,[WIDTH,HEIGHT]);                                       
    img_r = flipud(img_r);
    
    img_f(:,:,i)=img_r; 
end

figure;
subplot(1,6,1), imshow(img_f(:,:,1)/255), title('1st Shot')
subplot(1,6,2), imshow(img_f(:,:,2)/255), title('2nd Shot')
subplot(1,6,3), imshow(img_f(:,:,3)/255), title('3dr Shot')
subplot(1,6,4), imshow(img_f(:,:,4)/255), title('4th Shot')
subplot(1,6,5), imshow(img_f(:,:,5)/255), title('5th Shot')
subplot(1,6,6), imshow(img_f(:,:,6)/255), title('6th Shot')

fclose(s); 