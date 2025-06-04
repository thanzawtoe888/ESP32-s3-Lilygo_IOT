f0L = 941;
f0H = 1336;
fs = 16000;
N = 1600;
t = 0:1/fs:0.2;
t = t(1:N);
y = sin(2*pi*f0L*t) + sin(2*pi*f0H*t);
soundsc(y,fs);
plot(t,y)
f = fft(y);
sp = abs(f(1:N/2));
df = 0:fs/N:fs/2;
df = df(1:N/2);
figure
plot(df,sp)
[sp_amp, dtmf] = dtmf_detect(int32(10000*y), int32(fs), int32(N))