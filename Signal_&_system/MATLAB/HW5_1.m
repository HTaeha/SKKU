[x, Fs] = audioread('오.m4a');
x1 = fft(x, Fs);
x2 = abs(x1);

subplot(3,1,1);
plot(x);
subplot(3,1,2);
plot(x2(1:400));
subplot(3,1,3);
spectrogram(x);

