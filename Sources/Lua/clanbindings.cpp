/*
** Lua binding: clanbindings
** Generated automatically by tolua 4.0a on 08/21/01 19:05:11.
*/

#include <tolua.h>

/* Exported function */
int tolua_clanbindings_open (lua_State* tolua_S);
void tolua_clanbindings_close (lua_State* tolua_S);

#include <API/core.h>
#include <API/display.h>
#include <API/application.h>
#include <API/lua.h>
#define TRUE	1
#define FALSE	0

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CL_System");
 tolua_usertype(tolua_S,"CL_Palette");
 tolua_usertype(tolua_S,"CL_Canvas");
 tolua_usertype(tolua_S,"CL_Surface");
 tolua_usertype(tolua_S,"CL_Target");
 tolua_usertype(tolua_S,"CL_SurfaceProvider");
 tolua_usertype(tolua_S,"CL_DisplayCard");
 tolua_usertype(tolua_S,"CL_Display");
 tolua_usertype(tolua_S,"CL_Mouse");
 tolua_usertype(tolua_S,"CL_Keyboard");
 tolua_usertype(tolua_S,"CL_ClipRect");
 tolua_usertype(tolua_S,"CL_EventListener");
 tolua_usertype(tolua_S,"CL_ResourceManager");
 tolua_usertype(tolua_S,"CL_Rect");
 tolua_usertype(tolua_S,"CL_VidMode");
}

/* method: delete of class  CL_Display */
static int toluaI_clanbindings_CL_Display_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Display"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Display* self = (CL_Display*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: flip_display of class  CL_Display */
static int toluaI_clanbindings_CL_Display_flip_display00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  int sync = ((int)  tolua_getnumber(tolua_S,2,0));
 {
  CL_Display::flip_display(sync);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flip_display'.");
 return 0;
}

/* method: put_display of class  CL_Display */
static int toluaI_clanbindings_CL_Display_put_display00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_Rect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_Rect* rect = ((const CL_Rect*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_Display::put_display(*rect);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'put_display'.");
 return 0;
}

/* method: sync_buffers of class  CL_Display */
static int toluaI_clanbindings_CL_Display_sync_buffers00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Display::sync_buffers();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sync_buffers'.");
 return 0;
}

/* method: clear_display of class  CL_Display */
static int toluaI_clanbindings_CL_Display_clear_display00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  float red = ((float)  tolua_getnumber(tolua_S,2,0));
  float green = ((float)  tolua_getnumber(tolua_S,3,0));
  float blue = ((float)  tolua_getnumber(tolua_S,4,0));
  float alpha = ((float)  tolua_getnumber(tolua_S,5,1));
 {
  CL_Display::clear_display(red,green,blue,alpha);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_display'.");
 return 0;
}

/* method: set_palette of class  CL_Display */
static int toluaI_clanbindings_CL_Display_set_palette00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_Palette"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Palette* palette = ((CL_Palette*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_Display::set_palette(palette);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_palette'.");
 return 0;
}

/* method: get_palette of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_palette00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Palette* toluaI_ret = (CL_Palette*)  CL_Display::get_palette();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Palette"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_palette'.");
 return 0;
}

/* method: get_current_card of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_current_card00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_DisplayCard* toluaI_ret = (CL_DisplayCard*)  CL_Display::get_current_card();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_DisplayCard"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_current_card'.");
 return 0;
}

/* method: set_videomode of class  CL_Display */
static int toluaI_clanbindings_CL_Display_set_videomode00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_VidMode"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_VidMode* mode = ((CL_VidMode*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_Display::set_videomode(mode);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_videomode'.");
 return 0;
}

/* method: set_videomode of class  CL_Display */
static int toluaI_clanbindings_CL_Display_set_videomode01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  int width = ((int)  tolua_getnumber(tolua_S,2,0));
  int height = ((int)  tolua_getnumber(tolua_S,3,0));
  int bpp = ((int)  tolua_getnumber(tolua_S,4,0));
  bool fullscreen = ((bool)  tolua_getnumber(tolua_S,5,true));
  bool allow_resize = ((bool)  tolua_getnumber(tolua_S,6,false));
  bool video_memory = ((bool)  tolua_getnumber(tolua_S,7,true));
 {
  CL_Display::set_videomode(width,height,bpp,fullscreen,allow_resize,video_memory);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Display_set_videomode00(tolua_S);
}

/* method: get_target of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_target00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Target* toluaI_ret = (CL_Target*)  CL_Display::get_target();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Target"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_target'.");
 return 0;
}

/* method: get_width of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_width00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Display::get_width();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.");
 return 0;
}

/* method: get_height of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_height00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Display::get_height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.");
 return 0;
}

/* method: get_bpp of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_bpp00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Display::get_bpp();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_bpp'.");
 return 0;
}

/* method: push_clip_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_push_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Display::push_clip_rect();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_clip_rect'.");
 return 0;
}

/* method: push_clip_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_push_clip_rect01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_Display::push_clip_rect(*rect);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Display_push_clip_rect00(tolua_S);
}

/* method: get_clip_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_ClipRect toluaI_ret = (CL_ClipRect)  CL_Display::get_clip_rect();
 {
#ifdef __cplusplus
 void* toluaI_clone = new CL_ClipRect(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(CL_ClipRect));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"CL_ClipRect")),tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_clip_rect'.");
 return 0;
}

/* method: set_clip_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_set_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_Display::set_clip_rect(*rect);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_clip_rect'.");
 return 0;
}

/* method: pop_clip_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_pop_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Display::pop_clip_rect();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pop_clip_rect'.");
 return 0;
}

/* method: push_translate_offset of class  CL_Display */
static int toluaI_clanbindings_CL_Display_push_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Display::push_translate_offset();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_translate_offset'.");
 return 0;
}

/* method: push_translate_offset of class  CL_Display */
static int toluaI_clanbindings_CL_Display_push_translate_offset01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 {
  CL_Display::push_translate_offset(x,y);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Display_push_translate_offset00(tolua_S);
}

/* method: get_translate_offset_x of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_translate_offset_x00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Display::get_translate_offset_x();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_offset_x'.");
 return 0;
}

/* method: get_translate_offset_y of class  CL_Display */
static int toluaI_clanbindings_CL_Display_get_translate_offset_y00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Display::get_translate_offset_y();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_offset_y'.");
 return 0;
}

/* method: set_translate_offset of class  CL_Display */
static int toluaI_clanbindings_CL_Display_set_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 {
  CL_Display::set_translate_offset(x,y);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_translate_offset'.");
 return 0;
}

/* method: pop_translate_offset of class  CL_Display */
static int toluaI_clanbindings_CL_Display_pop_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_Display::pop_translate_offset();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pop_translate_offset'.");
 return 0;
}

/* method: draw_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_draw_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 {
  CL_Display::draw_rect(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_rect'.");
 return 0;
}

/* method: fill_rect of class  CL_Display */
static int toluaI_clanbindings_CL_Display_fill_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 {
  CL_Display::fill_rect(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fill_rect'.");
 return 0;
}

/* method: draw_line of class  CL_Display */
static int toluaI_clanbindings_CL_Display_draw_line00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 {
  CL_Display::draw_line(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_line'.");
 return 0;
}

/* method: keep_alive of class  CL_System */
static int toluaI_clanbindings_CL_System_keep_alive00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_System::keep_alive();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'keep_alive'.");
 return 0;
}

/* method: sleep of class  CL_System */
static int toluaI_clanbindings_CL_System_sleep00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  int millis = ((int)  tolua_getnumber(tolua_S,2,0));
 {
  CL_System::sleep(millis);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sleep'.");
 return 0;
}

/* method: get_time of class  CL_System */
static int toluaI_clanbindings_CL_System_get_time00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  unsigned int toluaI_ret = (unsigned int)  CL_System::get_time();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_time'.");
 return 0;
}

/* method: keep_alive of class  CL_System */
static int toluaI_clanbindings_CL_System_keep_alive01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_EventListener"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_EventListener* events = ((CL_EventListener*)  tolua_getusertype(tolua_S,2,0));
  int timeout = ((int)  tolua_getnumber(tolua_S,3,-1));
 {
  bool toluaI_ret = (bool)  CL_System::keep_alive(*events,timeout);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 return toluaI_clanbindings_CL_System_keep_alive00(tolua_S);
}

/* method: sleep of class  CL_System */
static int toluaI_clanbindings_CL_System_sleep01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  int millis = ((int)  tolua_getnumber(tolua_S,2,0));
 {
  CL_System::sleep(millis);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_System_sleep00(tolua_S);
}

/* method: detect_mmx of class  CL_System */
static int toluaI_clanbindings_CL_System_detect_mmx00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  CL_System::detect_mmx();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detect_mmx'.");
 return 0;
}

/* method: detect_3dnow of class  CL_System */
static int toluaI_clanbindings_CL_System_detect_3dnow00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  CL_System::detect_3dnow();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detect_3dnow'.");
 return 0;
}

/* method: delete of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Mouse"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Mouse* self = (CL_Mouse*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: get_x of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_get_x00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Mouse::get_x();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_x'.");
 return 0;
}

/* method: get_y of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_get_y00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Mouse::get_y();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_y'.");
 return 0;
}

/* method: left_pressed of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_left_pressed00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  CL_Mouse::left_pressed();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'left_pressed'.");
 return 0;
}

/* method: middle_pressed of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_middle_pressed00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Mouse::middle_pressed();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'middle_pressed'.");
 return 0;
}

/* method: right_pressed of class  CL_Mouse */
static int toluaI_clanbindings_CL_Mouse_right_pressed00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  int toluaI_ret = (int)  CL_Mouse::right_pressed();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'right_pressed'.");
 return 0;
}

/* method: delete of class  CL_Keyboard */
static int toluaI_clanbindings_CL_Keyboard_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Keyboard"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Keyboard* self = (CL_Keyboard*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: get_keycode of class  CL_Keyboard */
static int toluaI_clanbindings_CL_Keyboard_get_keycode00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  int button_no = ((int)  tolua_getnumber(tolua_S,2,0));
 {
  int toluaI_ret = (int)  CL_Keyboard::get_keycode(button_no);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_keycode'.");
 return 0;
}

/* method: load of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_load00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"CL_ResourceManager"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* resource_id = ((const char*)  tolua_getstring(tolua_S,2,0));
  CL_ResourceManager* manager = ((CL_ResourceManager*)  tolua_getusertype(tolua_S,3,0));
 {
  CL_Surface* toluaI_ret = (CL_Surface*)  CL_Surface::load(resource_id,manager);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.");
 return 0;
}

/* method: create of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_create00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* provider = ((CL_SurfaceProvider*)  tolua_getusertype(tolua_S,2,0));
  int delete_provider = ((int)  tolua_getnumber(tolua_S,3,0));
 {
  CL_Surface* toluaI_ret = (CL_Surface*)  CL_Surface::create(provider,delete_provider);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.");
 return 0;
}

/* method: create_dynamic of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_create_dynamic00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* provider = ((CL_SurfaceProvider*)  tolua_getusertype(tolua_S,2,0));
  bool delete_provider = ((bool)  tolua_getnumber(tolua_S,3,false));
 {
  CL_Surface* toluaI_ret = (CL_Surface*)  CL_Surface::create_dynamic(provider,delete_provider);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_dynamic'.");
 return 0;
}

/* method: delete of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: reload of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_reload00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reload'");
 {
  self->reload();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reload'.");
 return 0;
}

/* method: get_provider of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_get_provider00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_provider'");
 {
  CL_SurfaceProvider* toluaI_ret = (CL_SurfaceProvider*)  self->get_provider();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_SurfaceProvider"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_provider'.");
 return 0;
}

/* method: put_screen of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_put_screen00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  int spr_no = ((int)  tolua_getnumber(tolua_S,4,0));
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,5,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'put_screen'");
 {
  self->put_screen(x,y,spr_no,card);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'put_screen'.");
 return 0;
}

/* method: put_screen of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_put_screen01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  float scale_x = ((float)  tolua_getnumber(tolua_S,4,0));
  float scale_y = ((float)  tolua_getnumber(tolua_S,5,0));
  int spr_no = ((int)  tolua_getnumber(tolua_S,6,0));
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,7,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'put_screen'");
 {
  self->put_screen(x,y,scale_x,scale_y,spr_no,card);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Surface_put_screen00(tolua_S);
}

/* method: put_screen of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_put_screen02(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  int size_x = ((int)  tolua_getnumber(tolua_S,4,0));
  int size_y = ((int)  tolua_getnumber(tolua_S,5,0));
  int spr_no = ((int)  tolua_getnumber(tolua_S,6,0));
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,7,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'put_screen'");
 {
  self->put_screen(x,y,size_x,size_y,spr_no,card);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Surface_put_screen01(tolua_S);
}

/* method: put_target of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_put_target00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  int spr_no = ((int)  tolua_getnumber(tolua_S,4,0));
  CL_Target* target = ((CL_Target*)  tolua_getusertype(tolua_S,5,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'put_target'");
 {
  self->put_target(x,y,spr_no,target);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'put_target'.");
 return 0;
}

/* method: get_width of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_get_width00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_width'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_width();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.");
 return 0;
}

/* method: get_height of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_get_height00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_height'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.");
 return 0;
}

/* method: get_num_frames of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_get_num_frames00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_num_frames'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_num_frames();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_frames'.");
 return 0;
}

/* method: is_video of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_is_video00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,2,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_video'");
 {
  int toluaI_ret = (int)  self->is_video(card);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_video'.");
 return 0;
}

/* method: is_loaded of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_is_loaded00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_Surface* self = (const CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,2,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_loaded'");
 {
  int toluaI_ret = (int)  self->is_loaded(card);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_loaded'.");
 return 0;
}

/* method: convert_video of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_convert_video00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,2,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'convert_video'");
 {
  int toluaI_ret = (int)  self->convert_video(card);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'convert_video'.");
 return 0;
}

/* method: convert_system of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_convert_system00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,2,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'convert_system'");
 {
  int toluaI_ret = (int)  self->convert_system(card);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'convert_system'.");
 return 0;
}

/* method: flush of class  CL_Surface */
static int toluaI_clanbindings_CL_Surface_flush00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Surface"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_DisplayCard"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Surface* self = (CL_Surface*)  tolua_getusertype(tolua_S,1,0);
  CL_DisplayCard* card = ((CL_DisplayCard*)  tolua_getusertype(tolua_S,2,NULL));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flush'");
 {
  self->flush(card);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flush'.");
 return 0;
}

/* method: delete of class  CL_Target */
static int toluaI_clanbindings_CL_Target_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: is_video of class  CL_Target */
static int toluaI_clanbindings_CL_Target_is_video00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_video'");
 {
  int toluaI_ret = (int)  self->is_video();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_video'.");
 return 0;
}

/* method: lock of class  CL_Target */
static int toluaI_clanbindings_CL_Target_lock00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'lock'");
 {
  self->lock();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lock'.");
 return 0;
}

/* method: unlock of class  CL_Target */
static int toluaI_clanbindings_CL_Target_unlock00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unlock'");
 {
  self->unlock();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unlock'.");
 return 0;
}

/* method: get_data of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_data00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_data'");
 {
  void* toluaI_ret = (void*)  self->get_data();
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_data'.");
 return 0;
}

/* method: get_num_frames of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_num_frames00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_num_frames'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_num_frames();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_frames'.");
 return 0;
}

/* method: get_width of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_width00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_width'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_width();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.");
 return 0;
}

/* method: get_height of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_height00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_height'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.");
 return 0;
}

/* method: get_pitch of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_pitch00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pitch'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_pitch();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pitch'.");
 return 0;
}

/* method: get_depth of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_depth00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_depth'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_depth();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_depth'.");
 return 0;
}

/* method: get_bytes_per_pixel of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_bytes_per_pixel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_bytes_per_pixel'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_bytes_per_pixel();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_bytes_per_pixel'.");
 return 0;
}

/* method: is_indexed of class  CL_Target */
static int toluaI_clanbindings_CL_Target_is_indexed00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_indexed'");
 {
  int toluaI_ret = (int)  self->is_indexed();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_indexed'.");
 return 0;
}

/* method: get_red_mask of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_red_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_red_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_red_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_red_mask'.");
 return 0;
}

/* method: get_green_mask of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_green_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_green_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_green_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_green_mask'.");
 return 0;
}

/* method: get_blue_mask of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_blue_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_blue_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_blue_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_blue_mask'.");
 return 0;
}

/* method: get_alpha_mask of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_alpha_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_alpha_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_alpha_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_alpha_mask'.");
 return 0;
}

/* method: get_palette of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_palette00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_palette'");
 {
  CL_Palette* toluaI_ret = (CL_Palette*)  self->get_palette();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Palette"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_palette'.");
 return 0;
}

/* method: push_clip_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_push_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_clip_rect'");
 {
  self->push_clip_rect();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_clip_rect'.");
 return 0;
}

/* method: push_clip_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_push_clip_rect01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_clip_rect'");
 {
  self->push_clip_rect(*rect);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Target_push_clip_rect00(tolua_S);
}

/* method: get_clip_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_clip_rect'");
 {
  CL_ClipRect toluaI_ret = (CL_ClipRect)  self->get_clip_rect();
 {
#ifdef __cplusplus
 void* toluaI_clone = new CL_ClipRect(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(CL_ClipRect));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"CL_ClipRect")),tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_clip_rect'.");
 return 0;
}

/* method: set_clip_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_set_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_clip_rect'");
 {
  self->set_clip_rect(*rect);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_clip_rect'.");
 return 0;
}

/* method: pop_clip_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_pop_clip_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pop_clip_rect'");
 {
  self->pop_clip_rect();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pop_clip_rect'.");
 return 0;
}

/* method: push_translate_offset of class  CL_Target */
static int toluaI_clanbindings_CL_Target_push_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_translate_offset'");
 {
  self->push_translate_offset();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_translate_offset'.");
 return 0;
}

/* method: push_translate_offset of class  CL_Target */
static int toluaI_clanbindings_CL_Target_push_translate_offset01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_translate_offset'");
 {
  self->push_translate_offset(x,y);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Target_push_translate_offset00(tolua_S);
}

/* method: get_translate_offset_x of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_translate_offset_x00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_translate_offset_x'");
 {
  int toluaI_ret = (int)  self->get_translate_offset_x();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_offset_x'.");
 return 0;
}

/* method: get_translate_offset_y of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_translate_offset_y00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"const CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const CL_Target* self = (const CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_translate_offset_y'");
 {
  int toluaI_ret = (int)  self->get_translate_offset_y();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_offset_y'.");
 return 0;
}

/* method: set_translate_offset of class  CL_Target */
static int toluaI_clanbindings_CL_Target_set_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_translate_offset'");
 {
  self->set_translate_offset(x,y);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_translate_offset'.");
 return 0;
}

/* method: pop_translate_offset of class  CL_Target */
static int toluaI_clanbindings_CL_Target_pop_translate_offset00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pop_translate_offset'");
 {
  self->pop_translate_offset();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pop_translate_offset'.");
 return 0;
}

/* method: draw_pixel of class  CL_Target */
static int toluaI_clanbindings_CL_Target_draw_pixel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int color = ((int)  tolua_getnumber(tolua_S,4,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'draw_pixel'");
 {
  self->draw_pixel(x1,y1,color);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_pixel'.");
 return 0;
}

/* method: draw_pixel of class  CL_Target */
static int toluaI_clanbindings_CL_Target_draw_pixel01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  float r = ((float)  tolua_getnumber(tolua_S,4,0));
  float g = ((float)  tolua_getnumber(tolua_S,5,0));
  float b = ((float)  tolua_getnumber(tolua_S,6,0));
  float a = ((float)  tolua_getnumber(tolua_S,7,1.0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'draw_pixel'");
 {
  self->draw_pixel(x,y,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Target_draw_pixel00(tolua_S);
}

/* method: get_pixel of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_pixel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
  float r = ((float)  tolua_getnumber(tolua_S,4,0));
  float g = ((float)  tolua_getnumber(tolua_S,5,0));
  float b = ((float)  tolua_getnumber(tolua_S,6,0));
  float a = ((float)  tolua_getnumber(tolua_S,7,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pixel'");
 {
  self->get_pixel(x,y,&r,&g,&b,&a);
 tolua_pushnumber(tolua_S,(double)r);
 tolua_pushnumber(tolua_S,(double)g);
 tolua_pushnumber(tolua_S,(double)b);
 tolua_pushnumber(tolua_S,(double)a);
 }
 }
 return 4;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pixel'.");
 return 0;
}

/* method: get_pixel of class  CL_Target */
static int toluaI_clanbindings_CL_Target_get_pixel01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x = ((int)  tolua_getnumber(tolua_S,2,0));
  int y = ((int)  tolua_getnumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pixel'");
 {
  int toluaI_ret = (int)  self->get_pixel(x,y);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 return toluaI_clanbindings_CL_Target_get_pixel00(tolua_S);
}

/* method: flip_vertical of class  CL_Target */
static int toluaI_clanbindings_CL_Target_flip_vertical00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flip_vertical'");
 {
  self->flip_vertical();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flip_vertical'.");
 return 0;
}

/* method: flip_horizontal of class  CL_Target */
static int toluaI_clanbindings_CL_Target_flip_horizontal00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flip_horizontal'");
 {
  self->flip_horizontal();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flip_horizontal'.");
 return 0;
}

/* method: fill_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_fill_rect00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fill_rect'");
 {
  self->fill_rect(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fill_rect'.");
 return 0;
}

/* method: fill_rect of class  CL_Target */
static int toluaI_clanbindings_CL_Target_fill_rect01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fill_rect'");
 {
  self->fill_rect(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_clanbindings_CL_Target_fill_rect00(tolua_S);
}

/* method: draw_line of class  CL_Target */
static int toluaI_clanbindings_CL_Target_draw_line00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Target"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  CL_Target* self = (CL_Target*)  tolua_getusertype(tolua_S,1,0);
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
  float r = ((float)  tolua_getnumber(tolua_S,6,0));
  float g = ((float)  tolua_getnumber(tolua_S,7,0));
  float b = ((float)  tolua_getnumber(tolua_S,8,0));
  float a = ((float)  tolua_getnumber(tolua_S,9,1.0f));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'draw_line'");
 {
  self->draw_line(x1,y1,x2,y2,r,g,b,a);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_line'.");
 return 0;
}

/* method: load of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_load00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"CL_ResourceManager"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  const char* resource_id = ((const char*)  tolua_getstring(tolua_S,2,0));
  CL_ResourceManager* manager = ((CL_ResourceManager*)  tolua_getusertype(tolua_S,3,0));
 {
  CL_SurfaceProvider* toluaI_ret = (CL_SurfaceProvider*)  CL_SurfaceProvider::load(resource_id,manager);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_SurfaceProvider"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.");
 return 0;
}

/* method: delete of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* self = (CL_SurfaceProvider*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: get_translate_x of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_get_translate_x00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* self = (CL_SurfaceProvider*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_translate_x'");
 {
  int toluaI_ret = (int)  self->get_translate_x();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_x'.");
 return 0;
}

/* method: get_translate_y of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_get_translate_y00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* self = (CL_SurfaceProvider*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_translate_y'");
 {
  int toluaI_ret = (int)  self->get_translate_y();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_translate_y'.");
 return 0;
}

/* method: uses_src_colorkey of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_uses_src_colorkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* self = (CL_SurfaceProvider*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'uses_src_colorkey'");
 {
  int toluaI_ret = (int)  self->uses_src_colorkey();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'uses_src_colorkey'.");
 return 0;
}

/* method: get_src_colorkey of class  CL_SurfaceProvider */
static int toluaI_clanbindings_CL_SurfaceProvider_get_src_colorkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_SurfaceProvider"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_SurfaceProvider* self = (CL_SurfaceProvider*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_src_colorkey'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_src_colorkey();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_src_colorkey'.");
 return 0;
}

/* method: create of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_create00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,10,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,11)
 )
 goto tolua_lerror;
 else
 {
  int width = ((int)  tolua_getnumber(tolua_S,2,0));
  int height = ((int)  tolua_getnumber(tolua_S,3,0));
  int no_sprs = ((int)  tolua_getnumber(tolua_S,4,1));
  int red_mask = ((int)  tolua_getnumber(tolua_S,5,0xff000000));
  int green_mask = ((int)  tolua_getnumber(tolua_S,6,0x00ff0000));
  int blue_mask = ((int)  tolua_getnumber(tolua_S,7,0x0000ff00));
  int alpha_mask = ((int)  tolua_getnumber(tolua_S,8,0x000000ff));
  int use_transcol = ((int)  tolua_getnumber(tolua_S,9,false));
  unsigned int transcol = ((unsigned int)  tolua_getnumber(tolua_S,10,0));
 {
  CL_Surface* toluaI_ret = (CL_Surface*)  CL_Canvas::create(width,height,no_sprs,red_mask,green_mask,blue_mask,alpha_mask,use_transcol,transcol);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Surface"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.");
 return 0;
}

/* method: new of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_new00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,10,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,11)
 )
 goto tolua_lerror;
 else
 {
  int width = ((int)  tolua_getnumber(tolua_S,2,0));
  int height = ((int)  tolua_getnumber(tolua_S,3,0));
  int no_sprs = ((int)  tolua_getnumber(tolua_S,4,1));
  int red_mask = ((int)  tolua_getnumber(tolua_S,5,0xff000000));
  int green_mask = ((int)  tolua_getnumber(tolua_S,6,0x00ff0000));
  int blue_mask = ((int)  tolua_getnumber(tolua_S,7,0x0000ff00));
  int alpha_mask = ((int)  tolua_getnumber(tolua_S,8,0x000000ff));
  int use_transcol = ((int)  tolua_getnumber(tolua_S,9,false));
  unsigned int transcol = ((unsigned int)  tolua_getnumber(tolua_S,10,0));
 {
  CL_Canvas* toluaI_ret = (CL_Canvas*)  new CL_Canvas(width,height,no_sprs,red_mask,green_mask,blue_mask,alpha_mask,use_transcol,transcol);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Canvas"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.");
 return 0;
}

/* method: delete of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: get_width of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_width00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_width'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_width();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_width'.");
 return 0;
}

/* method: get_height of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_height00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_height'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_height();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_height'.");
 return 0;
}

/* method: get_num_frames of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_num_frames00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_num_frames'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_num_frames();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_frames'.");
 return 0;
}

/* method: get_red_mask of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_red_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_red_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_red_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_red_mask'.");
 return 0;
}

/* method: get_green_mask of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_green_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_green_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_green_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_green_mask'.");
 return 0;
}

/* method: get_blue_mask of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_blue_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_blue_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_blue_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_blue_mask'.");
 return 0;
}

/* method: get_alpha_mask of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_alpha_mask00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_alpha_mask'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_alpha_mask();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_alpha_mask'.");
 return 0;
}

/* method: get_pitch of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_pitch00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pitch'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_pitch();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pitch'.");
 return 0;
}

/* method: is_indexed of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_is_indexed00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_indexed'");
 {
  int toluaI_ret = (int)  self->is_indexed();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_indexed'.");
 return 0;
}

/* method: set_palette of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_set_palette00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"CL_Palette"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
  CL_Palette* palette = ((CL_Palette*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_palette'");
 {
  self->set_palette(palette);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_palette'.");
 return 0;
}

/* method: get_palette of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_palette00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_palette'");
 {
  CL_Palette* toluaI_ret = (CL_Palette*)  self->get_palette();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_Palette"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_palette'.");
 return 0;
}

/* method: set_src_colorkey of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_set_src_colorkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
  unsigned int trans_col = ((unsigned int)  tolua_getnumber(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_src_colorkey'");
 {
  self->set_src_colorkey(trans_col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_src_colorkey'.");
 return 0;
}

/* method: uses_src_colorkey of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_uses_src_colorkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'uses_src_colorkey'");
 {
  int toluaI_ret = (int)  self->uses_src_colorkey();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'uses_src_colorkey'.");
 return 0;
}

/* method: get_src_colorkey of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_src_colorkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_src_colorkey'");
 {
  unsigned int toluaI_ret = (unsigned int)  self->get_src_colorkey();
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_src_colorkey'.");
 return 0;
}

/* method: get_data of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_get_data00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_data'");
 {
  void* toluaI_ret = (void*)  self->get_data();
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_data'.");
 return 0;
}

/* method: lock of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_lock00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'lock'");
 {
  self->lock();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lock'.");
 return 0;
}

/* method: unlock of class  CL_Canvas */
static int toluaI_clanbindings_CL_Canvas_unlock00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_Canvas"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_Canvas* self = (CL_Canvas*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unlock'");
 {
  self->unlock();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unlock'.");
 return 0;
}

/* get function: m_x1 of class  CL_ClipRect */
static int toluaI_get_clanbindings_CL_ClipRect_m_x1(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x1'");
 tolua_pushnumber(tolua_S,(double)self->m_x1);
 return 1;
}

/* set function: m_x1 of class  CL_ClipRect */
static int toluaI_set_clanbindings_CL_ClipRect_m_x1(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x1'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->m_x1 = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_y1 of class  CL_ClipRect */
static int toluaI_get_clanbindings_CL_ClipRect_m_y1(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y1'");
 tolua_pushnumber(tolua_S,(double)self->m_y1);
 return 1;
}

/* set function: m_y1 of class  CL_ClipRect */
static int toluaI_set_clanbindings_CL_ClipRect_m_y1(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y1'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->m_y1 = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_x2 of class  CL_ClipRect */
static int toluaI_get_clanbindings_CL_ClipRect_m_x2(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x2'");
 tolua_pushnumber(tolua_S,(double)self->m_x2);
 return 1;
}

/* set function: m_x2 of class  CL_ClipRect */
static int toluaI_set_clanbindings_CL_ClipRect_m_x2(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_x2'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->m_x2 = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_y2 of class  CL_ClipRect */
static int toluaI_get_clanbindings_CL_ClipRect_m_y2(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y2'");
 tolua_pushnumber(tolua_S,(double)self->m_y2);
 return 1;
}

/* set function: m_y2 of class  CL_ClipRect */
static int toluaI_set_clanbindings_CL_ClipRect_m_y2(lua_State* tolua_S)
{
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_y2'");
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.");
  self->m_y2 = ((int)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* method: new of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_new00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
 {
  CL_ClipRect* toluaI_ret = (CL_ClipRect*)  new CL_ClipRect();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.");
 return 0;
}

/* method: new of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_new01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 {
  CL_ClipRect* toluaI_ret = (CL_ClipRect*)  new CL_ClipRect(*rect);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 return 1;
tolua_lerror:
 return toluaI_clanbindings_CL_ClipRect_new00(tolua_S);
}

/* method: new of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_new02(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  int x1 = ((int)  tolua_getnumber(tolua_S,2,0));
  int y1 = ((int)  tolua_getnumber(tolua_S,3,0));
  int x2 = ((int)  tolua_getnumber(tolua_S,4,0));
  int y2 = ((int)  tolua_getnumber(tolua_S,5,0));
 {
  CL_ClipRect* toluaI_ret = (CL_ClipRect*)  new CL_ClipRect(x1,y1,x2,y2);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 return 1;
tolua_lerror:
 return toluaI_clanbindings_CL_ClipRect_new01(tolua_S);
}

/* method: delete of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'");
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* method: test_clipped of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_test_clipped00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_ClipRect"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'test_clipped'");
 {
  int toluaI_ret = (int)  self->test_clipped(*rect);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test_clipped'.");
 return 0;
}

/* method: test_unclipped of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_test_unclipped00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_ClipRect"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'test_unclipped'");
 {
  int toluaI_ret = (int)  self->test_unclipped(*rect);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test_unclipped'.");
 return 0;
}

/* method: test_all_clipped of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_test_all_clipped00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_ClipRect"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'test_all_clipped'");
 {
  int toluaI_ret = (int)  self->test_all_clipped(*rect);
 tolua_pushnumber(tolua_S,(double)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test_all_clipped'.");
 return 0;
}

/* method: clip of class  CL_ClipRect */
static int toluaI_clanbindings_CL_ClipRect_clip00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"CL_ClipRect"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"const CL_ClipRect"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  CL_ClipRect* self = (CL_ClipRect*)  tolua_getusertype(tolua_S,1,0);
  const CL_ClipRect* rect = ((const CL_ClipRect*)  tolua_getusertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clip'");
 {
  CL_ClipRect toluaI_ret = (CL_ClipRect)  self->clip(*rect);
 {
#ifdef __cplusplus
 void* toluaI_clone = new CL_ClipRect(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(CL_ClipRect));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"CL_ClipRect")),tolua_tag(tolua_S,"CL_ClipRect"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clip'.");
 return 0;
}

/* Open function */
int tolua_clanbindings_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"TRUE",TRUE);
 tolua_constant(tolua_S,NULL,"FALSE",FALSE);
 tolua_cclass(tolua_S,"CL_Display","");
 tolua_function(tolua_S,"CL_Display","delete",toluaI_clanbindings_CL_Display_delete00);
 tolua_function(tolua_S,"CL_Display","flip_display",toluaI_clanbindings_CL_Display_flip_display00);
 tolua_function(tolua_S,"CL_Display","put_display",toluaI_clanbindings_CL_Display_put_display00);
 tolua_function(tolua_S,"CL_Display","sync_buffers",toluaI_clanbindings_CL_Display_sync_buffers00);
 tolua_function(tolua_S,"CL_Display","clear_display",toluaI_clanbindings_CL_Display_clear_display00);
 tolua_function(tolua_S,"CL_Display","set_palette",toluaI_clanbindings_CL_Display_set_palette00);
 tolua_function(tolua_S,"CL_Display","get_palette",toluaI_clanbindings_CL_Display_get_palette00);
 tolua_function(tolua_S,"CL_Display","get_current_card",toluaI_clanbindings_CL_Display_get_current_card00);
 tolua_function(tolua_S,"CL_Display","set_videomode",toluaI_clanbindings_CL_Display_set_videomode00);
 tolua_function(tolua_S,"CL_Display","set_videomode",toluaI_clanbindings_CL_Display_set_videomode01);
 tolua_function(tolua_S,"CL_Display","get_target",toluaI_clanbindings_CL_Display_get_target00);
 tolua_function(tolua_S,"CL_Display","get_width",toluaI_clanbindings_CL_Display_get_width00);
 tolua_function(tolua_S,"CL_Display","get_height",toluaI_clanbindings_CL_Display_get_height00);
 tolua_function(tolua_S,"CL_Display","get_bpp",toluaI_clanbindings_CL_Display_get_bpp00);
 tolua_function(tolua_S,"CL_Display","push_clip_rect",toluaI_clanbindings_CL_Display_push_clip_rect00);
 tolua_function(tolua_S,"CL_Display","push_clip_rect",toluaI_clanbindings_CL_Display_push_clip_rect01);
 tolua_function(tolua_S,"CL_Display","get_clip_rect",toluaI_clanbindings_CL_Display_get_clip_rect00);
 tolua_function(tolua_S,"CL_Display","set_clip_rect",toluaI_clanbindings_CL_Display_set_clip_rect00);
 tolua_function(tolua_S,"CL_Display","pop_clip_rect",toluaI_clanbindings_CL_Display_pop_clip_rect00);
 tolua_function(tolua_S,"CL_Display","push_translate_offset",toluaI_clanbindings_CL_Display_push_translate_offset00);
 tolua_function(tolua_S,"CL_Display","push_translate_offset",toluaI_clanbindings_CL_Display_push_translate_offset01);
 tolua_function(tolua_S,"CL_Display","get_translate_offset_x",toluaI_clanbindings_CL_Display_get_translate_offset_x00);
 tolua_function(tolua_S,"CL_Display","get_translate_offset_y",toluaI_clanbindings_CL_Display_get_translate_offset_y00);
 tolua_function(tolua_S,"CL_Display","set_translate_offset",toluaI_clanbindings_CL_Display_set_translate_offset00);
 tolua_function(tolua_S,"CL_Display","pop_translate_offset",toluaI_clanbindings_CL_Display_pop_translate_offset00);
 tolua_function(tolua_S,"CL_Display","draw_rect",toluaI_clanbindings_CL_Display_draw_rect00);
 tolua_function(tolua_S,"CL_Display","fill_rect",toluaI_clanbindings_CL_Display_fill_rect00);
 tolua_function(tolua_S,"CL_Display","draw_line",toluaI_clanbindings_CL_Display_draw_line00);
 tolua_cclass(tolua_S,"CL_System","");
 tolua_function(tolua_S,"CL_System","keep_alive",toluaI_clanbindings_CL_System_keep_alive00);
 tolua_function(tolua_S,"CL_System","sleep",toluaI_clanbindings_CL_System_sleep00);
 tolua_function(tolua_S,"CL_System","get_time",toluaI_clanbindings_CL_System_get_time00);
 tolua_function(tolua_S,"CL_System","keep_alive",toluaI_clanbindings_CL_System_keep_alive01);
 tolua_function(tolua_S,"CL_System","sleep",toluaI_clanbindings_CL_System_sleep01);
 tolua_function(tolua_S,"CL_System","detect_mmx",toluaI_clanbindings_CL_System_detect_mmx00);
 tolua_function(tolua_S,"CL_System","detect_3dnow",toluaI_clanbindings_CL_System_detect_3dnow00);
 tolua_cclass(tolua_S,"CL_Mouse","CL_InputDevice");
 tolua_function(tolua_S,"CL_Mouse","delete",toluaI_clanbindings_CL_Mouse_delete00);
 tolua_function(tolua_S,"CL_Mouse","get_x",toluaI_clanbindings_CL_Mouse_get_x00);
 tolua_function(tolua_S,"CL_Mouse","get_y",toluaI_clanbindings_CL_Mouse_get_y00);
 tolua_function(tolua_S,"CL_Mouse","left_pressed",toluaI_clanbindings_CL_Mouse_left_pressed00);
 tolua_function(tolua_S,"CL_Mouse","middle_pressed",toluaI_clanbindings_CL_Mouse_middle_pressed00);
 tolua_function(tolua_S,"CL_Mouse","right_pressed",toluaI_clanbindings_CL_Mouse_right_pressed00);
 tolua_cclass(tolua_S,"CL_Keyboard","CL_InputDevice");
 tolua_function(tolua_S,"CL_Keyboard","delete",toluaI_clanbindings_CL_Keyboard_delete00);
 tolua_function(tolua_S,"CL_Keyboard","get_keycode",toluaI_clanbindings_CL_Keyboard_get_keycode00);
 tolua_constant(tolua_S,NULL,"CL_KEY_A",CL_KEY_A);
 tolua_constant(tolua_S,NULL,"CL_KEY_B",CL_KEY_B);
 tolua_constant(tolua_S,NULL,"CL_KEY_C",CL_KEY_C);
 tolua_constant(tolua_S,NULL,"CL_KEY_D",CL_KEY_D);
 tolua_constant(tolua_S,NULL,"CL_KEY_E",CL_KEY_E);
 tolua_constant(tolua_S,NULL,"CL_KEY_F",CL_KEY_F);
 tolua_constant(tolua_S,NULL,"CL_KEY_G",CL_KEY_G);
 tolua_constant(tolua_S,NULL,"CL_KEY_H",CL_KEY_H);
 tolua_constant(tolua_S,NULL,"CL_KEY_I",CL_KEY_I);
 tolua_constant(tolua_S,NULL,"CL_KEY_J",CL_KEY_J);
 tolua_constant(tolua_S,NULL,"CL_KEY_K",CL_KEY_K);
 tolua_constant(tolua_S,NULL,"CL_KEY_L",CL_KEY_L);
 tolua_constant(tolua_S,NULL,"CL_KEY_M",CL_KEY_M);
 tolua_constant(tolua_S,NULL,"CL_KEY_N",CL_KEY_N);
 tolua_constant(tolua_S,NULL,"CL_KEY_O",CL_KEY_O);
 tolua_constant(tolua_S,NULL,"CL_KEY_P",CL_KEY_P);
 tolua_constant(tolua_S,NULL,"CL_KEY_Q",CL_KEY_Q);
 tolua_constant(tolua_S,NULL,"CL_KEY_R",CL_KEY_R);
 tolua_constant(tolua_S,NULL,"CL_KEY_S",CL_KEY_S);
 tolua_constant(tolua_S,NULL,"CL_KEY_T",CL_KEY_T);
 tolua_constant(tolua_S,NULL,"CL_KEY_U",CL_KEY_U);
 tolua_constant(tolua_S,NULL,"CL_KEY_V",CL_KEY_V);
 tolua_constant(tolua_S,NULL,"CL_KEY_W",CL_KEY_W);
 tolua_constant(tolua_S,NULL,"CL_KEY_X",CL_KEY_X);
 tolua_constant(tolua_S,NULL,"CL_KEY_Y",CL_KEY_Y);
 tolua_constant(tolua_S,NULL,"CL_KEY_Z",CL_KEY_Z);
 tolua_constant(tolua_S,NULL,"CL_KEY_0",CL_KEY_0);
 tolua_constant(tolua_S,NULL,"CL_KEY_1",CL_KEY_1);
 tolua_constant(tolua_S,NULL,"CL_KEY_2",CL_KEY_2);
 tolua_constant(tolua_S,NULL,"CL_KEY_3",CL_KEY_3);
 tolua_constant(tolua_S,NULL,"CL_KEY_4",CL_KEY_4);
 tolua_constant(tolua_S,NULL,"CL_KEY_5",CL_KEY_5);
 tolua_constant(tolua_S,NULL,"CL_KEY_6",CL_KEY_6);
 tolua_constant(tolua_S,NULL,"CL_KEY_7",CL_KEY_7);
 tolua_constant(tolua_S,NULL,"CL_KEY_8",CL_KEY_8);
 tolua_constant(tolua_S,NULL,"CL_KEY_9",CL_KEY_9);
 tolua_constant(tolua_S,NULL,"CL_KEY_F1",CL_KEY_F1);
 tolua_constant(tolua_S,NULL,"CL_KEY_F2",CL_KEY_F2);
 tolua_constant(tolua_S,NULL,"CL_KEY_F3",CL_KEY_F3);
 tolua_constant(tolua_S,NULL,"CL_KEY_F4",CL_KEY_F4);
 tolua_constant(tolua_S,NULL,"CL_KEY_F5",CL_KEY_F5);
 tolua_constant(tolua_S,NULL,"CL_KEY_F6",CL_KEY_F6);
 tolua_constant(tolua_S,NULL,"CL_KEY_F7",CL_KEY_F7);
 tolua_constant(tolua_S,NULL,"CL_KEY_F8",CL_KEY_F8);
 tolua_constant(tolua_S,NULL,"CL_KEY_F9",CL_KEY_F9);
 tolua_constant(tolua_S,NULL,"CL_KEY_F10",CL_KEY_F10);
 tolua_constant(tolua_S,NULL,"CL_KEY_F11",CL_KEY_F11);
 tolua_constant(tolua_S,NULL,"CL_KEY_F12",CL_KEY_F12);
 tolua_constant(tolua_S,NULL,"CL_KEY_ESCAPE",CL_KEY_ESCAPE);
 tolua_constant(tolua_S,NULL,"CL_KEY_LEFT",CL_KEY_LEFT);
 tolua_constant(tolua_S,NULL,"CL_KEY_RIGHT",CL_KEY_RIGHT);
 tolua_constant(tolua_S,NULL,"CL_KEY_UP",CL_KEY_UP);
 tolua_constant(tolua_S,NULL,"CL_KEY_DOWN",CL_KEY_DOWN);
 tolua_constant(tolua_S,NULL,"CL_KEY_LCTRL",CL_KEY_LCTRL);
 tolua_constant(tolua_S,NULL,"CL_KEY_RCTRL",CL_KEY_RCTRL);
 tolua_constant(tolua_S,NULL,"CL_KEY_LSHIFT",CL_KEY_LSHIFT);
 tolua_constant(tolua_S,NULL,"CL_KEY_RSHIFT",CL_KEY_RSHIFT);
 tolua_constant(tolua_S,NULL,"CL_KEY_ALT",CL_KEY_ALT);
 tolua_constant(tolua_S,NULL,"CL_KEY_ALTGR",CL_KEY_ALTGR);
 tolua_constant(tolua_S,NULL,"CL_KEY_TAB",CL_KEY_TAB);
 tolua_constant(tolua_S,NULL,"CL_KEY_ENTER",CL_KEY_ENTER);
 tolua_constant(tolua_S,NULL,"CL_KEY_SPACE",CL_KEY_SPACE);
 tolua_constant(tolua_S,NULL,"CL_KEY_BACKSPACE",CL_KEY_BACKSPACE);
 tolua_constant(tolua_S,NULL,"CL_KEY_INSERT",CL_KEY_INSERT);
 tolua_constant(tolua_S,NULL,"CL_KEY_DELETE",CL_KEY_DELETE);
 tolua_constant(tolua_S,NULL,"CL_KEY_HOME",CL_KEY_HOME);
 tolua_constant(tolua_S,NULL,"CL_KEY_END",CL_KEY_END);
 tolua_constant(tolua_S,NULL,"CL_KEY_PAGEUP",CL_KEY_PAGEUP);
 tolua_constant(tolua_S,NULL,"CL_KEY_PAGEDOWN",CL_KEY_PAGEDOWN);
 tolua_constant(tolua_S,NULL,"CL_KEY_CAPSLOCK",CL_KEY_CAPSLOCK);
 tolua_constant(tolua_S,NULL,"CL_KEY_NUMLOCK",CL_KEY_NUMLOCK);
 tolua_constant(tolua_S,NULL,"CL_KEY_SCRLOCK",CL_KEY_SCRLOCK);
 tolua_constant(tolua_S,NULL,"CL_KEY_PRINT",CL_KEY_PRINT);
 tolua_constant(tolua_S,NULL,"CL_KEY_PAUSE",CL_KEY_PAUSE);
 tolua_constant(tolua_S,NULL,"CL_KEY_KP_DIV",CL_KEY_KP_DIV);
 tolua_constant(tolua_S,NULL,"CL_KEY_KP_MULT",CL_KEY_KP_MULT);
 tolua_constant(tolua_S,NULL,"CL_KEY_KP_MINUS",CL_KEY_KP_MINUS);
 tolua_constant(tolua_S,NULL,"CL_KEY_KP_PLUS",CL_KEY_KP_PLUS);
 tolua_constant(tolua_S,NULL,"CL_KEY_KP_ENTER",CL_KEY_KP_ENTER);
 tolua_constant(tolua_S,NULL,"CL_KEY_NONE_OF_THE_ABOVE",CL_KEY_NONE_OF_THE_ABOVE);
 tolua_constant(tolua_S,NULL,"CL_NUM_KEYS",CL_NUM_KEYS);
 tolua_cclass(tolua_S,"CL_Surface","");
 tolua_function(tolua_S,"CL_Surface","load",toluaI_clanbindings_CL_Surface_load00);
 tolua_function(tolua_S,"CL_Surface","create",toluaI_clanbindings_CL_Surface_create00);
 tolua_function(tolua_S,"CL_Surface","create_dynamic",toluaI_clanbindings_CL_Surface_create_dynamic00);
 tolua_function(tolua_S,"CL_Surface","delete",toluaI_clanbindings_CL_Surface_delete00);
 tolua_function(tolua_S,"CL_Surface","reload",toluaI_clanbindings_CL_Surface_reload00);
 tolua_function(tolua_S,"CL_Surface","get_provider",toluaI_clanbindings_CL_Surface_get_provider00);
 tolua_function(tolua_S,"CL_Surface","put_screen",toluaI_clanbindings_CL_Surface_put_screen00);
 tolua_function(tolua_S,"CL_Surface","put_screen",toluaI_clanbindings_CL_Surface_put_screen01);
 tolua_function(tolua_S,"CL_Surface","put_screen",toluaI_clanbindings_CL_Surface_put_screen02);
 tolua_function(tolua_S,"CL_Surface","put_target",toluaI_clanbindings_CL_Surface_put_target00);
 tolua_function(tolua_S,"CL_Surface","get_width",toluaI_clanbindings_CL_Surface_get_width00);
 tolua_function(tolua_S,"CL_Surface","get_height",toluaI_clanbindings_CL_Surface_get_height00);
 tolua_function(tolua_S,"CL_Surface","get_num_frames",toluaI_clanbindings_CL_Surface_get_num_frames00);
 tolua_function(tolua_S,"CL_Surface","is_video",toluaI_clanbindings_CL_Surface_is_video00);
 tolua_function(tolua_S,"CL_Surface","is_loaded",toluaI_clanbindings_CL_Surface_is_loaded00);
 tolua_function(tolua_S,"CL_Surface","convert_video",toluaI_clanbindings_CL_Surface_convert_video00);
 tolua_function(tolua_S,"CL_Surface","convert_system",toluaI_clanbindings_CL_Surface_convert_system00);
 tolua_function(tolua_S,"CL_Surface","flush",toluaI_clanbindings_CL_Surface_flush00);
 tolua_cclass(tolua_S,"CL_Target","");
 tolua_function(tolua_S,"CL_Target","delete",toluaI_clanbindings_CL_Target_delete00);
 tolua_function(tolua_S,"CL_Target","is_video",toluaI_clanbindings_CL_Target_is_video00);
 tolua_function(tolua_S,"CL_Target","lock",toluaI_clanbindings_CL_Target_lock00);
 tolua_function(tolua_S,"CL_Target","unlock",toluaI_clanbindings_CL_Target_unlock00);
 tolua_function(tolua_S,"CL_Target","get_data",toluaI_clanbindings_CL_Target_get_data00);
 tolua_function(tolua_S,"CL_Target","get_num_frames",toluaI_clanbindings_CL_Target_get_num_frames00);
 tolua_function(tolua_S,"CL_Target","get_width",toluaI_clanbindings_CL_Target_get_width00);
 tolua_function(tolua_S,"CL_Target","get_height",toluaI_clanbindings_CL_Target_get_height00);
 tolua_function(tolua_S,"CL_Target","get_pitch",toluaI_clanbindings_CL_Target_get_pitch00);
 tolua_function(tolua_S,"CL_Target","get_depth",toluaI_clanbindings_CL_Target_get_depth00);
 tolua_function(tolua_S,"CL_Target","get_bytes_per_pixel",toluaI_clanbindings_CL_Target_get_bytes_per_pixel00);
 tolua_function(tolua_S,"CL_Target","is_indexed",toluaI_clanbindings_CL_Target_is_indexed00);
 tolua_function(tolua_S,"CL_Target","get_red_mask",toluaI_clanbindings_CL_Target_get_red_mask00);
 tolua_function(tolua_S,"CL_Target","get_green_mask",toluaI_clanbindings_CL_Target_get_green_mask00);
 tolua_function(tolua_S,"CL_Target","get_blue_mask",toluaI_clanbindings_CL_Target_get_blue_mask00);
 tolua_function(tolua_S,"CL_Target","get_alpha_mask",toluaI_clanbindings_CL_Target_get_alpha_mask00);
 tolua_function(tolua_S,"CL_Target","get_palette",toluaI_clanbindings_CL_Target_get_palette00);
 tolua_function(tolua_S,"CL_Target","push_clip_rect",toluaI_clanbindings_CL_Target_push_clip_rect00);
 tolua_function(tolua_S,"CL_Target","push_clip_rect",toluaI_clanbindings_CL_Target_push_clip_rect01);
 tolua_function(tolua_S,"CL_Target","get_clip_rect",toluaI_clanbindings_CL_Target_get_clip_rect00);
 tolua_function(tolua_S,"CL_Target","set_clip_rect",toluaI_clanbindings_CL_Target_set_clip_rect00);
 tolua_function(tolua_S,"CL_Target","pop_clip_rect",toluaI_clanbindings_CL_Target_pop_clip_rect00);
 tolua_function(tolua_S,"CL_Target","push_translate_offset",toluaI_clanbindings_CL_Target_push_translate_offset00);
 tolua_function(tolua_S,"CL_Target","push_translate_offset",toluaI_clanbindings_CL_Target_push_translate_offset01);
 tolua_function(tolua_S,"CL_Target","get_translate_offset_x",toluaI_clanbindings_CL_Target_get_translate_offset_x00);
 tolua_function(tolua_S,"CL_Target","get_translate_offset_y",toluaI_clanbindings_CL_Target_get_translate_offset_y00);
 tolua_function(tolua_S,"CL_Target","set_translate_offset",toluaI_clanbindings_CL_Target_set_translate_offset00);
 tolua_function(tolua_S,"CL_Target","pop_translate_offset",toluaI_clanbindings_CL_Target_pop_translate_offset00);
 tolua_function(tolua_S,"CL_Target","draw_pixel",toluaI_clanbindings_CL_Target_draw_pixel00);
 tolua_function(tolua_S,"CL_Target","draw_pixel",toluaI_clanbindings_CL_Target_draw_pixel01);
 tolua_function(tolua_S,"CL_Target","get_pixel",toluaI_clanbindings_CL_Target_get_pixel00);
 tolua_function(tolua_S,"CL_Target","get_pixel",toluaI_clanbindings_CL_Target_get_pixel01);
 tolua_function(tolua_S,"CL_Target","flip_vertical",toluaI_clanbindings_CL_Target_flip_vertical00);
 tolua_function(tolua_S,"CL_Target","flip_horizontal",toluaI_clanbindings_CL_Target_flip_horizontal00);
 tolua_function(tolua_S,"CL_Target","fill_rect",toluaI_clanbindings_CL_Target_fill_rect00);
 tolua_function(tolua_S,"CL_Target","fill_rect",toluaI_clanbindings_CL_Target_fill_rect01);
 tolua_function(tolua_S,"CL_Target","draw_line",toluaI_clanbindings_CL_Target_draw_line00);
 tolua_cclass(tolua_S,"CL_SurfaceProvider","CL_Target");
 tolua_function(tolua_S,"CL_SurfaceProvider","load",toluaI_clanbindings_CL_SurfaceProvider_load00);
 tolua_function(tolua_S,"CL_SurfaceProvider","delete",toluaI_clanbindings_CL_SurfaceProvider_delete00);
 tolua_function(tolua_S,"CL_SurfaceProvider","get_translate_x",toluaI_clanbindings_CL_SurfaceProvider_get_translate_x00);
 tolua_function(tolua_S,"CL_SurfaceProvider","get_translate_y",toluaI_clanbindings_CL_SurfaceProvider_get_translate_y00);
 tolua_function(tolua_S,"CL_SurfaceProvider","uses_src_colorkey",toluaI_clanbindings_CL_SurfaceProvider_uses_src_colorkey00);
 tolua_function(tolua_S,"CL_SurfaceProvider","get_src_colorkey",toluaI_clanbindings_CL_SurfaceProvider_get_src_colorkey00);
 tolua_cclass(tolua_S,"CL_Canvas","CL_SurfaceProvider");
 tolua_function(tolua_S,"CL_Canvas","create",toluaI_clanbindings_CL_Canvas_create00);
 tolua_function(tolua_S,"CL_Canvas","new",toluaI_clanbindings_CL_Canvas_new00);
 tolua_function(tolua_S,"CL_Canvas","delete",toluaI_clanbindings_CL_Canvas_delete00);
 tolua_function(tolua_S,"CL_Canvas","get_width",toluaI_clanbindings_CL_Canvas_get_width00);
 tolua_function(tolua_S,"CL_Canvas","get_height",toluaI_clanbindings_CL_Canvas_get_height00);
 tolua_function(tolua_S,"CL_Canvas","get_num_frames",toluaI_clanbindings_CL_Canvas_get_num_frames00);
 tolua_function(tolua_S,"CL_Canvas","get_red_mask",toluaI_clanbindings_CL_Canvas_get_red_mask00);
 tolua_function(tolua_S,"CL_Canvas","get_green_mask",toluaI_clanbindings_CL_Canvas_get_green_mask00);
 tolua_function(tolua_S,"CL_Canvas","get_blue_mask",toluaI_clanbindings_CL_Canvas_get_blue_mask00);
 tolua_function(tolua_S,"CL_Canvas","get_alpha_mask",toluaI_clanbindings_CL_Canvas_get_alpha_mask00);
 tolua_function(tolua_S,"CL_Canvas","get_pitch",toluaI_clanbindings_CL_Canvas_get_pitch00);
 tolua_function(tolua_S,"CL_Canvas","is_indexed",toluaI_clanbindings_CL_Canvas_is_indexed00);
 tolua_function(tolua_S,"CL_Canvas","set_palette",toluaI_clanbindings_CL_Canvas_set_palette00);
 tolua_function(tolua_S,"CL_Canvas","get_palette",toluaI_clanbindings_CL_Canvas_get_palette00);
 tolua_function(tolua_S,"CL_Canvas","set_src_colorkey",toluaI_clanbindings_CL_Canvas_set_src_colorkey00);
 tolua_function(tolua_S,"CL_Canvas","uses_src_colorkey",toluaI_clanbindings_CL_Canvas_uses_src_colorkey00);
 tolua_function(tolua_S,"CL_Canvas","get_src_colorkey",toluaI_clanbindings_CL_Canvas_get_src_colorkey00);
 tolua_function(tolua_S,"CL_Canvas","get_data",toluaI_clanbindings_CL_Canvas_get_data00);
 tolua_function(tolua_S,"CL_Canvas","lock",toluaI_clanbindings_CL_Canvas_lock00);
 tolua_function(tolua_S,"CL_Canvas","unlock",toluaI_clanbindings_CL_Canvas_unlock00);
 tolua_cclass(tolua_S,"CL_ClipRect","");
 tolua_tablevar(tolua_S,"CL_ClipRect","m_x1",toluaI_get_clanbindings_CL_ClipRect_m_x1,toluaI_set_clanbindings_CL_ClipRect_m_x1);
 tolua_tablevar(tolua_S,"CL_ClipRect","m_y1",toluaI_get_clanbindings_CL_ClipRect_m_y1,toluaI_set_clanbindings_CL_ClipRect_m_y1);
 tolua_tablevar(tolua_S,"CL_ClipRect","m_x2",toluaI_get_clanbindings_CL_ClipRect_m_x2,toluaI_set_clanbindings_CL_ClipRect_m_x2);
 tolua_tablevar(tolua_S,"CL_ClipRect","m_y2",toluaI_get_clanbindings_CL_ClipRect_m_y2,toluaI_set_clanbindings_CL_ClipRect_m_y2);
 tolua_function(tolua_S,"CL_ClipRect","new",toluaI_clanbindings_CL_ClipRect_new00);
 tolua_function(tolua_S,"CL_ClipRect","new",toluaI_clanbindings_CL_ClipRect_new01);
 tolua_function(tolua_S,"CL_ClipRect","new",toluaI_clanbindings_CL_ClipRect_new02);
 tolua_function(tolua_S,"CL_ClipRect","delete",toluaI_clanbindings_CL_ClipRect_delete00);
 tolua_function(tolua_S,"CL_ClipRect","test_clipped",toluaI_clanbindings_CL_ClipRect_test_clipped00);
 tolua_function(tolua_S,"CL_ClipRect","test_unclipped",toluaI_clanbindings_CL_ClipRect_test_unclipped00);
 tolua_function(tolua_S,"CL_ClipRect","test_all_clipped",toluaI_clanbindings_CL_ClipRect_test_all_clipped00);
 tolua_function(tolua_S,"CL_ClipRect","clip",toluaI_clanbindings_CL_ClipRect_clip00);
 return 1;
}
/* Close function */
void tolua_clanbindings_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TRUE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FALSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Display");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_System");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Mouse");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Keyboard");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_A");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_B");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_C");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_D");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_E");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_G");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_H");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_I");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_J");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_K");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_L");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_M");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_N");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_O");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_P");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_Q");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_R");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_S");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_T");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_U");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_V");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_W");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_X");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_Y");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_Z");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_0");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_1");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_3");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_4");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_5");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_6");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_7");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_8");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_9");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F1");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F3");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F4");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F5");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F6");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F7");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F8");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F9");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F10");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F11");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_F12");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_ESCAPE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_LEFT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_RIGHT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_UP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_DOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_LCTRL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_RCTRL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_LSHIFT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_RSHIFT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_ALT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_ALTGR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_TAB");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_ENTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_SPACE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_BACKSPACE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_INSERT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_DELETE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_HOME");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_END");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_PAGEUP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_PAGEDOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_CAPSLOCK");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_NUMLOCK");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_SCRLOCK");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_PRINT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_PAUSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_KP_DIV");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_KP_MULT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_KP_MINUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_KP_PLUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_KP_ENTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_KEY_NONE_OF_THE_ABOVE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_NUM_KEYS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Surface");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Target");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_SurfaceProvider");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_Canvas");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CL_ClipRect");
}
