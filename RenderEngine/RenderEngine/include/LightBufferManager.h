#pragma

#include "Scene.h"
#include "StorageTable.h"

namespace Engine
{
	namespace GPU
	{
		class LightBufferManager : public StorageTable
		{
		private:
			static LightBufferManager * INSTANCE;
		private:
			unsigned int gboDL;
			unsigned int gboPL;
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