#ifndef CR_PASSMANAGER
#define CR_PASSMANAGER

#include "..\Inc\CRSkyRenderPass.h"
#include "..\Inc\CRFinalPass.h"
#include "..\Inc\CRGPass.h"
#include "..\Inc\CRLightPass.h"
#include "..\Inc\CRSMPass.h"
#include "..\Inc\CRShadowPass.h"
#include "..\Inc\CRShadowBlurPass.h"
#include "..\Inc\CRSSAOPass.h"
#include "..\Inc\CRGIPass.h"
#include "..\Inc\CRSSRPass.h"
#include "..\Inc\CRVolumetricCloudPass.h"

class RenderPassManager
{
private:
	RenderPassManager();
	static RenderPassManager* ins;

	SkyRenderPass skyPass;
	FinalPass finalPass;
	GPass gPass;
	LightPass lightPass;
	SMPass smPass;
	ShadowPass shadowPass;
	ShadowBlurPass shadowBlurPass;
	SSAOPass ssaoPass;
	GIPass giPass;
	SSRPass ssrPass;
	VolumetricCloudPass cloudPass;

public:
	~RenderPassManager();
	static RenderPassManager* Instance();
	void Init();
	void Draw();
};

#endif
