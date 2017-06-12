const int Aphase =5 ;
const int Aenable =6;
const int Bphase = 9 ;
const int Benable =10 ;
const int EncoderPinA1 =2;
const int EncoderPinB1 =3;
const float posloop_ST = 100 ;
const float speed_ST = 100 ;
unsigned long lastmilli_l = 0;
unsigned long  lastmilli_r = 0;
unsigned long Lmilli_position = 0;
unsigned long Lmilli_speed = 0;
unsigned long PW_r = 10000 ;
unsigned long PW_l = 10000 ;

float d = 0 ;
float x = 0 ;
float y = 0 ;
float xt = 100 ;
float yt= 100 ;
float theta = 0 ;
float thetat = 0 ;
float tick_l = 0;
float tick_r = 0;
float vx = 0 ;
float vy = 0 ;
float velo = 0 ;
float wrobot = 0 ;
float omega = 0 ;
float speed_actl= 0;
float speed_actr = 0;
float speed_reql= 0 ;
float speed_reqr= 0 ;
float PWM_valr = 0;
float PWM_vall = 0;
float errory = 0 ;
float thetaprev = 0 ;
float yprevious = 0 ;
float errorx = 0 ;
float xprevious = 0 ;
float errortheta = 0 ;
volatile long countr = 0;
volatile long countl =0;
float Kpl = 50 ;
float Kil = 5 ;
float Kpr = 50 ;
float Kir = 5 ;
float Kpo = .4 ;
float Ktheta = 0.7 ;
float Kitheta = 0.001 ;
float error_r=0;
float last_error_r=0;
float PID_T_r = 0;
float lastPID_T_r = 0;
float error_l=0;
float last_error_l=0;
float PID_T_l = 0;
float lasterrortheta = 0 ;
float lastPID_T_l = 0;
float lastomega = 0 ;
float L = 9.9 ;
float R = 1.9 ;
float Dr = 0;
float Dl = 0;
float Dc = 0;
void setup() {
// initialize the controller
	pinMode(Aenable, OUTPUT);
	pinMode(Aphase, OUTPUT);
	pinMode(Benable, OUTPUT);
	pinMode(Bphase, OUTPUT);
	pinMode(EncoderPinA1, INPUT);
	pinMode(EncoderPinB1, INPUT);
	digitalWrite(EncoderPinA1, HIGH);
	digitalWrite(EncoderPinB1, HIGH);
	attachInterrupt(0, rencoder, RISING);
	attachInterrupt(1, rencoder1, RISING);
	digitalWrite(Aphase, LOW);
	digitalWrite(Bphase, LOW);
	digitalWrite(Benable, LOW);
	digitalWrite(Benable, LOW);
}
void loop() {
	if(millis()-Lmilli_position>posloop_ST)
	{
		Lmilli_position = millis() ;
		vx = updatevx( xt, x) ;
		vy = updatevy( yt,y) ;
	}
	if(millis()-Lmilli_speed > speed_ST)
	{
		Lmilli_speed = millis() ;
	if(atan2(yt,xt)>0)
		{
			thetat = abs(atan2(vy,vx)) ;
		}
	else
			{ 
				thetat = -1*abs(atan2(vy,vx));
			}
	d = sqrt((xt-x)*(xt-x) + (yt-y)*(yt-y)) ;
	velo = sqrt(vy*vy + vx*vx) ;
	wrobot = update_w(thetat,theta) ;
	if(velo>10)
				{ velo = 10 ;
				}
	if (d<4)
				{
					velo = 0 ;
					wrobot = 0 ;
				}
	speed_reqr = (2*velo + wrobot*L)/(2*R*2*3.14) ;
	speed_reql = (2*velo - wrobot*L)/(2*R*2*3.14) ;
	if(speed_reqr<0)
					{
						speed_reqr = 0 ;
					}
	if(speed_reql<0)
						{speed_reql = 0 ;
						}
						PWM_vall= updatePidl(PWM_vall, speed_reql, countl);
						PWM_valr= PWM_vall ;
	if(PWM_vall<0)
						{
							digitalWrite(Bphase,LOW);
							analogWrite(Benable,PWM_vall);
						}
	else
						{
							digitalWrite(Benable,LOW);
							analogWrite(Bphase,PWM_vall);
						}
	PWM_valr= updatePidr(PWM_valr, speed_reqr, countr);
	if(PWM_valr<0)
						{
							digitalWrite(Aphase,LOW);
							analogWrite(Aenable,PWM_valr);
						}
	else
						{
							digitalWrite(Aenable,LOW);
							analogWrite(Aphase,PWM_valr);
						}
	Dr = 2*3.14*R*countr/20 ;
	Dl = 2*3.14*R*countl/20 ;
	Dc = (Dr+Dl)/2 ;
	theta = thetaprev + (Dr-Dl)/L ;
	x = xprevious + Dc*cos(theta) ;
	y = yprevious + Dc*sin(theta) ;
	xprevious = x ;
	yprevious = y ;
	thetaprev = theta ;
	countr = 0 ;
	countl = 0;
		}
	}

				void rencoder()
				{
					countr = countr + 1;
				}
				void rencoder1()
				{
					countl = countl + 1 ;
				}

				float updatePidl(float command2, float targetValue2, float currentValue2) {
																									// compute PWM value
					tick_r = targetValue2*.02 speed_ST ;
					error_l = tick_r - currentValue2;
					PID_T_l = lastPID_T_l + Kpl*(error_l-last_error_l) +
					Kil*(last_error_l);
					if (PID_T_l>255)
						PID_T_l = 255 ;
					else if(PID_T_l<0)
						PID_T_l = 0 ;
					last_error_l = error_l ;
					lastPID_T_l = PID_T_l ;
					return(PID_T_l*1);
				}

				float updatevx(float targetvalue_x , float currentvalue_x)
				{
					errorx = targetvalue_x - currentvalue_x ;
					vx = Kpo*errorx ;
					return(vx) ;
				}
				float updatevy(float targetvalue_y , float currentvalue_y)
				{
					errory = targetvalue_y - currentvalue_y ;
					vy = Kpo*errory ;
					return(vy) ;
				}
				float update_w(float targetvalue_theta , float currentvalue_theta)
				{
					errortheta = targetvalue_theta - currentvalue_theta ;
					omega = lastomega + Ktheta*(errortheta-lasterrortheta) +
					Kitheta*lasterrortheta ;
					lastomega = omega ;
					lasterrortheta = errortheta ;
					return(omega) ;
				}
				float updatePidr(int command1,float targetValue1, float currentValue1) {
					tick_l = targetValue1*0.02 speed_ST ;
					error_r = tick_l - currentValue1 ;
					PID_T_r =lastPID_T_r + (Kpr*(error_r - last_error_r)) +
					Kir*last_error_r ;
					if(PID_T_r>255)
						PID_T_r = 255 ;
					else if(PID_T_r<0)
						PID_T_r = 0 ;
					last_error_r = error_r;
					lastPID_T_r = PID_T_r ;
					return(PID_T_r*1) ;
					}
				
