#include "Animation.h"

Animation::Animation(int delay)
{
	Delay = delay;
}

void Animation::AddFrame(int x, int y, int width, int height)
{
	Frames.push_back({x, y, width, height});
}

void Animation::GetRect(int frame, int &x, int &y, int& width, int& height) const
{
	int f = (frame / Delay) % Frames.size();
	x = Frames[f].X;
	y = Frames[f].Y;
	width = Frames[f].Width;
	height = Frames[f].Height;
}
