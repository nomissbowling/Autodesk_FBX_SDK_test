/*
  CreateCustomTetra.cs
*/

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CreateCustomTetra : EditorWindow {
  public string objname = "CustomTetra_New";
  public Vector3 pos = new Vector3(0.0f, 0.0f, 0.0f);
  public Vector3 rot = new Vector3(0.0f, 0.0f, 0.0f);
  public Vector3 scl = new Vector3(1.0f, 1.0f, 1.0f);
  public Color color = new Color(0.9f, 0.7f, 0.2f, 1.0f);
  public PhysicMaterial physicMaterial; // None
  public bool convex = true;
  public bool useGravity = false;
  static string meshfile = "Assets/custom-tools/Mesh/CustomTetra.asset";

/*
  [MenuItem("CustomTools/CreateCustomTetra")]
  static void Init(){
    EditorWindow w = GetWindow(typeof(CreateCustomTetra));
    w.Show();
  }
*/

  [MenuItem("CustomTools/CreateCustomTetra")]
  static void Open(){
    EditorWindow.GetWindow<CreateCustomTetra>(true, "CustomTetra"); // true: floating window only, false: dockable
  }

  private void OnGUI(){
    GUILayout.Label("Set parameters and Create it.", EditorStyles.boldLabel);
    objname = EditorGUILayout.TextField("Object Name", objname);
    pos = EditorGUILayout.Vector3Field("Position", pos);
    rot = EditorGUILayout.Vector3Field("Rotation", rot);
    scl = EditorGUILayout.Vector3Field("Scale", scl);
    color = EditorGUILayout.ColorField("Color", color);
    physicMaterial = (PhysicMaterial)EditorGUILayout.ObjectField("Material",
      physicMaterial, typeof(PhysicMaterial));
    convex = EditorGUILayout.Toggle("Convex Mesh", convex);
    useGravity = EditorGUILayout.Toggle("Use Gravity", useGravity);
    if(GUILayout.Button("Create")){
      create();
    }
  }

  private void create(){
    GameObject o = new GameObject(objname);
    Undo.RegisterCreatedObjectUndo(o, "Created CustomTetra");
    o.AddComponent<CustomTetra>();
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
    mat.color = color;
    meshRenderer.material = mat;
    meshCollider.sharedMesh = m;
    meshCollider.convex = convex;
    meshCollider.material = physicMaterial;
    rigidBody.useGravity = useGravity;
    o.transform.localScale = scl;
    o.transform.rotation = Quaternion.Euler(rot);
    o.transform.position = pos;
    CustomTetraEditor ed = ScriptableObject.CreateInstance<CustomTetraEditor>();
    ed.Msg("test");
  }
}
