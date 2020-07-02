/*
  test_FBX.cpp
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
  FbxImporter *importer = FbxImporter::Create(manager, "Importer");
  FbxScene *scene = FbxScene::Create(manager, "Scene");
  const char *fn = IMPFBX;
  fprintf(stdout, "Loading: [%s]\n", fn);
  importer->Initialize(fn);
  importer->Import(scene);
  fprintf(stdout, "Root=[%s]", scene->GetRootNode()->GetName()); // no CRLF
  GetMesh(scene->GetRootNode(), 0, 0);
  manager->Destroy();

  fprintf(stdout, "\ndone.\n");
  return 0;
}
