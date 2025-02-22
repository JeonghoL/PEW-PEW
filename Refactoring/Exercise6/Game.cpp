#include "pch.h"
#include "GameManager.h"

GameManager Manager;

int main(int argc, char* argv[]) {
	Manager.winset(argc, argv);
	Manager.loopgame();
	Manager.deleteAll();
}