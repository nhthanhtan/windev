Họ và tên : Trần Nhật Tâm
MSSV : 1512488
Email : nhattam231297@gmail.com

* App usage: 
	This is an simple app to help people note whatever they need. In here, user can create new note, view list of notes, view statistics, so on.. below functions are what I did:


	- WndProc: is main window contain background of app and process some event like: hook hotkey, add icon to notification area, set time to appear statistic dialog automatically and write List to File to save data.

	- Dialog_ShowDetailNote: a dialog to show all infomation of a note that user selected, include: tag, time to be created, content.

	- Dialog_ViewStatitistic: a dialog to show chart based on tags and number of corresponding notes. It's a Chart with max is 5.

	- Dialog_ViewNote: a dialog to show List all tag in treeview, when click on each Tag a List of note of it will be shown in ListView.

	- DialogCreateNew: a dialog have Edittext to type content of new note and Combobox to type Tag of new note which are suggest based on priority of appearance frequency.
	- Save data: All list will be save before quit app and loaded when open app.

	- After a while of using our app, user can see the statistics chart based on tags & corresponding number of notes.



* Additional flows:


	- Delete Tag when view Note: tag deleted means all note have that tag will deleted that Tag.

	- Delete Note of tag: Each tag have one or more than one note, user can delete any note of that tag but if other tags have that note, it can exist in other tags.

	- Search Tag, note in each tag.
- User can auto show statistic or not by clicking on Auto statistic on tools
  

Link youtube : https://youtu.be/RTF5SLpFHj4
