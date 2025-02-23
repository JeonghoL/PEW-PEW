#include "pch.h"
#include "GameManager.h"

int main(int argc, char* argv[]) {
	GameManager Manager;

	Manager.winset(argc, argv);
	Manager.loopgame();
	Manager.deleteAll();
}