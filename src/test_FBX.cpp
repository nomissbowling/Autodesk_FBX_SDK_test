/*
  test_FBX.cpp

  load FBX and display Mesh with Springhead

  http://marupeke296.com/FBX2019_No2_LoadAndTerminate.html
  http://marupeke296.com/FBX2019_No3_node.html
  http://marupeke296.com/FBX_No4_VertexAndPolygon.html (old)
  http://marupeke296.com/FBX_No5_Normal.html (old)
  http://marupeke296.com/FBX_No6_UV.html (old)
  http://marupeke296.com/FBX_No7_TextureMaterial.html (old)
  http://marupeke296.com/FBX_No8_Position.html (old)
  http://marupeke296.com/FBX_No9_Bone.html (old)
  http://marupeke296.com/FBX_No10_Camera.html (old)
  https://yttm-work.jp/model_render/model_render_0008.html
  https://yttm-work.jp/model_render/model_render_0009.html
  https://yttm-work.jp/model_render/model_render_0010.html
  https://yttm-work.jp/model_render/model_render_0011.html
  https://yttm-work.jp/model_render/model_render_0012.html
  http://gameprogrammingunit.web.fc2.com/fbx_sdk/initialize.htm
  http://gameprogrammingunit.web.fc2.com/fbx_sdk/node.htm
  http://gameprogrammingunit.web.fc2.com/fbx_sdk/mesh.htm
  http://gameprogrammingunit.web.fc2.com/fbx_sdk/material.htm
  http://gameprogrammingunit.web.fc2.com/fbx_sdk/unitychan.htm
  http://whaison.jugem.jp/?eid=713
  https://megalodon.jp/2014-0623-2325-26/ramemiso.hateblo.jp/entry/2014/06/21/150405
  https://qiita.com/kota017a/items/071c8b085c758c769758
  https://qiita.com/kota017a/items/1cdc347dec9800ae66bd
  https://qiita.com/kota017a/items/dd0fab59c06ca72dd3f6
*/

#include <Springhead.h>
#include <Framework/SprFWApp.h>

#include <array>
#include <vector>
#include <tuple>
#include <map>
#include <unordered_map>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbxsdk.h>

struct IMPInfo {
  int flag;
  float scl;
  const char *fbx;
  const char *sub;
  const char *sub_shader;
  const char *sub_materials;
  const char *sub_textures;
};

#define IMPBASE "D:\\prj\\__Unity\\Metasequoia_Blender"
IMPInfo impfbx[] = { // Shader: .cg .shader, Materials: .mat, Textures: .tga
  {1, 0.02f, "unitychan.fbx", "UnityChanShader", "Shader", "Materials", "Texture"},
  {1, 0.02f, "unitychan_SD_humanoid.fbx", "UnityChanSD", "Shader", "Materials", "Textures"},
  {1, 2.00f, "unityExportSphere.fbx", "UnityExportSphere", "Shader", "Materials", "Textures"},
  {1, 2.00f, "unitySphere.fbx", "UnitySphere", "Shader", "Materials", "Textures"},
  {1, 1.00f, "unityCube.fbx", "UnityCube", "Shader", "Materials", "Textures"},
  {1, 2.00f, "unityCustomTetra.fbx", "UnityCustomTetra", "Shader", "Materials", "Textures"},
  {1, 2.00f, "unityCustomTetraWithTexture.fbx", "UnityCustomTetraWithTexture", "Shader", "Materials", "Textures"},
  {1, 2.00f, "unityMikuVer2.fbx", "UnityMikuVer2", "Shader", "Materials", "Textures"},
  {0, 2.00f, "unityKizunaAI.fbx", "UnityKizunaAI", "Shader", "Materials", "Textures"},
  {0, 2.00f, "unityNemaUi.fbx", "UnityNemaUi", "Shader", "Materials", "Textures"}};

const char *typeNames[] = {
  "eUnknown", "eNull", "eMarker", "eSkeleton", "eMesh", "eNurbs",
  "ePatch", "eCamera", "eCameraStereo", "eCameraSwitcher", "eLight",
  "eOpticalReference", "eOpticalMarker", "eNurbsCurve", "eTrimNurbsSurface",
  "eBoundary", "eNurbsSurface", "eShape", "eLODGroup", "eSubDiv",
  "eCachedEffect", "eLine"};

struct MeshInfo {
  std::string meshName;
  std::vector<int> meshIndices;
  std::vector<FbxVector4> meshVertices;
  std::vector<FbxVector4> meshNormals;
};

void GetMesh(MeshInfo &mi, FbxNodeAttribute *a);

void depth(int d)
{
  char fmt[256];
  sprintf_s(fmt, sizeof(fmt), "%%%ds", d);
  fprintf(stdout, fmt, "");
}

void GetNodeAndAttributes(std::map<std::string, MeshInfo *> &meshMap,
  FbxNode *node, int d, int n)
{
  depth(d);
  fprintf(stdout, "%4d[%s]", n, node->GetName());
  static char buf[4096];
  buf[0] = '\0';
  int p = 0;
  int attrcount = node->GetNodeAttributeCount();
  for(int i = 0; i < attrcount; ++i){
    FbxNodeAttribute *a = node->GetNodeAttributeByIndex(i);
    FbxNodeAttribute::EType t = a->GetAttributeType();
    p += sprintf_s(buf + p, sizeof(buf) - p, ":%s", typeNames[t]);
  }
  fprintf(stdout, "(%d%s)", attrcount, buf);
  MeshInfo *mi = NULL;
  FbxMesh *mesh = node->GetMesh();
  if(!mesh) fprintf(stdout, "\n");
  else{
    fprintf(stdout, " MeshName[%s]\n", mesh->GetName());
    mi = new MeshInfo{mesh->GetName()};
    meshMap[node->GetName()] = mi;
  }
  for(int i = 0; i < attrcount; ++i){
    FbxNodeAttribute *a = node->GetNodeAttributeByIndex(i);
    FbxNodeAttribute::EType t = a->GetAttributeType();
    if(t == FbxNodeAttribute::eMesh){ depth(d); GetMesh(*mi, a); }
  }
#if 0
  int matcount = scene->GetMaterialCount();
  for(int i = 0; i < matcount; ++i){
    FbxSurfaceMaterial *mat = scene->GetMaterial(i);
    char *materialName = mat->GetName();
  }
#endif
#if 0
  int matcount = node->GetMaterialCount();
  for(int i = 0; i < matcount; ++i){
    FbxSurfaceMaterial *mat = node->GetMaterial(i);
  }
#endif
#if 0
  if(mat->GetClassId().Is(FbxSurfaceLambert::ClassId)){
    FbxSurfaceLambert *sl = (FbxSurfaceLambert *)mat;
    FbxDouble3 diffuse = sl->Diffuse; // float r|g|b = diffuse[0|1|2];
  }else if(mat->GetClassId().Is(FbxSurfacePhong::ClassId)){
    FbxSurfacePhong *sp = (FbxSurfacePhong *)mat;
    FbxDouble3 diffuse = sp->Diffuse;
  }
#endif
#if 0
  mat->FindProperty(FbxSurfaceMaterial::sDiffuse);
  int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
  FbxFileTexture *tex = scene->GetSrcObject<FbxFileTexture>(i);
  char *fn = tex->GetFileName();
#endif
#if 0
  FbxImplementation *impl = GetImplementation(mat, FBXSDK_IMPLEMENTATION_CGFX);
  FbxBindingTable *rootTable = impl->GetRootTable();
  size_t entryCount = rootTable->GetEntryCount();
  FbxBindingTableEntry &entry = rootTable->GetEntry(i);
  char *name = entry.GetSource();
  FbxProperty prop = mat->RootProperty.FindHierarchical(name);
  int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>(); // same
  FbxFileTexture *tex = prop.GetSrcObject<FbxFileTexture>(i); // prop<->scene
  char *fn = tex->GetFileName();
#endif
#if 0
  FbxDouble3 translation = node->LclTranslation.Get();
  FbxDouble3 rotation = node->LclRotation.Get();
  FbxDouble3 scaling = node->LclScaling.Get();
  FbxMatrix m = node->EvaluateGlobalTransform(
    FBXSDK_TIME_INFINITE, // FbxTime pTime
    FbxNode::eSourcePivot, // FbxNode::EPivotSet pPivotSet
    false); // bool pApplyTarget
#endif
  for(int i = 0; i < node->GetChildCount(); ++i)
    GetNodeAndAttributes(meshMap, node->GetChild(i), d + 1, i);
}

void GetMesh(MeshInfo &mi, FbxNodeAttribute *a)
{
  FbxMesh *m = (FbxMesh *)a;
  int polynum = m->GetPolygonCount();
  int vtxnum = m->GetPolygonVertexCount(); // count of indices
  static char buf[4096];
  buf[0] = '\0';
  sprintf_s(buf, sizeof(buf), "%6d polygons, %6d vertices", polynum, vtxnum);
  fprintf(stdout, "    %s\n", buf);
  mi.meshIndices.reserve(polynum * 3);
  for(int i = 0; i < polynum; ++i){
    int ps = m->GetPolygonSize(i); // vertices count [3|4]
    for(int j = 0; j < ps; ++j)
      mi.meshIndices.push_back(m->GetPolygonVertex(i, j)); // polyidx, vertidx
  }
#if 1 // ccw
  mi.meshVertices.reserve(mi.meshIndices.size());
  for(auto idx: mi.meshIndices){
    FbxVector4 vertex = m->GetControlPointAt(idx);
    assert(vertex[3] == 0.0); // set 1.0 ?
    mi.meshVertices.push_back(vertex);
  }
#else // ccw
  // int posnum = m->GetControlPointsCount(); // count of vertices
  FbxVector4 *vertices = m->GetControlPoints(); // must set vertices[n][3] = 1;
  int *indices = m->GetPolygonVertices();
  mi.meshVertices.reserve(vtxnum); // posnum
  for(int i = 0; i < vtxnum; ++i){ // posnum
    int idx = indices[i];
    FbxVector4 vertex = vertices[idx];
    assert(vertex[3] == 0.0); // set 1.0 ?
    mi.meshVertices.push_back(vertex);
  }
#endif
#if 0 // obsoleted ?
  // int layernum = m->GetLayerCount(); // may be 1
  // for(int i = 0; i < layernum; ++i){
  //   FbxLayer *layer = m->GetLayer(n);
  //   FbxLayerElementNormal *normelem = layer->GetNormals(); // may be layer 0
  //   if(normelem == 0) continue;
  //   // EMappingMode mm: eByControlPoint / eByPolygonVertex
  //   // EReferenceMode rm: eDirect / eIndexToDirect
  //   FbxLayerElement::EMappingMode mm = normelem->GetMappingMode();
  //   FbxLayerElement::EReferenceMode rm = normelem->GetReferenceMode();
  //   FbxLayerElementArrayTemplate<int> &idxAry = normelem->GetIndexArray();
  //   FbxLayerElementArrayTemplate<FbxVector4> &dirAry = normelem->GetDirectArray();
  //   int idxnum = idxAry.GetCount();
  //   int normnum = dirAry.GetCount();
  // }
#endif
  int elemnum = m->GetElementNormalCount();
  assert(elemnum == 1);
  auto normelem = m->GetElementNormal();
  assert(normelem);
  auto mm = normelem->GetMappingMode();
  auto rm = normelem->GetReferenceMode();
  const auto &idxAry = normelem->GetIndexArray();
  const auto &dirAry = normelem->GetDirectArray();
  int idxnum = idxAry.GetCount(); // may be 0 when eDirect
  int normnum = dirAry.GetCount();
  assert((rm == FbxGeometryElement::eDirect) || (rm == FbxGeometryElement::eIndexToDirect));
  // m->GetPolygonVertexNormal(p, n, norm); // update &norm
  FbxArray<FbxVector4> normals;
  m->GetPolygonVertexNormals(normals);
  mi.meshNormals.reserve(normals.Size());
  if(mm == FbxGeometryElement::eByControlPoint){
    for(auto idx: mi.meshIndices){
      int nidx = rm == FbxGeometryElement::eDirect ? idx : idxAry.GetAt(idx);
      FbxVector4 norm = dirAry.GetAt(nidx); // normals[...];
      // assert(norm[3] == 1.0); // check [3] == 1
      norm[3] = 1.0; // force set
      mi.meshNormals.push_back(norm);
    }
  }else if(mm == FbxGeometryElement::eByPolygonVertex){
    int idx = 0;
    for(int i = 0; i < polynum; ++i){
      int ps = m->GetPolygonSize(i); // vertices count [3|4]
      for(int j = 0; j < ps; ++j){
        int nidx = rm == FbxGeometryElement::eDirect ? idx : idxAry.GetAt(idx);
        FbxVector4 norm = dirAry.GetAt(nidx); // normals[...];
        // assert(norm[3] == 1.0); // check [3] == 1
        norm[3] = 1.0; // force set
        mi.meshNormals.push_back(norm);
        ++idx;
      }
    }
  }else{ // dummy (may not reach here)
    // assert(false);
    for(int i = 0; i < normals.Size(); ++i){
      FbxVector4 norm = normals[i];
      // assert(norm[3] == 1.0); // check [3] == 1
      norm[3] = 1.0; // force set
      mi.meshNormals.push_back(norm);
    }
  }
#if 0
  FbxStringList uvSetNameList;
  m->GetUVSetNames(uvSetNameList);
  char *uvSetName = uvSetNameList.GetStringAt(p);
  FbxVector2 uv;
  bool unmapped;
  m->GetPolygonVertexUV(p, n, uvSetName, uv, unmapped);
#endif
}

using namespace Spr;

class MyApp : public FWApp {
public:
  bool bDrawInfo;
public:
  MyApp();
  virtual ~MyApp();
  virtual void Init(int ac=0, char **av=0);
  virtual void TimerFunc(int id);
  virtual void Display();
  virtual void Keyboard(int key, int x, int y);
  void DisplayMeshMap(std::map<std::string, MeshInfo *> &meshMap,
    Vec3d pos, float scl, bool flag=true);
  int LoadFBX(Vec3d pos, const char *base, IMPInfo *imp);
};

MyApp::MyApp() : FWApp()
{
  bDrawInfo = false;
}

MyApp::~MyApp()
{
}

void MyApp::Init(int ac, char **av)
{
  FWApp::Init(ac, av);

  FWWinIf *w = GetWin(0);
  SetCurrentWin(w);
  w->SetTitle("Autodesk_FBX_SDK_test");
  w->SetPosition(120, 80);
  w->SetSize(800, 600);

  HITrackballIf *tb = w->GetTrackball();
  // tb->SetPosition(Vec3f(0.0f, 0.0f, 0.0f));
  tb->SetTarget(Vec3f(0.0f, 0.0f, 0.0f));
  tb->SetAngle(-3.14f / 4, 3.14f / 4);
  tb->SetDistance(15.0f);
  // grRender->SetViewMatrix(tb->GetAffine().inv());

  FWSceneIf *fwScene = GetSdk()->GetScene(0);
  w->SetScene(fwScene);
  // fwScene->SetRenderMode(true, false); // solid
  fwScene->SetRenderMode(false, true); // wire
  fwScene->EnableRenderAxis(true);
  fwScene->EnableRenderForce(true);
  fwScene->EnableRenderContact(true);
  // fwScene->EnableRenderGrid(true);

  PHSdkIf *phSdk = GetSdk()->GetPHSdk();
  PHSceneIf *phScene = fwScene->GetPHScene();
  phScene->SetTimeStep(0.050);
  phScene->SetContactMode(PHSceneDesc::MODE_LCP);

/*
  // FWSdk *fwSdk; // already defined (instance of FWApp)
  GRSdkIf *grSdk = GetSdk()->GetGRSdk();
  GRSceneIf *grScene = grSdk->GetScene(); // fwSdk->GetScene(0)->GetGRScene();
  GRFrameIf *frm = grScene->GetWorld();
  frm->SetTransform(Affinef::Trn(3.0f, 0.0f, 0.0f));
  GRCameraIf *cam = grScene->GetCamera(); // null
  GRCameraDesc camd;
  cam->GetDesc(&camd);
  camd.front = 3.0f;
  grScene->SetCamera(camd);
*/

  CDBoxDesc bd;
  PHSolidIf *floor = phScene->CreateSolid();
  floor->SetDynamical(false);
  floor->SetGravity(false);
  floor->SetMass(10000.0);
  bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
  floor->AddShape(phSdk->CreateShape(bd));
  floor->SetFramePosition(Vec3d(0.0, -1.0, 0.0));
  fwSdk->GetScene(0)->SetSolidMaterial(GRRenderBaseIf::HOTPINK, floor);
  fwSdk->GetScene(0)->SetWireMaterial(GRRenderBaseIf::HOTPINK, floor);

  CDShapeIf *shape = floor->GetShape(0);
  GRMeshDesc meshd;
  CDBoxIf *box = shape->Cast();
  Vec3f *vtxs = box->GetVertices();
  std::vector<Vec3f> vertices = std::vector<Vec3f>(8);
  for(int i = 0; i < vertices.size(); ++i) vertices[i] = vtxs[i];
  std::vector<GRMeshFace> faces = std::vector<GRMeshFace>{
    {4, {7, 3, 0, 4}}, {4, {7, 4, 5, 6}}, {4, {7, 6, 2, 3}},
    {4, {1, 5, 4, 0}}, {4, {1, 0, 3, 2}}, {4, {1, 2, 6, 5}}};

  std::vector<Vec2f> coords = {{1, 0}, {1, 1}, {0, 1}, {0, 0}};
  int nv = faces[0].nVertices;
  int nf = (int)faces.size();
  meshd.vertices = std::vector<Vec3f>(nv * nf);
  meshd.faces = std::vector<GRMeshFace>(nf);
  meshd.texCoords = std::vector<Vec2f>(meshd.vertices.size());
  for(int i = 0; i < nf; ++i){
    Vec2f cr = Vec2f(i % 4, 3 - (i / 4));
    GRMeshFace g = GRMeshFace{nv, {0, 0, 0, 0}};
    GRMeshFace &f = faces[i];
    for(int j = 0; j < nv; ++j){
      g.indices[j] = i * nv + j;
      meshd.vertices[g.indices[j]] = vertices[f.indices[j]];
      Vec2f coord = coords[j];
      meshd.texCoords[g.indices[j]] = (coord + cr) / 4.0f;
    }
    meshd.faces[i] = g;
  }

  Vec4f col = fwSdk->GetRender()->GetReservedColor(GRRenderBaseIf::HOTPINK);
  meshd.colors = std::vector<Vec4f>(meshd.vertices.size());
  for(int i = 0; i < meshd.vertices.size(); ++i) meshd.colors[i] = col;
  GRMaterialDesc matd = GRMaterialDesc(
    Vec4f(0.8f, 0.8f, 0.8f, 1.0f), // ambient
    Vec4f(0.6f, 0.6f, 0.6f, 1.0f), // diffuse
    Vec4f(0.2f, 0.2f, 0.2f, 1.0f), // specular
    Vec4f(0.5f, 0.5f, 0.5f, 1.0f), // emissive
    10.0); // power
  matd.texname = "../../../UnityAssets/custom-tools/Textures/hex_256x256.png";
  GRFrameDesc frmd = GRFrameDesc(); // .transform = Affinef();

  GRSceneIf *grScene = fwSdk->GetScene(0)->GetGRScene();
  GRFrameIf *frm = grScene->CreateVisual(frmd)->Cast(); // parent = world
  GRMeshIf *mesh = grScene->CreateVisual(meshd, frm)->Cast();
  GRMaterialIf *mat = grScene->CreateVisual(matd, frm)->Cast();
  mesh->AddChildObject(mat); // 0 -> meshd.materialList[0]
  FWObjectIf *fwObj = fwSdk->GetScene(0)->CreateFWObject();
  fwObj->SetPHSolid(floor);
  fwObj->SetGRFrame(frm);
  fwSdk->GetScene(0)->Sync();
}

void MyApp::TimerFunc(int id)
{
  FWApp::TimerFunc(id);
}

void MyApp::Display()
{
  FWSceneIf *fwScene = GetSdk()->GetScene(0);
  fwScene->EnableRenderAxis(bDrawInfo);
  fwScene->EnableRenderForce(bDrawInfo);
  fwScene->EnableRenderContact(bDrawInfo);
  // fwScene->EnableRenderGrid(bDrawInfo);

  FWApp::Display();
}

void MyApp::Keyboard(int key, int x, int y)
{
//  FWApp::Keyboard(key, x, y);

  switch(key){
  case 0x1b: // ESC
  case 'q':
    FWApp::Clear();
    exit(0);
    break;
  case 'g':
    GetSdk()->SetDebugMode(false);
    break;
  case 'p':
    GetSdk()->SetDebugMode(true);
    break;
  case 'd':
    bDrawInfo = !bDrawInfo;
    break;
  case 's': {
    FWSceneIf *fwScene = GetSdk()->GetScene(0);
    fwScene->SetRenderMode(true, false); // solid
  } break;
  case 'w': {
    FWSceneIf *fwScene = GetSdk()->GetScene(0);
    fwScene->SetRenderMode(false, true); // wire
  } break;
  default: break;
  }
}

void MyApp::DisplayMeshMap(std::map<std::string, MeshInfo *> &meshMap,
  Vec3d pos, float scl, bool flag)
{
  if(flag) fprintf(stdout, "%zd meshes\n", meshMap.size());
  for(auto it = meshMap.begin(); it != meshMap.end(); ++it){
    const std::string &name = it->first;
    MeshInfo *mi = it->second;
    const std::string &meshName = mi->meshName;
    size_t vtxnum = mi->meshVertices.size();
    if(!vtxnum) continue;
    std::vector<Vec2f> coords = {
      {0.5f, 0.633f}, {1, 0.922f}, {0.5f, 0.056f}, {0, 0.922f}};
    Vec4f col = fwSdk->GetRender()->GetReservedColor(GRRenderBaseIf::NAVY);
    GRMeshDesc meshd;
    meshd.vertices.reserve(vtxnum);
    meshd.texCoords.reserve(vtxnum);
    meshd.colors.reserve(vtxnum);
    meshd.faces.reserve(vtxnum / 3);
    for(int i = 0; i < vtxnum; i += 3)
      meshd.faces.push_back(GRMeshFace{3, {i, i + 2, i + 1, 0}}); // ccw to cw
    for(int i = 0; i < vtxnum; ++i){
      FbxVector4 vertex = mi->meshVertices[i];
      int idx = mi->meshIndices[i];
      meshd.vertices.push_back(scl * Vec3f(vertex[0], vertex[1], vertex[2]));
      meshd.texCoords.push_back(coords[idx % coords.size()]); // now unexpected UV
      meshd.colors.push_back(col);
      if(flag) fprintf(stdout, "%d: %f, %f, %f\n", idx, vertex[0], vertex[1], vertex[2]);
    }
    for(int i = 0; i < mi->meshNormals.size(); ++i){
      FbxVector4 norm = mi->meshNormals[i];
      if(flag) fprintf(stdout, "%d: %f, %f, %f\n", i, norm[0], norm[1], norm[2]);
    }
    delete mi;
#if 1
  FWSceneIf *fwScene = GetSdk()->GetScene(0);
  PHSceneIf *phScene = fwScene->GetPHScene();
  PHSolidDesc desc;
  desc.mass = 10.0;
  desc.inertia *= 0.033;
  PHSolidIf *so = phScene->CreateSolid(desc);
  so->SetDynamical(false);
  so->SetGravity(false);
  so->SetMass(10.0);
  CDConvexMeshDesc cmd; // ConvexMesh shown not same as GRMesh
  cmd.vertices = std::vector<Vec3f>(meshd.vertices.size()); // must copy items
  for(int i = 0; i < cmd.vertices.size(); ++i)
    cmd.vertices[i] = meshd.vertices[i];
  cmd.material.density = 1.0;
  cmd.material.e = 1.0f;
  CDShapeIf *shape = fwSdk->GetPHSdk()->CreateShape(cmd);
  so->AddShape(shape); // (auto fwObj->GenerateCDMesh() failure)
  so->SetFramePosition(pos);
  fwSdk->GetScene(0)->SetSolidMaterial(GRRenderBaseIf::NAVY, so);
  fwSdk->GetScene(0)->SetWireMaterial(GRRenderBaseIf::NAVY, so);

  GRMaterialDesc matd = GRMaterialDesc(
    Vec4f(0.8f, 0.8f, 0.8f, 1.0f), // ambient
    Vec4f(0.6f, 0.6f, 0.6f, 1.0f), // diffuse
    Vec4f(0.2f, 0.2f, 0.2f, 1.0f), // specular
    Vec4f(0.5f, 0.5f, 0.5f, 1.0f), // emissive
    10.0); // power
  matd.texname = "../../../UnityAssets/custom-tools/Textures/72dpi_256x256.png";
  GRFrameDesc frmd = GRFrameDesc(); // .transform = Affinef();

  GRSceneIf *grScene = fwSdk->GetScene(0)->GetGRScene();
  GRFrameIf *frm = grScene->CreateVisual(frmd)->Cast(); // parent = world
  GRMeshIf *mesh = grScene->CreateVisual(meshd, frm)->Cast();
  GRMaterialIf *mat = grScene->CreateVisual(matd, frm)->Cast();
  mesh->AddChildObject(mat); // 0 -> meshd.materialList[0]
  FWObjectIf *fwObj = fwSdk->GetScene(0)->CreateFWObject();
  fwObj->SetPHSolid(so);
  fwObj->SetGRFrame(frm);
//  fwObj->GenerateCDMesh();
  fwSdk->GetScene(0)->Sync();
#endif
  }
  meshMap.clear();
}

int MyApp::LoadFBX(Vec3d pos, const char *base, IMPInfo *imp)
{
  std::map<std::string, MeshInfo *> meshMap;
  FbxManager *manager = FbxManager::Create();
  FbxIOSettings *iosettings = FbxIOSettings::Create(manager, IOSROOT);
  manager->SetIOSettings(iosettings);
  FbxImporter *importer = FbxImporter::Create(manager, "Importer");
  char fn[1024];
  strcpy_s(fn, sizeof(fn), base);
  strcat_s(fn, sizeof(fn), "\\");
  strcat_s(fn, sizeof(fn), imp->fbx);
  fprintf(stdout, "Loading: [%s]\n", fn);
  if(!importer->Initialize(fn, -1, manager->GetIOSettings())){
    fprintf(stderr, "import: %s\n", importer->GetStatus().GetErrorString());
  }else{
    FbxScene *scene = FbxScene::Create(manager, "Scene");
    importer->Import(scene);
    importer->Destroy();
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true); // convert all polygons triangle
    FbxNode *root = scene->GetRootNode();
    if(!root) fprintf(stderr, "no root\n");
    else GetNodeAndAttributes(meshMap, root, 0, 0);
  }
  manager->Destroy();
  DisplayMeshMap(meshMap, pos, imp->scl, false);
  return 0;
}

int main(int ac, char **av)
{
  fprintf(stdout, "sizeof(size_t): %zd\n", sizeof(size_t));
  MyApp app;
  app.Init(ac, av);
  int num = sizeof(impfbx) / sizeof(impfbx[0]);
  for(int i = 0; i < num; ++i){
    if(!impfbx[i].flag) continue;
    app.LoadFBX(Vec3d((i - num / 2) * -2.0, 2.0, 0.0), IMPBASE, &impfbx[i]);
  }
  app.StartMainLoop();
  fprintf(stdout, "done.\n");
  return 0;
}
