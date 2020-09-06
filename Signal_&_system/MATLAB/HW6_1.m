Fs = 100;
t = -4:1/Fs:4;
x1 = sin(2*10*pi*t);
x1(t<0) = 0;
x1(t>2) = 0;
plot(t, x1);
