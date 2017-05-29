A = [-b/J   K/J
    -K/L   -R/L];
B = [0
    1/L];
C = [1   0];
D = 0;
motor_ss = ss(A,B,C,D)

rank(ctrb(A,B))

Ts = 1/100;
motor_ss_d = c2d(motor_ss,Ts,'zoh');

sys_cl = feedback(motor_ss_d,1);
[y,t] = step(sys_cl,3);
stairs(t,y);
xlabel('Time (s)')
ylabel('Velocity (rad/s)')
title('Stairstep Response: Original')
hold on

Kp = 10;
Ki = 0;
Kd = 0;

C = pid(Kp,Ki,Kd);
dC = c2d(C,Ts,'tustin');

sys_cl = feedback(dC*motor_ss_d,1);
[x2,t] = step(sys_cl,12);
stairs(t,x2)
xlabel('Time (seconds)')
ylabel('Velocity (rad/s)')
title('Stairstep Response: with PID controller')
