#include "pch.h"
#include "MainCharacter.h"
#include "AnimatedModel.h"
#include "ShadowMapping.h"
#include "Bullet.h"
#include "Camera.h"

//extern float getCamerahori();
//extern bool getFirstPersonView();
//extern bool wallcollapsed_w(), wallcollapsed_s(), wallcollapsed_a(), wallcollapsed_d();
//extern bool wallcollapsed_forward(const glm::vec3& forward);
//extern bool wallcollapsed_backward(const glm::vec3& forward);
//extern bool wallcollapsed_right(const glm::vec3& right);
//extern bool wallcollapsed_left(const glm::vec3& right);
//
//extern void setupBoneTransforms(std::vector<BoneInfo>& BoneInfoName, GLuint shadername);
//extern void UpdateAnimation(int j, std::vector<BoneInfo>& BoneInfoName, float deltaTime, AnimInfo& currentAnim);
//
//extern void loadGLBFile(int j, std::vector<BoneInfo>& BoneInfoName, const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& VBO2, GLuint& EBO, std::vector<unsigned int>& Indices);
//void setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);
//extern Texture textureLoader;
//extern AnimInfo player_CurrentAnim;
//extern bool firing;
//extern bool firing_induration;
//extern float light_angle;
//extern bool finish;

MainCharacter::MainCharacter()
{
	player_BoneInfo = new vector<BoneInfo>();
	animModel = new AnimatedModel();
    player_CurrentAnim = new AnimInfo();
    animLibrary = new AnimatedModel::AnimationLibrary();

    SaveAnimations();

    animModel->loadGLBFile(0, *player_BoneInfo, "Glb/cat_Tpose.glb", VAO, VBO, VBO2, EBO, Indices);
    Texture = LoadTexture("Texture/CatTexture.png");
    SetUpShader("Shaders/CatVert.glsl", "Shaders/CatFrag.glsl", shaderprogram);
}

MainCharacter::~MainCharacter()
{
    delete player_CurrentAnim;
    delete player_BoneInfo;
	delete animModel;

    if (animLibrary != nullptr) {
        delete animLibrary;
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &Texture);
	glDeleteProgram(shaderprogram);
}

void MainCharacter::Update()
{
    if (IsMoving()) {
        if (Shift_value())
            Run();
        else
            Walk();
    }
}

void MainCharacter::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, float deltaTime, float angle)
{
    //if (!dead)
    //{
        animModel->UpdateAnimation(0, *player_BoneInfo, deltaTime, *player_CurrentAnim);
        glUseProgram(shaderprogram);
        animModel->SetupBoneTransforms(*player_BoneInfo, shaderprogram);

        //if (!dying)
        //    lastangle = angle;

        model = glm::mat4(1.0f);
        model = glm::translate(model, characterPos);
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        //if (!dying)
        //    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        //else
        //    model = glm::rotate(model, lastangle, glm::vec3(0.0f, 1.0f, 0.0f));

        //if (finish)
        //    model = glm::scale(model, glm::vec3(20.0f));

        ViewLoc = glGetUniformLocation(shaderprogram, "view");
        glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, &view[0][0]);
        ProjLoc = glGetUniformLocation(shaderprogram, "projection");
        glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, &projection[0][0]);
        ModelLoc = glGetUniformLocation(shaderprogram, "model");
        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, &model[0][0]);

        GLuint lightPosLoc = glGetUniformLocation(shaderprogram, "lightPos");
        GLuint viewPosLoc = glGetUniformLocation(shaderprogram, "viewPos");
        glm::vec3 lightPos{ -37.3051f - (1000.0f * cos(light_angle)), 0.0f + 1000.0f, 42.5001f + (1000.0f * sin(light_angle)) };
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        TextureLoc = glGetUniformLocation(shaderprogram, "catTexture");
        glUniform1i(TextureLoc, 0);

        colorHitLoc = glGetUniformLocation(shaderprogram, "colorHit");
        glUniform4fv(colorHitLoc, 1, glm::value_ptr(hitcolor));

        UseTextureLoc = glGetUniformLocation(shaderprogram, "useTexture");
        glUniform1i(UseTextureLoc, 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //if (hitcolor == glm::vec4(1.0f, 0.6f, 0.6f, 1.0f))
        //{
        //    hit_cnt -= 1;
        //}

        //if (hit_cnt == 0)
        //{
        //    hitcolor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        //    hit_cnt = 200;
        //}
    //}
    //else if (dead && revive_timer > 0)
    //{
    //    revive_timer -= 1.0f;
    //}
    //else if (dead && revive_timer == 0)
    //{
    //    life = { 6 };
    //    dead = { false };
    //    dying = { false };
    //    revive_timer = { 1200.0f };
    //    characterPos = { -37.3051f, 0.0f, 42.5001f };
    //    hit_cnt = { 200 };
    //    firing = false;
    //    firing_induration = false;
    //}
}

void MainCharacter::Drawshadow(float angle, GLuint depthShaderProgram, const glm::mat4& lightSpaceMatrix)
{
    //if (!dead)
    //{
        model = glm::mat4(1.0f);
        model = glm::translate(model, characterPos);
        if (!dying)
            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        else
            model = glm::rotate(model, lastangle, glm::vec3(0.0f, 1.0f, 0.0f));
        //if (finish)
        //    model = glm::scale(model, glm::vec3(20.0f));

        ModelLoc = glGetUniformLocation(depthShaderProgram, "model");
        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        animModel->SetupBoneTransforms(*player_BoneInfo, depthShaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    //}
}

void MainCharacter::SetRight_on(bool in)
{
    _Right = in;
}

void MainCharacter::SetLeft_on(bool in)
{
    _Left = in;
}

void MainCharacter::SetTop_on(bool in)
{
    _Top = in;
}

void MainCharacter::SetBottom_on(bool in)
{
    _Bottom = in;
}

void MainCharacter::Shift_on(bool in)
{
    _Shift = in;
}

//void MainCharacter::hitboxOnOff(bool in)
//{
//    hitbox_on = in;
//}

void MainCharacter::Walk() {
    //if (!getFirstPersonView()) {
        if (_Right/* && !wallcollapsed_d()*/)
            characterPos.x += 0.005f;
        if (_Left/* && !wallcollapsed_a()*/)
            characterPos.x -= 0.005f;
        if (_Top/* && !wallcollapsed_w()*/)
            characterPos.z -= 0.005f;
        if (_Bottom/* && !wallcollapsed_s()*/)
            characterPos.z += 0.005f;
    //}
    /*else {
        glm::vec3 forward(
            sin(getCamerahori()),
            0,
            cos(getCamerahori())
        );
        glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

        glm::vec3 moveDir(0.0f);
        if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
        if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
        if (Right_on && !wallcollapsed_right(right)) moveDir += right;
        if (Left_on && !wallcollapsed_left(right)) moveDir -= right;

        if (glm::length(moveDir) > 0) {
            moveDir = glm::normalize(moveDir);
            glm::vec3 nextPos = characterPos + moveDir * 0.005f;
            characterPos = nextPos;
        }
    }*/
}

void MainCharacter::Run()
{
    //if (!getFirstPersonView()) {
        if (_Right/* && !wallcollapsed_d()*/)
            characterPos.x += 0.012f;
        if (_Left/* && !wallcollapsed_a()*/)
            characterPos.x -= 0.012f;
        if (_Top/* && !wallcollapsed_w()*/)
            characterPos.z -= 0.012f;
        if (_Bottom/* && !wallcollapsed_s()*/)
            characterPos.z += 0.012f;
    //}
    //else {
    //    glm::vec3 forward(
    //        sin(getCamerahori()),
    //        0,
    //        cos(getCamerahori())
    //    );
    //    glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));
    //
    //    glm::vec3 moveDir(0.0f);
    //    if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
    //    if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
    //    if (Right_on && !wallcollapsed_right(right)) moveDir += right;
    //    if (Left_on && !wallcollapsed_left(right)) moveDir -= right;
    //
    //    if (glm::length(moveDir) > 0) {
    //        moveDir = glm::normalize(moveDir);
    //        glm::vec3 nextPos = characterPos + moveDir * 0.01f;
    //        characterPos = nextPos;
    //    }
    //}
    
}

//void MainCharacter::jump(bool in)
//{
//    /*if (in)
//        characterPos.y += 0.05;*/   // 점프는 나중에
//}

//void MainCharacter::stop()
//{
//    Right_on = { false };
//    Left_on = { false };
//    Top_on = { false };
//    Bottom_on = { false };
//    Shift_on = { false };
//    hitbox_on = { false };
//}

//void MainCharacter::changehitColor()
//{
//    if (hitcolor.g == 1.0f && hitcolor.b == 1.0f)
//    {
//        hitcolor.g = 0.6f;
//        hitcolor.b = 0.6f;
//    }
//    else if (hitcolor.g == 0.6f && hitcolor.b == 0.6f)
//    {
//        hitcolor.g = 1.0f;
//        hitcolor.b = 1.0f;
//    }
//}

//void MainCharacter::setlife()
//{
//    if (life > 1)
//    {
//        life -= 1;
//        hitcolor = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
//    }
//    else if (life > 0)
//    {
//        life -= 1;
//        hit_cnt = 200;
//        hitcolor = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
//        dying = true;
//        Right_on = { false };
//        Left_on = { false };
//        Top_on = { false };
//        Bottom_on = { false };
//        Shift_on = { false };
//        hitbox_on = { false };
//    }
//}

void MainCharacter::ChangeCatAnimation(const glm::mat4& view, const glm::mat4& projection)
{
	float firetimer;
	glm::vec3 mousePick;

	if (!GetDying())
	{
		if (animLibrary->getcurrentAnimation() == "FireRun")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim->Duration * 0.56f;

			if (player_CurrentAnim->CurrentTime >= firetimer && !Bullet_cnt[0])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[0] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 150.0f && !Bullet_cnt[1])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[1] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 300.0f && !Bullet_cnt[2])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[2] = true;
			}

			if (player_CurrentAnim->CurrentTime + 10.0f >= player_CurrentAnim->Duration)
			{
				if (!firing)
				{
					if (IsMoving())
					{
						if (Shift_value())
							animLibrary->changeAnimation("Run", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("Walk", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Idle", *player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (IsMoving())
					{
						if (!Shift_value())
							animLibrary->changeAnimation("FireWalk", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("FireRun", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Fire", *player_CurrentAnim);
				}

				for (bool& bullet_nums : Bullet_cnt)
					bullet_nums = false;
			}
		}
		else if (animLibrary->getcurrentAnimation() == "FireWalk")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim->Duration * 0.56f;

			if (player_CurrentAnim->CurrentTime >= firetimer && !Bullet_cnt[0])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[0] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 150.0f && !Bullet_cnt[1])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[1] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 300.0f && !Bullet_cnt[2])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[2] = true;
			}

			if (player_CurrentAnim->CurrentTime + 10.0f >= player_CurrentAnim->Duration)
			{
				if (!firing)
				{
					if (IsMoving())
					{
						if (Shift_value())
							animLibrary->changeAnimation("Run", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("Walk", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Idle", *player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (IsMoving())
					{
						if (Shift_value())
							animLibrary->changeAnimation("FireRun", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("FireWalk", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Fire", *player_CurrentAnim);
				}

				for (bool& bullet_nums : Bullet_cnt)
					bullet_nums = false;
			}
		}
		else if (animLibrary->getcurrentAnimation() == "Fire")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim->Duration * 0.56f;

			if (player_CurrentAnim->CurrentTime >= firetimer && !Bullet_cnt[0])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[0] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 150.0f && !Bullet_cnt[1])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[1] = true;
			}

			if (player_CurrentAnim->CurrentTime >= firetimer + 300.0f && !Bullet_cnt[2])
			{
				mousePick = camera->GetMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->BulletSetting(this, camera, mousePick);
				bullets.push_back(newBullet);
				Bullet_cnt[2] = true;
			}

			if (player_CurrentAnim->CurrentTime + 10.0f >= player_CurrentAnim->Duration)
			{
				if (!firing)
				{
					if (IsMoving())
					{
						if (Shift_value())
							animLibrary->changeAnimation("Run", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("Walk", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Idle", *player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (IsMoving())
					{
						if (Shift_value())
							animLibrary->changeAnimation("FireRun", *player_CurrentAnim);
						else
							animLibrary->changeAnimation("FireWalk", *player_CurrentAnim);
					}
					else
						animLibrary->changeAnimation("Fire", *player_CurrentAnim);
				}

				for (bool& bullet_nums : Bullet_cnt)
					bullet_nums = false;
			}
		}
		else if (animLibrary->getcurrentAnimation() == "Die")
			animLibrary->changeAnimation("Idle", *player_CurrentAnim);
	}
	else
	{
		if (animLibrary->getcurrentAnimation() != "Die")
			animLibrary->changeAnimation("Die", *player_CurrentAnim);
		else
		{
			if (player_CurrentAnim->CurrentTime + 10 >= player_CurrentAnim->Duration)
				SetDead(true);
		}
	}
}

void MainCharacter::SaveAnimations()
{
    animLibrary->loadAnimation("Idle", "Animations/cat_animation_idle.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Die", "Animations/cat_animation_die.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Walk", "Animations/cat_animation_walking.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Run", "Animations/cat_animation_run.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Hit", "Animations/cat_animation_hit.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Jump", "Animations/cat_animation_jump.glb", animationImporters, animModel);
    animLibrary->loadAnimation("AimIdle", "Animations/cat_animation_idle_aim.glb", animationImporters, animModel);
    animLibrary->loadAnimation("AimWalk", "Animations/cat_animation_walking_aim.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Fire", "Animations/cat_animation_firing_aim.glb", animationImporters, animModel);
    animLibrary->loadAnimation("FireWalk", "Animations/cat_animation_firing_walk.glb", animationImporters, animModel);
    animLibrary->loadAnimation("FireRun", "Animations/cat_animation_firing_run.glb", animationImporters, animModel);
    animLibrary->loadAnimation("Dance", "Animations/cat_animation_dance.glb", animationImporters, animModel);

    animLibrary->changeAnimation("Idle", *player_CurrentAnim);
}

void MainCharacter::SetAnimationType(const std::string& animName)
{
    if (animLibrary && player_CurrentAnim) {
        animLibrary->changeAnimation(animName, *player_CurrentAnim);
    }
}
