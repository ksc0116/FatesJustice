#pragma once

#include "ZeldaEngine.h"
#include "FSM.h"
#include <list>

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

// vector, map의 serialize가 필요하다.....

// 컴포넌트 만들 때는 Component 클래스를 상속받아야한다.
// 아래는 현재 사용 가능한 이벤트함수 목록
// void Awake();							게임오브젝트가 활성화될 때, 단 한번만 호출된다.
// void Start();							게임오브젝트가 활성화될 때, 단 한번만 Awake 이후에 호출된다.
// void OnEnable();							게임오브젝트가 활성화될 때, 매번 호출된다.
// void OnDisable();						게임오브젝트가 비활성화될 때, 매번 호출된다.
// void OnDestroy();						게임오브젝트가 제거될 때, 단 한번만 호출된다.
// 
// void OnTriggerEnter();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와 충돌을 시작한 순간 호출된다.
// void OnTriggerStay();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와 충돌한 상태를 유지할 때 호출된다.
// void OnTriggerExit();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와의 충돌에서 벗어나는 순간 호출된다.
// void OnTriggerEnter(Collider* other);	// rigidbody를 가진 오브젝트가 other과 충돌하는 순간 호출된다.				(한 프레임에 여러번 호출될 수 있다.)
// void OnTriggerStay(Collider* other);		// rigidbody를 가진 오브젝트가 other과 충돌한 상태를 유지할 때 호출된다.		(한 프레임에 여러번 호출될 수 있다.)
// void OnTriggerExit(Collider* other);		// rigidbody를 가진 오브젝트가 other과 충돌에서 벗어나는 순간 호출된다		(한 프레임에 여러번 호출될 수 있다.)
// 
// void EarlyUpdate();						// 매 프레임 한번씩만 호출된다. Update 전에 처리할 것이 있는 경우 사용한다.
// void Update();							// 매 프레임 한번씩만 호출된다. 기본적인 Update를 위한 함수
// void LateUpdate();						// 매 프레임 한번씩만 호출된다. Update 후에 처리할 것이 있는 경우 사용한다. (EX.카메라의 플레이어 위치로의 이동)

class Dotween;

enum class PlayerState
{
	Idle,
	Move,
	Attack,
	SpecialAttack,
	Interaction,
	StateEnd
};

class NDComponent : public ZeldaEngine::Component
{
public:
	NDComponent();
	//~NDComponent();

	void Awake();
	void Start();
	void Update();

private:
	FSM<PlayerState> playerFSM = FSM<PlayerState>(PlayerState::Idle);

	// 움직이는 속도
	float moveSpeed;
	// 회전 각 속도
	float rotationAngleSpeed;

	// 축이 근접할시 오차범위
	float errorRange;

	// 대쉬기 충전시간
	float dashChargingTime;

	// 플레이어의 월드 축
	GameObject* playerAxis;

	//
	GameObject* playerfrontObject;

	// 1. Dotween 컴포넌트 선언
	Dotween* playerDotween;

public:
	// Idle 상태로 진입 했을 때
	void IdleEngage();

	// Move 상태로 진입 했을 때
	void MoveEngage();

	// Attack 상태로 진입 했을 때
	void AttackEngage();

	// SpecialAttack 상태로 진입 했을 때
	void SpecialAttackEngage();

	// Interaction 상태로 진입 했을 때
	void InteractionEngage();


	// Idle 상태가 유지 중일 때
	void IdleState();

	// Move 상태가 유지 중일 때
	void MoveState();

	// Attack 상태가 유지 중일 때
	void AttackState();

	// SpecialAttack 상태가 유지 중일 때
	void SpecialAttackState();

	// Interaction 상태가 유지 중일 때
	void InteractionState();

private:
	// 플레이어의 움직임에 관한 함수
	void MoveEvent();

	// 회전할 최소각을 구한다.
	float GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle);

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};