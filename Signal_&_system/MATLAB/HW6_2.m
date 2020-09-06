Fs = 100;
t = -4:1/Fs:4;

h = -sin(2*10*pi*t);
h(t>0) = 0;
h(t<-2) = 0;
plot(t, h);
