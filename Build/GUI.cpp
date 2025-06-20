#include "GUI.h"
#include "GameEngine.h"
#include "renderer.h"
#include "World.h"
#include "gameobject.h"
#include "Scene.h"
#include "ProjectSetting.h"
#include "AssetsManager.h"
#include "Assets.h"
#include "Material.h"
#include "TerrainMaterial.h"
#include "DX11Texture.h"
#include "EditerCamera.h"
#include "SaveSystem.h"
#include "SceneManager.h"
#include "SceneAssetsData.h"


//com
#include "component.h"
#include "transformcomponent.h"
#include "CameraComponent.h"
#include "AnimationControlerComponent.h"
#include "ColliderComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "primitivecomponent.h"
#include "MeshComponent.h"
#include "QuadComponent.h"
#include "SpriteComponent.h"
#include "TerrainColliderComponent.h"
#include "TerrainComponent.h"
#include "TextMeshComponent.h"
#include "TextWriteComponent.h"
#include "RigidBodyComponent.h"
#include "SkinMeshLinkerComponent.h"
#include "SkinMeshComponent.h"
#include "BoneComponent.h"
#include "SpriteComponent.h"
#include "SoundSpeakerComponent.h"
#include "TerrainColliderComponent.h"
#include "PointColliderComponent.h"
#include "LineColliderComponent.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "RotBoxColliderComponent.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND g_hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr float radianToDegree = (180.0f / XM_PI);
constexpr float degreeToRadian = (XM_PI / 180.0f);



GUI::GUI(World* world)
{
    bool result;

    pWorld = world;
    pGameEngine = world->GetGameEngine();
    pRenderer = pGameEngine->GetRenderer();
    pProjectSetting = pGameEngine->GetProjectSetting();
    pAssetsManager = pGameEngine->GetAssetsManager();
    pEditerCamera = pWorld->GetEditerCamera();
    pSaveSystem = pGameEngine->GetSaveSystem();
    pSceneManager = pWorld->GetSceneManager();
    ID3D11Device* pDevice = pRenderer->GetDevice();
    ID3D11DeviceContext* pImmediateContext = pRenderer->GetDeviceContext();
    HWND hwnd = pGameEngine->GetWindowHandle();

    // imgui初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    guiIO = ImGui::GetIO();
    guiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    guiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    guiIO.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::StyleColorsClassic();
    result = ImGui_ImplWin32_Init(hwnd);
    result = ImGui_ImplDX11_Init(pDevice, pImmediateContext);

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    gizmoOperation = ImGuizmo::TRANSLATE;	// ギズモの初期モードのセット
    lastSelectSceneOrGameObject = nullptr;
}

GUI::~GUI()
{
    // imgui 終了処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Update(void)
{
    // imgui更新
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    //imguiIOの取得
    ImGuiIO& io = ImGui::GetIO();
    XMFLOAT2 windowSize =pGameEngine->GetWindowSize();
    //ビューポート表示サイズの取得
    XMFLOAT2 viewportSize = XMFLOAT2(windowSize.x, windowSize.y);
    XMFLOAT2 viewportPos = XMFLOAT2(windowSize.x * 0.0f, windowSize.y * 0.0f);

    //renderer->GetViewPort(viewportSize, viewportPos);
    io.DisplaySize.x = viewportSize.x;
    io.DisplaySize.y = viewportSize.y;

    ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);


    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) == false)
    {
        if (ImGui::IsKeyDown(ImGuiKey_W))
            gizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_E))
            gizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_R))
            gizmoOperation = ImGuizmo::SCALE;
        if (ImGui::IsKeyDown(ImGuiKey_Q))
            gizmoOperation = ImGuizmo::BOUNDS;
    }


    XMStoreFloat4x4(&proj, pEditerCamera->GetProjectionMatrix());
    XMStoreFloat4x4(&view, pEditerCamera->GetViewMatrix());

    UpdateToolWindow();
    UpdateHierarchyWindow();
    UpdateAssetsWindow();
    UpdateInspectorWindow();
    UpdateGizmo();
}

void GUI::Draw(void)
{
    pRenderer->SetRenderTargetBackBuffer();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();
}

void GUI::UpdateToolWindow(void)
{
    XMFLOAT2 screen = pGameEngine->GetWindowSize();
    // ウィンドウサイズの取得
    float sizeX = screen.x * 0.6f;
    float sizeY = screen.y * 0.1f;

    ImGui::SetNextWindowPos(ImVec2(screen.x*0.2f, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Once);
    ImGui::Begin("Tool", NULL, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {

        if (ImGui::MenuItem("Play"))
        {

            pGameEngine->TestPlay();
        }
        if (ImGui::MenuItem("SaveProject"))
        {

            pSaveSystem->SaveProject();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();

}

void GUI::UpdateHierarchyWindow(void)
{
    XMFLOAT2 screen = pGameEngine->GetWindowSize();
    // ウィンドウサイズの取得
    float sizeX = screen.x * 0.2f;
    float sizeY = screen.y * 0.7f;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Once);
    ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_MenuBar);

    if (selectedObject)
    {
        if (selectedObject->GetType() == Object::Type::Scene)
        {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("CreateGameObject"))
                {

                    for (string typeName : pProjectSetting->GetDefaultGameObjectTypeNameArray())
                    {
                        string removeName = "class ";
                        string showName = typeName;
                        showName.erase(0, removeName.size() - 1);
                        if (ImGui::MenuItem(showName.c_str()))
                        {
                            Scene* scene = static_cast<Scene*>(selectedObject);

                            scene->DynamicCreateGameObjectByTypeName(typeName);

                        }

                    }


                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }


        }
        else if (selectedObject->GetType() == Object::Type::GameObject)
        {

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("CreateGameObject"))
                {

                    for (string typeName : pProjectSetting->GetDefaultGameObjectTypeNameArray())
                    {
                        string removeName = "class ";
                        string showName = typeName;
                        showName.erase(0, removeName.size() - 1);
                        if (ImGui::MenuItem(showName.c_str()))
                        {
                            GameObject* gameObject = static_cast<GameObject*>(selectedObject);

                            GameObject*newObj= gameObject->DynamicCreateChildByTypeName(typeName);

                        }

                    }


                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }



        }

    }
    else
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("LoadScene"))
            {
                {
                    string text = loadSceneName;
                    vector<char> inputText(text.begin(), text.end());
                    inputText.resize(128);

                    if (ImGui::InputText("##FileName", inputText.data(), inputText.size()))
                    {
                        text = inputText.data();
                        loadSceneName = text;
                    }

                }

                if (ImGui::MenuItem("Load"))
                {
                    pSceneManager->LoadScene(loadSceneName);


                    loadSceneName = "";
                }


                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

    }








    for (Scene* scene : pWorld->GetActiveSceneList())
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once); // 初期状態を開いた状態にする
        ImGuiTreeNodeFlags nodeFlags = (selectedObject == scene) ? ImGuiTreeNodeFlags_Selected : 0;
        nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow; // ここでフラグを追加する


        bool open = ImGui::TreeNodeEx(scene->GetName().c_str(), nodeFlags);

        if (ImGui::IsItemClicked())
        {
            selectedObject = scene;  // 選択されたオブジェクトを記憶


        }

        if (open)
        {

            ShowObject(scene);

            ShowAllGameObject(scene);
            ImGui::TreePop();
        }


    }

    ImGui::End();
}

void GUI::UpdateInspectorWindow(void)
{
    XMFLOAT2 screen = pGameEngine->GetWindowSize();
    // ウィンドウサイズの取得
    float sizeX = screen.x * 0.2f;
    float sizeY = screen.y * 0.7f;


    float posX = screen.x * 0.8f;
    float posY = 0.0f;

    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Once);
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_MenuBar);

    if (selectedObject == nullptr)
    {
        ImGui::End();
        return;
    }

    {
        string text = selectedObject->GetName();
        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);

        ImGui::Text("Name:");
        ImGui::SameLine();
        if (ImGui::InputText("##Name", inputText.data(), inputText.size()))
        {
            text = inputText.data();
            selectedObject->SetName(text);
        }

        string idText = "ID:" + to_string(selectedObject->GetID());
        ImGui::Text(idText.c_str());


    }
    if (selectedObject->GetType() == Object::Type::Scene)
    {
        Scene* scene = static_cast<Scene*>(selectedObject);
        lastSelectSceneOrGameObject = scene;

        if (ImGui::BeginMenuBar()) {


            if (ImGui::MenuItem("SaveScene"))
            {
                pSaveSystem->SaveScene(scene);

            }

            ImGui::EndMenuBar();
        }


    }

    if (selectedObject->GetType() == Object::Type::GameObject)
    {
        GameObject* gameObject = static_cast<GameObject*>(selectedObject);
        lastSelectSceneOrGameObject = gameObject;

        bool active = gameObject->GetActive();

        ImGui::Text("Active:");
        ImGui::SameLine();

        if (ImGui::Checkbox("##Active", &active))
        {
            gameObject->SetActive(active);

        }

        bool notAnim = gameObject->GetNotAnim();

        ImGui::Text("NotAnim:");
        ImGui::SameLine();

        if (ImGui::Checkbox("##NotAnim", &notAnim))
        {
            gameObject->SetNotAnim(notAnim);

        }

        remCom = nullptr;
        for (Component* com : gameObject->GetComponentList())
        {



            ShowComponent(com);




        }
        if (remCom)
        {
            gameObject->RemoveComponent(remCom);
        }


        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("LoadFile"))
            {
                {
                    string text = loadFileName;
                    vector<char> inputText(text.begin(), text.end());
                    inputText.resize(128);

                    if (ImGui::InputText("##FileName", inputText.data(), inputText.size()))
                    {
                        text = inputText.data();
                        loadFileName = text;
                    }

                }

                if (ImGui::MenuItem("LoadMesh"))
                {
                    gameObject->DynamicLoadFbxFileMesh(loadFileName);
                    loadFileName = "";
                }

                if (ImGui::MenuItem("LoadSkinMesh"))
                {
                    gameObject->DynamicLoadFbxFileSkinMesh(loadFileName);
                    loadFileName = "";

                }


                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("AddComponent"))
            {
                for (string typeName : pProjectSetting->GetComponentNameArray())
                {
                    string removeName = "class ";
                    string showName = typeName;
                    showName.erase(0, removeName.size() - 1);
                    if (ImGui::MenuItem(showName.c_str()))
                    {
                        gameObject->DynamicAddComponentByTypeName(typeName);
                    }

                }



                ImGui::EndMenu();
            }


            if (ImGui::MenuItem("Remove"))
            {
                gameObject->GetScene()->DynamicDeleteGameObject(gameObject);
                selectedObject = nullptr;


            }
            if (ImGui::MenuItem("Clone"))
            {
                gameObject->GetScene()->DynamicCroneGameObject(gameObject);

            }
            ImGui::EndMenuBar();
        }



    }

    if (selectedObject)
    {
        if (selectedObject->GetType() == Object::Type::Assets)
        {
            Assets* assets = static_cast<Assets*>(selectedObject);

            ShowAssets(assets);
        }

    }





    ImGui::End();



}

void GUI::UpdateAssetsWindow(void)
{

    XMFLOAT2 screen = pGameEngine->GetWindowSize();
    // ウィンドウサイズの取得
    float sizeX = screen.x;
    float sizeY = screen.y * 0.3f;


    float posX = 0.0f;
    float posY = screen.y * 0.7f;

    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Once);
    ImGui::Begin("Assets", NULL, ImGuiWindowFlags_MenuBar);


    ImGui::BeginTabBar("AssetsBar");

    if (ImGui::BeginTabItem("Scene")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::Scene)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Texture")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::Texture)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Material")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::Material)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("MeshData")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::MeshData)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }


        //if (ImGui::BeginMenuBar()) {

        //    if (ImGui::Button("CreateOnScene"))
        //    {

        //        if (Scene* scene = dynamic_cast<Scene*>(lastSelectSceneOrGameObject))
        //        {
        //            scene->DynamicCreateGameObjectByTypeName(typeid(GameObject).name())->LoadFbxFileMesh()
        //        }
        //    }


        //    ImGui::EndMenuBar();
        //}

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("SkinMeshData")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::SkinMeshData)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("AnimationData")) {

        for (Assets* assets : pAssetsManager->GetAssetsList())
        {
            if (assets->GetAssetsType() == Assets::AssetsType::AnimationData)
            {
                if (ImGui::Button(assets->GetName().c_str()))
                {
                    selectedObject = assets;
                }

            }
        }

        ImGui::EndTabItem();
    }


    ImGui::EndTabBar();
    ImGui::End();

}

void GUI::UpdateGizmo(void)
{



    // GameObjectのマニピュレータ
    if (GameObject* gameObject = dynamic_cast<GameObject*>(selectedObject))
    {
        TransformComponent* transform = gameObject->GetTransFormComponent();

        XMFLOAT4X4 Ftransform;
        XMStoreFloat4x4(&Ftransform, transform->GetWorldMtx());



       
        //ImGuizmo::SetDrawlist();

        ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
        if (ImGuizmo::Manipulate(*view.m, *proj.m, gizmoOperation, mCurrentGizmoMode, *Ftransform.m, NULL, false, NULL))
        {
            gameObject->GetTransFormComponent()->SetWorldMtx(XMLoadFloat4x4(&Ftransform));

        }



    }


}

void GUI::ShowAllGameObject(Scene* scene)
{
    for (GameObject* gameObject : scene->GetGameObject())
    {
        ImGuiTreeNodeFlags nodeFlags = (selectedObject == gameObject) ? ImGuiTreeNodeFlags_Selected : 0;
        nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow; // ここでフラグを追加する

        bool open = ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags);
        {
            if (ImGui::IsItemClicked())
            {
                selectedObject = gameObject;  // 選択されたオブジェクトを記憶


            }
            if (open)
            {
                ShowObject(gameObject);

                ShowAllGameObject(gameObject);
                ImGui::TreePop();

            }
        }
    }
}

void GUI::ShowAllGameObject(GameObject* gameObject)
{
    for (GameObject* child : gameObject->GetChild())
    {
        ImGuiTreeNodeFlags nodeFlags = (selectedObject == child) ? ImGuiTreeNodeFlags_Selected : 0;

        bool open = ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags);

        if (ImGui::IsItemClicked())
        {
            selectedObject = child;  // 選択されたオブジェクトを記憶


        }


        if (open)
        {

            ShowObject(child);
            ShowAllGameObject(child);
            ImGui::TreePop();
        }
    }
}

void GUI::ShowObject(Object* object)
{



    if (GameObject* gameObject = dynamic_cast<GameObject*>(object))
    {



        if (ImGui::IsItemHovered()&&ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            

            pEditerCamera->FocusObject(gameObject);
        }

    }

}

void GUI::ShowComponent(Component* com)
{

    string removeName = "class ";

    string showName = com->GetTypeName();
    showName.erase(0, removeName.size() - 1);
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);//初期状態を開いた状態にする


    if (ImGui::TreeNode(showName.c_str()))
    {
        if (ImGui::Button("Remove"))
        {
            remCom = com;
        }


        bool active = com->GetActive();



        ImGui::Text("Active:");
        ImGui::SameLine();
        if (ImGui::Checkbox("##Active", &active))
        {
            com->SetActive(active);

        }


        if (TransformComponent* transformCom = dynamic_cast<TransformComponent*>(com))
        {
            ShowTransformCom(transformCom);

        }

        if (CameraComponent* camCom = dynamic_cast<CameraComponent*>(com))
        {

            ShowCamCom(camCom);
        }

        if (TerrainComponent* terCom = dynamic_cast<TerrainComponent*>(com))
        {

            ShowTerrainComponent(terCom);
        }

        if (AnimationControlerComponent* aniCom = dynamic_cast<AnimationControlerComponent*>(com))
        {

            ShowAnimationControlerComponent(aniCom);
        }
        if (ColliderComponent* cCom = dynamic_cast<ColliderComponent*>(com))
        {
            ShowColliderComponent(cCom);
        }
        if (RigidBodyComponent* rCom = dynamic_cast<RigidBodyComponent*>(com))
        {
            ShowRigidBodyComponent(rCom);
        }
        if (BoneComponent * bCom = dynamic_cast<BoneComponent*>(com))
        {
            ShowBoneComponent(bCom);
        }


        ImGui::TreePop();
    }

}
void GUI::ShowTransformCom(TransformComponent* com)
{
    XMFLOAT2 screen = pGameEngine->GetWindowSize();

    XMFLOAT3 pos = com->GetPosition();
    float fPos[3] = { pos.x,pos.y,pos.z };




    XMFLOAT3 rot = com->GetRotation();

    float fRot[3] = { XMConvertToDegrees(rot.x),XMConvertToDegrees(rot.y),XMConvertToDegrees(rot.z) };

    XMFLOAT3 scl = com->GetScale();
    float fScl[3] = { scl.x,scl.y,scl.z };


    if (ImGui::Button("Reset"))
    {
        com->Reset();

    }
    // 位置の編集
    ImGui::Text("Pos");
    if (ImGui::DragFloat3("##Pos", (float*)&fPos))
    {
        com->SetPosition(fPos[0], fPos[1], fPos[2]);
    }

    // 回転の編集
    ImGui::Text("Rot");
    if (ImGui::DragFloat3("##Rot", (float*)&fRot))
    {
        com->SetRotation(XMConvertToRadians(fRot[0]), XMConvertToRadians(fRot[1]), XMConvertToRadians(fRot[2]));
    }

    // スケールの編集
    ImGui::Text("Scl");
    if (ImGui::DragFloat3("##Scl", (float*)&fScl))
    {
        com->SetScale(fScl[0], fScl[1], fScl[2]);
    }




}

void GUI::ShowColliderComponent(ColliderComponent* com)
{

    bool enable = com->GetEnable();
    
    XMFLOAT3 pivot;
    XMStoreFloat3(&pivot, com->GetPivot());
    float fPivot[3] = { pivot.x,pivot.y,pivot.z };

    ImGui::Text("enable");

    if (ImGui::Checkbox("##enable", &enable))
    {
        if (enable)
        {
            com->OnCollider();

        }
        else
        {
            com->OffCollider();
        }
    }

    // 位置の編集
    ImGui::Text("Pivot");
    if (ImGui::DragFloat3("##Pivot", (float*)&fPivot))
    {
        com->SetPivot(XMFLOAT3(fPivot[0], fPivot[1], fPivot[2]));
    }



    if (PointColliderComponent* pCom = dynamic_cast<PointColliderComponent*>(com))
    {
        // PointColliderComponent用のコード
    }
    else if (LineColliderComponent* lCom = dynamic_cast<LineColliderComponent*>(com))
    {
        // LineColliderComponent用のコード
    }
    else if (SphereColliderComponent* sCom = dynamic_cast<SphereColliderComponent*>(com))
    {
        float rad = sCom->GetCheckRadius();

        ImGui::Text("Radius");
        if (ImGui::DragFloat("##Radius", &rad))
        {
            sCom->SetRadius(rad);
        }


    }
    else if (BoxColliderComponent* bCom = dynamic_cast<BoxColliderComponent*>(com))
    {
        XMFLOAT3 size = bCom->GetSize();
        float fSize[3] = { size.x,size.y,size.z };
        ImGui::Text("Size");
        if (ImGui::DragFloat3("##Size", (float*)&fSize))
        {
            bCom->SetBox(XMFLOAT3(fSize[0], fSize[1], fSize[2]));
        }


    }
    else if (CapsuleColliderComponent* cCom = dynamic_cast<CapsuleColliderComponent*>(com))
    {
        XMFLOAT3 start = cCom->GetStart();
        float fStart[3] = { start.x,start.y,start.z };

        XMFLOAT3 end = cCom->GetEnd();
        float fEnd[3] = { end.x,end.y,end.z };

        float rad = cCom->GetRadius();
        //
        ImGui::Text("Start");
        if (ImGui::DragFloat3("##Start", (float*)&fStart))
        {
            cCom->SetStart(XMFLOAT3(fStart[0], fStart[1], fStart[2]));
        }
        //
        ImGui::Text("End");
        if (ImGui::DragFloat3("##End", (float*)&fEnd))
        {
            cCom->SetEnd(XMFLOAT3(fEnd[0], fEnd[1], fEnd[2]));
        }
        //
        ImGui::Text("Radius");
        if (ImGui::DragFloat("##Radius", &rad))
        {
            cCom->SetRadius(rad);
        }

    }
    else if (RotBoxColliderComponent* rCom = dynamic_cast<RotBoxColliderComponent*>(com))
    {
        XMFLOAT3 size = rCom->GetSize();
        float fSize[3] = { size.x,size.y,size.z };
        ImGui::Text("Size");
        if (ImGui::DragFloat3("##Size", (float*)&fSize))
        {
            rCom->SetRotBox(XMFLOAT3(fSize[0], fSize[1], fSize[2]));
        }
    }
}

void GUI::ShowRigidBodyComponent(RigidBodyComponent* com)
{
    bool useGravity = com->GetUseGarvity();
    bool isKinematic = com->GetIsKinematic();
    bool isStatic = com->GetIsStatic();
    bool isFixTerrain = com->GetIsFixTerrain();

    float mass = com->GetMass();
    float drag = com->GetDrag();
    float friction = com->GetFriction();
    float angularDrag = com->GetAngularDrag();

    ImGui::Text("UseGravity");
    if (ImGui::Checkbox("##UseGravity", &useGravity))
    {
        com->SetUseGarvity(useGravity);
    }

    ImGui::Text("IsKinematic");
    if (ImGui::Checkbox("##IsKinematic", &isKinematic))
    {
        com->SetIsKinematic(isKinematic);
    }

    ImGui::Text("IsStatic");
    if (ImGui::Checkbox("##IsStatic", &isStatic))
    {
        com->SetIsStatic(isStatic);
    }

    ImGui::Text("IsFixTerrain");
    if (ImGui::Checkbox("##IsFixTerrain", &isFixTerrain))
    {
        com->SetIsFixTerrain(isFixTerrain);
    }


    ImGui::Text("Mass");
    if (ImGui::DragFloat("##Mass", &mass))
    {
        com->SetMass(mass);
    }

    ImGui::Text("Drag");
    if (ImGui::DragFloat("##Drag", &drag))
    {
        com->SetDrag(drag);
    }

    ImGui::Text("Friction");
    if (ImGui::DragFloat("##Friction", &friction))
    {
        com->SetFriction(friction);
    }

    ImGui::Text("AngularDrag");
    if (ImGui::DragFloat("##AngularDrag", &angularDrag))
    {
        com->SetAngularDrag(angularDrag);
    }

    float oy = com->offsetY;

    ImGui::Text("OffSetY");
    if (ImGui::DragFloat("##OffSetY", &oy))
    {
        com->offsetY = oy;
    }



}

void GUI::ShowBoneComponent(BoneComponent* com)
{
    bool isPhysics = com->GetIsPhysics();

    ImGui::Text("IsPhysics");
    if (ImGui::Checkbox("##IsPhysics", &isPhysics))
    {
        com->SetIsPhysics(isPhysics);
    }






    if (isPhysics)
    {
        // メニュー項目を保持するベクトル
        vector<string> items = { "Standard", "Spring" ,"Hair"};
        BoneComponent::Joint joint = com->GetJoint();

        // ドロップダウンメニューを表示
        if (ImGui::BeginCombo("Joint", items[(int)joint].c_str()))
        {
            for (int i = 0; i < items.size(); i++)
            {
                bool isSelected = ((int)joint == i);
                if (ImGui::Selectable(items[i].c_str(), isSelected))
                {
                    joint = (BoneComponent::Joint)i;
                    com->SetJoint(joint);
                }
                // 既に選択されている項目にフォーカスする
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        float mass = com->GetMass();
        float tention = com->GetTension();
        float resist = com->GetResistance();

        ImGui::Text("Mass");
        ImGui::SameLine();
        if (ImGui::DragFloat("##Mass", &mass))
        {
            com->SetMass(mass);
        }

        ImGui::Text("Tention");
        ImGui::SameLine();
        if (ImGui::DragFloat("##Tention", &tention,100.0f))
        {
            com->SetTension(tention);
        }

        ImGui::Text("Resistance");
        ImGui::SameLine();
        if (ImGui::DragFloat("##Resistance", &resist))
        {
            com->SetResistance(resist);
        }



    }
}



void GUI::ShowAnimationControlerComponent(AnimationControlerComponent* com)
{
    float blendWeight = com->GetBlendWeight();
    ImGui::Text("BlendWeight");
    ImGui::SameLine();
    if (ImGui::DragFloat("##BlendWeight", &blendWeight, 0.01f, 0.0f, 1.0f))
    {
        com->SetBlendWeight(blendWeight);
    }


    float angle = com->GetAngle();
    ImGui::Text("Angle");
    ImGui::SameLine();
    if (ImGui::DragFloat("##Angle", &angle, 0.01f))
    {
        com->SetAngle(angle);
    }


    if (AnimationNode* dNode = com->GetDefaultNode())
    {

        for (int i = 0; i < 4; i++)
        {
            string text = com->GetLoadFileName(i);
            vector<char> inputText(text.begin(), text.end());
            inputText.resize(128);

            string tag = "##FileName" + to_string(i);

            if (ImGui::InputText(tag.c_str(), inputText.data(), inputText.size()))
            {
                text = inputText.data();
                com->SetLoadFileName(text, i);
            }

        }

        if (ImGui::Button("LoadAnim"))
        {
            com->LoadAnimation(TRUE);
        }
        if (ImGui::Button("LoadAnim2"))
        {
            com->LoadAnimation2(TRUE);
        }
        if (ImGui::Button("LoadAnim4"))
        {
            com->LoadAnimation4(TRUE);
        }

    }
    else
    {
        {
            string text = com->GetLoadFileName(0);
            vector<char> inputText(text.begin(), text.end());
            inputText.resize(128);

            if (ImGui::InputText("##FileName", inputText.data(), inputText.size()))
            {
                text = inputText.data();
                com->SetLoadFileName(text,0);
            }

        }

        if (ImGui::Button("LoadDefaultAnim"))
        {
            com->LoadDefaulAnimation(com->GetLoadFileName(0), com->GetLoadFileName(0));
            com->SetLoadFileName("",0);
        }

    }


    for (pair<AnimParameter, string> condition : com->GetConditionList())
    {

        bool value = condition.first.value;
        bool isTrigger = condition.first.isTrigger;


        if (ImGui::TreeNode(condition.second.c_str()))
        {

            if (ImGui::Checkbox("Value",&value))
            {
                com->SetCondition(condition.second, value);
            }

            if (ImGui::Checkbox("IsTrigger",&isTrigger))
            {
                com->SetIsTrigger(condition.second, isTrigger);
            }

            ImGui::TreePop();
        }
    }


    string text = com->createConditionName;
    vector<char> inputText(text.begin(), text.end());
    inputText.resize(128);

    string tag = "##CreateConditionName";

    if (ImGui::InputText(tag.c_str(), inputText.data(), inputText.size()))
    {
        text = inputText.data();
        com->createConditionName = text;
    }

    if (ImGui::Button("CreateCondition"))
    {
        AnimParameter para;
        para.isTrigger = FALSE;
        para.value = FALSE;
        com->CreateCondition(com->createConditionName, para);
    }

    for (AnimationNode* node : com->GetAnimNodeList())
    {

        if (ImGui::TreeNode(node->GetName().c_str()))
        {

            if (ImGui::TreeNode("Create"))
            {

                for (AnimationNode* anode : com->GetAnimNodeList())
                {
                    if (anode == node)
                        continue;




                    if (ImGui::TreeNode(anode->GetName().c_str()))
                    {

                        if (ImGui::Button("CreateExitTransition"))
                        {

                            com->CreateNotLoopAnimExitTransition(node->GetName(), anode->GetName());

                        }


                        for (pair<AnimParameter, string> condition : com->GetConditionList())
                        {
                            if (ImGui::TreeNode(condition.second.c_str()))
                            {

                                if (ImGui::Button("CreateTransition"))
                                {

                                    com->CreateTransition(node->GetName(), anode->GetName(), condition.second, TRUE);

                                }

                                ImGui::TreePop();
                            }

                        }

                        ImGui::TreePop();

                    }


                }

                ImGui::TreePop();

            }

            if (ImGui::TreeNode("Edit"))
            {



                for (AnimationTransition* transition : node->GetTransitionList())
                {
                    if (ImGui::TreeNode(transition->GetName().c_str()))
                    {
                        string text = transition->GetNeedConditionName();
                        vector<char> inputText(text.begin(), text.end());
                        inputText.resize(128);

                        string tag = "##NeedConditionNam";

                        if (ImGui::InputText(tag.c_str(), inputText.data(), inputText.size()))
                        {
                            text = inputText.data();
                            transition->SetNeedConditionName(text);
                        }

                        bool needCon = transition->GetNeedCondition();

                        if (ImGui::Checkbox("NeedCondition", &needCon))
                        {
                            transition->SetNeedCondition(needCon);
                        }


                        ImGui::TreePop();

                    }

                }
                ImGui::TreePop();

            }

            ImGui::TreePop();

        }

    }

}


void GUI::ShowAssets(Assets* assets)
{

    if (assets->GetAssetsType() == Assets::AssetsType::Material)
    {
        Material* mat = static_cast<Material*>(assets);

        ShowMaterial(mat);
    }

    if (assets->GetAssetsType() == Assets::AssetsType::Scene)
    {
        SceneAssetsData* sceneData = static_cast<SceneAssetsData*>(assets);

    }



}
void GUI::ShowMaterial(Material* mat)
{

    float diffuse[4] = { mat->diffuse.x,mat->diffuse.y,mat->diffuse.z,mat->diffuse.w };
    float ambient[4] = { mat->ambient.x,mat->ambient.y,mat->ambient.z,mat->ambient.w };
    float specular[4] = { mat->specular.x,mat->specular.y,mat->specular.z,mat->specular.w };
    float emission[4] = { mat->emission.x,mat->emission.y,mat->emission.z,mat->emission.w };
    float shininess = mat->shininess;

    // diffuseの編集
    ImGui::Text("Diffuse");
    if (ImGui::DragFloat4("##Diffuse", (float*)&diffuse, 0.01f))
    {
        diffuse[0] = clamp(diffuse[0], 0.f, 1.f);
        diffuse[1] = clamp(diffuse[1], 0.f, 1.f);
        diffuse[2] = clamp(diffuse[2], 0.f, 1.f);
        diffuse[3] = clamp(diffuse[3], 0.f, 1.f);
        mat->SetDiffuse(XMFLOAT4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
    }

    ImGui::Text("Ambient");
    if (ImGui::DragFloat4("##Ambient", (float*)&ambient, 0.01f))
    {

        ambient[0] = clamp(ambient[0], 0.f, 1.f);
        ambient[1] = clamp(ambient[1], 0.f, 1.f);
        ambient[2] = clamp(ambient[2], 0.f, 1.f);
        ambient[3] = clamp(ambient[3], 0.f, 1.f);

        mat->SetAmbient(XMFLOAT4(ambient[0], ambient[1], ambient[2], ambient[3]));
    }

    ImGui::Text("Specular");
    if (ImGui::DragFloat4("##Specular", (float*)&specular, 0.01f))
    {
        specular[0] = clamp(specular[0], 0.f, 1.f);
        specular[1] = clamp(specular[1], 0.f, 1.f);
        specular[2] = clamp(specular[2], 0.f, 1.f);
        specular[3] = clamp(specular[3], 0.f, 1.f);

        mat->SetSpecular(XMFLOAT4(specular[0], specular[1], specular[2], specular[3]));
    }

    ImGui::Text("Emission");
    if (ImGui::DragFloat4("##Emission", (float*)&emission, 0.01f))
    {
        emission[0] = clamp(emission[0], 0.f, 1.f);
        emission[1] = clamp(emission[1], 0.f, 1.f);
        emission[2] = clamp(emission[2], 0.f, 1.f);
        emission[3] = clamp(emission[3], 0.f, 1.f);

        mat->SetEmission(XMFLOAT4(emission[0], emission[1], emission[2], emission[3]));
    }

    ImGui::Text("Shininess");
    if (ImGui::DragFloat("##Shininess", &shininess, 1.f))
    {
        mat->SetShininess(shininess);
    }




    if (TerrainMaterial* tmat = dynamic_cast<TerrainMaterial*>(mat))
    {
        ShowTerrainMaterial(tmat);
    }

    {//diftex
        string text = mat->GetDifPath();

        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);


        ImGui::Text("LoadDiffuseTex");
        if (ImGui::InputText("##LoadDiffuseTex", inputText.data(), inputText.size()))
        {
            text = inputText.data();

            mat->SetDifPath(text);

        }
        ImGui::SameLine();
        if (ImGui::Button("LoadDiffuseTexture"))
        {
            mat->LoadDiffuseTex(mat->GetDifPath());
        }

    }

    {//nortex
        string text = mat->GetNorPath();

        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);


        ImGui::Text("LoadNormalTex");
        if (ImGui::InputText("##LoadNormalTex", inputText.data(), inputText.size()))
        {
            text = inputText.data();

            mat->SetNorPath(text);

        }
        ImGui::SameLine();
        if (ImGui::Button("LoadNormalTexture"))
        {
            mat->LoadNormalTex(mat->GetNorPath());
        }

    }

    {//spetex
        string text = mat->GetSpePath();

        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);


        ImGui::Text("LoadSpeculerTex");
        if (ImGui::InputText("##LoadSpeculerTex", inputText.data(), inputText.size()))
        {
            text = inputText.data();

            mat->SetSpePath(text);

        }
        ImGui::SameLine();
        if (ImGui::Button("LoadSpeculerTexture"))
        {
            mat->LoadSpeculerTex(mat->GetSpePath());
        }

    }



}
void GUI::ShowSceneAssets(SceneAssetsData* sad)
{
    string text = sad->GetName();
    vector<char> inputText(text.begin(), text.end());
    inputText.resize(128);

    ImGui::Text("Name:");
    ImGui::SameLine();
    if (ImGui::InputText("##Name", inputText.data(), inputText.size()))
    {
        text = inputText.data();
        sad->SetName(text);
    }


}
void GUI::ShowTerrainMaterial(TerrainMaterial* mat)
{



    float heightFacter = mat->GetHeightFacter();

    ImGui::Text("HeightFacter");
    if (ImGui::DragFloat("##HeightFacter", &heightFacter, 1.f))
    {

        mat->SetHeightFacter(heightFacter);
    }

    
}
void GUI::ShowTerrainComponent(TerrainComponent* com)
{


    {//tertex
        string text = com->GetHeightMapPath();

        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);


        ImGui::Text("LoadHeightMap");
        if (ImGui::InputText("##LoadHeightMap", inputText.data(), inputText.size()))
        {
            text = inputText.data();

            com->SetHeightMapPath(text);

        }
        ImGui::SameLine();
        if (ImGui::Button("LoadHeightMap"))
        {
            com->LoadHeightMap(com->GetHeightMapPath());
        }

    }


    float heightFacter = com->GetHeightFacter();

    ImGui::Text("HeightFacter");
    if (ImGui::DragFloat("##HeightFacter", &heightFacter, 1.f))
    {

        com->SetHeightFacter(heightFacter);
    }




}
void GUI::ShowCamCom(CameraComponent* com)
{

    string clearMode;

    if (pWorld->GetMainCamera() != com)
    {
        if (ImGui::Button("SetMainCamera"))
        {
            com->SetMainCamera();
        }

    }



    CameraComponent::ClearMode cm = com->GetClearMode();

    switch (cm)
    {
    case CameraComponent::ClearMode::None:
        clearMode = "NONE";
        break;
    case CameraComponent::ClearMode::Color:
        clearMode = "Color";

        break;
    case CameraComponent::ClearMode::SkySphere:
        clearMode = "Sky";

        break;
    default:
        break;
    }

    string str = "ClearMode::" + clearMode;

    if (ImGui::BeginMenu(str.c_str()))
    {

        if (ImGui::MenuItem("NONE"))
        {
            com->SetClearMode(CameraComponent::ClearMode::None);
        }
        if (ImGui::MenuItem("Color"))
        {
            com->SetClearMode(CameraComponent::ClearMode::Color);
        }
        if (ImGui::MenuItem("Sky"))
        {
            com->SetClearMode(CameraComponent::ClearMode::SkySphere);
        }


        ImGui::EndMenu();

    }


    float angle = com->GetAngle() * radianToDegree;
    float nearz = com->GetNear();
    float farz = com->GetFar();
    // 視野角の編集
    ImGui::Text("Angle");
    if (ImGui::DragFloat("##Angle", &angle))
    {
        com->SetAngle(angle * degreeToRadian);
    }
    // nearの編集
    ImGui::Text("Near");
    if (ImGui::DragFloat("##Near", &nearz))
    {
        com->SetNear(nearz);
    }
    // farの編集
    ImGui::Text("Far");
    if (ImGui::DragFloat("##Far", &farz))
    {
        com->SetFar(farz);
    }



    if (ImGui::TreeNode("LayerCulling"))
    {

        for (string* layer: pProjectSetting->GetLayerList())
        {
            bool cull = com->GetLayerCulling(layer);
            
            ImGui::Text(layer->data());
            ImGui::SameLine();

            string label = "##" + *layer;

            if (ImGui::Checkbox(label.c_str(), &cull))
            {
                com->SetLayerCulling(layer, cull);

            }

        }

        ImGui::TreePop();

    }

}

BOOL GUI::GuiWndPro(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(g_hWnd, message, wParam, lParam)) return TRUE;
    return FALSE;
}
