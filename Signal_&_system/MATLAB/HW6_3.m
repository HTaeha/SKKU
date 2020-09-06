Fs = 100;
t = -4:1/Fs:4;
x1 = sin(2*10*pi*t);
x1(t<0) = 0;
x1(t>2) = 0;

h = -sin(2*10*pi*t);
h(t>0) = 0;
h(t<-2) = 0;

y = conv(x1, h);

subplot(3,1,1);
plot(abs(fft(x1)));
subplot(3,1,2);
plot(abs(fft(h)));
subplot(3,1,3);
plot(abs(fft(y(Fs*4:Fs*12))));

