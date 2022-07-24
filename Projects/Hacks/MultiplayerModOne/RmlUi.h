#pragma once

#if GTAC_RMLUI
#include <RmlGUI/RmlUi.h>

class CRmlRenderInterface : public CRmlBaseRenderInterface
{
public:
	CRmlRenderInterface(Context* pContext);

	bool m_bScissor;
	Rml::Vector2i m_vecScissorPos;
	Rml::Vector2i m_vecScissorSize;

	virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

	virtual void EnableScissorRegion(bool enable) override;
	virtual void SetScissorRegion(int x, int y, int width, int height) override;
	void UpdateScissor();
};

class CRmlUi2 : public CRmlUi
{
public:
	CRmlUi2(Context* pContext);
	~CRmlUi2();

	Rml::ElementDocument* m_pDocument;

	bool Initialise();
	void Reload();
};
#endif
