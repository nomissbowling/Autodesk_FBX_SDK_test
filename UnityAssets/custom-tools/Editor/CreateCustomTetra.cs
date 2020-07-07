/*
  CreateCustomTetra.cs
*/

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class CreateCustomTetra : EditorWindow {
  public string objname = "CustomTetra_New";
  public Vector3 pos = new Vector3(0.0f, 0.0f, 0.0f);
  public Vector3 rot = new Vector3(0.0f, 0.0f, 0.0f);
  public Vector3 scl = new Vector3(1.0f, 1.0f, 1.0f);
  public Color color = new Color(0.9f, 0.7f, 0.2f, 1.0f);
  public PhysicMaterial physicMaterial; // None
  public bool convex = true;
  public bool useGravity = false;
  public Texture2D texture;
  static string meshfile = "Assets/custom-tools/Mesh/CustomTetra.asset";
  static string texfile = "Assets/custom-tools/Textures/hex_256x256.png";
  static string texname = "lbrbrtlt_256x256";

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
    texture = (Texture2D)EditorGUILayout.ObjectField("Texture", texture, typeof(Texture2D), false);
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
    // mat.mainTexture = texture; // OK link to selected
    // mat.mainTexture = loadTexture(texfile); // OK but loadTexture not link
    mat.mainTexture = Resources.Load("Textures/" + texname, typeof(Texture2D)) as Texture2D; // texname from Assets/custom-tools/Resources/Textures (check Read/Write Enabled) OK link
    // Texture2D[] textures = Resources.LoadAll("Textures", typeof(Texture2D)) as Texture2D[]; // NullReferenceException
    // mat.mainTexture = textures[0];
    mat.mainTextureScale = new Vector2(2, 2);
    mat.mainTextureOffset = new Vector2(0, 0);
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

  public Texture2D loadTexture(string tfn){
    string fullpath = Application.dataPath + "/../" + tfn;
    FileStream fs = new FileStream(fullpath, FileMode.Open, FileAccess.Read);
    BinaryReader br = new BinaryReader(fs);
    byte[] dat = br.ReadBytes((int)br.BaseStream.Length);
    br.Close();
    fs.Close();
    int s = 16, w = 0, h = 0;
    for(int i = 0; i < 4; ++i) w = w * 256 + dat[s++];
    for(int i = 0; i < 4; ++i) h = h * 256 + dat[s++];
    Texture2D tex = new Texture2D(w, h);
    tex.LoadImage(dat);
    return tex;
  }
}
