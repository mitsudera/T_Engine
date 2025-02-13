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




	// 名前が重複しないことを確認
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

	// 名前を設定
	Object::SetName(name, count);
	return;

}

void Assets::SetName(string name)
{
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
