Fc = 10;
n = 1;

% Digital Version of LPF
Fs = 44100;
Wn = 2*pi*Fc/Fs;
[zd,pd,kd] = butter(n,Wn);
[bd,ad] = zp2tf(zd,pd,kd);
[hd,wd] = freqz(bd,ad,Fs,Fs);
plot(wd/pi,20*log10(abs(hd)));
axis([0 40 -20 5])

tf(bd,ad,1/Fs)

% Analog Version of LPF
% [zb,pb,kb] = butter(n,2*pi*Fc,'s');
% [bb,ab] = zp2tf(zb,pb,kb);
% [hb,wb] = freqs(bb,ab,10000);
% plot(wb/(2*pi),20*log(abs(hb)))
% axis([0 20 -40 5])