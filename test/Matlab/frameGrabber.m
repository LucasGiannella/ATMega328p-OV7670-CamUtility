%% Cleaning Phase
clear all; 
close all; 
clc;

%% Image Dimensions
WIDTH = 320;
HEIGHT = 240;

%% Serial Port Communication

s = serial("COM4", 'BaudRate', 2000000, 'Timeout', 30, 'Terminator', 'LF'); % setting port and port parameters (see function reference)
s.InputBufferSize = 76806;                                                  % Number o bytes read from serial -> *RDY* Img \n -> 5 bytes 76800 bytes 1 byte
fopen(s);                                                                   % Opening serial port
disp('Waiting for Data:')                                                   % Console warning to be removed
tic;
data = fread(s);                                                            % Reading data as a byte stream, serial.read() DOESN'T work for this
disp('Sucssesfull')                                                         % Data read confirmation to be removed
fclose(s);                                                                  % Closing serial port

%% Image treatment

img = data(6:end-1);                                                        % Removing starter and terminator
img_r = reshape(img, [WIDTH,HEIGHT]);                                       % Reshape to Width x Height format
img_r = flipud(img_r);                                                      % Fliping image to the right orientation (see optics book)

%% Displaying image
figure;
imshow(img_r/255);                                                          % Normalizing the pixel value -> double images have values between 0 and 1
toc;