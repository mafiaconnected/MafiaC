#pragma once

#if MAFIAC_RMLUI
#include <RmlGUI/RmlUi.h>

class CRmlRwRenderInterface : public CRmlBaseRenderInterface
{
public:
	CRmlRwRenderInterface(Context* pContext);

	virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;
	virtual void UpdateScissor() override;
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
