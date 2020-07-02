/*
  test_FBX.cpp

  http://marupeke296.com/FBX2019_No2_LoadAndTerminate.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbxsdk.h>

#define IMPFBX "D:\\prj\\__Unity\\Metasequoia_Blender\\unitychan.fbx"

void GetMesh(FbxNode *node, int d, int n)
{
  FbxMesh *mesh = node->GetMesh();
  if(mesh) fprintf(stdout, "\n%4d:%4d: Mesh=[%s] Node=[%s]",
    d, n, mesh->GetName(), node->GetName());
  else fprintf(stdout, "."); // no CRLF
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
    fprintf(stdout, "Root=[%s]", scene->GetRootNode()->GetName()); // no CRLF
    GetMesh(scene->GetRootNode(), 0, 0);
  }
  manager->Destroy();

  fprintf(stdout, "\ndone.\n");
  return 0;
}
