/*
  CreateCustomTetra.cs
*/

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CreateCustomTetra : MonoBehaviour {
  //public Vector3 scl = new Vector3(1.0f, 1.0f, 1.0f);
  //public Color color = new Color(0.9f, 0.7f, 0.2f, 1.0f);
  //public PhysicMaterial physicMaterial;
  //public bool convex = true;
  //public bool useGravity = false;
  static string meshfile = "Assets/custom-tools/Mesh/CustomTetra.asset";

  [MenuItem("CustomTools/CreateCustomTetra")]
  static void Create(){
    GameObject o = new GameObject("CustomTetra_New");
    MeshFilter meshFilter = o.AddComponent<MeshFilter>();
    MeshRenderer meshRenderer = o.AddComponent<MeshRenderer>();
    MeshCollider meshCollider = o.AddComponent<MeshCollider>();
    Rigidbody rigidBody = o.AddComponent<Rigidbody>();
    Mesh m = (Mesh)AssetDatabase.LoadAssetAtPath(meshfile, typeof(Mesh));
    if(m == null){
      Vector3[] vertices = new Vector3[]{
        new Vector3(0.0f, 0.0f, 0.0f),
        new Vector3(1.0f, 0.0f, 0.0f),
        new Vector3(0.0f, 1.0f, 0.0f),
        new Vector3(0.0f, 0.0f, 1.0f)};
      Vector2[] uv = new Vector2[]{
        new Vector2(1.0f, 1.0f),
        new Vector2(0.0f, 0.0f),
        new Vector2(0.0f, 1.0f),
        new Vector2(1.0f, 0.0f)};
      Color[] colors = new Color[]{
        new Color(1.0f, 1.0f, 1.0f, 1.0f),
        new Color(1.0f, 0.0f, 0.0f, 1.0f),
        new Color(0.0f, 1.0f, 0.0f, 1.0f),
        new Color(0.0f, 0.0f, 1.0f, 1.0f)};
      int[] triangles = new int[]{
        0, 2, 1,
        0, 3, 2,
        0, 1, 3,
        1, 2, 3};
      m = new Mesh();
      //m.Clear();
      m.name = "CustomTetra";
      m.vertices = vertices;
      //m.normals = normals;
      m.uv = uv;
      m.colors = colors;
      m.triangles = triangles;
      m.RecalculateNormals();
      AssetDatabase.CreateAsset(m, meshfile);
      AssetDatabase.SaveAssets();
    }
    meshFilter.sharedMesh = m;
    m.RecalculateBounds();
    Material mat = new Material(Shader.Find("Specular"));
    mat.color = new Color(0.9f, 0.7f, 0.2f, 1.0f); // color;
    meshRenderer.material = mat;
    meshCollider.sharedMesh = m;
    meshCollider.convex = true; // convex;
    //meshCollider.material = physicMaterial;
    rigidBody.useGravity = false; // useGravity;
    //o.transform.localScale = scl;
  }
}
