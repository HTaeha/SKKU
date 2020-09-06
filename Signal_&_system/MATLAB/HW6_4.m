Fs = 100;
t = -4:1/Fs:4;
x1 = sin(2*10*pi*t);
x1(t<0) = 0;
x1(t>2) = 0;

h = -sin(2*10*pi*t);
h(t>0) = 0;
h(t<-2) = 0;

random = rand(1, 801);

x2 = x1 + random;
y = conv(x2, h);

subplot(4, 1, 1);
plot(t, random);
subplot(4, 1, 2);
plot(t, x2);
subplot(4, 1, 3);
plot(t, h);
subplot(4, 1, 4);
plot(t, y(4*Fs:12*Fs));
