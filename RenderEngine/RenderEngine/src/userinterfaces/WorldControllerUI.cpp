#include "userinterfaces/WorldControllerUI.h"

#include <iomanip>
#include <sstream>

#include "imgui/imgui.h"
#include "WorldConfig.h"
#include "TimeAccesor.h"
#include "Scene.h"


Engine::Window::WorldControllerUI::WorldControllerUI(GLFWwindow * surface)
	:Engine::Window::UserInterface(surface)
{
}

void Engine::Window::WorldControllerUI::drawGraphics()
{
	glm::ivec2 windowSize = getWindowSize();
	ImVec2 pos(float(windowSize.x) - 300.0f, 0.0f);
	ImGui::SetNextWindowPos(pos);
	ImVec2 size(300.0f, 1000.0f);
	ImGui::SetNextWindowSize(size);
	ImGui::StyleColorsDark();
	
	if (ImGui::Begin("World Controller##app"))
	{
		int fps = int(floor(1.0f / Time::deltaTime));
		std::string fpsStr = "FPS: " + std::to_string(fps);
		ImGui::Text(fpsStr.c_str());

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		Engine::Camera * cam = Engine::SceneManager::getInstance().getActiveScene()->getCamera();
		const glm::vec3 & camPos = cam->getPosition();
		
		std::ostringstream ss;
		ss << std::setprecision(5);
		ss << -camPos.x << ", " << -camPos.y << ", " << -camPos.z;

		std::string camPosStr = "Current position: " + ss.str();
		ImGui::Text(camPosStr.c_str());

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Drawing settings");
		ImGui::PushItemWidth(150.0f);
		ImGui::Combo("Drawing method##app", reinterpret_cast< int32_t* >(&Engine::Settings::drawingMethod), "Shaded\0Wireframe\0", 2);
		ImGui::Spacing();
		ImGui::Combo("Travel method##app", reinterpret_cast<int32_t*>(&Engine::Settings::travelMethod), "Manual\0Bezier\0Straight", 3);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Light settings");
		ImGui::ColorEdit3("Light color", &Engine::Settings::lightColor[0]);
		ImGui::Spacing();
		ImGui::SliderFloat3("Light direction", &Engine::Settings::lightDirection[0], -1.f, 1.f);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Wind settings");
		ImGui::SliderFloat3("Wind direction", &Engine::Settings::windDirection[0], -1.0f, 1.0f);
		ImGui::SliderFloat("Wind strength", &Engine::Settings::windStrength, 0.0f, 50.0f);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Terrain settings");
		ImGui::SliderFloat("Amplitude##app", &Engine::Settings::terrainAmplitude, 0.0f, 2.0f);
		ImGui::SliderFloat("Frecuency##app",&Engine::Settings::terrainFrecuency, 0.0f, 2.0f);
		ImGui::SliderFloat("Scale##app", &Engine::Settings::terrainScale, 0.0f, 10.0f);
		ImGui::InputInt("Octaves##app", reinterpret_cast< int32_t*>(&Engine::Settings::terrainOctaves));
		ImGui::SliderFloat("Grass coverage##app", &Engine::Settings::grassCoverage, 0.0f, 1.0f);
		ImGui::SliderFloat("Vegetation max height##app", &Engine::Settings::vegetationMaxHeight, 0.0f, 1.0f);
		ImGui::ColorEdit3("Grass color##app", &Engine::Settings::grassColor[0]);
		ImGui::ColorEdit3("Sand color##app", &Engine::Settings::sandColor[0]);
		ImGui::ColorEdit3("Rock color##app", &Engine::Settings::rockColor[0]);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Water settings");
		ImGui::ColorEdit3("Water color", &Engine::Settings::waterColor[0]);
		ImGui::Spacing();
		ImGui::SliderFloat("Water speed", &Engine::Settings::waterSpeed, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat("Water height", &Engine::Settings::waterHeight, 0.0f, 1.0f);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Sky settings");
		ImGui::ColorEdit3("Sky zenit color", &Engine::Settings::skyZenitColor[0]);
		ImGui::ColorEdit3("Sky horizon color", &Engine::Settings::skyHorizonColor[0]);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Cloud settings");
		ImGui::SliderFloat("Cloud type##app", &Engine::Settings::cloudType, 0.0f, 1.0f);
		ImGui::SliderFloat("Cloud coverage##app", &Engine::Settings::coverageMultiplier, 0.0f, 1.0f);
		ImGui::ColorEdit3("Cloud Color multiplier##app", &Engine::Settings::cloudColor[0]);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("Depth of Field");
		ImGui::SliderFloat("Focal distance", &Engine::Settings::dofFocalDist, 0.0f, 100.0f);
		ImGui::SliderFloat("Max Distance factor", &Engine::Settings::dofMaxDist, 0.0f, 10.0f);

		ImGui::Spacing(); ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing(); ImGui::Spacing();

		ImGui::Text("God Rays settings");
		ImGui::SliderFloat("God rays exposure##app", &Engine::Settings::godRaysExposure, 0.0f, 10.0f);
		ImGui::SliderFloat("Weight##app", &Engine::Settings::godRaysWeight, 0.0f, 10.0f);
		ImGui::SliderFloat("Decay##app", &Engine::Settings::godRaysDecay, 0.0f, 1.0f);
		ImGui::SliderFloat("Density##app", &Engine::Settings::godRaysDensity, 0.1f, 10.0f);
		ImGui::End();
	}
}