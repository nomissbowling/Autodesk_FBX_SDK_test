/*
  test_FBX.cpp

  http://marupeke296.com/FBX2019_No2_LoadAndTerminate.html
  http://marupeke296.com/FBX2019_No3_node.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbxsdk.h>

#define IMPFBX "D:\\prj\\__Unity\\Metasequoia_Blender\\unitychan.fbx"

void depth(int d)
{
  char fmt[256];
  sprintf_s(fmt, sizeof(fmt), "%%%ds", d);
  fprintf(stdout, fmt, "");
}

void GetMesh(FbxNode *node, int d, int n)
{
  depth(d);
  fprintf(stdout, "%4d[%s]", n, node->GetName());
  FbxMesh *mesh = node->GetMesh();
  if(mesh) fprintf(stdout, " Mesh=[%s]\n", mesh->GetName());
  else fprintf(stdout, "\n");
  for(int i = 0; i < node->GetChildCount(); ++i)
    GetMesh(node->GetChild(i), d + 1, i);
}

int main(int ac, char **av)
{
  fprintf(stdout, "sizeof(size_t): %zd\n", sizeof(size_t));

  FbxManager *manager = FbxManager::Create();
  FbxIOSettings *iosettings = FbxIOSettings::Create(manager, IOSROOT);
  FbxImporter *importer = FbxImporter::Create(manager, "Importer");
  const char *fn = IMPFBX;
  fprintf(stdout, "Loading: [%s]\n", fn);
  if(importer->Initialize(fn, -1, manager->GetIOSettings()) == false){
    fprintf(stderr, "import error\n");
  }else{
    FbxScene *scene = FbxScene::Create(manager, "Scene");
    importer->Import(scene);
    importer->Destroy();
    FbxNode *root = scene->GetRootNode();
    if(!root) fprintf(stderr, "no root\n");
    else GetMesh(root, 0, 0);
  }
  manager->Destroy();

  fprintf(stdout, "done.\n");
  return 0;
}
