using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[System.Serializable]
public class FuzzyLogic
{
    //[NamedArrayAttribute (new string[] {})]
    public List<AnimationCurve> m_fuzzySet;

    void Init()
    {}
}

public class NamedArrayAttribute : PropertyAttribute
{
    public readonly string[] names;
    public NamedArrayAttribute(string[] names) { this.names = names; }
}
 
[CustomPropertyDrawer (typeof(NamedArrayAttribute))]public class NamedArrayDrawer : PropertyDrawer
{
    public override void OnGUI(Rect rect, SerializedProperty property, GUIContent label)
    {
        try {
            int pos = int.Parse(property.propertyPath.Split('[', ']')[1]);
            EditorGUI.CurveField(Rect.MinMaxRect(0, 0, 10, 100), property, Color.blue, Rect.MinMaxRect(0, 0, 1, 1),  new GUIContent(((NamedArrayAttribute)attribute).names[pos]));
        } catch {
            EditorGUI.CurveField(rect, property, Color.blue, Rect.MinMaxRect(0, 0, 1, 1), label);
        }
    }
}

