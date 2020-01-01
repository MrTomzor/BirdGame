#pragma once

class InputData {
public:
	bool numsHeld;
	bool nums[10];
	bool use1Pressed;
	bool use2Pressed;
	bool shoot1Pressed;
	bool shoot1Held;
	bool sprintHeld;
	bool clickedLeft;
	bool clickedRight;

	/*bool slapHeld;
	bool shootHeld;
	bool Lheld;
	bool upPressed;
	bool slapPressed;*/

	bool leftHeld[6];
	bool rightHeld[6];
	float upHeld[6];
	float downHeld[6];
	bool shootHeld[6];
	bool flapPressed[6];
	bool slapPressed[6];

	bool slapHeld[6]; //because of joysticks dont have released event
	float axisX[6];

	bool confirmPressed[6]; //for Menu
	bool masterConfirmPressed = 0;
	bool masterBackPressed = 0;
	bool backPressed[6];
	bool upPressed[6];
	bool downPressed[6];
	bool leftPressed[6];
	bool rightPressed[6];
};