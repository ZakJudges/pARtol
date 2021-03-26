#pragma once


class EventManager
{
public:
	void OnCrossPressed();
	void OnSquarePressed();
	void OnCirclePressed();
	void OnTrianglePressed();
	void LeftStickX(float x);
	void LeftStickY(float y);
	void RightStickX(float x);
	void RightStickY(float y);
	void Update();
};