%values
R_a=11
L_a=0.1
J=0.000028
B=0.1
L=0.1
R=0.02
kt=0.18
kb=0.18
k_pwm=30
%changable
z1=1
z2=5
z3=5.5
g1=7.5
g2=5
g3=9
k_theta1=0.5
k_theta2=1
k_theta3=2
s = tf('s');
K_innerloop1=((g1*(s+z1))/s)*(100/(s+100))^2
K_innerloop2=((g2*(s+z2))/s)*(100/(s+100))^2
K_innerloop3=((g3*(s+z3))/s)*(100/(s+100))^2

P_innerloop=3300/[(s+1.02)*(s+5000)]

G_innerloop1=P_innerloop*K_innerloop1
G_innerloop2=P_innerloop*K_innerloop2
G_innerloop3=P_innerloop*K_innerloop3

H_innerloop1=G_innerloop1/(G_innerloop1+1)
H_innerloop2=G_innerloop2/(G_innerloop2+1)
H_innerloop3=G_innerloop3/(G_innerloop3+1)

P_theta=24570/[(s+4995)*(s+4.957)*s]
G_theta1=P_theta*k_theta1
G_theta2=P_theta*k_theta2
G_theta3=P_theta*k_theta3

H_theta1=G_theta1/(G_theta1+1)
H_theta2=G_theta2/(G_theta2+1)
H_theta3=G_theta3/(G_theta3+1)


