clc ;
clear all ;
close all ;
m=0;
k1 = 10;
n= 0 ;
xn = zeros(4,10000) ;
yn = zeros(4,10000);
Tnew = zeros(4,10000) ;
thetan = zeros(4,10000) ;
v = zeros(4,10000) ;
w = zeros(4,10000) ;
for K=0.1:0.2:0.7
m= m+1 ;
e1 = 0;
l = 0;
e2 = 0;
eo1 = 0;
eo2 = 0;
xf = 0;
yf = 0;
E = [e1 ; e2] ;
U = [0 ; 0] ;
theta1 = 0;
xp = 0;
yp = 0 ;
thetap = 0 ;
setheta = 0;
edisp = 0 ;
xt = 100 ;
yt = 100 ;
kp = .5 ;
ki = 0 ;
DelT = 0.01 ;
c = 5 ;
d = 5 ;
n = 0 ;
lastw = 0;
while(n<10000)
n=n+1;
Tnew(m,n+1) = DelT*n ;
E(1)= xt - xn(m,n) ;
E(2) = yt - yn(m,n) ;
thetar = atan2(E(2),E(1)) ;
d = sqrt(E(1)^2 + E(2)^2);
etheta = thetar - thetan(m,n) ;
edisp = sqrt(E(1)^2 + E(2)^2)*cos(etheta) ;
v(m,n+1) = K*edisp ;
w(m,n+1) = kp*etheta ;
if (d==0)
w(m,n+1)= 0 ;
v(m,n+1) = 0 ;
end
Tnew(m,n+1) = DelT*n ;
thetan(m,n+1) = w(m,n+1)*DelT + thetap ;
xndot = (v(m,n+1)*cos(thetan(m,n)));
yndot = (v(m,n+1)*sin(thetan(m,n)));
xn(m,n+1) = xndot*DelT + xp ;
yn(m,n+1) = yndot*DelT + yp ;
xp = xn(m,n+1) ;
yp = yn(m,n+1) ;
thetap= thetan(m,n+1) ;
end
end