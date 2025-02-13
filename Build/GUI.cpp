#include "GUI.h"
#include "GameEngine.h"
#include "renderer.h"
#include "World.h"
#include "gameobject.h"
#include "component.h"
#include "Scene.h"
#include "transformcomponent.h"
#include "CameraComponent.h"
#include "ProjectSetting.h"
#include "AssetsManager.h"
#include "Assets.h"
#include "Material.h"
#include "TerrainMaterial.h"
#include "DX11Texture.h"
#include "TerrainComponent.h"
#include "EditerCamera.h"
#include "AnimationControlerComponent.h"
#include "SaveSystem.h"
#include "SceneManager.h"
#include "SceneAssetsData.h"

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
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);//初期状態を開いた状態にする
        ImGuiTreeNodeFlags nodeFlags = (selectedObject == scene) ? ImGuiTreeNodeFlags_Selected : 0;

        if (ImGui::TreeNodeEx(scene->GetName().c_str(),nodeFlags))
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
        bool active = gameObject->GetActive();

        ImGui::Text("Active:");
        ImGui::SameLine();

        if (ImGui::Checkbox("##Active", &active))
        {
            gameObject->SetActive(active);

        }


        for (Component* com : gameObject->GetComponentList())
        {



            ShowComponent(com);




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
                        gameObject->AddComponentByTypeName(typeName);
                    }

                }



                ImGui::EndMenu();
            }


            if (ImGui::MenuItem("Remove"))
            {
                gameObject->GetScene()->DeleteGameObject(gameObject);
                selectedObject = nullptr;


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

    for (Assets* assets : pAssetsManager->GetAssetsList())
    {
        if (ImGui::Button(assets->GetName().c_str()))
        {
            selectedObject = assets;
        }

    }





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

        if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags))
        {

            ShowObject(gameObject);

            ShowAllGameObject(gameObject);
            ImGui::TreePop();
        }
    }
}

void GUI::ShowAllGameObject(GameObject* gameObject)
{
    for (GameObject* child : gameObject->GetChild())
    {
        ImGuiTreeNodeFlags nodeFlags = (selectedObject == gameObject) ? ImGuiTreeNodeFlags_Selected : 0;

        if (ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags))
        {
            ShowObject(child);
            ShowAllGameObject(child);
            ImGui::TreePop();
        }
    }
}

void GUI::ShowObject(Object* object)
{

    if (ImGui::IsItemClicked())
    {
        selectedObject = object;  // 選択されたオブジェクトを記憶


    }


    if (GameObject* gameObject = dynamic_cast<GameObject*>(object))
    {



        if (ImGui::IsItemClicked())
        {


            pEditerCamera->FocusObject(gameObject);
        }

    }

}

void GUI::ShowComponent(Component* com)
{
    string removeName = "class ";


    ImGui::SetNextItemOpen(true, ImGuiCond_Once);//初期状態を開いた状態にする
    if (TransformComponent* transformCom = dynamic_cast<TransformComponent*>(com))
    {
        string showName = typeid(TransformComponent).name();
        showName.erase(0, removeName.size() - 1);

        if (ImGui::TreeNode(showName.c_str()))
        {
            ShowTransformCom(transformCom);
            ImGui::TreePop();
        }

    }

    if (CameraComponent* camCom = dynamic_cast<CameraComponent*>(com))
    {
        string showName = typeid(CameraComponent).name();
        showName.erase(0, removeName.size() - 1);

        if (ImGui::TreeNode(showName.c_str()))
        {

            ShowCamCom(camCom);
            ImGui::TreePop();

        }
    }

    if (TerrainComponent* terCom = dynamic_cast<TerrainComponent*>(com))
    {
        string showName = typeid(TerrainComponent).name();
        showName.erase(0, removeName.size() - 1);

        if (ImGui::TreeNode(showName.c_str()))
        {
            ShowTerrainComponent(terCom);
            ImGui::TreePop();

        }
    }

    if (AnimationControlerComponent* aniCom = dynamic_cast<AnimationControlerComponent*>(com))
    {
        string showName = typeid(AnimationControlerComponent).name();
        showName.erase(0, removeName.size() - 1);

        if (ImGui::TreeNode(showName.c_str()))
        {
            ShowAnimationControlerComponent(aniCom);
            ImGui::TreePop();

        }
    }


}

void GUI::ShowTransformCom(TransformComponent* com)
{
    XMFLOAT2 screen = pGameEngine->GetWindowSize();

    XMFLOAT3 pos = com->GetPosition();
    float fPos[3] = { pos.x,pos.y,pos.z };




    XMFLOAT3 rot = com->GetRotation();

    float fRot[3] = { XMConvertToDegrees(rot.x),XMConvertToDegrees(rot.y),XMConvertToDegrees(rot.z)};

    XMFLOAT3 scl = com->GetScale();
    float fScl[3] = { scl.x,scl.y,scl.z };

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
        com->SetRotation(XMConvertToRadians(fRot[0]),XMConvertToRadians(fRot[1]),XMConvertToRadians(fRot[2]));
    }

    // スケールの編集
    ImGui::Text("Scl");
    if (ImGui::DragFloat3("##Scl", (float*)&fScl))
    {
        com->SetScale(fScl[0], fScl[1], fScl[2]);
    }




}
void GUI::ShowAnimationControlerComponent(AnimationControlerComponent* com)
{
    {
        string text = com->GetLoadFileName();
        vector<char> inputText(text.begin(), text.end());
        inputText.resize(128);

        if (ImGui::InputText("##FileName", inputText.data(), inputText.size()))
        {
            text = inputText.data();
            com->SetLoadFileName(text);
        }

    }

    if (ImGui::MenuItem("LoadDefaultAnim"))
    {
        com->LoadDefaulAnimation(com->GetLoadFileName(), com->GetLoadFileName());
        com->SetLoadFileName("");
    }


    for (AnimationNode* node : com->GetAnimNodeList())
    {
        if (ImGui::TreeNode(node->GetName().c_str()))
        {



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
        if (ImGui::Button("LoadTexture"))
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
        if (ImGui::Button("LoadTexture"))
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
        if (ImGui::Button("LoadTexture"))
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

}

BOOL GUI::GuiWndPro(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(g_hWnd, message, wParam, lParam)) return TRUE;
    return FALSE;
}
