Fs = 44100;
t = 0:1/Fs:1;

x1 = sin(2*pi*116*t);
x2 = sin(2*pi*232*t);
x3 = sin(2*pi*347*t);

x = x1+x2+x3;

X1 = fft(x);
X2 = abs(X1);

subplot(3,1,1);
plot(x);
subplot(3,1,2);
plot(X2(1:5000));
subplot(3,1,3);
spectrogram(x);

sound = audioplayer(x, Fs);
play(sound);
