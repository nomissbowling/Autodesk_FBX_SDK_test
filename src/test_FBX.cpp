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
  https://qiita.com/kota017a/items/071c8b085c758c769758
  https://qiita.com/kota017a/items/1cdc347dec9800ae66bd
  https://qiita.com/kota017a/items/dd0fab59c06ca72dd3f6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbxsdk.h>

#define IMPBASE "D:\\prj\\__Unity\\Metasequoia_Blender\\"
#if 0
#define IMPFBX IMPBASE"unitychan.fbx"
#define IMPSUB IMPBASE"UnityChanShader\\"
#else
#define IMPFBX IMPBASE"unitychan_SD_humanoid.fbx"
#define IMPSUB IMPBASE"UnityChanSD\\"
#endif
#define IMPSHADER IMPSUB"Shader\\" // .cg .shader
#define IMPMATERIALS IMPSUB"Materials\\" // .mat
#define IMPTEXTURE IMPSUB"Texture\\" // .tga

const char *typeNames[] = {
  "eUnknown", "eNull", "eMarker", "eSkeleton", "eMesh", "eNurbs",
  "ePatch", "eCamera", "eCameraStereo", "eCameraSwitcher", "eLight",
  "eOpticalReference", "eOpticalMarker", "eNurbsCurve", "eTrimNurbsSurface",
  "eBoundary", "eNurbsSurface", "eShape", "eLODGroup", "eSubDiv",
  "eCachedEffect", "eLine"};

void GetMesh(FbxNodeAttribute *a);

void depth(int d)
{
  char fmt[256];
  sprintf_s(fmt, sizeof(fmt), "%%%ds", d);
  fprintf(stdout, fmt, "");
}

void GetNodeAndAttributes(FbxNode *node, int d, int n)
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
  FbxMesh *mesh = node->GetMesh();
  if(mesh) fprintf(stdout, " MeshName[%s]\n", mesh->GetName());
  else fprintf(stdout, "\n");
  for(int i = 0; i < attrcount; ++i){
    FbxNodeAttribute *a = node->GetNodeAttributeByIndex(i);
    FbxNodeAttribute::EType t = a->GetAttributeType();
    if(t == FbxNodeAttribute::eMesh){ depth(d); GetMesh(a); }
  }
  for(int i = 0; i < node->GetChildCount(); ++i)
    GetNodeAndAttributes(node->GetChild(i), d + 1, i);
}

void GetMesh(FbxNodeAttribute *a)
{
  FbxMesh *m = (FbxMesh *)a;
  int polynum = m->GetPolygonCount();
  int vtxnum = m->GetPolygonVertexCount(); // count of indices
  int *indexAry = m->GetPolygonVertices();
#if 0
  int index = m->GetPolygonVertex(p, n); // 0-polynum, 0-2 (when all triangle)
  int posnum = m->GetControlPointsCount(); // count of vertices
  FbxVector4 *posAry = m->GetControlPoints(); // must set posAry[m][3] = 1;
#endif
  static char buf[4096];
  buf[0] = '\0';
  sprintf_s(buf, sizeof(buf), "%6d polygons, %6d vertices", polynum, vtxnum);
  fprintf(stdout, "    %s\n", buf);
}

int main(int ac, char **av)
{
  fprintf(stdout, "sizeof(size_t): %zd\n", sizeof(size_t));

  FbxManager *manager = FbxManager::Create();
  FbxIOSettings *iosettings = FbxIOSettings::Create(manager, IOSROOT);
  manager->SetIOSettings(iosettings);
  FbxImporter *importer = FbxImporter::Create(manager, "Importer");
  const char *fn = IMPFBX;
  fprintf(stdout, "Loading: [%s]\n", fn);
  if(importer->Initialize(fn, -1, manager->GetIOSettings()) == false){
    fprintf(stderr, "import: %s\n", importer->GetStatus().GetErrorString());
  }else{
    FbxScene *scene = FbxScene::Create(manager, "Scene");
    importer->Import(scene);
    importer->Destroy();
#if 1
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true); // convert all polygons triangle
#endif
    FbxNode *root = scene->GetRootNode();
    if(!root) fprintf(stderr, "no root\n");
    else GetNodeAndAttributes(root, 0, 0);
  }
  manager->Destroy();

  fprintf(stdout, "done.\n");
  return 0;
}
