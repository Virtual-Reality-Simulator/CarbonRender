#include "..\Inc\CRGPass.h"

void GPass::GetReady4Render(PassOutput * input)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint rt_n, rt_albedo, rt_p, rt_stencil;
	WindowSize size = WindowManager::Instance()->GetWindowSize();
	rt_albedo = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_COLOR_ATTACHMENT0);
	rt_n = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT1);
	rt_p = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_COLOR_ATTACHMENT2);
	rt_stencil = GLHelper::SetGLRenderTexture(size.w, size.h, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR, GL_COLOR_ATTACHMENT3);

	dBuffer = GLHelper::SetGLDepthBuffer(size.w, size.h);

	GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	output.cout = 4;
	output.RTS = new GLuint[output.cout];
	output.RTS[0] = rt_albedo;
	output.RTS[1] = rt_n;
	output.RTS[2] = rt_p;
	output.RTS[3] = rt_stencil;
}

void GPass::Render(PassOutput * input)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::Instance()->UseShader(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "depthClampPara");
	Camera* curCam = CameraManager::Instance()->GetCurrentCamera();
	float3 camPara = curCam->GetCameraPara();
	glUniform2f(location, camPara.y, 1.0f / camPara.z);

	SceneManager::Instance()->DrawScene(shaderProgram);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void GPass::Init()
{
	shaderProgram = ShaderManager::Instance()->LoadShader("GPass.vert", "GPass.frag");
}
