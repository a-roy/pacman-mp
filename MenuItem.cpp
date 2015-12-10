#include "MenuItem.h"
#include "Renderer.h"
#include "NetworkManager.h"
#include "MainState.h"
#include <sstream>

void FunctionalMenuItem::Render(int x, int y) const
{
	Renderer::DrawText(Text, 24, x, y);
}

void FieldMenuItem::Render(int x, int y) const
{
	std::ostringstream ss;
	ss << "< Field " << *Value << " >";
	std::string str = ss.str();
	Renderer::DrawText(str, 24, x, y);
}

void CharacterMenuItem::Render(int x, int y) const
{
	std::string text;
	Sprite *sprite;
	bool flip = false;
	int frame = 0;
	if (*Value == PacMan)
	{
		text = "< Pac-Man    >";
		sprite = &ClientConnectedState::PacmanSprite;
		flip = true;
		frame = 4;
	}
	else if (*Value == Blinky)
	{
		text = "< Blinky     >";
		sprite = &ClientConnectedState::GhostSprites[0];
	}
	else if (*Value == Inky)
	{
		text = "< Inky       >";
		sprite = &ClientConnectedState::GhostSprites[2];
	}
	else if (*Value == Pinky)
	{
		text = "< Pinky      >";
		sprite = &ClientConnectedState::GhostSprites[1];
	}
	else if (*Value == Clyde)
	{
		text = "< Clyde      >";
		sprite = &ClientConnectedState::GhostSprites[3];
	}
	Renderer::DrawText(text, 18, x, y);
	int s_x = (x + 9 * 18 - 12) * TILE_SIZE / 24;
	int s_y = (y - 24) * TILE_SIZE / 24;
	Renderer::DrawSprite(*sprite, s_x, s_y, 0.f, flip, 0, frame);
}

void ReadyMenuItem::Forward()
{
	if (!*Value)
	{
		*Value = true;
		std::vector<char> data_s(PlayerReady_size);
		data_s[PlayerReady_Character] = ClientConnectedState::SelectedCharacter;
		NetworkManager::Send(
				NetworkManager::PlayerReady, data_s, 0);
	}
}

void ReadyMenuItem::Backward()
{
	if (*Value)
	{
		*Value = false;
		std::vector<char> data_s(PlayerNotReady_size);
		NetworkManager::Send(NetworkManager::PlayerNotReady, data_s, 0);
	}
}

void ReadyMenuItem::Render(int x, int y) const
{
	if (*Value)
	{
		Renderer::DrawText("< Ready!", 18, x, y);
	}
	else
	{
		Renderer::DrawText("  Ready? >", 18, x, y);
	}
}
