#ifndef _LIB_EXAM_H_
#define _LIB_EXAM_H_

	int initFND(void);	//이 함수는 반드시  FND 를 켜기 전에 1회만 불러야 한다.
	int displayFND(int number, int ifPadding);
		//FND 에  number 를 출력한다.
		//단  if Padding  이 1이라면 자릿수가 5자리 이하더라도 '0' 으로 채운다.
		//만약  if Padding 이 0이라면 자릿수가 5자리 이하더라도 앞에 '0'을 붙이지 않는다.
		//example: 012345 vs 12345
	int exitFND(void);

	int initBuzzer(void);		// 이 함부는 반드시 Buzzer 를 켜지 전에 1회만 불러야 한다.
	int freqBuzzer(int frequency);	// Freq 에 지정된 음을 재생한다.
	int stopBuzzer(void);		// Buzzer 에 소리가 나오고 있다면 끈다
	int exitBuzzer(void);

	int initLED(void);		// 이 함수는 반드시  LED 를 켜기 전에 1회만 불러야 한다.
	int setLED(int ledState);	//Bit 로 인코딩된 LED  ON/OFF 정보 각 비트는 GPIO LED  하나에 대응된다.
	int exitLED(void);
#endif
