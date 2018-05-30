#include "postprocessprograms/BloomProgram.h"

const std::string Engine::BloomProgram::PROGRAM_NAME = "BloomProgram";

Engine::BloomProgram::BloomProgram(std::string name, unsigned long long parameters)
	:Engine::SSAAProgram(name, parameters)
{
	fShaderFile = "shaders/postprocess/Bloom.frag";

	buf[0].pass = new Engine::DeferredRenderObject(2, false);
	buf[0].color = buf[0].pass->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	buf[0].emissive = buf[0].pass->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	buf[0].pass->addDepthBuffer24(500, 500);
	buf[0].pass->initialize();

	buf[1].pass = new Engine::DeferredRenderObject(2, false);
	buf[1].color = buf[1].pass->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	buf[1].emissive = buf[1].pass->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	buf[1].pass->addDepthBuffer24(500, 500);
	buf[1].pass->initialize();

	passes = 10;
}

Engine::BloomProgram::BloomProgram(const Engine::BloomProgram & other)
	: SSAAProgram(other)
{
	uHorizontal = other.uHorizontal;
	uBlend = other.uBlend;
}

void Engine::BloomProgram::configureProgram()
{
	Engine::SSAAProgram::configureProgram();

	uHorizontal = glGetUniformLocation(glProgram, "horizontal");
	uBlend = glGetUniformLocation(glProgram, "blend");
}

void Engine::BloomProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::SSAAProgram::onRenderObject(obj, view, proj);

	int prevFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);

	int bufferIndex;
	for (unsigned int i = 0; i < passes - 1; i++)
	{
		bufferIndex = i % 2;
		SwitchableBuffers & sb = buf[bufferIndex];

		// Bind current pass buffer
		glBindFramebuffer(GL_FRAMEBUFFER, sb.pass->getFrameBufferId());

		glUniform1i(uHorizontal, bufferIndex);
		glUniform1i(uBlend, 0);

		// Draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Attach output for next pass
		glUniform1i(uRenderedTextures[0], 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sb.color->getTexture()->getTextureId());

		glUniform1i(uRenderedTextures[1], 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, sb.emissive->getTexture()->getTextureId());
	}

	glUniform1i(uBlend, 1);
	glUniform1i(uHorizontal, bufferIndex == 0 ? 1 : 0);
	glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
}

// ==================================================================

Engine::Program * Engine::BloomProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::BloomProgram * bp = new Engine::BloomProgram(Engine::BloomProgram::PROGRAM_NAME, parameters);
	bp->initialize();
	return bp;
}