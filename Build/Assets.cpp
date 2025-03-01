#include "Assets.h"
#include "AssetsManager.h"
Assets::Assets()
{
	type = Object::Type::Assets;
}

Assets::~Assets()
{
}

Assets::AssetsType Assets::GetAssetsType(void)
{
	return assetsType;
}

void Assets::SetName(string name, int count)
{




	// ���O���d�����Ȃ����Ƃ��m�F
	for (Assets* assets : pAssetsManager->GetAssetsList())
	{
		if (assets->GetAssetsType() != assetsType)
			continue;

		if (count == 0)
		{
			if (assets == this && assets->GetName() == name)
			{
				Object::SetName(name, count);
				return;

			}


			if (assets->GetName() == name)
			{
				SetName(name, count + 1);
				return;
			}

		}
		else
		{

			if (assets == this && assets->GetName() == name + "(" + to_string(count) + ")")
			{
				Object::SetName(name, count);
				return;
			}

			if (assets->GetName() == (name + "(" + to_string(count) + ")"))
			{
				SetName(name, count + 1);
				return;
			}

		}

	}

	// ���O��ݒ�
	Object::SetName(name, count);
	return;

}

void Assets::SetName(string name)
{
	// �Ō�� '/' �̈ʒu��������
	size_t pos = name.rfind('/');

	// �Ō�� '/' �����������ꍇ
	if (pos != string::npos) {
		// '/' ����̕�����؂�o��
		name = name.substr(pos + 1);
	}
	// SetName(name) ���Ăяo��
	SetName(name, 0);
}
void Assets::SetConstName(string name)
{
	// �Ō�� '/' �̈ʒu��������
	size_t pos = name.rfind('/');

	// �Ō�� '/' �����������ꍇ
	if (pos != string::npos) {
		// '/' ����̕�����؂�o��
		name = name.substr(pos + 1);
	}
	// SetName(name) ���Ăяo��
	Object::SetName(name);
}
void Assets::SetPath(string path)
{
	this->path = path;
}

string Assets::GetPath(void)
{
	return path;
}

