#include "RailTriggerPoint.h"

#include "../Boss1.h"
#include "../Player.h"

#include "../EffectPlayer.h"

#include <cmath>

void RailTriggerPoint::Awake()
{
	sceneName = SceneManager::GetInstance().GetName();

	if (!cam)
	{
		cam = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform();
	}

	if (sceneName == L"Directing Scene 0")
	{
		effectObj = CreateGameObjectFromFBX(L"VFX_Teleport");
		teleportEffectPlayer = effectObj->AddComponent<EffectPlayer>();
		teleportEffectPlayer->SetMeshRenderer(effectObj->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		teleportEffectPlayer->SetMaxValue(0.0f);
		teleportEffectPlayer->SetPlaySpeed(0.2f);
		teleportEffectPlayer->SetRemainTime(10.0f);
		teleportEffectPlayer->SetDirection(Vector2D(1, 0));
		effectObj->SetActive(false);
	}
}

void RailTriggerPoint::OnTriggerEnter()
{
	if (sceneName == L"Directing Scene 0")
	{
		switch (index)
		{
			case 0:
			{
				if (boss)
				{
					// 텔레포트 시작
					boss->SetActive(true);
					SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Telepo);

					effectObj->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));

					// 1. 포탈이 나온다
					effectObj->SetActive(true);
					teleportEffectPlayer->SetMaxValue(0.5f);
					effectPlay = true;
				}
			}
			break;
			case 1:
			{
				if (boss)
				{
					/// 바닥 내려치는 거
					SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern4);
					boss->GetComponent<Animator>()->Play(L"root|PhaseTransition");
				}
			}
			break;
			case 2:
			{
				if (player)
				{
					SoundManager::GetInstance().PlaySFX(SoundManager::Player::BasicAttack);
					player->GetComponent<Animator>()->Play(L"Armature|attack");
				}
			}
			break;
			case 7:
			{
				LoadScene(L"GameScene");
			}
			break;
		}
	}
	else if (sceneName == L"Directing Scene 1")
	{
		switch (index)
		{
			case 0:
			{
				dynamic_cam = true;
			}
			break;
			case 1:
			{
				count++;
			}
			break;
			case 7:
			{
				LoadScene(L"GameSceneBossPhase3");
			}
			break;
			default:
				break;
		}
	}
}

void RailTriggerPoint::Update()
{
	if (sceneName == L"Directing Scene 0")
	{
		switch (index)
		{
			case 0:
			{
				if (boss->GetRootActive())
				{
					float speed = TimeController::GetInstance().GetDeltaTime() * 3;
					auto beforePos = boss->GetTransform()->GetLocalPosition();
					beforePos.y += speed;

					if (effectPlay && beforePos.y >= 2.4f)
					{
						teleportEffectPlayer->SetMaxValue(0.0f);
						teleportEffectPlayer->InverseDirection();
						effectPlay = false;
					}

					if (beforePos.y <= 4.2f)
					{
						boss->GetTransform()->SetLocalPosition(beforePos);
					}
				}
			}
		}
	}
	else if (sceneName == L"Directing Scene 1")
	{
		switch (index)
		{
			case 0:
			{
				if (dynamic_cam)
				{
					//원운동 시작
					float dt = TimeController::GetInstance().GetDeltaTime();

					Vector3D curpos = cam->GetLocalPosition();
					Vector2D curpos2D = Vector2D(curpos.x, curpos.z);

					auto ang = DegreeToRadian(angV * dt);
					ang *= -1;

					Vector2D nextpos = { cos(ang) * curpos2D.x + sin(ang) * curpos2D.y, -sin(ang) * curpos2D.x + cos(ang) * curpos2D.y };
					cam->SetLocalPosition(Vector3D(nextpos.x, curpos.y, nextpos.y));
					cam->Rotate(Vector3D::up, ang);

					if (count >= 2)
					{
						dynamic_cam = false;
					}
				}
			}
			break;
			case 1:
			{
				if (count >= 2)
				{
					auto pos = cam->GetLocalPosition();

					auto dir = -pos;
					dir.y = 0;
					dir.Normalize();

					cam->SetLocalPosition(pos + dir * lineV);
				}
			}
		}
	}
}

void RailTriggerPoint::SetIndex(int index)
{
	this->index = index;
}

int RailTriggerPoint::GetIndex() const
{
	return index;
}

void RailTriggerPoint::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(RailTriggerPoint);

	PRESERIALIZE_VALUE(index);
}

void RailTriggerPoint::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(RailTriggerPoint);

	PREDESERIALIZE_VALUE(index);
}

void RailTriggerPoint::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(boss);
	POSTSERIALIZE_PTR(player);
	POSTSERIALIZE_PTR(collider);
}

void RailTriggerPoint::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(boss);
	POSTDESERIALIZE_PTR(player);
	POSTDESERIALIZE_PTR(collider);
}
