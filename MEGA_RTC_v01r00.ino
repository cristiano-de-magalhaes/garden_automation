#include <Wire.h>
#include <RTC.h>


#define GENERAL_PUMP 22
#define PUMP_NO_1 	23
#define PUMP_NO_2 	24
#define PUMP_NO_3 	25
#define PUMP_NO_4 	26
#define PUMP_NO_5 	27
// GARDEN_LIGHTS control not needed nowadays to be implemented in the future
// #define GARDEN_LIGHTS	28

// LOW state turns ON the relays (pumps)
// HIGH state turns OFF the relays (pumps)

static DS3231 RTC;


bool day_flag = false;
bool delay_flag = false;


void acts_pump(int pump, bool mode, int time_delay) {
	digitalWrite(GENERAL_PUMP, mode);
	delay(time_delay);
	digitalWrite(pump, mode);
}//acts_pump()


void all_pumps_off() {
	for(int i=22; i<=27; i++){
	    acts_pump(i, HIGH, 0);
	}//for
}//all_pumps_off()


void prints_info_on_serial() {

	switch (RTC.getWeek()) {
		case 1:
			Serial.print("SUN");
			break;
		case 2:
			Serial.print("MON");
			break;
		case 3:
			Serial.print("TUE");
			break;
		case 4:
			Serial.print("WED");
			break;
		case 5:
			Serial.print("THU");
			break;
		case 6:
			Serial.print("FRI");
			break;
		case 7:
			Serial.print("SAT");
			break;
	}//switch


	Serial.print(" ");
	Serial.print(RTC.getDay());
	Serial.print("-");
	Serial.print(RTC.getMonth());
	Serial.print("-");
	Serial.print(RTC.getYear());

	Serial.print(" ");

	Serial.print(RTC.getHours());
	Serial.print(":");
	Serial.print(RTC.getMinutes());
	Serial.print(":");
	Serial.print(RTC.getSeconds());

	if (RTC.getHourMode() == CLOCK_H12) {


		switch (RTC.getMeridiem()) {
			case HOUR_AM :
				Serial.print(" AM");
				break;
			case HOUR_PM :
				Serial.print(" PM");
				break;
		}//switch


	}//if

	Serial.println();

}//prints_info_on_serial()



void setup() {


	// pinMode(GARDEN_LIGHTS, OUTPUT);
	pinMode(GENERAL_PUMP, OUTPUT);
	pinMode(PUMP_NO_1, OUTPUT);
	pinMode(PUMP_NO_2, OUTPUT);
	pinMode(PUMP_NO_3, OUTPUT);
	pinMode(PUMP_NO_4, OUTPUT);
	pinMode(PUMP_NO_5, OUTPUT);


	// digitalWrite(GARDEN_LIGHTS, HIGH);
	digitalWrite(GENERAL_PUMP, HIGH);
	digitalWrite(PUMP_NO_1, HIGH);
	digitalWrite(PUMP_NO_2, HIGH);
	digitalWrite(PUMP_NO_3, HIGH);
	digitalWrite(PUMP_NO_4, HIGH);
	digitalWrite(PUMP_NO_5, HIGH);


	Serial.begin(9600);
	

	// code below from library examples
	RTC.begin();

	
	// DateTime A1, A2;

	
	//print info
	Serial.println();
	Serial.println("*** RTC 3231 ***");

	Serial.print("Is Clock Running : ");
	if (RTC.isRunning()) Serial.println("Yes");
	else Serial.println("No. Time may be Inaccurate");

	Serial.print("Hour Mode :");
	if (RTC.getHourMode() == CLOCK_H12) Serial.println("12 Hours");
	else Serial.println("24 Hours");

	//Alarms1

	A1 = RTC.getAlarm1();

	Serial.print("Alarm1 Time : ");
	Serial.print(A1.hours);
	Serial.print(":");
	Serial.print(A1.minutes);
	Serial.print(":");
	Serial.print(A1.seconds);
	if (RTC.getHourMode() == CLOCK_H12)
	{
		switch (RTC.getMeridiem())
		{
			case HOUR_AM :
				Serial.print(" AM");
				break;
			case HOUR_PM :
				Serial.print(" PM");
				break;
		}
	}
	Serial.println();

	//Alarms Trigger Status

	Serial.print("Alarm 1 Tiggered: ");
	if (RTC.isAlarm1Tiggered())
		Serial.println("Yes");
	else
		Serial.println("No");

	Serial.print("Alarm2 Tiggered : ");
	if (RTC.isAlarm2Tiggered())
		Serial.println("Yes");
	else
		Serial.println("No");
	
	Serial.print("INT/SQW Pin Mode : ");
	if (RTC.getINTPinMode())
		Serial.println("Alarm");
	else
		Serial.println("SQW");

	//Aging Offset Value
	Serial.print("Aging Offset : "); 
	Serial.print(RTC.getAgingOffset());
	Serial.println();
	
	Serial.print("RTC Temperature : "); 
	Serial.print(RTC.getTemp());
	Serial.print("'C");
	Serial.println();
	Serial.println();

	// end of library example code
}//setup()

void loop(){


	if ((RTC.getWeek() == 2) || (RTC.getWeek() == 4) || (RTC.getWeek() == 6)) {
		if (RTC.getHours() == 4) {
			if (RTC.getMinutes() < 20) {
				if (!delay_flag) {
					acts_pump(PUMP_NO_1, LOW, 2000);
					delay_flag = true;
				}
			} else if (RTC.getMinutes() == 20) {
				acts_pump(PUMP_NO_1, HIGH, 0);
				delay_flag = false;
			} else if (RTC.getMinutes() < 40) {
				if (!delay_flag) {
					acts_pump(PUMP_NO_2, LOW, 2000);
					delay_flag = true;
				}
			} else if (RTC.getMinutes() == 40) {
				acts_pump(PUMP_NO_2, HIGH, 0);
				delay_flag = false;
			} else if (RTC.getMinutes() < 59) {
				if (!delay_flag) {
					acts_pump(PUMP_NO_3, LOW, 2000);
					delay_flag = true;
				}
			} else {
				acts_pump(PUMP_NO_3, HIGH, 0);
				delay_flag = false;
			}
		} else if (RTC.getHours() == 5) {
			if (RTC.getMinutes() < 20) {
				if (!delay_flag) {
					acts_pump(PUMP_NO_4, LOW, 2000);
					delay_flag = true;
				}
			} else if (RTC.getMinutes() == 20) {
				acts_pump(PUMP_NO_4, HIGH, 0);
				delay_flag = false;
			} else if (RTC.getMinutes() < 40) {
				if (!delay_flag) {
					acts_pump(PUMP_NO_5, LOW, 2000);
					delay_flag = true;
				}
			} else if (RTC.getMinutes() == 40) {
				acts_pump(PUMP_NO_5, HIGH, 0);
				delay_flag = false;
			} else {
				all_pumps_off();
			}
		} else {
			all_pumps_off();
		}//if-else if-else
	} else {
		all_pumps_off();
	}//if


	prints_info_on_serial();


	delay(1000);


}//loop()

