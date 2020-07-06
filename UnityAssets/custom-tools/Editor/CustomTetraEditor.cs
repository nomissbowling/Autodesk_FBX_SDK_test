/*
  CustomTetraEditor.cs
*/

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

[CustomEditor(typeof(CustomTetra))]
public class CustomTetraEditor : Editor {
  public float posy = 0.0f; // not CustomTetra object's member
  SerializedProperty scalingProp;

  private void OnEnable(){
    try{
      CustomTetra t = target as CustomTetra;
      posy = t.transform.position.y;
      scalingProp = serializedObject.FindProperty("scaling");
    }catch(System.NullReferenceException e){ // target
    }catch(System.IndexOutOfRangeException e){ // target
    }finally{
    }
  }

  public override void OnInspectorGUI(){
    CustomTetra t = target as CustomTetra;
    serializedObject.Update();
    EditorGUI.BeginChangeCheck();
    DrawDefaultInspector();
//    EditorGUILayout.PropertyField(scalingProp);
    EditorGUILayout.Slider(scalingProp, 0.1f, 10.0f);
    posy = EditorGUILayout.Slider("PosY", posy, -8.0f, 5.0f);
    if(EditorGUI.EndChangeCheck()){
      Vector3 p = t.transform.position;
      t.transform.position = new Vector3(p.x, posy, p.z);
      float f = scalingProp.floatValue;
      f = Mathf.Max(0.1f, f);
      f = Mathf.Min(10.0f, f);
      scalingProp.floatValue = f;
      t.transform.localScale = new Vector3(f, f, f);
    }
    if(GUILayout.Button("Reload Transform")){
      posy = t.transform.position.y;
      scalingProp.floatValue = t.transform.localScale.y; // x y z ?
    }
    if(GUILayout.Button("Analyze")){
      // Undo.RecordObject(t, "Analyze"); // no change
      t.Analyze();
    }
    if(GUILayout.Button("ChangeUV")){
      Undo.RecordObject(t, "ChangeUV");
      t.ChangeUV();
    }
    serializedObject.ApplyModifiedProperties();
  }

  public void Msg(string s){
    Debug.Log(s);
  }
}
