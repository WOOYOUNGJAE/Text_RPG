#include "stdafx.h"
#include "CShop.h"

CShop::CShop()
{
	m_pItemArr[ITEMS_MAX] = {};
}

CShop::~CShop()
{
	for (int i = 0; i < ITEMS_MAX; ++i)
	{
		DELETE_MAC(m_pItemArr[i]);
	}
}

void CShop::Initialize(CPlayer* _pPlayer, CInventory* _pInventory)
{
	pPlayer = _pPlayer;
	pInventory = _pInventory;

	m_pItemArr[0] = new CWeapon("초급 무기", 1001 ,10, 1, 1);
	m_pItemArr[1] = new CWeapon("중급 무기", 1002, 20, 2, 5);
	m_pItemArr[2] = new CWeapon("고급 무기", 1003, 30, 3, 10);

	// 방어구 3개
}

void CShop::Release()
{
}

void CShop::ShowItemList()
{
	cout << "현재 소지금: " << pPlayer->Get_PlayerOnlyInfo().iCurrnetMoney << endl << endl;
	for (size_t i = 0; i < ITEMS_MAX; i++)
	{
		//tEquipmentInfo& refTmpInfo = m_pItemArr[i]->GetEuipmentInfo(); 왜 안될까
		if (m_pItemArr[i])
		{

			cout << i + 1 << ") 아이템 이름: " << m_pItemArr[i]->GetEuipmentInfo().szName << endl;
			cout << i + 1 << ") 가격: " << m_pItemArr[i]->GetEuipmentInfo().iDefaultPrice << endl;
			cout << "-----------------------------\n";
		}
	}
}

void CShop::BuyMode()
{
	while (true)
	{
		system("cls");

		ShowItemList();

		cout << "버튼을 누르세요 (1~10.아이템 고르기 0.뒤로 가기): ";
		int iBtn = 0;
		cin >> iBtn;

		if (iBtn == 0)
		{
			return;
		}
		else
		{
			Try_to_Buy(iBtn);
		}

		system("pause");
	}
}

void CShop::SellMode()
{
	while (true)
	{
		system("cls");

		pInventory->RenderInventory();

		cout << "버튼을 누르세요 (1~10.아이템 고르기 0.뒤로 가기): ";
		int iBtn = 0;
		cin >> iBtn;

		if (iBtn == 0)
		{
			return;
		}
		else
		{
			cout << endl << iBtn << "번 아이템을 선택하였습니다." << endl;
			pInventory->Get_Inventory().at(iBtn - 1)->ShowDetails();
			cout << "판매 가격: " << ( m_pItemArr[iBtn - 1]->GetEuipmentInfo().iDefaultPrice >> 1 ) << endl; // 판매 가격 반값
			cout << "-----------------------------\n";
			cout << "버튼을 누르세요 (1.판매하기 2.이전으로): ";
			int _iBtn = 0;
			cin >> _iBtn;
			switch (_iBtn)
			{
			case 1: // 판매
			{
				CItemBase*& ref_pItem = pInventory->Get_Inventory().at(iBtn - 1);
				if (ref_pItem->Get_IfEquipting() == true)
				{
					cout << "장착 중인 아이템은 판매 할 수 없습니다." << endl;
					return;
				}

				pPlayer->Set_CurrentMoney(ref_pItem->GetEuipmentInfo().iDefaultPrice >> 1);
				DELETE_MAC(ref_pItem);
				pInventory->Get_Inventory().erase(pInventory->Get_Inventory().begin() + iBtn - 1); // 삭제
				break;
			}
			case 2: // 이전으로
				return;
			default:
				return;
			}
		}

		system("pause");
	}
}

void CShop::Try_to_Buy(int _iInput)
{
	cout << endl << _iInput << "번 아이템을 선택하였습니다." << endl;
	cout << "버튼을 누르세요 (1.구매하기 2.자세히 3.이전으로): ";
	int _iBtn = 0;
	cin >> _iBtn;
	switch (_iBtn)
	{
	case 1:
		if (CalculateMoney(m_pItemArr[_iInput - 1],
			&(pPlayer->Get_PlayerOnlyInfo())) == true)
		{
			BuyComplete(_iInput);
		}
		else
		{
			cout << "돈이 부족합니다." << endl;
		}
		break;
	case 2:
		cout << "가격: " << m_pItemArr[_iInput - 1]->GetEuipmentInfo().iDefaultPrice << endl;
		m_pItemArr[_iInput - 1]->ShowDetails();
		break;
	case 3:
		return;
	default:
		break;
	}

}

void CShop::ShowDetails(CItemBase* _pItem)
{
//	_pItem->ShowDetails();
//
//	cout << "아이템 이름: " << _pItem->GetEuipmentInfo().szName << endl;
//	cout << "레벨 제한: " << _pItem->GetEuipmentInfo().iMinLevel << endl;
//
//	if (dynamic_cast<CWeapon*>(_pItem))
//	{
//		cout << "공격력 증가: +" << _pItem->GetEuipmentInfo().iAtkPlus << endl;
//
//	}
//	//else if (typeid(m_pItemArr[i]) == typeid(CArmor))
//	else
//	{
//		cout << "오류: 무기도 방어구도 아님" << endl;
//	}
//	cout << "-----------------------------\n";
}

bool CShop::CalculateMoney(CItemBase* _pItem, tPlayerOnlyInfo* _playerOnly)
{
	if (_pItem->GetEuipmentInfo().iDefaultPrice <= _playerOnly->iCurrnetMoney)
	{
		_playerOnly->iCurrnetMoney -= _pItem->GetEuipmentInfo().iDefaultPrice;
		return true;
	}
	else
	{
		return false;
	}
}

void CShop::BuyComplete(int _iInput)
{
	if (dynamic_cast<CWeapon*>(m_pItemArr[_iInput - 1])) // 무기면
	{
		CWeapon* pWeapon_ToBeAdded = new CWeapon;
		memcpy_s(pWeapon_ToBeAdded, sizeof(CWeapon), m_pItemArr[_iInput - 1], sizeof(CWeapon));
		pInventory->Get_Inventory().push_back(pWeapon_ToBeAdded);
	}
	else // 수정 필요
	{
		CItemBase* pTmpItem = m_pItemArr[_iInput - 1];
		pInventory->Get_Inventory().push_back(pTmpItem);
	}
}