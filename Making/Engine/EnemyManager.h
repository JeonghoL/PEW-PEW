#pragma once

class Enemy;

class EnemyManager
{
public:
	EnemyManager();

	void Update();
	void Draw();
	void Release();

private:
	Enemy* enemy[3][9];
};