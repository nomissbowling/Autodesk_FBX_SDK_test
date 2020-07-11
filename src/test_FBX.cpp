/*
  test_FBX.cpp

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

#define IMPBASE "D:\\prj\\__Unity\\Metasequoia_Blender\\"
#if 0
#if 0
#define IMPFBX IMPBASE"unitychan.fbx"
#define IMPSUB IMPBASE"UnityChanShader\\"
#define IMPSHADER IMPSUB"Shader\\" // .cg .shader
#define IMPMATERIALS IMPSUB"Materials\\" // .mat
#define IMPTEXTURES IMPSUB"Texture\\" // .tga
#else
#define IMPFBX IMPBASE"unitychan_SD_humanoid.fbx"
#define IMPSUB IMPBASE"UnityChanSD\\"
#define IMPSHADER IMPSUB"Shader\\" // .cg .shader
#define IMPMATERIALS IMPSUB"Materials\\" // .mat
#define IMPTEXTURES IMPSUB"Textures\\" // .tga
#endif
#else
#if 1
//#define IMPFBX IMPBASE"unityExportSphere.fbx"
//#define IMPSUB IMPBASE"UnityExportSphere\\" // empty
//#define IMPFBX IMPBASE"unitySphere.fbx"
//#define IMPSUB IMPBASE"UnitySphere\\" // empty
//#define IMPFBX IMPBASE"unityCube.fbx"
//#define IMPSUB IMPBASE"UnityCube\\" // empty
//#define IMPFBX IMPBASE"unityCustomTetra.fbx"
//#define IMPSUB IMPBASE"UnityCustomTetra\\" // empty
#define IMPFBX IMPBASE"unityCustomTetraWithTexture.fbx"
#define IMPSUB IMPBASE"UnityCustomTetraWithTexture\\" // empty
#define IMPSHADER IMPSUB"Shader\\" // empty
#define IMPMATERIALS IMPSUB"Materials\\" // empty
#define IMPTEXTURES IMPSUB"Textures\\" // empty
#else
#define IMPFBX IMPBASE"unityMikuVer2.fbx"
#define IMPSUB IMPBASE"UnityMikuVer2\\"
#define IMPSHADER IMPSUB"Shader\\" // empty
#define IMPMATERIALS IMPSUB"Materials\\" // .asset
#define IMPTEXTURES IMPSUB"Textures\\" // empty
#endif
#endif

const char *typeNames[] = {
  "eUnknown", "eNull", "eMarker", "eSkeleton", "eMesh", "eNurbs",
  "ePatch", "eCamera", "eCameraStereo", "eCameraSwitcher", "eLight",
  "eOpticalReference", "eOpticalMarker", "eNurbsCurve", "eTrimNurbsSurface",
  "eBoundary", "eNurbsSurface", "eShape", "eLODGroup", "eSubDiv",
  "eCachedEffect", "eLine"};

struct MeshInfo {
  FbxNode *meshNode;
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
    mi = new MeshInfo{node};
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
    for(int j = 0; j < m->GetPolygonSize(i); ++j) // vertices count [3|4]
      mi.meshIndices.push_back(m->GetPolygonVertex(i, j)); // polyidx, vertidx
  }
#if 1 // ccw
  // int posnum = m->GetControlPointsCount(); // count of vertices
  mi.meshVertices.reserve(mi.meshIndices.size());
  for(auto idx: mi.meshIndices){
    FbxVector4 vertex = m->GetControlPointAt(idx);
    assert(vertex[3] == 0.0); // set 1.0 ?
    mi.meshVertices.push_back(vertex);
  }
#else // ccw
  FbxVector4 *vertices = m->GetControlPoints(); // must set vertices[n][3] = 1;
  int *indices = m->GetPolygonVertices();
  mi.meshVertices.reserve(vtxnum);
  for(int i = 0; i < vtxnum; ++i){
    int idx = indices[i];
    FbxVector4 vertex = vertices[idx];
    assert(vertex[3] == 0.0); // set 1.0 ?
    mi.meshVertices.push_back(vertex);
  }
#endif
  // m->GetPolygonVertexNormal(p, n, norm); // update &norm
  FbxArray<FbxVector4> normals;
  m->GetPolygonVertexNormals(normals);
  mi.meshNormals.reserve(normals.Size());
  for(int i = 0; i < normals.Size(); ++i){
    FbxVector4 norm = normals[i];
    assert(norm[3] == 0.0);
    mi.meshNormals.push_back(norm);
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

int main(int ac, char **av)
{
  fprintf(stdout, "sizeof(size_t): %zd\n", sizeof(size_t));

  std::map<std::string, MeshInfo *> meshMap;
  FbxManager *manager = FbxManager::Create();
  FbxIOSettings *iosettings = FbxIOSettings::Create(manager, IOSROOT);
  manager->SetIOSettings(iosettings);
  FbxImporter *importer = FbxImporter::Create(manager, "Importer");
  const char *fn = IMPFBX;
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
  fprintf(stdout, "%zd meshes\n", meshMap.size());
  for(auto it = meshMap.begin(); it != meshMap.end(); ++it){
    const std::string &name = it->first;
    MeshInfo *mi = it->second;
    FbxMesh *m = mi->meshNode->GetMesh();
    int polynum = m->GetPolygonCount();
    fprintf(stdout, "%s: [%s], %d\n", name.c_str(), m->GetName(), polynum);
    int *indices = m->GetPolygonVertices();
    for(int i = 0; i < mi->meshVertices.size(); ++i){
      FbxVector4 vertex = mi->meshVertices[i];
      int idx = mi->meshIndices[i];
      assert(idx == indices[i]);
      fprintf(stdout, "%d: %f, %f, %f\n", idx, vertex[0], vertex[1], vertex[2]);
    }
    for(int i = 0; i < mi->meshNormals.size(); ++i){
      FbxVector4 norm = mi->meshNormals[i];
      fprintf(stdout, "%d: %f, %f, %f\n", i, norm[0], norm[1], norm[2]);
    }
    delete mi;
  }
  manager->Destroy();

  fprintf(stdout, "done.\n");
  return 0;
}
