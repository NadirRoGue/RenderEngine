/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma

#include "Scene.h"
#include "StorageTable.h"

namespace Engine
{
	namespace GPU
	{
		// Class which manages all light uniform buffer objects. Implements the update
		// functionality, as well as the partial update functionality
		// Its also in charge of clean up
		class LightBufferManager : public StorageTable
		{
		private:
			static LightBufferManager * INSTANCE;
		private:
			// Directional light buffer
			unsigned int gboDL;
			// Point light buffer
			unsigned int gboPL;
			// Spot light buffer
			unsigned int gboSL;
		private:
			LightBufferManager();
		public:
			static LightBufferManager & getInstance();
		public:
			~LightBufferManager();
			void onSceneStart();

			void enableDirectionalLightBuffer();
			void enablePointLightBuffer();
			void enableSpotLightBuffer();
			void enableDirectionalLightBufferAtIndex(unsigned int index);
			void enablePointLightBufferAtIndex(unsigned int index);
			void enableSpotLightBufferAtIndex(unsigned int index);
			void updateDirectionalLight(DirectionalLight * dl, bool onlyViewDependent = false);
			void updatePointLight(PointLight * pl, bool onlyViewDependent = false);
			void updateSpotLight(SpotLight * sl, bool onlyViewDependent = false);

			void clean();
		private:
			void initializeBuffers(Scene * scene);
		};
	}
}