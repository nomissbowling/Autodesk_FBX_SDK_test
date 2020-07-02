/*
  test_FBX.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbxsdk.h>

#define IMPFBX "D:\\prj\\__Unity\\Metasequoia_Blender\\unitychan.fbx"

void GetMesh(FbxNode *node)
{
  FbxMesh *mesh = node->GetMesh();
  if(mesh)
    fprintf(stdout, "\nMesh=[%s] Node=[%s]", mesh->GetName(), node->GetName());
  else fprintf(stdout, "."); // no CRLF
  for(int i = 0; i < node->GetChildCount(); ++i) GetMesh(node->GetChild(i));
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
  GetMesh(scene->GetRootNode());
  manager->Destroy();

  fprintf(stdout, "\ndone.\n");
  return 0;
}
