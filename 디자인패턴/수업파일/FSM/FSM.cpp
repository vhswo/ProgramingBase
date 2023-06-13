#include <iostream>
#include "Miner.h"
#include "ConsoleUtils.h"
#include "EntityNames.h"

/*
HP 를 소모해서 적을 쓰러트린다, EXP 를 정산한다, 레벨을 올린다

레벨이 올라가면 새로운 무기를 사러 되돌아간다

HP 가 다 되기 전에 체력을 회복하러 되돌아간다

*/

int main()
{
	Miner miner(ent_Miner_Bob);

	for (int i = 0; 20 > i; i++)
	{
		miner.Update();
		Sleep(1000);
	}

	PressAnyKeyToContinue();
}