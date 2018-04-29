#include "userinterfaces/WorldControllerUI.h"

#include "imgui/imgui.h"

Engine::Window::WorldControllerUI::WorldControllerUI(GLFWwindow * surface)
	:Engine::Window::UserInterface(surface)
{
}

void Engine::Window::WorldControllerUI::drawGraphics()
{
	ImVec2 pos(1024.0f - 280.0f, 0.0f);
	ImGui::SetNextWindowPos(pos);
	ImVec2 size(280.0f, 175.0f);
	ImGui::SetNextWindowSize(size);
	if (ImGui::Begin("World Controller##app"))
	{
		
		int show = 0;
		ImGui::Combo("Options##app", reinterpret_cast< int32_t* >(&show), "Catmull Rom\0Bezier\0Perlin\0Cellular Map\0Fractal\0NoNe\0", 6);

		float zero = 0.0f;
		ImGui::InputFloat("Amplitude##app", &zero);
		ImGui::InputFloat("Frecuency##app",&zero);
		ImGui::InputFloat("Scale##app", &zero);
		ImGui::InputInt("Octaves##app", reinterpret_cast< int32_t*>(&show));

		ImGui::End();

		
	}
}