clear all
clc

w=[0 2 3 4 5 10 20 100 1000];
G1=(200./(100+w.^2))-20.*j*w./(100+w.^2)

M=20./sqrt(100+w.^2)
a=-(atand(w/10))
g=20*log(M)

%%

clear all
clc

w=[0 2 3 4 5 10 20 100 1000];

G2=(-25.*w.^2+25.^2)./((-w.^2+25).^2+16.*w.^2)-(100.*j.*w)./((-w.^2+25).^2+16.*w.^2)
M=25./sqrt((-w.^2+25).^2+(4.*w).^2)                         
a=-(atand(4.*w./(-w.^2+25)))
g=20*log(M)

%% 9.23¿« ∫∏µÂº±µµ, ∫§≈Õ±À¿˚, ¿ÃµÊ¿ßªÛµµ
clear all
clc

num=[0 20];
dem=[1 10];

subplot(1,3,1); bode(num,dem)
grid on
subplot(1,3,2); nyquist(num,dem)
grid on
subplot(1,3,3); nichols(num,dem)
grid on
%% 9.24¿« ∫∏µÂº±µµ, ∫§≈Õ±À¿˚, ¿ÃµÊ¿ßªÛµµ
clear all
clc

num=[0 0 25];
dem=[1 4 25];

subplot(1,3,1); bode(num,dem)
grid on

subplot(1,3,2); nyquist(num,dem)
grid on
subplot(1,3,3); nichols(num,dem)
grid on

%% 9.25¿« ∫∏µÂº±µµ, ∫§≈Õ±À¿˚, ¿ÃµÊ¿ßªÛµµ
clear all
clc

num=[0 0 25];
dem=[1 2 25];

subplot(1,3,1); bode(num,dem)

subplot(1,3,2); nyquist(num,dem)
subplot(1,3,3); nichols(num,dem)

