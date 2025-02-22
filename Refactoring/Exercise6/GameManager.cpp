#include "pch.h"
#include "Texture.h"
#include "MainCharacter.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexData.h"
#include "BoundingBox.h"
#include "Enemy.h"
#include "Input.h"
#include "CrossHair.h"
#include "StaticObjectManager.h"
#include "ShadowMapping.h"
#include "Skybox.h"
#include "GameManager.h"
// BuildObject, Debugging, OBJLader.h 은 현재 사용중이지 않음.

GLFWwindow* window;
HitboxRenderer* catbox, * enemybox[3][9];
CrossHair* fpscrosshair;
CrossHair* tpscrosshair;
StaticObjectManager* staticobjectM;
vector<Bullet*> bullets;
vector<vector<vector<Bullet*>>> enemyBullets;
ShadowMapping* shadowMap;
Skybox* skybox;
MainCharacter* mainCat;
Enemy* enemy[3][9];
Texture textureLoader;
AnimInfo player_CurrentAnim;
AnimInfo enemy_CurrentAnim[3][9];
ISoundEngine* soundEngine = nullptr;
ISound* startbgm, * bgm, * basebgm;

void GameManager::winset(int argc, char* argv[])
{
	const char* basename = getBaseName(argv[0]);
	glfwInit();
	window = glfwCreateWindow(WIN_W, WIN_H, basename, NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();
	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, keyFunc);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouseFunc);

	mainCat = new MainCharacter();
	catbox = new HitboxRenderer();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			enemy[i][j] = new Enemy(i + 1, j);
			enemybox[i][j] = new HitboxRenderer();
		}
	}

	enemyBullets.resize(3);
	for (int i = 0; i < 3; i++) {
		enemyBullets[i].resize(9);
	}

	fpscrosshair = new CrossHair(1, 7.0f, window);
	tpscrosshair = new CrossHair(3, 10.0f, window);
	staticobjectM = new StaticObjectManager();
	shadowMap = new ShadowMapping();
	shadowMap->init();
	skybox = new Skybox();
	soundEngine = createIrrKlangDevice();

	setAnimation();
	setCollision();
	playeranimLib.changeAnimation("Idle", player_CurrentAnim);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
			enemyanimLib[i][j].changeAnimation("Idle", enemy_CurrentAnim[i][j]);
	}

	glm::vec3 pos = mainCat->getPosition();
	ISoundSource* startsource = soundEngine->addSoundSourceFromFile("music/Startsong.mp3");
	ISoundSource* source = soundEngine->addSoundSourceFromFile("music/FunkyTown.mp3");
	ISoundSource* source2 = soundEngine->addSoundSourceFromFile("music/chipi.mp3");
	startbgm = soundEngine->play2D(startsource, true, true);
	bgm = soundEngine->play2D(source, true, true);
	basebgm = soundEngine->play2D(source2, true, true);
	startbgm->setIsPaused(false);

	glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
}

void GameManager::drawfunc()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	static float lastTime = glfwGetTime();
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (mainCat->isMoving()) {
		if (mainCat->shift_value())
			mainCat->run();
		else
			mainCat->walk();
	}

	glfwGetCursorPos(window, &cur_x, &cur_y);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_W / (float)WIN_H, 0.1f, 1000.0f);
	glm::mat4 view = glm::mat4(1.0f);
	if (FirstPersonView)
		view = camera.get1stPersonViewMatrix(mainCat->getPosition());
	else
		view = camera.get3rdPersonViewMatrix(mainCat->getPosition());

	if (!camera.isAltPressed()) {
		mouseDir = camera.getMouseWorldDirection(cur_x, cur_y, projection, view, mainCat->getPosition());
	}
	glm::vec3 viewPos = camera.getPosition(mainCat->getPosition());
	camera.handleMouseMovement(cur_x, cur_y);
	float angle;
	if (!FirstPersonView) {
		angle = (float)PI + atan2(mouseDir.x, mouseDir.z);
	}
	else {
		angle = camera.get_horizontal_angle();
	}

	changeMainCharacterAnimation(view, projection);
	changeEnemiesAnimation();

	shadowMap->updateLightSpaceMatrix(mainCat->getPosition());		// Shadow Pass 시작
	shadowMap->bindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(shadowMap->getDepthShaderProgram());			// Depth map 렌더링
	glm::mat4 lightSpaceMatrix = shadowMap->getLightSpaceMatrix();
	glm::mat4 lightSpaceMatrix2 = shadowMap->getLightSpaceMatrix_Enemy();
	GLuint lightSpaceMatrixLoc = glGetUniformLocation(shadowMap->getDepthShaderProgram(), "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	mainCat->drawshadow(mainCat, angle, shadowMap);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j)
		{
			enemy[i][j]->setAnimation();
			enemy[i][j]->movetoward();
			enemy[i][j]->drawshadow(j, enemy[i][j], shadowMap, deltaTime);
		}
	}
	staticobjectM->DrawStaticObjectsShadow(lightSpaceMatrix, shadowMap->getstaticDepthShaderProgram());

	for (auto& bullet : bullets)
	{
		bullet->renderShadow(lightSpaceMatrix, shadowMap->getstaticDepthShaderProgram());
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			for (auto& bullet : enemyBullets[i][j]) {
				bullet->renderShadow(lightSpaceMatrix, shadowMap->getstaticDepthShaderProgram());
			}
		}
	}

	shadowMap->unbindFramebuffer();
	glViewport(0, 0, WIN_W, WIN_H);			// Shadow Pass 종료

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Normal Pass 시작
	skybox->draw(view, projection);

	mainCat->draw(deltaTime, mainCat, angle, view, projection, viewPos, lightSpaceMatrix, shadowMap->getDepthMap());

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			enemy[i][j]->draw(j, deltaTime, enemy[i][j], view, projection, viewPos, lightSpaceMatrix, shadowMap->getDepthMap());
			if (enemy[i][j]->getstate() != 4 && mainCat->hitbox_ison())
				enemybox[i][j]->renderenemy(enemy[i][j]->getlastangle(), view, projection, enemy[i][j]);
		}
	}

	if (mainCat->hitbox_ison())
		catbox->rendercat(angle, view, projection);

	staticobjectM->DrawStaticObjects(view, projection, viewPos, lightSpaceMatrix, shadowMap->getDepthMap());

	for (auto& bullet : bullets)
	{
		bullet->bulletUpdate();
		bullet->render(view, projection, viewPos, lightSpaceMatrix, shadowMap->getDepthMap());
	}
	for (int i = 0; i < bullets.size(); ) {
		if (bullets[i]->isCollapsed()) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
		else {
			++i;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			for (auto& bullet : enemyBullets[i][j]) {
				bullet->bulletUpdate();
				bullet->render(view, projection, viewPos, lightSpaceMatrix, shadowMap->getDepthMap());
			}

			for (int k = 0; k < enemyBullets[i][j].size();) {
				if (enemyBullets[i][j][k]->isCollapsed()) {
					delete enemyBullets[i][j][k];
					enemyBullets[i][j].erase(enemyBullets[i][j].begin() + k);
				}
				else {
					++k;
				}
			}
		}
	}

	if (FirstPersonView)
		fpscrosshair->draw();
	else
		tpscrosshair->draw();

	if (start)
		camera.starting();

	glm::vec3 cPos = mainCat->getPosition();
	if (cPos.x < -41.0f && cPos.x > -48.5f)
	{
		if (cPos.z < -50.5f)
		{
			if (bgm->getIsPaused() == true)
				bgm->setIsPaused(false);

			if (basebgm->getIsPaused() == false)
				basebgm->setIsPaused(true);

			if (!set_fin)
			{
				finish = { true };
				mainCat->stop();
				camera.handleAltKey(false);
				FirstPersonView = { false };
				mainCat->setFinishPos();
				if (playeranimLib.getcurrentAnimation() != "Dance")
					playeranimLib.changeAnimation("Dance", player_CurrentAnim);

				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 9; ++j)
					{
						enemy[i][j]->setrevive_timer();
						enemy[i][j]->setdie();
					}
				}

				set_fin = { true };
			}
		}
	}
	if(finish)
		camera.addfinishpos();
	

	if (light_angle > 6.28f)
		light_angle -= 6.28f;
	light_angle += 0.0005f;

	glFinish();
}

void GameManager::loopgame()
{
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawfunc();
		soundEngine->setSoundVolume(soundVol);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GameManager::deleteAll()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			for (Bullet* bullet : enemyBullets[i][j]) {
				delete bullet;
			}
			enemyBullets[i][j].clear();
		}
	}

	delete soundEngine;
	delete skybox;
	delete shadowMap;
	for (auto bullet : bullets)
		delete bullet;
	delete staticobjectM;
	delete tpscrosshair;
	delete fpscrosshair;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			delete enemybox[i][j];
			delete enemy[i][j];
		}
	}
	delete catbox;
	delete mainCat;
	glfwTerminate();
}