using System;
using UnityEngine;

namespace Vive
{

	class SubjectCombo
	{
		private static GUIContent[] comboBoxList;
		private static GUIStyle     listStyle;
		private SubjectList  list;
		private ComboBox combo;
		private Rect     rect;

		public int Show()
		{
			return combo.Show ();
		}

		public void Update()
		{

			comboBoxList = new GUIContent[ list.subjects.Count + 1 ];
			
			comboBoxList [0] = new GUIContent ("--Subjects--");

			for(int i = 0; i < list.subjects.Count; i++)
			{
				comboBoxList[i+1] = new GUIContent(list.subjects[i].name);
			}
			
			combo = new ComboBox (rect, comboBoxList [0], comboBoxList, "button", "box", listStyle);
		}


		public SubjectCombo(SubjectList l, Rect r) 
		{
			listStyle = new GUIStyle ();
			rect      = r;
			list      = l;

			listStyle.normal.textColor   = Color.white; 
			listStyle.onHover.background = new Texture2D (2, 2);
			listStyle.hover.background   = new Texture2D(2, 2);
			listStyle.padding.left       = 4;
			listStyle.padding.right      = 4;
			listStyle.padding.top        = 4;
			listStyle.padding.bottom     = 4;

			Update ();

		}

	};

}