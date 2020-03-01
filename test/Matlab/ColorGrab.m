clear all; 
close all; 
clc;

y1=zeros(1,38400);
v0=zeros(1,38400);
y2=zeros(1,38400);
u0=zeros(1,38400);

Y=zeros(1,76800);
U=zeros(1,76800);
V=zeros(1,76800);

WIDTH = 320;
HEIGHT = 240;

if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end
% prompt = 'Select COM Port: ';
% port = input(prompt,'s');
% s = serial(port, 'BaudRate', 2000000, 'Timeout', 30, 'Terminator', 'LF');

s = serial('COM3', 'BaudRate', 2000000, 'Timeout', 30, 'Terminator', 'LF');

s.InputBufferSize = 153606;                                                 
fopen(s);

for t = 1:6
      
%     data = fread(s);
%     img = data(6:end-1);

    img = fread(s);

    k=1;
    for i = 1:4:153600
        y1(k)=img(i);
        u0(k)=img(i+1);
        y2(k)=img(i+2);
        v0(k)=img(i+3);
        k=k+1;
    end

    o=1;
    for p=1:38400
        Y(o)  =y1(p);
        Y(o+1)=y2(p);

        U(o)  =u0(p);
        U(o+1)=u0(p);

        V(o)  =v0(p);
        V(o+1)=v0(p);
        
        o=o+2;
    end

    B(:) = 1.164*( Y(:) - 16)+ 2.018*(U(:)-128);

    G(:) = 1.164*( Y(:)-16)-0.813*(V(:)-128)-0.391*(U(:)-128);

    R(:) = 1.164*( Y(:)-16)+ 1.596*(V(:)-128);

    Y_r=reshape(Y,[WIDTH,HEIGHT]);
    R_r=reshape(R,[WIDTH,HEIGHT]);
    G_r=reshape(G,[WIDTH,HEIGHT]);
    B_r=reshape(B,[WIDTH,HEIGHT]);

    img_r(:,:,1) = R_r;
    img_r(:,:,2) = G_r;
    img_r(:,:,3) = B_r;


    figure(t);
    subplot(1,2,1);
    imshow(img_r/256);
    subplot(1,2,2);
    imshow(Y_r/256);
end

fclose(s); 