#pragma once

#include "Light.h"
#include "Camera.h"
#include "DeferredNodeCallbacks.h"
#include "PostProcessProgram.h"
#include "Renderer.h"

namespace Engine
{
	namespace TestImplementation
	{
		class LightMovement : public KeyboardHandler
		{
		private:
			PointLight * lightSource;
		public:
			LightMovement(PointLight * light);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class LightIntensityMod : public KeyboardHandler
		{
		private:
			PointLight * lightSource;
		public:
			LightIntensityMod(PointLight * light);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class CameraMovement : public KeyboardHandler
		{
		private:
			Camera * camera;
		public:
			CameraMovement(Camera * cam);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class MotionBlurManagement : public KeyboardHandler
		{
		private:
			MotionBlurImpl * mbi;
		public:
			MotionBlurManagement(MotionBlurImpl * m);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class DepthOfFieldManagement : public KeyboardHandler
		{
		private:
			DepthOfFieldProgram * dofShader;
		public:
			DepthOfFieldManagement(DepthOfFieldProgram * dof);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class ConvolutionMaskManagement : public KeyboardHandler
		{
		private:
			//GaussianBlurProgram * gaussShader;
			GaussianBlurProgram * gaussShader;

			unsigned int maskSize;
			float maskFactor;

			float convolutionMask1[25];
			float convolutionMask2[25];
			float convolutionMask3[25];

			glm::vec2 affectedTexels[25];

		public:
			ConvolutionMaskManagement(GaussianBlurProgram * gauss);
			void onKeyPressed(unsigned char key, int x, int y);
		};

		// =========================================================================

		class RendererSwitcher : public KeyboardHandler
		{
		private:
			ForwardRenderer * standarFR;
			DeferredRenderer * standarDR;
			DeferredRenderer * depthDR;
			DeferredRenderer * threeGaussFilterDR;
			DeferredRenderer * fullPostProcessDR;
			SideBySideRenderer * sideBySideRenderer;
			DeferredRenderer * colorDR;
			DeferredRenderer * normalDR;
			DeferredRenderer * specularDR;
			DeferredRenderer * toonDR;
		public:
			RendererSwitcher();
			void onKeyPressed(unsigned char key, int x, int y);

		private:
			void initializeFullPPRenderer();
			void initialize3GaussRenderer();
			void initializeSideBySideRenderer();
		};

		// ===========================================================

		class CameraMotion : public MouseMotionHandler
		{
		private:
			Camera * camera;

			int previousX, previousY;

			float horizontalSpeed;
			float verticalSpeed;
		public:
			CameraMotion(std::string nam, Camera * cam);

			void handleMotion(int x, int y);
			void finishMotion();
		};
	}
}