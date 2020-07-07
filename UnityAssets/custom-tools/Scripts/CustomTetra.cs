/*
  CustomTetra.cs
*/

using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CustomTetra : MonoBehaviour {
  [SerializeField, HideInInspector]
  private float scaling = 1.0f;
  [SerializeField, HideInInspector]
  private Texture2D texture;
  [SerializeField, HideInInspector]
  private Vector2 texSz = new Vector2(1, 1);
  [SerializeField, HideInInspector]
  private Vector2 texPos = new Vector2(0, 0);

  public void Analyze(){
    Mesh mesh = gameObject.GetComponent<MeshFilter>().sharedMesh;
    for(int i = 0; i < mesh.vertices.Length; ++i)
      print("vertices[" + i + "] : " + mesh.vertices[i]);
    for(int i = 0; i < mesh.uv.Length; ++i)
      print("uv[" + i + "] : " + mesh.uv[i]);
    for(int i = 0; i < mesh.triangles.Length; ++i)
      print("triangles[" + i + "] : " + mesh.triangles[i]);
  }

  public void ChangeUV(){
    MeshFilter meshFilter = gameObject.GetComponent<MeshFilter>();
    Vector2[] uv = new Vector2[4];
    uv[0] = new Vector2(0.5f, 0.5f);
    uv[1] = new Vector2(0.0f, 0.0f);
    uv[2] = new Vector2(0.0f, 0.5f);
    uv[3] = new Vector2(0.5f, 0.0f);
    meshFilter.sharedMesh.uv = uv;
  }

  void Start(){
  }

  void Update(){
  }
}
