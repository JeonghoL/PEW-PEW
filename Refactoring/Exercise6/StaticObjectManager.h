#pragma once
#include "StaticObject.h"

class StaticObjectManager
{
public:
    StaticObjectManager()
    {
		title = new StaticObject("glb/title.glb", "texture/title.png");
		ground = new StaticObject("glb/ground.glb", "texture/map.png");
		fence = new StaticObject("glb/fence.glb", "texture/fence.png");
		tree1 = new StaticObject("glb/tree1.glb", "texture/tree1.png");
		tree2 = new StaticObject("glb/tree2.glb", "texture/tree2.png");
		tree3 = new StaticObject("glb/tree3.glb", "texture/tree3.png");
		tree4 = new StaticObject("glb/tree4.glb", "texture/tree4.png");
		bridge = new StaticObject("glb/bridge.glb", "texture/bridge.png");
		box = new StaticObject("glb/box.glb", "texture/box.png");
		cart = new StaticObject("glb/cart.glb", "texture/cart.png");
		house = new StaticObject("glb/housemain.glb", "texture/housemain.png");
		house1 = new StaticObject("glb/house1.glb", "texture/house1.png");
		house2 = new StaticObject("glb/house2.glb", "texture/house2.png");
		house3 = new StaticObject("glb/house3.glb", "texture/house3.png");
		house4 = new StaticObject("glb/house4.glb", "texture/house4.png");
		house5 = new StaticObject("glb/house5.glb", "texture/house5.png");
		house6 = new StaticObject("glb/house6.glb", "texture/house6.png");
		house7 = new StaticObject("glb/house7.glb", "texture/house7.png");
		house8 = new StaticObject("glb/house8.glb", "texture/house8.png");
		house9 = new StaticObject("glb/house9.glb", "texture/house9.png");
		rock1 = new StaticObject("glb/rock1.glb", "texture/rock1.png");
		rock2 = new StaticObject("glb/rock2.glb", "texture/rock2.png");
		waterwheel = new StaticObject("glb/waterwheel.glb", "texture/waterwheel.png");
		windmill = new StaticObject("glb/windmill.glb", "texture/windmill.png");
		cloud = new StaticObject("glb/cloud.glb", "texture/cloud.png");
		cave = new StaticObject("glb/cave.glb", "texture/cave.png");
		startLogo = new StaticObject("glb/startLogo.glb", "texture/startLogo.png");
		gameclear = new StaticObject("glb/gameclear.glb", "texture/gameclear.png");
    }

	void DrawStaticObjects(const glm::mat4& view, const glm::mat4& projection, glm::vec3 viewPos,
		const glm::mat4& lightSpaceMatrix, GLuint shadowMap)
	{
		title->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		ground->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		fence->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		tree1->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		tree2->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		tree3->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		tree4->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		bridge->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		box->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		cart->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house1->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house2->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house3->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house4->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house5->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house6->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house7->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house8->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		house9->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		rock1->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		rock2->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		waterwheel->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		windmill->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		cave->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		startLogo->drawStaticobject(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		gameclear->drawEnd(view, projection, viewPos, lightSpaceMatrix, shadowMap);
		cloud->drawcloud(view, projection, viewPos, lightSpaceMatrix, shadowMap);
	}

	void DrawStaticObjectsShadow(const glm::mat4& lightSpaceMatrix, GLuint depthShader)
	{
		title->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		ground->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		fence->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		tree1->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		tree2->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		tree3->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		tree4->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		bridge->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		box->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		cart->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house1->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house2->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house3->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house4->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house5->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house6->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house7->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house8->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		house9->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		rock1->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		rock2->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		waterwheel->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		windmill->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		cave->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		cloud->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		startLogo->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
		gameclear->drawStaticobjectShadow(lightSpaceMatrix, depthShader);
	}

	~StaticObjectManager()
	{
		delete gameclear;
		delete startLogo;
		delete cave;
		delete cloud;
		delete title;
		delete windmill;
		delete waterwheel;
		delete rock2;
		delete rock1;
		delete house9;
		delete house8;
		delete house7;
		delete house6;
		delete house5;
		delete house4;
		delete house3;
		delete house2;
		delete house1;
		delete house;
		delete cart;
		delete box;
		delete bridge;
		delete tree4;
		delete tree3;
		delete tree2;
		delete tree1;
		delete fence;
		delete ground;
	}

private:
    StaticObject* ground, * fence;
    StaticObject* tree1, * tree2, * tree3, * tree4;
    StaticObject* bridge, * box, * cart;
    StaticObject* house, * house1, * house2, * house3, * house4;
    StaticObject* house5, * house6, * house7, * house8, * house9;
    StaticObject* rock1, * rock2;
    StaticObject* waterwheel, * windmill;
	StaticObject* title, * cloud, * cave;
	StaticObject* startLogo, * gameclear;
};