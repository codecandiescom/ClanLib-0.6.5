/*
	$Id: dx_errors.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "dx_errors.h"

char *get_dderr(HRESULT errcode)
{
	switch(errcode)
	{
	case DDERR_ALREADYINITIALIZED:
		return "DDERR_ALREADYINITIALIZED: This object is already initialized";

	case DDERR_CANNOTATTACHSURFACE:
		return "DDERR_CANNOTATTACHSURFACE: This surface can not be attached to the requested surface.";

	case DDERR_CANNOTDETACHSURFACE:
		return "DDERR_CANNOTDETACHSURFACE: This surface can not be detached from the requested surface.";

	case DDERR_CURRENTLYNOTAVAIL:
		return "DDERR_CURRENTLYNOTAVAIL : Support is currently not available.";

	case DDERR_EXCEPTION:
		return "DDERR_EXCEPTION: An exception was encountered while performing the requested operation";

	case DDERR_GENERIC:
		return "DDERR_GENERIC: Generic failure.";

	case DDERR_HEIGHTALIGN:
		return "DDERR_HEIGHTALIGN: Height of rectangle provided is not a multiple of reqd alignment";

	case DDERR_INCOMPATIBLEPRIMARY:
		return "DDERR_INCOMPATIBLEPRIMARY: Unable to match primary surface creation request with existing";

	case DDERR_INVALIDCAPS:
		return "DDERR_INVALIDCAPS: One or more of the caps bits passed to the callback are incorrect.";

	case DDERR_INVALIDCLIPLIST:
		return "DDERR_INVALIDCLIPLIST: DirectDraw does not support provided Cliplist.";

	case DDERR_INVALIDMODE:
		return "DDERR_INVALIDMODE: DirectDraw does not support the requested mode";

	case DDERR_INVALIDOBJECT:
		return "DDERR_INVALIDOBJECT: DirectDraw received a pointer that was an invalid DIRECTDRAW object.";

	case DDERR_INVALIDPARAMS:
		return "DDERR_INVALIDPARAMS: One or more of the parameters passed to the callback function are incorrect";

	case DDERR_INVALIDPIXELFORMAT:
		return "DDERR_INVALIDPIXELFORMAT: pixel format was invalid as specified";

	case DDERR_INVALIDRECT:
		return "DDERR_INVALIDRECT: Rectangle provided was invalid.";

	case DDERR_LOCKEDSURFACES:
		return "DDERR_LOCKEDSURFACES: Operation could not be carried out because one or more surfaces are locked";

	case DDERR_NO3D:
		return "DDERR_NO3D: There is no 3D present.";

	case DDERR_NOALPHAHW:
		return "DDERR_NOALPHAHW: Operation could not be carried out because there is no alpha accleration";

	case DDERR_NOCLIPLIST:
		return "DDERR_NOCLIPLIST: no clip list available";

	case DDERR_NOCOLORCONVHW:
		return "DDERR_NOCOLORCONVHW: Operation could not be carried out because there is no color conversion hardware present or available.";

	case DDERR_NOCOOPERATIVELEVELSET:
		return "DDERR_NOCOOPERATIVELEVELSET: Create function called without DirectDraw object method SetCooperativeLevel being called.";

	case DDERR_NOCOLORKEY:
		return "DDERR_NOCOLORKEY: Surface doesn't currently have a color key";

	case DDERR_NOCOLORKEYHW:
		return "DDERR_NOCOLORKEYHW: Operation could not be carried out because there is no hardware support of the dest color key.";

	case DDERR_NODIRECTDRAWSUPPORT:
		return "DDERR_NODIRECTDRAWSUPPORT: No DirectDraw support possible with current display driver";

	case DDERR_NOEXCLUSIVEMODE:
		return "DDERR_NOEXCLUSIVEMODE: Operation requires the application to have exclusive mode but the application does not have exclusive mode.";

	case DDERR_NOFLIPHW:
		return "DDERR_NOFLIPHW: Flipping visible surfaces is not supported.";

	case DDERR_NOGDI:
		return "DDERR_NOGDI: There is no GDI present.";

	case DDERR_NOMIRRORHW:
		return "DDERR_NOMIRRORHW: Operation could not be carried out because there is no hardware present or available.";

	case DDERR_NOTFOUND:
		return "DDERR_NOTFOUND: Requested item was not found";

	case DDERR_NOOVERLAYHW:
		return "DDERR_NOOVERLAYHW: Operation could not be carried out because there is no overlay hardware present or available.";

	case DDERR_NORASTEROPHW:
		return "DDERR_NORASTEROPHW: Operation could not be carried out because there is no appropriate raster op hardware present or available.";

	case DDERR_NOROTATIONHW:
		return "DDERR_NOROTATIONHW: Operation could not be carried out because there is no rotation hardware present or available.";

	case DDERR_NOSTRETCHHW:
		return "DDERR_NOSTRETCHHW: Operation could not be carried out because there is no hardware support for stretching";

	case DDERR_NOT4BITCOLOR:
		return "DDERR_NOT4BITCOLOR: DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.";

	case DDERR_NOT4BITCOLORINDEX:
		return "DDERR_NOT4BITCOLORINDEX: DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.";

	case DDERR_NOT8BITCOLOR:
		return "DDERR_NOT8BITCOLOR: DirectDraw Surface is not in 8 bit color mode and the requested operation requires 8 bit color.";

	case DDERR_NOTEXTUREHW:
		return "DDERR_NOTEXTUREHW: Operation could not be carried out because there is no texture mapping hardware present or available.";

	case DDERR_NOVSYNCHW:
		return "DDERR_NOVSYNCHW: Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.";

	case DDERR_NOZBUFFERHW:
		return "DDERR_NOZBUFFERHW: Operation could not be carried out because there is no hardware support for zbuffer blting.";

	case DDERR_NOZOVERLAYHW:
		return "DDERR_NOZOVERLAYHW: Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.";

	case DDERR_OUTOFCAPS:
		return "DDERR_OUTOFCAPS: The hardware needed for the requested operation has already been allocated.";

	case DDERR_OUTOFMEMORY:
		return "DDERR_OUTOFMEMORY: DirectDraw does not have enough memory to perform the operation.";

	case DDERR_OUTOFVIDEOMEMORY:
		return "DDERR_OUTOFVIDEOMEMORY: DirectDraw does not have enough memory to perform the operation.";

	case DDERR_OVERLAYCANTCLIP:
		return "DDERR_OVERLAYCANTCLIP: hardware does not support clipped overlays";

	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
		return "DDERR_OVERLAYCOLORKEYONLYONEACTIVE: Can only have ony color key active at one time for overlays";

	case DDERR_PALETTEBUSY:
		return "DDERR_PALETTEBUSY: Access to this palette is being refused because the palette is already locked by another thread.";

	case DDERR_COLORKEYNOTSET:
		return "DDERR_COLORKEYNOTSET: No src color key specified for this operation.";

	case DDERR_SURFACEALREADYATTACHED:
		return "DDERR_SURFACEALREADYATTACHED: This surface is already attached to the surface it is being attached to.";

	case DDERR_SURFACEALREADYDEPENDENT:
		return "DDERR_SURFACEALREADYDEPENDENT: This surface is already a dependency of the surface it is being made a dependency of.";

	case DDERR_SURFACEBUSY:
		return "DDERR_SURFACEBUSY: Access to this surface is being refused because the surface is already locked by another thread.";

	case DDERR_CANTLOCKSURFACE:
		return "DDERR_CANTLOCKSURFACE: Access to this surface is being refused because no driver exists which can supply a pointer to the surface. This is most likely to happen when attempting to lock the primary surface when no DCI provider is present. Will also happen on attempts to lock an optimized surface.";

	case DDERR_SURFACEISOBSCURED:
		return "DDERR_SURFACEISOBSCURED: Access to Surface refused because Surface is obscured.";

	case DDERR_SURFACELOST:
		return "DDERR_SURFACELOST: Access to this surface is being refused because the surface is gone. The DIRECTDRAWSURFACE object representing this surface should have Restore called on it.";

	case DDERR_SURFACENOTATTACHED:
		return "DDERR_SURFACENOTATTACHED: The requested surface is not attached.";

	case DDERR_TOOBIGHEIGHT:
		return "DDERR_TOOBIGHEIGHT: Height requested by DirectDraw is too large.";

	case DDERR_TOOBIGSIZE:
		return "DDERR_TOOBIGSIZE: Size requested by DirectDraw is too large --	 The individual height and width are OK.";

	case DDERR_TOOBIGWIDTH:
		return "DDERR_TOOBIGWIDTH: Width requested by DirectDraw is too large.";

	case DDERR_UNSUPPORTED:
		return "DDERR_UNSUPPORTED: Action not supported.";

	case DDERR_UNSUPPORTEDFORMAT:
		return "DDERR_UNSUPPORTEDFORMAT: FOURCC format requested is unsupported by DirectDraw";

	case DDERR_UNSUPPORTEDMASK:
		return "DDERR_UNSUPPORTEDMASK: Bitmask in the pixel format requested is unsupported by DirectDraw";

	case DDERR_VERTICALBLANKINPROGRESS:
		return "DDERR_VERTICALBLANKINPROGRESS: vertical blank is in progress";

	case DDERR_WASSTILLDRAWING:
		return "DDERR_WASSTILLDRAWING: Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.";

	case DDERR_XALIGN:
		return "DDERR_XALIGN: Rectangle provided was not horizontally aligned on reqd. boundary";

	case DDERR_INVALIDDIRECTDRAWGUID:
		return "DDERR_INVALIDDIRECTDRAWGUID: The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.";

	case DDERR_DIRECTDRAWALREADYCREATED:
		return "DDERR_DIRECTDRAWALREADYCREATED: A DirectDraw object representing this driver has already been created for this process.";

	case DDERR_NODIRECTDRAWHW:
		return "DDERR_NODIRECTDRAWHW: A hardware only DirectDraw object creation was attempted but the driver did not support any hardware.";

	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		return "DDERR_PRIMARYSURFACEALREADYEXISTS: this process already has created a primary surface";

	case DDERR_NOEMULATION:
		return "DDERR_NOEMULATION: software emulation not available.";

	case DDERR_REGIONTOOSMALL:
		return "DDERR_REGIONTOOSMALL: region passed to Clipper::GetClipList is too small.";

	case DDERR_CLIPPERISUSINGHWND:
		return "DDERR_CLIPPERISUSINGHWND: an attempt was made to set a clip list for a clipper objec that is already monitoring an hwnd.";

	case DDERR_NOCLIPPERATTACHED:
		return "DDERR_NOCLIPPERATTACHED: No clipper object attached to surface object";

	case DDERR_NOHWND:
		return "DDERR_NOHWND: Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.";

	case DDERR_HWNDSUBCLASSED:
		return "DDERR_HWNDSUBCLASSED: HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.";

	case DDERR_HWNDALREADYSET:
		return "DDERR_HWNDALREADYSET: The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.";

	case DDERR_NOPALETTEATTACHED:
		return "DDERR_NOPALETTEATTACHED: No palette object attached to this surface.";

	case DDERR_NOPALETTEHW:
		return "DDERR_NOPALETTEHW: No hardware support for 16 or 256 color palettes.";

	case DDERR_BLTFASTCANTCLIP:
		return "DDERR_BLTFASTCANTCLIP: If a clipper object is attached to the source surface passed into a BltFast call.";

	case DDERR_NOBLTHW:
		return "DDERR_NOBLTHW: No blter.";

	case DDERR_NODDROPSHW:
		return "DDERR_NODDROPSHW: No DirectDraw ROP hardware.";

	case DDERR_OVERLAYNOTVISIBLE:
		return "DDERR_OVERLAYNOTVISIBLE: returned when GetOverlayPosition is called on a hidden overlay";

	case DDERR_NOOVERLAYDEST:
		return "DDERR_NOOVERLAYDEST: returned when GetOverlayPosition is called on a overlay that UpdateOverlay has never been called on to establish a destionation.";

	case DDERR_INVALIDPOSITION:
		return "DDERR_INVALIDPOSITION: returned when the position of the overlay on the destionation is no longer legal for that destionation.";

	case DDERR_NOTAOVERLAYSURFACE:
		return "DDERR_NOTAOVERLAYSURFACE: returned when an overlay member is called for a non-overlay surface";

	case DDERR_EXCLUSIVEMODEALREADYSET:
		return "DDERR_EXCLUSIVEMODEALREADYSET: An attempt was made to set the cooperative level when it was already set to exclusive.";

	case DDERR_NOTFLIPPABLE:
		return "DDERR_NOTFLIPPABLE: An attempt has been made to flip a surface that is not flippable.";

	case DDERR_CANTDUPLICATE:
		return "DDERR_CANTDUPLICATE: Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.";

	case DDERR_NOTLOCKED:
		return "DDERR_NOTLOCKED: Surface was not locked.	An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.";

	case DDERR_CANTCREATEDC:
		return "DDERR_CANTCREATEDC: Windows can not create any more DCs";

	case DDERR_NODC:
		return "DDERR_NODC: No DC was ever created for this surface.";

	case DDERR_WRONGMODE:
		return "DDERR_WRONGMODE: This surface can not be restored because it was created in a different mode.";

	case DDERR_IMPLICITLYCREATED:
		return "DDERR_IMPLICITLYCREATED: This surface can not be restored because it is an implicitly created surface.";

	case DDERR_NOTPALETTIZED:
		return "DDERR_NOTPALETTIZED: The surface being used is not a palette-based surface";

	case DDERR_UNSUPPORTEDMODE:
		return "DDERR_UNSUPPORTEDMODE: The display is currently in an unsupported mode";

	case DDERR_NOMIPMAPHW:
		return "DDERR_NOMIPMAPHW: Operation could not be carried out because there is no mip-map texture mapping hardware present or available.";

	case DDERR_INVALIDSURFACETYPE:
		return "DDERR_INVALIDSURFACETYPE: The requested action could not be performed because the surface was of the wrong type.";

	case DDERR_NOOPTIMIZEHW:
		return "DDERR_NOOPTIMIZEHW: Device does not support optimized surfaces, therefore no video memory optimized surfaces";

	case DDERR_NOTLOADED:
		return "DDERR_NOTLOADED: Surface is an optimized surface, but has not yet been allocated any memory";

	case DDERR_DCALREADYCREATED:
		return "DDERR_DCALREADYCREATED: A DC has already been returned for this surface. Only one DC can be retrieved per surface.";

	case DDERR_NONONLOCALVIDMEM:
		return "DDERR_NONONLOCALVIDMEM: An attempt was made to allocate non-local video memory from a device that does not support non-local video memory.";

	case DDERR_CANTPAGELOCK:
		return "DDERR_CANTPAGELOCK: The attempt to page lock a surface failed.";

	case DDERR_CANTPAGEUNLOCK:
		return "DDERR_CANTPAGEUNLOCK: The attempt to page unlock a surface failed.";

	case DDERR_NOTPAGELOCKED:
		return "DDERR_NOTPAGELOCKED: An attempt was made to page unlock a surface with no outstanding page locks.";

	case DDERR_MOREDATA:
		return "DDERR_MOREDATA: There is more data available than the specified buffer size could hold";

	case DDERR_VIDEONOTACTIVE:
		return "DDERR_VIDEONOTACTIVE: The video port is not active";

	case DDERR_DEVICEDOESNTOWNSURFACE:
		return "DDERR_DEVICEDOESNTOWNSURFACE: Surfaces created by one direct draw device cannot be used directly by another direct draw device.";

	case DDERR_NOTINITIALIZED:
		return "DDERR_NOTINITIALIZED: An attempt was made to invoke an interface member of a DirectDraw object created by CoCreateInstance() before it was initialized.";

		default:
			return "Unknown generic directdraw error";
	}
}

char *get_d3derr(HRESULT err)
{
	char *res1 = get_dderr(err);
	if (res1 != NULL) return res1;
	switch(err) 
	{
		case D3DERR_BADMAJORVERSION:
				return "D3DERR_BADMAJORVERSION\0";
		case D3DERR_BADMINORVERSION:
				return "D3DERR_BADMINORVERSION\0";
		case D3DERR_EXECUTE_LOCKED:
				return "D3DERR_EXECUTE_LOCKED\0";
		case D3DERR_EXECUTE_NOT_LOCKED:
				return "D3DERR_EXECUTE_NOT_LOCKED\0";
		case D3DERR_EXECUTE_CREATE_FAILED:
				return "D3DERR_EXECUTE_CREATE_FAILED\0";
		case D3DERR_EXECUTE_DESTROY_FAILED:
				return "D3DERR_EXECUTE_DESTROY_FAILED\0";
		case D3DERR_EXECUTE_LOCK_FAILED:
				return "D3DERR_EXECUTE_LOCK_FAILED\0";
		case D3DERR_EXECUTE_UNLOCK_FAILED:
				return "D3DERR_EXECUTE_UNLOCK_FAILED\0";
		case D3DERR_EXECUTE_FAILED:
				return "D3DERR_EXECUTE_FAILED\0";
		case D3DERR_EXECUTE_CLIPPED_FAILED:
				return "D3DERR_EXECUTE_CLIPPED_FAILED\0";
		case D3DERR_TEXTURE_NO_SUPPORT:
				return "D3DERR_TEXTURE_NO_SUPPORT\0";
		case D3DERR_TEXTURE_NOT_LOCKED:
				return "D3DERR_TEXTURE_NOT_LOCKED\0";
		case D3DERR_TEXTURE_LOCKED:
				return "D3DERR_TEXTURELOCKED\0";
		case D3DERR_TEXTURE_CREATE_FAILED:
				return "D3DERR_TEXTURE_CREATE_FAILED\0";
		case D3DERR_TEXTURE_DESTROY_FAILED:
				return "D3DERR_TEXTURE_DESTROY_FAILED\0";
		case D3DERR_TEXTURE_LOCK_FAILED:
				return "D3DERR_TEXTURE_LOCK_FAILED\0";
		case D3DERR_TEXTURE_UNLOCK_FAILED:
				return "D3DERR_TEXTURE_UNLOCK_FAILED\0";
		case D3DERR_TEXTURE_LOAD_FAILED:
				return "D3DERR_TEXTURE_LOAD_FAILED\0";
		case D3DERR_MATRIX_CREATE_FAILED:
				return "D3DERR_MATRIX_CREATE_FAILED\0";
		case D3DERR_MATRIX_DESTROY_FAILED:
				return "D3DERR_MATRIX_DESTROY_FAILED\0";
		case D3DERR_MATRIX_SETDATA_FAILED:
				return "D3DERR_MATRIX_SETDATA_FAILED\0";
		case D3DERR_SETVIEWPORTDATA_FAILED:
				return "D3DERR_SETVIEWPORTDATA_FAILED\0";
		case D3DERR_MATERIAL_CREATE_FAILED:
				return "D3DERR_MATERIAL_CREATE_FAILED\0";
		case D3DERR_MATERIAL_DESTROY_FAILED:
				return "D3DERR_MATERIAL_DESTROY_FAILED\0";
		case D3DERR_MATERIAL_SETDATA_FAILED:
				return "D3DERR_MATERIAL_SETDATA_FAILED\0";
		case D3DERR_LIGHT_SET_FAILED:
				return "D3DERR_LIGHT_SET_FAILED\0";
		case D3DERR_INVALID_DEVICE:
			return "Invalid device";
		case D3DERR_INITFAILED:
			return "Init failed";
		case D3DERR_DEVICEAGGREGATED:
			return "SetRenderTarget attempted on a device that was\nQI'd off the render target.";
		case D3DERR_INVALIDCURRENTVIEWPORT:
			return "Invalid current viewport";
		case D3DERR_INVALIDPRIMITIVETYPE:
			return "Invalid primitivetype";
		case D3DERR_INVALIDVERTEXTYPE:
			return "Invalid vertex type";
		case D3DERR_TEXTURE_BADSIZE:
			return "Bad texturesize";
		case D3DERR_INVALIDRAMPTEXTURE:
			return "Invalid ramptexture";
		case D3DERR_MATERIAL_GETDATA_FAILED:
			return "Material-getdata failed";
		case D3DERR_INVALIDPALETTE:
			return "Invalid palette";
		case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY:
			return "Z-buffer needs systemmemory";
		case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY:
			return "Z-buffer needs videomemory";
		case D3DERR_SURFACENOTINVIDMEM:
			return "Surface not in videomemory";
		case D3DERR_LIGHTHASVIEWPORT:
			return "Light has viewport";
		case D3DERR_LIGHTNOTINTHISVIEWPORT:
			return "Light not in this viewport";
		case D3DERR_SCENE_IN_SCENE:
			return "Scene in scene";
		case D3DERR_SCENE_NOT_IN_SCENE:
			return "Scene not in scene";
		case D3DERR_SCENE_BEGIN_FAILED:
			return "Scene-begin failed";
		case D3DERR_SCENE_END_FAILED:
			return "Scene-end failed";
		case D3DERR_INBEGIN:
			return "In begin";
		case D3DERR_NOTINBEGIN:
			return "Not in begin";
		case D3DERR_NOVIEWPORTS:
			return "There are no viewports";
		case D3DERR_VIEWPORTDATANOTSET:
			return "Viewport has not been set";
		case D3DERR_VIEWPORTHASNODEVICE:
			return "Viewport has no device";
		case D3DERR_NOCURRENTVIEWPORT:
			return "There are no current viewport";
	}
	return "Unknown d3d-error";
}

char *get_dsounderr(HRESULT err)
{
	switch (err)
	{
	case DSERR_ALLOCATED:
		return "DSERR_ALLOCATED: The call failed because resources (such as a priority level) were already being used by another caller.";
	case DSERR_CONTROLUNAVAIL:
		return "DSERR_CONTROLUNAVAIL: The control (vol,pan,etc.) requested by the caller is not available.";
	case DSERR_INVALIDPARAM:
		return "DSERR_INVALIDPARAM: An invalid parameter was passed to the returning function";
	case DSERR_INVALIDCALL:
		return "DSERR_INVALIDCALL: This call is not valid for the current state of this object";
	case DSERR_GENERIC:
		return "DSERR_GENERIC: An undetermined error occured inside the DirectSound subsystem";
	case DSERR_PRIOLEVELNEEDED:
		return "DSERR_PRIOLEVELNEEDED: The caller does not have the priority level required for the function to\nsucceed.";
	case DSERR_OUTOFMEMORY:
		return "DSERR_OUTOFMEMORY: Not enough free memory is available to complete the operation";
	case DSERR_BADFORMAT:
		return "DSERR_BADFORMAT: The specified WAVE format is not supported";
	case DSERR_UNSUPPORTED:
		return "DSERR_UNSUPPORTED: The function called is not supported at this time";
	case DSERR_NODRIVER:
		return "DSERR_NODRIVER: No sound driver is available for use";
	case DSERR_ALREADYINITIALIZED:
		return "DSERR_ALREADYINITIALIZED: This object is already initialized";
	case DSERR_NOAGGREGATION:
		return "DSERR_NOAGGREGATION: This object does not support aggregation";
	case DSERR_BUFFERLOST:
		return "DSERR_BUFFERLOST: The buffer memory has been lost, and must be restored.";
	case DSERR_OTHERAPPHASPRIO:
		return "DSERR_OTHERAPPHASPRIO: Another app has a higher priority level, preventing this call from\nsucceeding.";
	case DSERR_UNINITIALIZED:
		return "DSERR_UNINITIALIZED: This object has not been initialized";
	case DSERR_NOINTERFACE:
		return "DSERR_NOINTERFACE: The requested COM interface is not available";
	}
	return "Generic unknown strange and fatal DSound-error";
}

char *get_dinputerr(HRESULT err)
{
	switch (err)
	{
/*	case DI_OK:
		return "The operation completed successfully.";
	case DI_NOTATTACHED:
		return "The device exists but is not currently attached.";
	case DI_BUFFEROVERFLOW:
		return "The device buffer overflowed.	 Some input was lost.";
	case DI_PROPNOEFFECT:
		return "The change in device properties had no effect.";
	case DI_NOEFFECT:
		return "The operation had no effect.";
*/
	case DI_POLLEDDEVICE:
		return "The device is a polled device.	As a result, device buffering\nwill not collect any data and event notifications will not be\nsignalled until GetDeviceState is called.";
	case DI_DOWNLOADSKIPPED:
		return "The parameters of the effect were successfully updated by\nIDirectInputEffect::SetParameters, but the effect was not\ndownloaded because the device is not exclusively acquired\nor because the DIEP_NODOWNLOAD flag was passed.";
	case DI_EFFECTRESTARTED:
		return "The parameters of the effect were successfully updated by\nIDirectInputEffect::SetParameters, but in order to change\nthe parameters, the effect needed to be restarted.";
	case DI_TRUNCATED:
		return "The parameters of the effect were successfully updated by\nIDirectInputEffect::SetParameters, but some of them were\nbeyond the capabilities of the device and were truncated.";
	case DI_TRUNCATEDANDRESTARTED:
		return "Equal to DI_EFFECTRESTARTED | DI_TRUNCATED.";
	case DIERR_OLDDIRECTINPUTVERSION:
		return "The application requires a newer version of DirectInput.";
	case DIERR_BETADIRECTINPUTVERSION:
		return "The application was written for an unsupported prerelease version\nof DirectInput.";
	case DIERR_BADDRIVERVER:
		return "The object could not be created due to an incompatible driver version\nor mismatched or incomplete driver components.";
	case DIERR_DEVICENOTREG:
		return "The device or device instance or effect is not registered with DirectInput.";
	case DIERR_NOTFOUND:
		return "The requested object does not exist.";
//	case DIERR_OBJECTNOTFOUND:
//		return "The requested object does not exist.";
	case DIERR_INVALIDPARAM:
		return "An invalid parameter was passed to the returning function,\nor the object was not in a state that admitted the function\nto be called.";
	case DIERR_NOINTERFACE:
		return "The specified interface is not supported by the object";
	case DIERR_GENERIC:
		return "An undetermined error occured inside the DInput subsystem";
	case DIERR_OUTOFMEMORY:
		return "The DInput subsystem couldn't allocate sufficient memory to complete the\ncaller's request.";
	case DIERR_UNSUPPORTED:
		return "The function called is not supported at this time";
	case DIERR_NOTINITIALIZED:
		return "This object has not been initialized";
	case DIERR_ALREADYINITIALIZED:
		return "This object is already initialized";
	case DIERR_NOAGGREGATION:
		return "This object does not support aggregation";
	case DIERR_OTHERAPPHASPRIO:
		return "Another app has a higher priority level, preventing this call from\nsucceeding.";
	case DIERR_INPUTLOST:
		return "Access to the device has been lost.	 It must be re-acquired.";
	case DIERR_ACQUIRED:
		return "The operation cannot be performed while the device is acquired.";
	case DIERR_NOTACQUIRED:
		return "The operation cannot be performed unless the device is acquired.";
//	case DIERR_READONLY:
//		return "The specified property cannot be changed.";
//	case DIERR_HANDLEEXISTS:
//		return "The device already has an event notification associated with it.";
	case E_PENDING:
		return "Data is not yet available.";
	case DIERR_INSUFFICIENTPRIVS:
		return "Unable to IDirectInputJoyConfig_Acquire because the user\ndoes not have sufficient privileges to change the joystick\nconfiguration.";
	case DIERR_DEVICEFULL:
		return "The device is full.";
	case DIERR_MOREDATA:
		return "Not all the requested information fit into the buffer.";
	case DIERR_NOTDOWNLOADED:
		return "The effect is not downloaded.";
	case DIERR_HASEFFECTS:
		return "The device cannot be reinitialized because there are still effects\nattached to it.";
	case DIERR_NOTEXCLUSIVEACQUIRED:
		return "The operation cannot be performed unless the device is acquired\nin DISCL_EXCLUSIVE mode.";
	case DIERR_INCOMPLETEEFFECT:
		return "The effect could not be downloaded because essential information\nis missing.	 For example, no axes have been associated with the\neffect, or no type-specific information has been created.";
	case DIERR_NOTBUFFERED:
		return "Attempted to read buffered device data from a device that is\nnot buffered.";
	case DIERR_EFFECTPLAYING:
		return "An attempt was made to modify parameters of an effect while it is\nplaying.	 Not all hardware devices support altering the parameters\nof an effect while it is playing.";
	}
	return "Strange error reported by the DirectInput sub-sub-subsystem";
}
