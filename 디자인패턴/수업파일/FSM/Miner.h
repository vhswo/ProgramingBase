#pragma once
#include "BaseGameEntity.h"
#include "Locations.h"

#define COMFORT_LEVEL		5	// 안락함을 느끼는 금덩이의 양.
#define MAX_NUGGETS			3	// 최대 소지금.
#define THIRST_LEVEL		5	// 갈증 한계.
#define TIREDNESS_THRESHOLD	5	// 피로 한계.

class Miner; // 이걸 왜 먼저 선언했는지

class State // 스테이트를 순수 가상함수로 만들어둔다
{
public:
	virtual ~State() {}

	virtual void Enter(Miner*) abstract;
	virtual void Excute(Miner*) abstract;
	virtual void Exit(Miner*) abstract;
};

class Miner : public BaseGameEntity // NPC(베이스게임엔타이트) 를 상속한다  광부
{
private: // 광부가 필요한 스테이트 
	State*			m_pCurrState; //상태확인을 위한 것
	Location_Type	m_Location;  //이동 가능한 공간
	int				m_nGoldCarried;	// 광부가 광산에서 캔 금덩이의 양.
	int				m_nMoneyInBank;	// 저축 금액.
	int				m_nThirst;		// 갈증도.
	int				m_nFatigue;		// 피로도.

public:
	Miner(const int& id); // NPC의 번호를 받아 하나의 객체를 생성
	virtual ~Miner() {}

	//주요 함수
	void Update() override;   //지속적인 상태를 알려주는 함수
	void ChangeState(State*);  // 상태에 따른 장소 변경	
		
	Location_Type Location() const { return m_Location; }
	void ChangeLocation(const Location_Type& loc_type) { m_Location = loc_type; }

	int GoldCarried() const { return m_nGoldCarried; }
	void SetGoldCarried(const int& value) { m_nGoldCarried = value; }
	void AddToGoldCarried(const int&);
	bool PocketsFull() const { return (MAX_NUGGETS <= m_nGoldCarried); }

	bool Fatigued() const;
	void DecreaseFatigue() { m_nFatigue--; }
	void IncreaseFatigue() { m_nFatigue++; }

	int Wealth() const { return m_nMoneyInBank; }
	void SetWealth(const int& value) { m_nMoneyInBank = value; }
	void AddToWealth(const int&);

	bool Thirsty() const;
	void BuyAndDrinkAWhiskey() { m_nThirst = 0; m_nMoneyInBank -= 2; }
};