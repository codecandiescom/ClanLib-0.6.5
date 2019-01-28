         Name: GUIEditor
       Status: Windows(Y), Linux(Y)
        Level: Intermediate
      Modules: app, core, gui, display
Last modified: 16. october 2001
     Progress: Not complete
   Maintainer: Jeroen (japj)
      Summary: gui editor

This is the Example GUI Editor...

The "final" goal is to have an editor that is able to load/save & edit .gui files.


"project" == editable gui in the editor

Ideas:

* component type list: shows all available gui component types (based upon CL_ComponentManager::component_types)
  - add component; add a selected component of "component type" to the current project (name that component!)

* project component list (GUI_Project_Window): shows all components in the current project

* component properties/options: show the available componentoptions + values! of a selected component
  (this might require a slight modification in the current component_type API)

mbn putty paste:

+------------------------------------------------------------------+
|<Create compone> <load> <save>                                    |
+-----------------+------------------------------------------------+
|Gui tree         |                                                |
|+ Label          |   Hey, this is a label on the gui              |
|+ Button         |                                                |
|+ Button         |                   <ok> <cancel>                |
|                 |                                                |
+-----------------+                                                |
|Properties       |                                                |
|Name      Value  |                                                |
|type      button |                                                |
|name      ok     |                                                |
|x         150    |                                                |
|...       ...    |                                                |
+-----------------+------------------------------------------------+

Something like that :)
probably put 'create new component' and 'load' and 'save' etc etc as toolbar at the top instead of the bottom.
but besides from that...
ok, going to write that then.
We need the following classes then: Mainframe, Treebar, Propertybar, Toolbar, EditView
The GUI tree on my drawing is the treebar.

