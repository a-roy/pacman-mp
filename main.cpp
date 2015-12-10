//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "MainState.h"
#include "StateMachine.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "NetworkManager.h"

int main()
{
	Renderer::LoadFont("../prstartk.ttf");
	Renderer::Scale = 3.0f;
	Renderer::CreateWindow(768, 768, "My window");

	MainMenuState::Init();
	JoinState::Init();
	HostLobbyState::Init();
	ClientConnectedState::Init();

	NetworkManager::Init();

	StateMachine::Change(new MainMenuState());
    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		if (InputHandler::WindowClosed)
		{
			break;
		}

		StateMachine::CurrentState->ProcessData();
		StateMachine::CurrentState->LocalUpdate();
		Renderer::Clear();
		StateMachine::CurrentState->Render();
		Renderer::Display();
    }

	Renderer::Deinit();
	delete StateMachine::CurrentState;
	return 0;
}
